#pragma once
#include "KVARCO.h"
//#define USE_WCHAR

#ifndef USE_WCHAR
	#include <boost/array.hpp>
	typedef array<int,0xFF>		FontGraphArray;
#else
	typedef vector<wchar_t>		FontGraphArray
#endif
typedef vector<int>		CharArray;
typedef lPoint*			lPointPtr;
typedef optional<lSize>	lSize_o;

//this class run faster if USE_WCHAR hadn't defined
class CFont
{
protected:
	FontGraphArray Chars;
	void LoadFontGraph(const GR_INFO* set_gr_info,const string& set_string,const lPoint& offset,lSize_o size);

	lSize FontSize;
	DWORD GetLineNum(const string& string_);
	DWORD GetMaxCharNumInLine(const string& string_);

	static const string StdSetString;
	static const lPoint StdOffset;

public:
	CFont();
	CFont(string& set_gr_name);
	CFont(string& set_gr_name,string& set_string);
	CFont(string& set_gr_name,string& set_string,lSize& size);
	CFont(string& set_gr_name,string& set_string,lPoint& offset);
	CFont(string& set_gr_name,string& set_string,lPoint& offset,lSize& size);

	//â¸çsÇÕ-1Ç≈ï\Ç∑
	inline CharArray ToStringInFont(const string& string_);
	void DrawInFont(long x,long y,const string& string_);
};

class CFontX : public CFont
{
public:
	NAME_IN_X(CFont)(xtal::StringPtr set_gr_name,xtal::StringPtr set_string,lPointPtrX offset,lSizePtrX size);
	xtal::ArrayPtr NAME_IN_X(ToStringInFont)(xtal::StringPtr string_);
	void NAME_IN_X(DrawInFont)(long x,long y,xtal::StringPtr string_);

	static void bind(const xtal::ClassPtr it);
};