#include <windows.h>
#include <stdio.h>
#include <direct.h>
#include <io.h>
int main()
{
	FILE *fp = NULL;
	char file[] = "E:\\diskWakeOn.txt";
	errno_t err;
	int totalTime = 0;
	int thisTime = 0;
	int bootCounter = 0;
	err = fopen_s(&fp, file, "r");
	if (err == 0){
		fscanf_s(fp,"%d\n%d:",&totalTime,&bootCounter);
		fclose(fp);
	}
	bootCounter++;
	totalTime *= 60;
	err = fopen_s(&fp,file,"w");
	if (err != 0)
		exit(0);
	fprintf_s(fp, "%d\n%d:%d", totalTime/60, bootCounter, thisTime/60);
	fclose(fp);

	while (true){
		err = fopen_s(&fp,file,"w");
		if (err != 0)
			exit(0);
		fprintf_s(fp, "%d\n%d:%d", totalTime/60, bootCounter, thisTime/60);
		fclose(fp);
		Sleep(17000);
		thisTime += 17;
		totalTime += 17;
	}
	return 0;
}