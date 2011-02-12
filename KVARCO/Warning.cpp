#include "pch.h"
#include "Warning.h"

int Warning::Retry_LoadGraph(string FileName,string ErrorMes , int RetryTime , int NotUse3DFlag)
{
	int GrHandle=0,i=0;
	do{
		i++;
		if(i>RetryTime)
		{
			string tmp=ErrorMes; tmp.append(FileName);
			int result=
				MessageBox(NULL,tmp.c_str(),"エラーが発生しました",MB_RETRYCANCEL | MB_ICONWARNING);
			if(result==IDCANCEL)	exit(0);
			else					GrHandle=i=0;
		}

		GrHandle=LoadGraph(FileName.c_str(),NotUse3DFlag);
	}while(GrHandle==-1);

	return GrHandle;
}

int Warning::Retry_LoadDivGraph(string FileName, int AllNum, int XNum, int YNum, int XSize, int YSize, int *HandleBuf, string ErrorMes , int RetryTime, int NotUse3DFlag )
{
	int result=0,i=0;
	bool go=false;

	do{
		i++;
		if(i>RetryTime)
		{
			string tmp=ErrorMes; tmp.append(FileName);
			int result=
				MessageBox(GetMainWindowHandle(),tmp.c_str(),"エラーが発生しました",MB_RETRYCANCEL | MB_ICONWARNING);
			if(result==IDCANCEL)	exit(0);
			else					{ go=false; i=0; result=0; }
		}

		result=LoadDivGraph(FileName.c_str(),AllNum,XNum,YNum,XSize,YSize,HandleBuf,NotUse3DFlag);
		for(int i=0; i<AllNum; i++)	go=go&&(HandleBuf[i]==-1);

	}while(go);

	return result;
}