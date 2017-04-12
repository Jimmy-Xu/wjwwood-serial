
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
