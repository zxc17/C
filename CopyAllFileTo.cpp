#include "stdio.h" 
#include "windows.h"
#include <direct.h>
#include <io.h>
//#define _CRT_SECURE_NO_WARNINGS

char dstPath[MAX_PATH] = "mergeout";
char execPath[MAX_PATH];
char logfile[MAX_PATH];
char chRE[] = "*";
int copyFileNum;
int order = 1;
int findin(int deep, char *fileName)
{
	struct _finddata_t data;
	char cmd[MAX_PATH * 2];
	int len = strnlen_s(fileName, MAX_PATH);
	deep ++;
	long hnd = _findfirst(chRE, &data);    // 查找文件名与正则表达式chRE的匹配第一个文件
	if (hnd < 0)
	{
		perror(chRE);
	}
	int nRet = hnd < 0 ? -1 : 1;
	bool locked = false;
	while (nRet >= 0)
	{
		if (data.attrib == _A_SUBDIR)  // 如果是目录
		{
			if (strncmp(".", data.name, MAX_PATH) != 0 && strncmp("..", data.name, MAX_PATH) != 0)
			{
				for (int i = 0; i < deep*3;i++) putchar(' ');
				printf("%s\\\n", data.name);
				strcat_s(fileName, MAX_PATH, data.name);
				strcat_s(fileName, MAX_PATH, "_");
				if (-1 == _chdir(data.name)) return 0;
				findin(deep, fileName);
				if (-1 == _chdir("..")) return 0;
				fileName[len] = '\0';	//还原缓冲区
			}			
		}
		else
		{			

				for (int i = 0; i < deep*3;i++) putchar(' ');
				printf("%s\n", data.name);
				switch (order)
				{
				case 1: sprintf_s(cmd, "copy \"%s\" \"%s\\%s%s\"", data.name, dstPath, fileName, data.name); break;
				case 2: 
					//char tmp[MAX_PATH];
					for (int jk = 1; jk <= 99; jk++)
					{

						if (!_access(data.name, 0))
						{
							sprintf_s(cmd, "copy \"%s\" \"%s\\%s\"", data.name, dstPath, data.name); break;
						}
					} break;
				}
				freopen(logfile, "a", stdout);
				system(cmd);
				freopen("CON", "w", stdout);
				copyFileNum++;
		}
		nRet = _findnext(hnd, &data);
	}
	_findclose(hnd);     // 关闭当前句柄
	return 0;
}
int main()
{
	char fileName[MAX_PATH]="";	//文件名缓冲区
	char srcPath[MAX_PATH];		//源目录
	int deep = 0;			//输出空格前缀长度
	copyFileNum = 0;
	_getcwd(execPath, MAX_PATH);	//获得当前程序执行路径
	printf("输入要操作的目录：");
	scanf("%s", srcPath);
	fflush(stdin);
	printf("输入结果输出目录：");
	scanf("%s", dstPath);
	fflush(stdin);
	_mkdir(dstPath);				//创建相对或绝对路径
	if (!((dstPath[1] == ':'&& dstPath[2] == '\\') || (dstPath[0] == '\\')))
	{	//转为绝对路径		
		char tmp[MAX_PATH];
		sprintf(tmp, "%s\\%s", execPath, dstPath);
		strcpy_s(dstPath,tmp);
	}
	strcpy_s(logfile, execPath);	//指定重定向输出路径
	strcat_s(logfile,MAX_PATH,"\\copy.log");	
	printf("%s\n", srcPath);
	if (-1 == _chdir(srcPath)) return 0;
	findin(deep, fileName);	//进行深度优先搜索
	if (-1 == _chdir("..")) return 0;
	printf_s("共复制了 %d 个文件！\n", copyFileNum);
	system("PAUSE");	
	return 0;
}


/*
输入要操作的目录：.
输入结果输出目录：F:\sedd
.
   copy.log
   Debug\
      lsdir.log
      lsdir.tlog\
         cl.command.1.tlog
         CL.read.1.tlog
         CL.write.1.tlog
         link.command.1.tlog
         link.read.1.tlog
         link.write.1.tlog
         lsdir.lastbuildstate
      main.obj
      vc120.idb
      vc120.pdb
   lsdir.exe
   lsdir.vcxproj
   lsdir.vcxproj.filters
   main.cpp
   nfd\
      0001_a.xls
      0001_b.xls
      0002_a.xls
      0002_b.xls
      0003_a.xls
      0003_b.xls
      0004_a.xls
      0004_b.xls
      map.txt
   户籍\
      河南村\
         第一组\
            承包方代表.xls
            身份证号码无效的户籍.xls
         第二组\
            承包方代表.xls
            身份证号码无效的户籍.xls
      淮北村\
         第一组\
            承包方代表.xls
            身份证号码无效的户籍.xls
         第二组\
            承包方代表.xls
            身份证号码无效的户籍.xls
   批量合并及格式.xlsm
共复制了 34 个文件！
*/