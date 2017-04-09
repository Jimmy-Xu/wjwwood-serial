@REM re-register HyperStartService
x64\Release\hyperstart_service.exe -uninstall
x64\Release\hyperstart_service.exe -install
x64\Release\hyperstart_service.exe -status

@REM Run Service as a frontend program
ServiceMonitor.exe HyperStartService
