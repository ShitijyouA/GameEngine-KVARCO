#pragma once

#define RETRY	10
namespace Warning
{
	int Retry_LoadGraph(string FileName,string ErrorMes = "���̉摜�̓ǂݍ��݂Ɏ��s���܂���:\n" , int RetryTime = RETRY,int NotUse3DFlag = FALSE ) ;
	int Retry_LoadDivGraph(string FileName, int AllNum, int XNum, int YNum, int XSize, int YSize, int* HandleBuf,string ErrorMes = "���̉摜�̓ǂݍ��݂Ɏ��s���܂���:\n" , int RetryTime = RETRY , int NotUse3DFlag = FALSE ) ;
};