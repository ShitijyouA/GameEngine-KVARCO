#include "pch.h"
#include "XtalHelper.h"
#include <cassert>

using namespace xtal;

namespace XtalHelper
{
void Init()
{
	static xtal::CStdioStdStreamLib cstdStdStreamLib;
	static xtal::WinThreadLib winThreadLib;
	static xtal::WinFilesystemLib winFilesystemLib;
	static xtal::SJISChCodeLib chCodeLib;

	xtal::Setting setting;
	setting.thread_lib = &winThreadLib;
	setting.std_stream_lib = &cstdStdStreamLib;
	setting.filesystem_lib = &winFilesystemLib;
	setting.ch_code_lib = &chCodeLib;

	xtal::initialize(setting);
	xtal::bind_error_message();
}

/*
  デフォルトのエラーハンドラ
*/
bool defaultHandler(const xtal::StringPtr& errorMessage){
	OutputDebugString(errorMessage->c_str());
	int ret = MessageBox(NULL, errorMessage->c_str(), "Xtalエラー", MB_ABORTRETRYIGNORE);
	switch(ret){
		case IDABORT:	// 終了
			assert("中断します。");
			return false;
			break;
		case IDRETRY:	// 再試行
			ScriptManager::GetInst()->ReLoad();
			return true;
		case IDIGNORE:	// 無視
			return false;
		default:
			assert("いるはずがない。");
			break;
	}
	return false;
}

/*
  Xtalの例外を処理する
  例外が吐かれていたらエラーハンドラを呼び出す
*/
void processExcept(const xtal::StringPtr& errorHeader, bool* pIsFailed, bool* pIsRetry)
{
	*pIsFailed = false;
	XTAL_CATCH_EXCEPT(e){
		*pIsFailed = true;
		xtal::StringPtr errorMessage(errorHeader);
		errorMessage = errorMessage->cat(e->to_s())->cat("\n");
		*pIsRetry = defaultHandler(errorMessage);
	}
}
/*
xtal::AnyPtr send(const xtal::AnyPtr& p, const xtal::IDPtr& primary_key){
	xtal::AnyPtr pRet;
	bool isFailed = false, isRetry = false;
	do{
		pRet = p->send(primary_key);
		XtalHelper::processExcept(_T("Xtal runtime error on send\n"), &isFailed, &isRetry);
	} while (isFailed && isRetry);
	return pRet;
}
xtal::AnyPtr call(const xtal::AnyPtr& p){
	xtal::AnyPtr pRet;
	bool isFailed = false, isRetry = false;
	do{
		pRet = p->call();
		XtalHelper::processExcept(_T("Xtal runtime error on call\n"), &isFailed, &isRetry);
	} while (isFailed && isRetry);
	return pRet;
}
*/
	AnyPtr send(const AnyPtr& p,const IDPtr& primary_key)
	{
		xtal::AnyPtr res;
		bool isFailed = false, isRetry = true;
		do{
			res = p->send(primary_key);
			XtalHelper::processExcept(_T("Xtal runtime error on send\n"), &isFailed, &isRetry);
		} while (isFailed && isRetry);
		return res;
	}

	AnyPtr call(const AnyPtr& p)
	{
		xtal::AnyPtr res;
		bool isFailed = false, isRetry = false;
		do{
			res = p->call();
			XtalHelper::processExcept(_T("Xtal runtime error on call\n"), &isFailed, &isRetry);
		} while (isFailed && isRetry);
		return res;
	}

	template<class A0 >
	extern AnyPtr send(const AnyPtr& p,const IDPtr& primary_key, const A0& a0  )
	{
		xtal::AnyPtr res;
		bool isFailed = false, isRetry = false;
		do{
			res = p->send(primary_key,a0);
			XtalHelper::processExcept(_T("Xtal runtime error on send\n"), &isFailed, &isRetry);
		} while (isFailed && isRetry);
		return res;
	}

	template<class A0 >
	extern AnyPtr call(const AnyPtr& p,const A0& a0  )
	{
		xtal::AnyPtr res;
		bool isFailed = false, isRetry = false;
		do{
			res = p->call(a0);
			XtalHelper::processExcept(_T("Xtal runtime error on call\n"), &isFailed, &isRetry);
		} while (isFailed && isRetry);
		return res;
	}


