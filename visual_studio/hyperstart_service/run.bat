@REM uninstall old HyperStartService
x64\Release\hyperstart_service.exe -uninstall

@REM install and start new HyperStartService
x64\Release\hyperstart_service.exe -install
x64\Release\hyperstart_service.exe -status

@REM stop HyperStartService
x64\Release\hyperstart_service.exe -stop
x64\Release\hyperstart_service.exe -status

@REM start HyperStartService
x64\Release\hyperstart_service.exe -start
x64\Release\hyperstart_service.exe -status

@REM Run Service as a frontend program
ServiceMonitor.exe HyperStartService
