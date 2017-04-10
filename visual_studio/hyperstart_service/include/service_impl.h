#include "service_base.h"

#include <fstream>
#include <string>


const CString SERVICE_NAME = _T("HyperStartService");
const CString SERVICE_DISPLAY_NAME = _T("HyperStart Service");

class HyperStartService : public ServiceBase {
 public:
  HyperStartService(const HyperStartService& other) = delete;
  HyperStartService& operator=(const HyperStartService& other) = delete;

  HyperStartService(HyperStartService&& other) = delete;
  HyperStartService& operator=(HyperStartService&& other) = delete;

  HyperStartService()
   : ServiceBase(SERVICE_NAME,
                 SERVICE_DISPLAY_NAME,
                 SERVICE_DEMAND_START,
                 SERVICE_ERROR_NORMAL,
                 SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SESSIONCHANGE) {}
 private:
   void OnStart(DWORD argc, _TCHAR* argv[]) override;
   void OnStop() override;
   void OnSessionChange(DWORD evtType,
                        WTSSESSION_NOTIFICATION* notification) override;
   
   void enumerate_ports();
   int run(DWORD argc, _TCHAR* argv[]);

#ifdef UNICODE
   using tofstream = std::wofstream;
#else
   using tofstream = std::ofstream;
#endif

   tofstream m_logFile;
};
