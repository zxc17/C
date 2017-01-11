#include <stdio.h>
#include <windows.h>
#include <direct.h>
#include <io.h>
//#define _CRT_SECURE_NO_WARNINGS
void main()
{
	char dstPath[] = "\\mergeout";	//复制且重命名后的文件存放的目的路径
	char dstFileName[MAX_PATH];		//目的文件名
	char srcFileName[MAX_PATH];		//源文件名
	char cmd[MAX_PATH];				//命令行缓冲区
	int fileCounter[2] = { 0 };			//文件计数器
	FILE *fp;
	_mkdir(dstPath);				//创建目的路径	
	if ((fp = fopen("map.txt", "r")) == NULL)	//打开映射文件eg:0001_a.xls "河南村_第一组_承包方代表.xls"												
	{
		printf("Failure to open map.txt file!\n");
		exit(0);
	}
	while (fscanf(fp, "%s \"%s\"", srcFileName, dstFileName) != EOF)
	{
		if (srcFileName[5] == 'b')
		{
			if (!_access(srcFileName, 0))
			{
				sprintf_s(cmd,MAX_PATH, "copy \"%s\" \"%s\\%s", srcFileName, dstPath, dstFileName);	
				puts(cmd);
				freopen("logfile.log", "w", stdout);	//临时切换输出流
				system(cmd);
				freopen("CON", "w", stdout);
				fileCounter[0]++;
			}
			else
			{
				printf("%s 文件不存在！\n", srcFileName);
				fileCounter[1]++;
			}
			
		}
	}
	printf_s("共复制了 %d/%d 个文件！\n", fileCounter[0],fileCounter[0]+fileCounter[1]);
	fclose(fp);
	system("PAUSE");
}

/* Comment
1.map.txt样例:
0001_a.xls "河南村_第一组_承包方代表.xls"
0001_b.xls "河南村_第一组_身份证号码无效的户籍.xls"
0002_a.xls "河南村_第二组_承包方代表.xls"
0002_b.xls "河南村_第二组_身份证号码无效的户籍.xls"
0003_a.xls "淮北村_第一组_承包方代表.xls"
0003_b.xls "淮北村_第一组_身份证号码无效的户籍.xls"
0004_a.xls "淮北村_第二组_承包方代表.xls"
0004_b.xls "淮北村_第二组_身份证号码无效的户籍.xls"

2.access函数判断文件夹或者文件是否存在
函数原型：	int access(const char *filename, int mode);
所属头文件：io.h
filename：可以填写文件夹路径或者文件路径
mode：	0 （F_OK） 只判断是否存在
		2 （R_OK） 判断写入权限
		4 （W_OK） 判断读取权限
		6 （X_OK） 判断执行权限
用于判断文件夹是否存在的时候，mode取0，判断文件是否存在的时候，mode可以取0、2、4、6。 若存在或者具有权限，返回值为0；不存在或者无权限，返回值为-1。
错误代码
EACCESS 参数pathname 所指定的文件不符合所要求测试的权限。
EROFS 欲测试写入权限的文件存在于只读文件系统内。
EFAULT 参数pathname指针超出可存取内存空间。
EINVAL 参数mode 不正确。
ENAMETOOLONG 参数pathname太长。
ENOTDIR 参数pathname为一目录。
ENOMEM 核心内存不足
ELOOP 参数pathname有过多符号连接问题。
EIO I/O 存取错误。
特别提醒：使用access()作用户认证方面的判断要特别小心，例如在access()后再做open()的空文件可能会造成系统安全上的问题。
*/