#pragma once

#define RETRY	10
namespace Warning
{
	int Retry_LoadGraph(string FileName,string ErrorMes = "次の画像の読み込みに失敗しました:\n" , int RetryTime = RETRY,int NotUse3DFlag = FALSE ) ;
	int Retry_LoadDivGraph(string FileName, int AllNum, int XNum, int YNum, int XSize, int YSize, int* HandleBuf,string ErrorMes = "次の画像の読み込みに失敗しました:\n" , int RetryTime = RETRY , int NotUse3DFlag = FALSE ) ;
};