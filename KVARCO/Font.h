#pragma once
#include "KVARCO.h"
//#define USE_WCHAR

#ifndef USE_WCHAR
	#include <boost/array.hpp>
	typedef boost::array<int,0xFF>		FontGraphArray;
#else
	typedef std::vector<wchar_t>		FontGraphArray
#endif

typedef std::vector<int>		CharArray;
typedef lPoint*					lPointPtr;
typedef boost::optional<lSize>	lSize_o;

//this class run faster if USE_WCHAR hadn't defined
class Font
{
protected:
	FontGraphArray Chars;
	void LoadFontGraph(const GrInfo* set_gr_info,const string& set_string,const lPoint& offset,lSize_o size);

	lSize FontSize;
	DWORD GetLineNum(const string& string_);
	DWORD GetMaxCharNumInLine(const string& string_);

	static const string StdSetString;
	static const lPoint StdOffset;

public:
	Font();
	Font(string& set_gr_name);
	Font(string& set_gr_name,string& set_string);
	Font(string& set_gr_name,string& set_string,lSize& size);
	Font(string& set_gr_name,string& set_string,lPoint& offset);
	Font(string& set_gr_name,string& set_string,lPoint& offset,lSize& size);

	//â¸çsÇÕ-1Ç≈ï\Ç∑
	inline CharArray ToStringInFont(const string& string_);
	void DrawInFont(long x,long y,const string& string_);
};

class FontX : public Font
{
public:
	NAME_IN_X(Font)(xtal::StringPtr set_gr_name,xtal::StringPtr set_string,lPointPtrX offset,lSizePtrX size);
	xtal::ArrayPtr NAME_IN_X(ToStringInFont)(xtal::StringPtr string_);
	void NAME_IN_X(DrawInFont)(long x,long y,xtal::StringPtr string_);

	static void bind(const xtal::ClassPtr it);
};
