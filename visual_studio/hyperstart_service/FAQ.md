
# FAQ

## FAQ 1: ����exeֱ���˳�

### ����
```
������exeֱ�Ӹ��Ƶ�nanoserver�У�ִ�к�ֱ���˳��������κ����
```

### ԭ��
```
nanoserver�п���ȱ�ٶ�̬���ӿ�
```

### ���
```
//�ҳ�exe������dll

����1:
git bash����ldd����鿴exe������dll

����2:
��Dependency Walker(http://www.dependencywalker.com/),�ҳ�������dll


//������Щdll��exe����Ŀ¼��������������exe

//nanoserver��docker�����У�hyperstart_service.exe������dll����:
ntdll.dll        Windows/System32/ntdll.dll һ��
msvcrt.dll       Windows/System32/msvcrt.dll һ��
sechost.dll      Windows/System32/sechost.dll һ��
RPCRT4.dll       Windows/System32/rpcrt4.dll ��Сд
KERNELBASE.dll   Windows/System32/KernelBase.dll ��Сд
KERNEL32.DLL     Windows/System32/forwarders/kernel32.dll λ�ü���Сд
ADVAPI32.dll     Windows/System32/forwarders/advapi32.dll λ�ü���Сд
WTSAPI32.dll     <missing>

ֻҪ��WTSAPI32.dll�ŵ�hyperstart_service.exeһ�𼴿�
```

## FAQ 2: Debugģʽ����ʱ��Ĭ�Ͽ�MSVCRT���������ʹ�ó�ͻ

### ����
```
LINK : warning LNK4098: Ĭ�Ͽ�"MSVCRT"���������ʹ�ó�ͻ����ʹ�� /NODEFAULTLIB:library
```

### ԭ��
```
���������߾��� LNK4098
https://msdn.microsoft.com/zh-cn/library/6wtdswk0.aspx
```

### ���
```
Relaseģʽ�£���Ŀ����->��������->C/C++->��������->���п�: 
	���߳�/MT
��VisualStudio�У���Ŀ����->��������->������->����->�����ض�Ĭ�Ͽ⣺
	libc.lib,msvcrt.lib,libcd.lib,libcmtd.lib,msvcrtd.lib

Debugģʽ�£���Ŀ����->��������->C/C++->��������->���п�: 
	���̵߳��� DLL (/MDd)
��VisualStudio�У���Ŀ����->��������->������->����->�����ض�Ĭ�Ͽ⣺
	libc.lib;libcmt.lib;msvcrt.lib;libcd.lib;libcmtd.lib
```
