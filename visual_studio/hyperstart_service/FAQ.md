
# FAQ

## FAQ 1: 运行exe直接退出

### 现象
```
编译后的exe直接复制到nanoserver中，执行后直接退出，且无任何输出
```

### 原因
```
nanoserver中可能缺少动态链接库
```

### 解决
```
//找出exe依赖的dll

方法1:
git bash下用ldd命令查看exe依赖的dll

方法2:
用Dependency Walker(http://www.dependencywalker.com/),找出依赖的dll


//复制这些dll到exe所在目录，即可正常运行exe

//nanoserver的docker镜像中，hyperstart_service.exe依赖的dll如下:
ntdll.dll        Windows/System32/ntdll.dll 一致
msvcrt.dll       Windows/System32/msvcrt.dll 一致
sechost.dll      Windows/System32/sechost.dll 一致
RPCRT4.dll       Windows/System32/rpcrt4.dll 大小写
KERNELBASE.dll   Windows/System32/KernelBase.dll 大小写
KERNEL32.DLL     Windows/System32/forwarders/kernel32.dll 位置及大小写
ADVAPI32.dll     Windows/System32/forwarders/advapi32.dll 位置及大小写
WTSAPI32.dll     <missing>

只要把WTSAPI32.dll放到hyperstart_service.exe一起即可
```

## FAQ 2: Debug模式编译时，默认库MSVCRT与其他库的使用冲突

### 现象
```
LINK : warning LNK4098: 默认库"MSVCRT"与其他库的使用冲突；请使用 /NODEFAULTLIB:library
```

### 原因
```
链接器工具警告 LNK4098
https://msdn.microsoft.com/zh-cn/library/6wtdswk0.aspx
```

### 解决
```
Relase模式下，项目属性->配置属性->C/C++->代码生成->运行库: 
	多线程/MT
则VisualStudio中，项目属性->配置属性->链接器->输入->忽略特定默认库：
	libc.lib,msvcrt.lib,libcd.lib,libcmtd.lib,msvcrtd.lib

Debug模式下，项目属性->配置属性->C/C++->代码生成->运行库: 
	多线程调试 DLL (/MDd)
则VisualStudio中，项目属性->配置属性->链接器->输入->忽略特定默认库：
	libc.lib;libcmt.lib;msvcrt.lib;libcd.lib;libcmtd.lib
```
