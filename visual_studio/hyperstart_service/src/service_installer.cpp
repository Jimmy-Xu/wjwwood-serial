#include "service_installer.h"

#include <Windows.h>
#include <atlstr.h>

#include "hyperstart_service.h"

namespace {
class ServiceHandle {
 public:
  ServiceHandle(SC_HANDLE handle)
   : m_handle(handle) {}

  ~ServiceHandle() {
    if (m_handle) {
      ::CloseServiceHandle(m_handle);
    }
  }

  operator SC_HANDLE() {
    return m_handle;
  }

 private:
  SC_HANDLE m_handle = nullptr;
};
}

//static
bool ServiceInstaller::Install(const ServiceBase& service) {
  CString escapedPath;
  TCHAR* modulePath = escapedPath.GetBufferSetLength(MAX_PATH);

  if (::GetModuleFileName(nullptr, modulePath, MAX_PATH) == 0) {
    _tprintf(_T("Couldn't get module file name: %d\n"), ::GetLastError());
    return false;
  }

  escapedPath.ReleaseBuffer();
  escapedPath.Remove(_T('\"'));

  escapedPath = _T('\"') + escapedPath + _T('\"');

  ServiceHandle svcControlManager = ::OpenSCManager(nullptr, nullptr,
                                                    SC_MANAGER_CONNECT | 
                                                    SC_MANAGER_CREATE_SERVICE);
  if (!svcControlManager) {
    _tprintf(_T("Couldn't open service control manager: %d\n"), GetLastError());
    return false;
  }
  
  const CString& depends = service.GetDependencies();
  const CString& acc = service.GetAccount();
  const CString& pass = service.GetPassword();

  _tprintf(_T("Create %s %s\n"), (LPCTSTR)service.GetName(),  (LPCTSTR)escapedPath);
  ServiceHandle servHandle = ::CreateService(svcControlManager,
                                             service.GetName(),
                                             service.GetDisplayName(),
                                             SERVICE_QUERY_STATUS,
                                             SERVICE_WIN32_OWN_PROCESS,
                                             SERVICE_AUTO_START,
                                             service.GetErrorControlType(),
                                             escapedPath,
                                             nullptr,
                                             nullptr,
                                             (depends.IsEmpty() ? nullptr : depends.GetString()),
                                             (acc.IsEmpty() ? nullptr : acc.GetString()),
                                             (pass.IsEmpty() ? nullptr : pass.GetString()));
  if (!servHandle) {
    _tprintf(_T("Couldn't create service: %d\n"), ::GetLastError());
    return false;
  }
  return true;
}

//static
bool ServiceInstaller::Status(const ServiceBase& service) {
	ServiceHandle svcControlManager = ::OpenSCManager(nullptr, nullptr, SC_MANAGER_CONNECT);

	if (!svcControlManager) {
		_tprintf(_T("Couldn't open service control manager: %d\n"), GetLastError());
		return false;
	}

	ServiceHandle servHandle = ::OpenService(svcControlManager, service.GetName(),
		SERVICE_QUERY_STATUS |
		SERVICE_STOP |
		DELETE);

	if (!servHandle) {
		_tprintf(_T("Couldn't open service control manager: %d\n"), ::GetLastError());
		return false;
	}

	SERVICE_STATUS servStatus = {};
	if (::QueryServiceStatus(servHandle, &servStatus)) {

		switch (servStatus.dwCurrentState)
		{
		case SERVICE_STOPPED:
			_tprintf(_T("STOPPED\n"));
			break;
		case SERVICE_START_PENDING:
			_tprintf(_T("START_PENDING\n"));
			break;
		case SERVICE_STOP_PENDING:
			_tprintf(_T("STOP_PENDING\n"));
			break;
		case SERVICE_RUNNING:
			_tprintf(_T("RUNNING\n"));
			break;
		case SERVICE_CONTINUE_PENDING:
			_tprintf(_T("CONTINUE_PENDING\n"));
			break;
		case SERVICE_PAUSE_PENDING:
			_tprintf(_T("PAUSE_PENDING\n"));
			break;
		case SERVICE_PAUSED:
			_tprintf(_T("PAUSED\n"));
			break;
		default:
			_tprintf(_T("Unknow status\n"));
			break;
		}
	}
	else {
		_tprintf(_T("Didn't control service: %d\n"), ::GetLastError());
		return false;
	}
	return true;
}


