#pragma once
#include "KVARCO.h"
#include "Config.h"
#include "Texture.h"
#include "TextureManager.h"

#ifndef USE_WCHAR
#	include <boost/array.hpp>
#endif

//this class run faster if USE_WCHAR hadn't defined
class Font
{
public:
	typedef TextureManager::TexturePtr		TextureType;
	typedef std::vector<const TextureType>	CharArray;
	typedef lPoint*							lPointPtr;
	typedef boost::optional<lSize>			lSize_o;

#ifndef USE_WCHAR
	typedef boost::array<TextureType,0xFF>	FontGraphArray;
#else
	typedef std::vector<Texture>			FontGraphArray
#endif

protected:
	FontGraphArray Chars;
	void LoadFontGraph(TextureType texture,const std::string& set_string,const lPoint& offset,lSize_o size);

	lSize FontSize;
	DWORD GetLineNum(const std::string& string_);
	DWORD GetMaxCharNumInLine(const std::string& string_);

	static const std::string StdSetString;
	static const lPoint StdOffset;

public:
	Font() {}
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
