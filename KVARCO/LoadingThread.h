#pragma once
#include "KVARCO.h"
#include "LoadItem.h"
typedef xtal::SmartPtr<LoadItem::BaseLoadItem>	LoadBasePtrX;
typedef std::vector<LoadBasePtrX>					CLoadBase_v;

//xtal::AnyPtr(CLoadItem入りxtal::array)からvector<CLoadItem_std>へ
extern CLoadBase_v& AnyPtrToVector_CloadItem(xtal::AnyPtr load_list);

class CLoadingThread
{
	CLoadBase_v LoadFileList;

	void AnyPtrToVector_CloadItem(xtal::AnyPtr load_list);
public:
	
	CLoadingThread(xtal::AnyPtr load_list);
	virtual ~CLoadingThread();

	void DataLoad();

	typedef boost::shared_ptr<boost::thread> thread_ptr;
	static thread_ptr ThisThread;
	static bool IsEnded();
	static void Release();
};