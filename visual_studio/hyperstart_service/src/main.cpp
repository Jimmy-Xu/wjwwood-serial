#include "hyperstart_service.h"
#include "service_installer.h"

int _tmain(int argc, TCHAR* argv[]) {
  HyperStartService service;

  if (argc > 1) {
    if (_tcscmp(argv[1], _T("-install")) == 0) {
      _tprintf(_T("Installing service\n"));
      if (!ServiceInstaller::Install(service)) {
        _tprintf(_T("Couldn't install service: %d\n"), ::GetLastError());
        return -1;
      }
      _tprintf(_T("Service installed\n"));
      return 0;
    }
	else if (_tcscmp(argv[1], _T("-status")) == 0) {
		_tprintf(_T("Service status:\n"));
		if (!ServiceInstaller::Status(service)) {
			_tprintf(_T("Check Service status failed!\n"));
			return -1;
		}
		return 0;
	}
	else if (_tcscmp(argv[1], _T("-stop")) == 0) {
		_tprintf(_T("Stopping service\n"));
		if (!ServiceInstaller::Stop(service)) {
			_tprintf(_T("Couldn't stop service: %d\n"), ::GetLastError());
			return -1;
		}
		_tprintf(_T("Service stopped\n"));
		return 0;
	}
	else if (_tcscmp(argv[1], _T("-uninstall")) == 0) {
		_tprintf(_T("Uninstalling service\n"));
		if (!ServiceInstaller::Uninstall(service)) {
			_tprintf(_T("Couldn't uninstall service: %d\n"), ::GetLastError());
			return -1;
		}
		_tprintf(_T("Service uninstalled\n"));
		return 0;
	}
	else if (_tcscmp(argv[1], _T("-console")) == 0) {
		_tprintf(_T("Run hyperstart as frontend\n"));
		return 0;
	}

    _tprintf(_T("Invalid argument\n"));
    return -1;
  }
  else {
	  wprintf(L"Parameters:\n");
	  wprintf(L" -install     to install the service.\n");
	  wprintf(L" -status      to check service status.\n");
	  wprintf(L" -stop        to stop the service.\n");
	  wprintf(L" -uninstall   to install the service.\n");
	  wprintf(L" -console     to run as console program.\n");
  }
  
  service.Run();

  return 0;
}