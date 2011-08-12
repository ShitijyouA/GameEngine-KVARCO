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
	typedef TextureManager::TexturePtr	TextureType;
	typedef std::vector<TextureType>	CharArray;
	typedef lPoint*						lPointPtr;
	typedef boost::optional<lSize>		lSize_o;

#ifndef USE_WCHAR
	typedef char									CharType;
	typedef std::string								StringType;
	typedef boost::array<TextureType,0xFF>			FontGraphArray;
#else
	typedef wchar_t									CharType;
	typedef std::wstring							StringType;
	typedef boost::unordered_map<CharType,Texture>	FontGraphArray
#endif

protected:
	FontGraphArray Chars;
	void LoadFontGraph(TextureType texture,const StringType& set_string,const lPoint& offset,lSize_o size);

	fSize FontSize;
	DWORD GetLineNum(const StringType& string_);
	DWORD GetMaxCharNumInLine(const StringType& string_);

public:
	static const StringType StdSetString;
	static const lPoint StdOffset;

	Font() {}
	Font(std::string& set_gr_name);
	Font(std::string& set_gr_name,StringType& set_string);
	Font(std::string& set_gr_name,StringType& set_string,lSize& size);
	Font(std::string& set_gr_name,StringType& set_string,lPoint& offset);
	Font(std::string& set_gr_name,StringType& set_string,lPoint& offset,lSize& size);

	virtual ~Font();

	//â¸çsÇÕ-1Ç≈ï\Ç∑
	inline CharArray ToStringInFont(const StringType& string_);
	void DrawInFont(long x,long y,const StringType& string_);
};

class FontX : public Font
{
public:
	NAME_IN_X(Font)(xtal::StringPtr set_gr_name,lSizePtrX size,lPointPtrX offset,xtal::StringPtr set_string);
	xtal::ArrayPtr NAME_IN_X(ToStringInFont)(xtal::StringPtr string_);
	void NAME_IN_X(DrawInFont)(long x,long y,xtal::StringPtr string_);
	~NAME_IN_X(Font)();

	static void bind(const xtal::ClassPtr it);
};