	template<class A0 , class A1>
	extern AnyPtr send(const AnyPtr& p,const IDPtr& primary_key, const A0& a0  , const A1& a1)
	{
		xtal::AnyPtr res;
		bool isFailed = false, isRetry = false;
		do{
			res = p->send(primary_key,a0,a1);
			XtalHelper::processExcept(_T("Xtal runtime error on send\n"), &isFailed, &isRetry);
		} while (isFailed && isRetry);
		return res;
	}

	template<class A0 , class A1>
	extern AnyPtr call(const AnyPtr& p,const A0& a0  , const A1& a1)
	{
		xtal::AnyPtr res;
		bool isFailed = false, isRetry = false;
		do{
			res = p->call(a0,a1);
			XtalHelper::processExcept(_T("Xtal runtime error on call\n"), &isFailed, &isRetry);
		} while (isFailed && isRetry);
		return res;
	}

	template<class A0 , class A1, class A2>
	extern AnyPtr send(const AnyPtr& p,const IDPtr& primary_key, const A0& a0  , const A1& a1, const A2& a2)
	{
		xtal::AnyPtr res;
		bool isFailed = false, isRetry = false;
		do{
			res = p->send(primary_key,a0,a1,a2);
			XtalHelper::processExcept(_T("Xtal runtime error on send\n"), &isFailed, &isRetry);
		} while (isFailed && isRetry);
		return res;
	}

	template<class A0 , class A1, class A2>
	extern AnyPtr call(const AnyPtr& p,const A0& a0  , const A1& a1, const A2& a2)
	{
		xtal::AnyPtr res;
		bool isFailed = false, isRetry = false;
		do{
			res = p->call(a0,a1,a2);
			XtalHelper::processExcept(_T("Xtal runtime error on call\n"), &isFailed, &isRetry);
		} while (isFailed && isRetry);
		return res;
	}

	template<class A0 , class A1, class A2, class A3>
	extern AnyPtr send(const AnyPtr& p,const IDPtr& primary_key, const A0& a0  , const A1& a1, const A2& a2, const A3& a3)
	{
		xtal::AnyPtr res;
		bool isFailed = false, isRetry = false;
		do{
			res = p->send(primary_key,a0,a1,a2,a3);
			XtalHelper::processExcept(_T("Xtal runtime error on send\n"), &isFailed, &isRetry);
		} while (isFailed && isRetry);
		return res;
	}

	template<class A0 , class A1, class A2, class A3>
	extern AnyPtr call(const AnyPtr& p,const A0& a0  , const A1& a1, const A2& a2, const A3& a3)
	{
		xtal::AnyPtr res;
		bool isFailed = false, isRetry = false;
		do{
			res = p->call(a0,a1,a2,a3);
			XtalHelper::processExcept(_T("Xtal runtime error on call\n"), &isFailed, &isRetry);
		} while (isFailed && isRetry);
		return res;
	}

	template<class A0 , class A1, class A2, class A3, class A4>
	extern AnyPtr send(const AnyPtr& p,const IDPtr& primary_key, const A0& a0  , const A1& a1, const A2& a2, const A3& a3, const A4& a4)
	{
		xtal::AnyPtr res;
		bool isFailed = false, isRetry = false;
		do{
			res = p->send(primary_key,a0,a1,a2,a3,a4);
			XtalHelper::processExcept(_T("Xtal runtime error on send\n"), &isFailed, &isRetry);
		} while (isFailed && isRetry);
		return res;
	}

	template<class A0 , class A1, class A2, class A3, class A4>
	extern AnyPtr call(const AnyPtr& p,const A0& a0  , const A1& a1, const A2& a2, const A3& a3, const A4& a4)
	{
		xtal::AnyPtr res;
		bool isFailed = false, isRetry = false;
		do{
			res = p->call(a0,a1,a2,a3,a4);
			XtalHelper::processExcept(_T("Xtal runtime error on call\n"), &isFailed, &isRetry);
		} while (isFailed && isRetry);
		return res;
	}

	template<class A0 , class A1, class A2, class A3, class A4, class A5>
	extern AnyPtr send(const AnyPtr& p,const IDPtr& primary_key, const A0& a0  , const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5)
	{
		xtal::AnyPtr res;
		bool isFailed = false, isRetry = false;
		do{
			res = p->send(primary_key,a0,a1,a2,a3,a5);
			XtalHelper::processExcept(_T("Xtal runtime error on send\n"), &isFailed, &isRetry);
		} while (isFailed && isRetry);
		return res;
	}

