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
	void LoadFontGraph(const GrInfo* set_gr_info,const std::string& set_string,const lPoint& offset,lSize_o size);

	lSize FontSize;
	DWORD GetLineNum(const std::string& string_);
	DWORD GetMaxCharNumInLine(const std::string& string_);

	static const std::string StdSetString;
	static const lPoint StdOffset;

public:
	Font();
	Font(std::string& set_gr_name);
	Font(std::string& set_gr_name,std::string& set_string);
	Font(std::string& set_gr_name,std::string& set_string,lSize& size);
	Font(std::string& set_gr_name,std::string& set_string,lPoint& offset);
	Font(std::string& set_gr_name,std::string& set_string,lPoint& offset,lSize& size);

	//â¸çsÇÕ-1Ç≈ï\Ç∑
	inline CharArray ToStringInFont(const std::string& string_);
	void DrawInFont(long x,long y,const std::string& string_);
};

class FontX : public Font
{
public:
	NAME_IN_X(Font)(xtal::StringPtr set_gr_name,xtal::StringPtr set_string,lPointPtrX offset,lSizePtrX size);
	xtal::ArrayPtr NAME_IN_X(ToStringInFont)(xtal::StringPtr string_);
	void NAME_IN_X(DrawInFont)(long x,long y,xtal::StringPtr string_);

	static void bind(const xtal::ClassPtr it);
};
