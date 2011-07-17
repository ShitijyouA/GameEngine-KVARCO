#include "pch.h"
#include "Font.h"
#include "CharacterParam.h"
const std::string Font::StdSetString=" !\"#$%&\'()*+,-./012\n3456789:;<=>?@ABCDE\nFGHIJKLMNOPQRSTUVWX\nYZ[\\]^_`abcdefghijk\nlmnopqrstuvwxyz{|}~";
const lPoint Font::StdOffset(0,0);

void Font::LoadFontGraph(TextureType texture,const std::string& set_string,const lPoint& offset,lSize_o size)
{
	DWORD line_num		=GetLineNum(set_string);
	DWORD max_char_num	=GetMaxCharNumInLine(set_string);

	lSize set_size;
	if(!size)
	{
		set_size.width_		=texture->GetSize().width_ -offset.x;
		set_size.height_	=texture->GetSize().height_-offset.y;
	}
	else
		FontSize=*size;

	FontSize.width_	=texture->GetSize().width_/max_char_num;
	FontSize.height_=texture->GetSize().height_/line_num;

	DWORD char_num	=set_string.size();

	lPoint cut_offset;
	BOOST_FOREACH(const char& i,set_string)
	{
		if(i!='\n')
		{
			TextureManager::RectType rect(cut_offset.x,cut_offset.y,FontSize.width_,FontSize.height_);
			Chars[set_string[i]]=TextureManager::GetInst()->CutTexture(texture.get(),"",rect);

			cut_offset.x+=FontSize.width_;
		}
		else
			cut_offset.y+=FontSize.height_;
	}
}

DWORD Font::GetLineNum(const std::string& string_)
{
	DWORD res=0;
	BOOST_FOREACH(const char& i,string_)
	{
		if(i=='\n') ++res;
	}
	return res;
}

DWORD Font::GetMaxCharNumInLine(const std::string& string_)
{
	DWORD current_line_char_num	=0;
	DWORD max_char_num			=0;
	BOOST_FOREACH(const char& i,string_)
	{
		if(i=='\n')
			max_char_num=std::max(current_line_char_num,max_char_num);
		else
			++current_line_char_num;
	}
	return max_char_num;
}

Font::CharArray Font::ToStringInFont(const std::string& string_)
{
	CharArray res(string_.size());
	res.clear();	//追加をappend()に統一するため

	BOOST_FOREACH(const char& i,string_)
	{
		if(i=='\n') res.push_back(TextureType());
		else		res.push_back(Chars[i]);
	}

	return res;
}

void Font::DrawInFont(long x,long y,const std::string& string_)
{
	lPoint draw_point(x,y);
	CharArray in_font=ToStringInFont(string_);

	BOOST_FOREACH(TextureManager::TexturePtr& tex,in_font)
	{
		if(tex->Get()!=-1)
		{
			kvarco::detail::DrawGraphRaw(tex->Get(),draw_point.x,draw_point.y,true,false);
			draw_point.x+=FontSize.width_;
		}
		else
			draw_point.y+=FontSize.height_;
	}
}

Font::Font(std::string& set_gr_name)
{
	TextureManager::TexturePtr tmp(TextureManager::GetInst()->GetAsTexture(set_gr_name.c_str()).get());
	LoadFontGraph(tmp,StdSetString,StdOffset,lSize_o());
}

Font::Font(std::string& set_gr_name,std::string& set_string)
{
	TextureManager::TexturePtr tmp(TextureManager::GetInst()->GetAsTexture(set_gr_name.c_str()).get());
	LoadFontGraph(tmp,set_string,StdOffset,lSize_o());
}

Font::Font(std::string& set_gr_name,std::string& set_string,lSize& size)
{
	TextureManager::TexturePtr tmp(TextureManager::GetInst()->GetAsTexture(set_gr_name.c_str()).get());
	LoadFontGraph(tmp,set_string,StdOffset,lSize_o(size));
}

Font::Font(std::string& set_gr_name,std::string& set_string,lPoint& offset)
{
	TextureManager::TexturePtr tmp(TextureManager::GetInst()->GetAsTexture(set_gr_name.c_str()).get());
	LoadFontGraph(tmp,set_string,offset,lSize_o());
}

Font::Font(std::string& set_gr_name,std::string& set_string,lPoint& offset,lSize& size)
{
	TextureManager::TexturePtr tmp(TextureManager::GetInst()->GetAsTexture(set_gr_name.c_str()).get());
	LoadFontGraph(tmp,set_string,offset,lSize_o(size));
}

//NAME_IN_X(Font)
NAME_IN_X(Font)::NAME_IN_X(Font)(xtal::StringPtr set_gr_name,xtal::StringPtr set_string,lPointPtrX offset,lSizePtrX size)
{
	std::string tmp_str_set_string=set_gr_name->c_str();
	TextureManager::TexturePtr tmp(TextureManager::GetInst()->GetAsTexture(set_gr_name->c_str()).get());
	LoadFontGraph(tmp,tmp_str_set_string,*offset,lSize_o(*size));
}

xtal::ArrayPtr NAME_IN_X(Font)::NAME_IN_X(ToStringInFont)(xtal::StringPtr string_)
{
	std::string tmp_str	=string_->c_str();
	CharArray tmp_array	=ToStringInFont(tmp_str);

	xtal::ArrayPtr res	=xtal::xnew<xtal::Array>(tmp_array.size());
	for(DWORD i=0; i<tmp_array.size(); ++i)
	{
		res->set_at(i,&tmp_array[i]);
	}

	return res;
}

void NAME_IN_X(Font)::NAME_IN_X(DrawInFont)(long x,long y,xtal::StringPtr string_)
{
	std::string tmp_str=string_->c_str();
	DrawInFont(x,y,tmp_str);
}

void NAME_IN_X(Font)::bind(const xtal::ClassPtr it)
{
	USE_XDEFZ(NAME_IN_X(Font));
	
	Xdef_method(NAME_IN_X(ToStringInFont));
	Xdef_method(NAME_IN_X(DrawInFont));
}