	template<class A0 , class A1, class A2, class A3, class A4, class A5>
	extern 	AnyPtr call(const AnyPtr& p,const A0& a0  , const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5)
	{
		xtal::AnyPtr res;
		bool isFailed = false, isRetry = false;
		do{
			res = p->call(a0,a1,a2,a3,a5);
			XtalHelper::processExcept(_T("Xtal runtime error on call\n"), &isFailed, &isRetry);
		} while (isFailed && isRetry);
		return res;
	}

	template<class A0 , class A1, class A2, class A3, class A4, class A5, class A6>
	extern AnyPtr send(const AnyPtr& p,const IDPtr& primary_key, const A0& a0  , const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6)
	{
		xtal::AnyPtr res;
		bool isFailed = false, isRetry = false;
		do{
			res = p->send(primary_key,a0,a1,a2,a3,a5,a6);
			XtalHelper::processExcept(_T("Xtal runtime error on send\n"), &isFailed, &isRetry);
		} while (isFailed && isRetry);
		return res;
	}

	template<class A0 , class A1, class A2, class A3, class A4, class A5, class A6>
	extern AnyPtr call(const AnyPtr& p,const A0& a0  , const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6)
	{
		xtal::AnyPtr res;
		bool isFailed = false, isRetry = false;
		do{
			res = p->call(a0,a1,a2,a3,a5,a6);
			XtalHelper::processExcept(_T("Xtal runtime error on call\n"), &isFailed, &isRetry);
		} while (isFailed && isRetry);
		return res;
	}

	template<class A0 , class A1, class A2, class A3, class A4, class A5, class A6, class A7>
	extern AnyPtr send(const AnyPtr& p,const IDPtr& primary_key, const A0& a0  , const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7)
	{
		xtal::AnyPtr res;
		bool isFailed = false, isRetry = false;
		do{
			res = p->send(primary_key,a0,a1,a2,a3,a5,a6,a7);
			XtalHelper::processExcept(_T("Xtal runtime error on send\n"), &isFailed, &isRetry);
		} while (isFailed && isRetry);
		return res;
	}

	template<class A0 , class A1, class A2, class A3, class A4, class A5, class A6, class A7>
	extern AnyPtr call(const AnyPtr& p,const A0& a0  , const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7)
	{
		xtal::AnyPtr res;
		bool isFailed = false, isRetry = false;
		do{
			res = p->call(a0,a1,a2,a3,a5,a6,a7);
			XtalHelper::processExcept(_T("Xtal runtime error on call\n"), &isFailed, &isRetry);
		} while (isFailed && isRetry);
		return res;
	}

/*
	template<class A0 , class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
	extern AnyPtr send(const AnyPtr& p,const IDPtr& primary_key, const A0& a0  , const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8);

	template<class A0 , class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
	extern AnyPtr call(const AnyPtr& p,const A0& a0  , const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8);

	template<class A0 , class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9>
	extern AnyPtr send(const AnyPtr& p,const IDPtr& primary_key, const A0& a0  , const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8, const A9& a9);

	template<class A0 , class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9>
	extern AnyPtr call(const AnyPtr& p,const A0& a0  , const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8, const A9& a9);

	template<class A0 , class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10>
	extern AnyPtr send(const AnyPtr& p,const IDPtr& primary_key, const A0& a0  , const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8, const A9& a9, const A10& a10);

	template<class A0 , class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10>
	extern AnyPtr call(const AnyPtr& p,const A0& a0  , const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8, const A9& a9, const A10& a10);

	template<class A0 , class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11>
	extern AnyPtr send(const AnyPtr& p,const IDPtr& primary_key, const A0& a0  , const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8, const A9& a9, const A10& a10, const A11& a11);

	template<class A0 , class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11>
	extern AnyPtr call(const AnyPtr& p,const A0& a0  , const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8, const A9& a9, const A10& a10, const A11& a11);

	template<class A0 , class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12>
	extern AnyPtr send(const AnyPtr& p,const IDPtr& primary_key, const A0& a0  , const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8, const A9& a9, const A10& a10, const A11& a11, const A12& a12);

	template<class A0 , class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12>
	extern AnyPtr call(const AnyPtr& p,const A0& a0  , const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8, const A9& a9, const A10& a10, const A11& a11, const A12& a12);

	template<class A0 , class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13>
	extern AnyPtr send(const AnyPtr& p,const IDPtr& primary_key, const A0& a0  , const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8, const A9& a9, const A10& a10, const A11& a11, const A12& a12, const A13& a13);

	template<class A0 , class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13>
	extern AnyPtr call(const AnyPtr& p,const A0& a0  , const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8, const A9& a9, const A10& a10, const A11& a11, const A12& a12, const A13& a13);
*/

}