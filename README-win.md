# 依赖

- Cmake(cmake.exe)
  - cmake-3.8.0-rc4-win64-x64.msi
- Visual Studio (cl.exe)
   - VS 2017 Community

```
C:\>where cmake
D:\Program Files\CMake\bin\cmake.exe

C:\>where msbuild
D:\Program Files (x86)\Microsoft Visual Studio\2017\Community\MSBuild\15.0\Bin\MSBuild.exe

c:\>where cl
D:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.10.25017\bin\HostX64\x64\cl.exe
```

# VS2017下编译

## 一般编译
```
cd visual_studio
msbuild.exe
```

## 生成Release
```
msbuild  /p:Configuration=Release
```
## 生成Release,并指定平台
```
//VS2017 IDE中，解决方案配置设置为Release，解决方案平台设置为x64
msbuild /p:Configuration=Release /p:Platform=x64`
```

# 执行
```
.\x64\Release\test_serial.exe -e
(COM1, ELTIMA Virtual Serial Port (COM1), EVSERIAL7)
```
