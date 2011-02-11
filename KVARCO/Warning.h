#pragma once

#define RETRY	10
namespace Warning
{
	int Retry_LoadGraph(string FileName,string ErrorMes = "Ÿ‚Ì‰æ‘œ‚Ì“Ç‚İ‚İ‚É¸”s‚µ‚Ü‚µ‚½:\n" , int RetryTime = RETRY,int NotUse3DFlag = FALSE ) ;
	int Retry_LoadDivGraph(string FileName, int AllNum, int XNum, int YNum, int XSize, int YSize, int* HandleBuf,string ErrorMes = "Ÿ‚Ì‰æ‘œ‚Ì“Ç‚İ‚İ‚É¸”s‚µ‚Ü‚µ‚½:\n" , int RetryTime = RETRY , int NotUse3DFlag = FALSE ) ;
};