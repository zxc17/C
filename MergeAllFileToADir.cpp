#include "stdio.h" 
#include "windows.h"
#include <direct.h>
#include <io.h>
//#define _CRT_SECURE_NO_WARNINGS

char dstPath[MAX_PATH] = "mergeout";
char execPath[MAX_PATH];
char logfile[MAX_PATH];
char chRE[] = "*";
int pairsNum;
int copyFileNum, allFileNum;
FILE *fp;
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
			if(strncmp("承包方代表.xls",data.name,26)==0)
			{
				if (!locked)
				{
					pairsNum++;
					locked = true;
				}
				for (int i = 0; i < deep*3;i++) putchar(' ');
				printf("%s\n", data.name);
				fprintf(fp,"s_%s%s\n", fileName,data.name);
				fprintf(fp,"t_%04d_a.xls\n",pairsNum);
				sprintf_s(cmd, "copy \"%s\" \"%s\\%04d_a.xls\"", data.name, dstPath, pairsNum);
				freopen(logfile, "a", stdout);
				system(cmd);
				freopen("CON", "w", stdout);
				copyFileNum++;
			}
			else if(strncmp("身份证号码无效的户籍.xls",data.name,26)==0)
			{
				if (!locked)
				{
					pairsNum++;
					locked = true;
				}
				for (int i = 0; i < deep*3;i++) putchar(' ');
				printf("%s\n", data.name);
				fprintf(fp, "s_%s%s\n", fileName, data.name);
				fprintf(fp, "t_%04d_b.xls\n", pairsNum);
				sprintf_s(cmd, "copy \"%s\" \"%s\\%04d_b.xls\"", data.name, dstPath, pairsNum);
				freopen(logfile, "a", stdout);
				system(cmd);
				freopen("CON", "w", stdout);
				copyFileNum++;
			}
			allFileNum++;
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
	copyFileNum = pairsNum = allFileNum = 0;
	_getcwd(execPath, MAX_PATH);	//获得当前程序执行路径
	printf("输入要操作的目录：");
	scanf("%s", srcPath);
	fflush(stdin);
	printf("输入结果输出目录：");
	scanf("%s", dstPath);
	fflush(stdin);
	_mkdir(dstPath);				//创建相对或绝对路径
	char tmp[MAX_PATH];
	if (!((dstPath[1] == ':'&& dstPath[2] == '\\') || (dstPath[0] == '\\')))
	{	//转为绝对路径		
		sprintf(tmp, "%s\\%s", execPath, dstPath);
		strcpy_s(dstPath,tmp);
	}
	strcat_s(tmp,"\\map.txt");
	if ((fp = fopen(tmp, "w")) == NULL)
	{
		printf("Failure to open map.txt file!\n");
		exit(0);
	}
	strcpy_s(logfile, execPath);	//指定重定向输出路径
	strcat_s(logfile,MAX_PATH,"\\copy.log");	
	printf("%s\n", srcPath);
	if (-1 == _chdir(srcPath)) return 0;
	findin(deep, fileName);	//进行深度优先搜索
	if (-1 == _chdir("..")) return 0;
	printf_s("共复制了 %d/%d 个文件！\n", copyFileNum, allFileNum);
	fclose(fp);
	system("PAUSE");	
	return 0;
}

/*
s_河南村_第一组_承包方代表.xls
t_0001_a.xls
s_河南村_第一组_身份证号码无效的户籍.xls
t_0001_b.xls
s_河南村_第二组_承包方代表.xls
t_0002_a.xls
s_河南村_第二组_身份证号码无效的户籍.xls
t_0002_b.xls
s_淮北村_第一组_承包方代表.xls
t_0003_a.xls
s_淮北村_第一组_身份证号码无效的户籍.xls
t_0003_b.xls
s_淮北村_第二组_承包方代表.xls
t_0004_a.xls
s_淮北村_第二组_身份证号码无效的户籍.xls
t_0004_b.xls

输入要操作的目录：户籍
输入结果输出目录：nfd
户籍
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
共复制了 8/8 个文件！
*/
