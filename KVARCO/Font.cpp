#include "pch.h"
#include "Font.h"
const string CFont::StdSetString=" !\"#$%&\'()*+,-./012\n3456789:;<=>?@ABCDE\nFGHIJKLMNOPQRSTUVWX\nYZ[\\]^_`abcdefghijk\nlmnopqrstuvwxyz{|}~";
const lPoint CFont::StdOffset(0,0);

void CFont::LoadFontGraph(const GR_INFO* set_gr_info,const string& set_string,const lPoint& offset,lSize_o size)
{
	DWORD line_num		=GetLineNum(set_string);
	DWORD max_char_num	=GetMaxCharNumInLine(set_string);

	lSize set_size;
	if(!size)
	{
#ifdef USE_SIZE_STRCT
		set_size.width	=set_gr_info.Size.width	-offset.x;
		set_size.height	=set_gr_info.Size.height-offset.y;
#else
		set_size.width	=set_gr_info->Size.right	-offset.x;
		set_size.height	=set_gr_info->Size.bottom-offset.y;
#endif
	}
	else
		FontSize=*size;

	FontSize.width	=set_size.width		/max_char_num;
	FontSize.height	=set_size.height	/line_num;

	DWORD char_num	=set_string.size();

	lPoint cut_offset;
	BOOST_FOREACH(const char& i,set_string)
	{
		if(i!='\n')
		{
			Chars[set_string[i]]=
				KVARCO::LoadCutGraph_H("",set_gr_info->GrHandle,cut_offset.x,cut_offset.y,FontSize.width,FontSize.height);

			cut_offset.x+=FontSize.width;
		}
		else
			cut_offset.y+=FontSize.height;
	}
}

DWORD CFont::GetLineNum(const string& string_)
{
	DWORD res=0;
	BOOST_FOREACH(const char& i,string_)
	{
		if(i=='\n') ++res;
	}
	return res;
}

DWORD CFont::GetMaxCharNumInLine(const string& string_)
{
	DWORD current_line_char_num	=0;
	DWORD max_char_num			=0;
	BOOST_FOREACH(const char& i,string_)
	{
		if(i=='\n')
		{
			max_char_num=max(current_line_char_num,max_char_num);
		}
		else
			++current_line_char_num;
	}
	return max_char_num;
}

CharArray CFont::ToStringInFont(const string& string_)
{
	CharArray res(string_.size());
	res.clear();	//�ǉ���append()�ɓ��ꂷ�邽��

	BOOST_FOREACH(const char& i,string_)
	{
		if(i=='\n') res.push_back(-1);
		else		res.push_back(Chars[i]);
	}

	return res;
}

void CFont::DrawInFont(long x,long y,const string& string_)
{
	lPoint draw_point(x,y);
	CharArray in_font=ToStringInFont(string_);
	BOOST_FOREACH(const char& i,string_)
	{
		if(i!=-1)
		{
			KVARCO::DrawGraph_H(i,draw_point.x,draw_point.y);
			draw_point.x+=FontSize.width;
		}
		else
			draw_point.y+=FontSize.height;
	}
}

CFont::CFont()
{
	BOOST_FOREACH(int& i,Chars) { i=-1; }
}

CFont::CFont(string& set_gr_name)
{
	CFont();
	LoadFontGraph(KVARCO::GetGrInfo_p(set_gr_name.c_str()),StdSetString,StdOffset,lSize_o());
}

CFont::CFont(string& set_gr_name,string& set_string)
{
	CFont();
	LoadFontGraph(KVARCO::GetGrInfo_p(set_gr_name.c_str()),set_string,StdOffset,lSize_o());
}

CFont::CFont(string& set_gr_name,string& set_string,lSize& size)
{
	CFont();
	LoadFontGraph(KVARCO::GetGrInfo_p(set_gr_name.c_str()),set_string,StdOffset,lSize_o(size));
}

CFont::CFont(string& set_gr_name,string& set_string,lPoint& offset)
{
	CFont();
	LoadFontGraph(KVARCO::GetGrInfo_p(set_gr_name.c_str()),set_string,offset,lSize_o());
}

CFont::CFont(string& set_gr_name,string& set_string,lPoint& offset,lSize& size)
{
	CFont();
	LoadFontGraph(KVARCO::GetGrInfo_p(set_gr_name.c_str()),set_string,offset,lSize_o(size));
}

//NAME_IN_X(CFont)
NAME_IN_X(CFont)::NAME_IN_X(CFont)(xtal::StringPtr set_gr_name,xtal::StringPtr set_string,lPointPtrX offset,lSizePtrX size)
{
	string tmp_str_set_string=set_gr_name->c_str();
	LoadFontGraph(KVARCO::GetGrInfo_p(set_gr_name->c_str()),tmp_str_set_string,*offset,lSize_o(*size));
}

xtal::ArrayPtr NAME_IN_X(CFont)::NAME_IN_X(ToStringInFont)(xtal::StringPtr string_)
{
	string tmp_str		=string_->c_str();
	CharArray tmp_array	=ToStringInFont(tmp_str);

	xtal::ArrayPtr res	=xtal::xnew<xtal::Array>(tmp_array.size());
	for(DWORD i=0; i<tmp_array.size(); ++i)
	{
		res->set_at(i,tmp_array[i]);
	}

	return res;
}

void NAME_IN_X(CFont)::NAME_IN_X(DrawInFont)(long x,long y,xtal::StringPtr string_)
{
	string tmp_str=string_->c_str();
	DrawInFont(x,y,tmp_str);
}

void NAME_IN_X(CFont)::bind(const xtal::ClassPtr it)
{
	USE_XDEFZ(NAME_IN_X(CFont));
	
	Xdef_method(NAME_IN_X(ToStringInFont));
	Xdef_method(NAME_IN_X(DrawInFont));
}