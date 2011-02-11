#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <xtal_lib/xtal_cstdiostream.h>
#include <xtal_lib/xtal_winthread.h>
#include <xtal_lib/xtal_winfilesystem.h>
#include <xtal_lib/xtal_chcode.h>
#include <xtal_lib/xtal_errormessage.h>
#include "ScManager.h"

namespace XtalHelper
{
	extern void Init();
	extern void processExcept(const xtal::StringPtr& errorHeader, bool* pIsFailed, bool* pIsRetry);

/*
  Any::call, Any::sendのラッパ(参考ソース : by @sukai1989)
*/

	extern xtal::AnyPtr send(const xtal::AnyPtr& p,const xtal::IDPtr& primary_key);
	extern xtal::AnyPtr call(const xtal::AnyPtr& p);

	template<class A0 >
	extern xtal::AnyPtr send(const xtal::AnyPtr& p,const xtal::IDPtr& primary_key, const A0& a0  );

	template<class A0 >
	extern xtal::AnyPtr call(const xtal::AnyPtr& p,const A0& a0  );

	template<class A0 , class A1>
	extern xtal::AnyPtr send(const xtal::AnyPtr& p,const xtal::IDPtr& primary_key, const A0& a0  , const A1& a1);

	template<class A0 , class A1>
	extern xtal::AnyPtr call(const xtal::AnyPtr& p,const A0& a0  , const A1& a1);

	template<class A0 , class A1, class A2>
	extern xtal::AnyPtr send(const xtal::AnyPtr& p,const xtal::IDPtr& primary_key, const A0& a0  , const A1& a1, const A2& a2);

	template<class A0 , class A1, class A2>
	extern xtal::AnyPtr call(const xtal::AnyPtr& p,const A0& a0  , const A1& a1, const A2& a2);

	template<class A0 , class A1, class A2, class A3>
	extern xtal::AnyPtr send(const xtal::AnyPtr& p,const xtal::IDPtr& primary_key, const A0& a0  , const A1& a1, const A2& a2, const A3& a3);

	template<class A0 , class A1, class A2, class A3>
	extern xtal::AnyPtr call(const xtal::AnyPtr& p,const A0& a0  , const A1& a1, const A2& a2, const A3& a3);

	template<class A0 , class A1, class A2, class A3, class A4>
	extern xtal::AnyPtr send(const xtal::AnyPtr& p,const xtal::IDPtr& primary_key, const A0& a0  , const A1& a1, const A2& a2, const A3& a3, const A4& a4);

	template<class A0 , class A1, class A2, class A3, class A4>
	extern xtal::AnyPtr call(const xtal::AnyPtr& p,const A0& a0  , const A1& a1, const A2& a2, const A3& a3, const A4& a4);

	template<class A0 , class A1, class A2, class A3, class A4, class A5>
	extern xtal::AnyPtr send(const xtal::AnyPtr& p,const xtal::IDPtr& primary_key, const A0& a0  , const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5);

	template<class A0 , class A1, class A2, class A3, class A4, class A5>
	extern 	xtal::AnyPtr call(const xtal::AnyPtr& p,const A0& a0  , const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5);

	template<class A0 , class A1, class A2, class A3, class A4, class A5, class A6>
	extern xtal::AnyPtr send(const xtal::AnyPtr& p,const xtal::IDPtr& primary_key, const A0& a0  , const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6);

	template<class A0 , class A1, class A2, class A3, class A4, class A5, class A6>
	extern xtal::AnyPtr call(const xtal::AnyPtr& p,const A0& a0  , const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6);

	template<class A0 , class A1, class A2, class A3, class A4, class A5, class A6, class A7>
	extern xtal::AnyPtr send(const xtal::AnyPtr& p,const xtal::IDPtr& primary_key, const A0& a0  , const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7);

	template<class A0 , class A1, class A2, class A3, class A4, class A5, class A6, class A7>
	extern xtal::AnyPtr call(const xtal::AnyPtr& p,const A0& a0  , const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7);
/*
	template<class A0 , class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
	extern xtal::AnyPtr send(const xtal::AnyPtr& p,const xtal::IDPtr& primary_key, const A0& a0  , const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8);

	template<class A0 , class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
	extern xtal::AnyPtr call(const xtal::AnyPtr& p,const A0& a0  , const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8);

	template<class A0 , class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9>
	extern xtal::AnyPtr send(const xtal::AnyPtr& p,const xtal::IDPtr& primary_key, const A0& a0  , const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8, const A9& a9);

	template<class A0 , class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9>
	extern xtal::AnyPtr call(const xtal::AnyPtr& p,const A0& a0  , const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8, const A9& a9);

	template<class A0 , class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10>
	extern xtal::AnyPtr send(const xtal::AnyPtr& p,const xtal::IDPtr& primary_key, const A0& a0  , const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8, const A9& a9, const A10& a10);

	template<class A0 , class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10>
	extern xtal::AnyPtr call(const xtal::AnyPtr& p,const A0& a0  , const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8, const A9& a9, const A10& a10);

	template<class A0 , class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11>
	extern xtal::AnyPtr send(const xtal::AnyPtr& p,const xtal::IDPtr& primary_key, const A0& a0  , const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8, const A9& a9, const A10& a10, const A11& a11);

	template<class A0 , class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11>
	extern xtal::AnyPtr call(const xtal::AnyPtr& p,const A0& a0  , const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8, const A9& a9, const A10& a10, const A11& a11);

	template<class A0 , class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12>
	extern xtal::AnyPtr send(const xtal::AnyPtr& p,const xtal::IDPtr& primary_key, const A0& a0  , const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8, const A9& a9, const A10& a10, const A11& a11, const A12& a12);

	template<class A0 , class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12>
	extern xtal::AnyPtr call(const xtal::AnyPtr& p,const A0& a0  , const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8, const A9& a9, const A10& a10, const A11& a11, const A12& a12);

	template<class A0 , class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13>
	extern xtal::AnyPtr send(const xtal::AnyPtr& p,const xtal::IDPtr& primary_key, const A0& a0  , const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8, const A9& a9, const A10& a10, const A11& a11, const A12& a12, const A13& a13);

	template<class A0 , class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13>
	extern xtal::AnyPtr call(const xtal::AnyPtr& p,const A0& a0  , const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8, const A9& a9, const A10& a10, const A11& a11, const A12& a12, const A13& a13);
*/
}