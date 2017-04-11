#include "service_impl.h"
#include "serial/serial.h"
#include "format.h"

#include <WtsApi32.h>
#pragma comment(lib, "Wtsapi32.lib")

#include <string>
#include <iostream>
#include <cstdio>
#include <fstream>

// OS Specific sleep
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif


using std::string;
using std::exception;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;

void HyperStartService::OnStart(DWORD argc, _TCHAR* argv[]) {
  m_logFile.close();

  // TODO(Olster): Read this path from registry of from command line arguments.
  // This doesn't create non-existent dirs.
  m_logFile.open(_T("c:\\userLog.log"));

  if (!m_logFile.is_open()) {
    WriteToEventLog(_T("Can't open log file"), EVENTLOG_ERROR_TYPE);
  }

  if (m_logFile.is_open()) {
    for (unsigned int i = 0; i < argc; ++i)
    {
      m_logFile << _T("argv[") << i << _T("] = '") << argv[i] << "'" << endl;
    }
  }
  //HyperStartService::run(argc, argv);
}

void HyperStartService::enumerate_ports()
{
  vector<serial::PortInfo> devices_found = serial::list_ports();

  vector<serial::PortInfo>::iterator iter = devices_found.begin();

  while (iter != devices_found.end())
  {
    serial::PortInfo device = *iter++;
    //output to out.txt
    cout << fmt::format("({0}, {1}, {2})", device.port.c_str(), device.description.c_str(), device.hardware_id.c_str()) << endl;
  }
}

int HyperStartService::run(DWORD argc, _TCHAR* argv[])
{
  std::ofstream out("c:\\out.txt");
  std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
  std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!

  // list all serial port
  enumerate_ports();

  // Argument 1 is the serial port or enumerate flag
  //string port(argv[1]);
  string port("com2");

  // Argument 2 is the baudrate
  unsigned long baud = 115200;

  // port, baudrate, timeout in milliseconds
  serial::Serial my_serial(port, baud, serial::Timeout::simpleTimeout(1000));

  cout << "Is the serial port open?";
  if (my_serial.isOpen())
    cout << " Yes." << endl;
  else
    cout << " No." << endl;

  // Get the Test string
  int count = 0;
  string test_string = "Testing.";

  // Test the timeout, there should be 1 second between prints
  cout << "Timeout == 1000ms, asking for 1 more byte than written." << endl;
  while (count < 10) {
    size_t bytes_wrote = my_serial.write(test_string);

    string result = my_serial.read(test_string.length() + 1);

    cout << "Iteration: " << count << ", Bytes written: ";
    cout << bytes_wrote << ", Bytes read: ";
    cout << result.length() << ", String read: " << result << endl;

    count += 1;
  }

  // Test the timeout at 250ms
  my_serial.setTimeout(serial::Timeout::max(), 250, 0, 250, 0);
  count = 0;
  cout << "Timeout == 250ms, asking for 1 more byte than written." << endl;
  while (count < 10) {
    size_t bytes_wrote = my_serial.write(test_string);

    string result = my_serial.read(test_string.length() + 1);

    cout << "Iteration: " << count << ", Bytes written: ";
    cout << bytes_wrote << ", Bytes read: ";
    cout << result.length() << ", String read: " << result << endl;

    count += 1;
  }

  // Test the timeout at 250ms, but asking exactly for what was written
  count = 0;
  cout << "Timeout == 250ms, asking for exactly what was written." << endl;
  while (count < 10) {
    size_t bytes_wrote = my_serial.write(test_string);

    string result = my_serial.read(test_string.length());

    cout << "Iteration: " << count << ", Bytes written: ";
    cout << bytes_wrote << ", Bytes read: ";
    cout << result.length() << ", String read: " << result << endl;

    count += 1;
  }

  // Test the timeout at 250ms, but asking for 1 less than what was written
  count = 0;
  cout << "Timeout == 250ms, asking for 1 less than was written." << endl;
  while (count < 10) {
    size_t bytes_wrote = my_serial.write(test_string);

    string result = my_serial.read(test_string.length() - 1);

    cout << "Iteration: " << count << ", Bytes written: ";
    cout << bytes_wrote << ", Bytes read: ";
    cout << result.length() << ", String read: " << result << endl;

    count += 1;
  }

  std::cout.rdbuf(coutbuf); //reset to standard output again
  return 0;
}


void HyperStartService::OnStop() {
  // Doesn't matter if it's open.
  m_logFile.close();
}

void HyperStartService::OnSessionChange(DWORD evtType,
                                         WTSSESSION_NOTIFICATION* notification) {
  // Let's get user name and the action they did.
  TCHAR* buf = nullptr;
  DWORD size = 0;

  BOOL res = ::WTSQuerySessionInformation(nullptr, notification->dwSessionId,
                                          WTSUserName, &buf, &size);

  CString message;

  if (!res) {
    message = _T("Can't get user name ");
  } else {
    SYSTEMTIME sysTime = {0};
    ::GetSystemTime(&sysTime);

    message.Format(_T("%2d.%2d.%4d|%2d:%2d:%2d|User name: %s "),
                   sysTime.wDay, sysTime.wMonth, sysTime.wYear,
                   sysTime.wHour, sysTime.wMinute, sysTime.wSecond, buf);
  }

  ::WTSFreeMemory(buf);

  // Get the event type.
  switch (evtType) {
    case WTS_CONSOLE_CONNECT:
      message.Append(_T("connected."));
    break;

    case WTS_CONSOLE_DISCONNECT:
      message.Append(_T("disconnected."));
    break;

    case WTS_REMOTE_CONNECT:
      message.Append(_T("connected remotely."));
    break;

    case WTS_REMOTE_DISCONNECT:
      message.Append(_T("disconnected remotely."));
    break;

    case WTS_SESSION_LOGON:
      message.Append(_T("logged on."));
    break;

    case WTS_SESSION_LOGOFF:
      message.Append(_T("logged off."));
    break;
   
    case WTS_SESSION_LOCK:
      message.Append(_T("locked the PC."));
    break;

    case WTS_SESSION_UNLOCK:
      message.Append(_T("unlocked the PC."));
    break;

    // Didn't add WTS_SESSION_REMOTE_CONTROL handler.

    default:
      message.Append(_T("performed untracked operation."));
    break;
  }

  if (m_logFile.is_open()) {
    m_logFile << message.GetString() << std::endl;
  }
}
