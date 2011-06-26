#pragma once

#define RETRY	10
namespace Warning
{
	int Retry_LoadGraph
		(std::string FileName,std::string ErrorMes = "次の画像の読み込みに失敗しました:\n" , int RetryTime = RETRY,int NotUse3DFlag = FALSE ) ;
	int Retry_LoadDivGraph
		(std::string FileName, int AllNum, int XNum, int YNum, int XSize, int YSize, int* HandleBuf,std::string ErrorMes = "次の画像の読み込みに失敗しました:\n" , int RetryTime = RETRY , int NotUse3DFlag = FALSE ) ;
};