//static
bool ServiceInstaller::Start(const ServiceBase& service) {
	ServiceHandle svcControlManager = ::OpenSCManager(nullptr, nullptr,
		SC_MANAGER_CONNECT);

	if (!svcControlManager) {
		_tprintf(_T("Couldn't open service control manager: %d\n"), GetLastError());
		return false;
	}

	ServiceHandle servHandle = ::OpenService(svcControlManager, service.GetName(),
		SERVICE_QUERY_STATUS |
		SERVICE_START);

	if (!servHandle) {
		_tprintf(_T("Couldn't open service control manager: %d\n"), ::GetLastError());
		return false;
	}

	SERVICE_STATUS servStatus = {};
	if (::StartService(servHandle, 0, NULL)) {
		//_tprintf(_T("Service start pending %s\n"), service.GetName());
		while (::QueryServiceStatus(servHandle, &servStatus)) {
			if (servStatus.dwCurrentState != SERVICE_START_PENDING) {
				break;
			}
		}
		if (servStatus.dwCurrentState != SERVICE_RUNNING) {
			_tprintf(_T("Failed to start the service\n"));
			return false;
		}
	}
	else {
		_tprintf(_T("Didn't control service: %d\n"), ::GetLastError());
		return false;
	}
	return true;
}

//static
bool ServiceInstaller::Stop(const ServiceBase& service) {
	ServiceHandle svcControlManager = ::OpenSCManager(nullptr, nullptr,
		SC_MANAGER_CONNECT);

	if (!svcControlManager) {
		_tprintf(_T("Couldn't open service control manager: %d\n"), GetLastError());
		return false;
	}

	ServiceHandle servHandle = ::OpenService(svcControlManager, service.GetName(),
		SERVICE_QUERY_STATUS |
		SERVICE_STOP);

	if (!servHandle) {
		_tprintf(_T("Couldn't open service control manager: %d\n"), ::GetLastError());
		return false;
	}

	SERVICE_STATUS servStatus = {};
	if (::ControlService(servHandle, SERVICE_CONTROL_STOP, &servStatus)) {
		//_tprintf(_T("Service stop pending %s\n"), service.GetName());
		while (::QueryServiceStatus(servHandle, &servStatus)) {
			if (servStatus.dwCurrentState != SERVICE_STOP_PENDING) {
				break;
			}
		}
		if (servStatus.dwCurrentState != SERVICE_STOPPED) {
			_tprintf(_T("Failed to stop the service\n"));
			return false;
		}
	}
	else {
		_tprintf(_T("Didn't control service: %d\n"), ::GetLastError());
		return false;
	}
	return true;
}

//static
bool ServiceInstaller::Uninstall(const ServiceBase& service) {
	ServiceHandle svcControlManager = ::OpenSCManager(nullptr, nullptr,
		SC_MANAGER_CONNECT);

	if (!svcControlManager) {
		_tprintf(_T("Couldn't open service control manager: %d\n"), GetLastError());
		return false;
	}

	ServiceHandle servHandle = ::OpenService(svcControlManager, service.GetName(),
		SERVICE_QUERY_STATUS |
		SERVICE_STOP |
		DELETE);

	if (!servHandle) {
		_tprintf(_T("Couldn't open service control manager: %d\n"), ::GetLastError());
		return false;
	}

	//stop service first
	SERVICE_STATUS servStatus = {};
	if (::ControlService(servHandle, SERVICE_CONTROL_STOP, &servStatus)) {
		//_tprintf(_T("Stoping service %s\n"), service.GetName());
		while (::QueryServiceStatus(servHandle, &servStatus)) {
			if (servStatus.dwCurrentState != SERVICE_STOP_PENDING) {
				break;
			}
		}
		if (servStatus.dwCurrentState != SERVICE_STOPPED) {
			_tprintf(_T("Failed to stop the service\n"));
			return false;
		}
		//_tprintf(_T("Service stopped\n"));
	}
	else {
		_tprintf(_T("Didn't control service: %d\n"), ::GetLastError());
	}

	//delete service
	if (!::DeleteService(servHandle)) {
		_tprintf(_T("Failed to delete the service: %d\n"), GetLastError());
		return false;
	}
	return true;
}