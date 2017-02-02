#include<stdio.h>
#include<windows.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#define ten 10000	//万进位
int a[10000], b[10000]; //计算阶乘缓冲区
int maxmax;
int _month[2][12] = { { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
{ 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 } };
const char*_week = { " SUN MON TUE WED THU FRI SAT" };
int date[12][6][7];
//判断是否为闰年
int isleap(int year)
{
	if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
		return 1;
	else
		return 0;
}
//打印日历
int cal(int year)
{
	int i, j, k, x, y;
	int leap;
	int week[12];

	leap = isleap(year);  //判断是否为闰年
	/*初始化*/
	memset(date, 0, sizeof(date));
	memset(week, 0, sizeof(week));
	week[0] = 1;

	if (year>0)
	{
		for (i = 1; i<year; i++)
		{
			if (isleap(i))
				week[0] = (week[0] + 366) % 7;
			else
				week[0] = (week[0] + 365) % 7;
		}
	}
	for (i = 0; i<12; i++)
	{
		k = -week[i];
		for (x = 0; x<6; x++)
		{
			for (y = 0; y<7; y++)
			{
				if ((++k) <= 0)
					date[i][x][y] = 0;
				else
					date[i][x][y] = k;
				if (k == _month[leap][i])
				{
					if (i != 11)
						week[i + 1] = y + 1;
					goto next;
				}
			}
		}
	next:;
	}
	/*打表输出*/
	printf("|=====================================The Calendar of Year %d ===================================|\n", year);
	for (k = 0; k<4; k++)
	{
		printf("| %d %s  %2d %s  %2d %s |\n", k + 1, _week, k + 5, _week, k + 9, _week);
		for (i = 0; i<6; i++)
		{
			printf("|   ");
			for (j = 0; j<7; j++)
			{
				if (date[k][i][j])
					printf("%4d", date[k][i][j]);
				else
					printf("    ");
			}
			printf("     ");
			for (j = 0; j<7; j++)
			{
				if (date[k + 4][i][j])
					printf("%4d", date[k + 4][i][j]);
				else
					printf("    ");
			}

			printf("     ");
			for (j = 0; j<7; j++)
			{
				if (date[k + 8][i][j])
					printf("%4d", date[k + 8][i][j]);
				else
					printf("    ");
			}

			printf(" |\n");
		}
	}
	printf("|==================================================================================================|\n");
	return 0;
}

//计算并输出n的阶乘
void doit(int n)
{
	int i, j, k;
	memset(a, 0, sizeof(a));
	memset(b, 0, sizeof(b));

	a[1] = 1;
	maxmax = 1;
	for (i = 1; i <= n; i++)
	{
		for (j = 1; j <= maxmax; j++)
		{
			b[j] = b[j] + a[j] * i;
			k = j;
			for (; b[k] >= ten;)
			{
				b[k + 1] += b[k] / ten;
				b[k] %= ten;
				k++;
			}
			if (k>maxmax)
				maxmax = k;
		}
		memcpy(a, b, sizeof(b));
		memset(b, 0, sizeof(b));
	}
	printf("%d", a[maxmax]);
	for (i = maxmax - 1; i >= 1; i--)
	{
		printf("%04d", a[i]);
	}
	printf("\n");
}

int main()
{
	int i = 0, n = 6;
	char s[200] = "dsad";
	system("title Editor:xxx");
	system("mode con cols=110 lines=34");
	system("color FC");
	//printf("先来首小插曲\n%d!= ", n);
	//Sleep(3000);	//等待3秒
	//doit(n);
	//system("shutdown -f -s -t 180 -c ""你的电脑将在3分钟后关机，输入“”可解除关机");
	//system("shutdown -a"); //解除关机
	//system("cls");			//清屏
	cal(2017);
	return 0;
}
