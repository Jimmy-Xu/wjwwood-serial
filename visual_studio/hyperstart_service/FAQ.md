
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
