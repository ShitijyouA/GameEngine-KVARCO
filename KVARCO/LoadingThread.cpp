#include "pch.h"
#include "LoadingThread.h"
//#include <process.h>

shared_ptr<thread> CLoadingThread::ThisThread;

CLoadingThread::CLoadingThread(xtal::AnyPtr load_list)
{
	AnyPtrToVector_CloadItem(load_list);
	ThisThread=thread_ptr( new thread(bind(&CLoadingThread::DataLoad,&(*this))) );
}

void CLoadingThread::DataLoad()
{
	BOOST_FOREACH(CLoadBasePtr& i,LoadFileList)
	{
		i->Load();
	}
}

void CLoadingThread::Release()
{
	if(ThisThread)
	{
		ThisThread->join();
		ThisThread->interrupt();
		ThisThread.reset();
	}
}

CLoadingThread::~CLoadingThread()
{
	Release();
}

bool CLoadingThread::IsEnded()
{
	if(!ThisThread) return true;
	if(ThisThread->timed_join(boost::posix_time::milliseconds(0)))
	{
		ThisThread->interrupt();
		ThisThread.reset();
		return true;
	}
	return false;
}

//Xtal�ˑ�
void CLoadingThread::AnyPtrToVector_CloadItem(xtal::AnyPtr load_list)
{
	xtal::ArrayPtr xlist=load_list.to_a();
	if(xtal::is_undefined(xlist)) return;

	for(DWORD i=0; i<xlist->size(); i++)
	{
		//����ł����|�������Ȃ�SmartPtr�ɓ����
		xtal::SmartPtr<LoadItem::CBaseLoadItem> tmp=
			xtal::unchecked_ptr_cast<LoadItem::CBaseLoadItem>(xlist->at(i));
		LoadFileList.push_back(tmp);
	}
}