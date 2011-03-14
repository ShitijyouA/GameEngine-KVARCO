#pragma once

//Xtalバインド用マクロ
#define USE_XDEFZ(class_)	typedef class_ Self
//あとはXtal標準のXdef_fun,Xdef_method_Xdef_varを使用

//Xtalにバインドするclass
//Release()は各自で用意する
//friendは使用するXtalによって違うので注意
//コンパイルエラーが起きた場合はこのfriend宣言を全て削除し、
//コンパイルエラーを参考に書きなおす
#define FRIENDS_FOR_XTAL(class_)\
	friend xtal::SmartPtr<class_> xtal::xnew();\
	friend class xtal::SmartPtr<class_>;\
	friend struct xtal::XNew<class_>;\
	friend struct xtal::XXNew<class_,3>;

#define SINGLETON_PATTERNX(class_,class_ptr_)\
private:\
	FRIENDS_FOR_XTAL(class_)\
	class_() { ;}\
public:\
	static class_ptr_ GetInst();

//XtalとC++をできるだけ分離するためのバインド用マクロ

#define NAME_IN_X(name_)	name_##X

#ifndef Xdef_funx
#define Xdef_funx(name_)	xtal::lib()->def(Xid(name_),xtal::fun(&NAME_IN_X(name_)))
#endif

#ifndef Xdef_methodx
#define Xdef_methodx(name_) it->def(Xid(name_),xtal::method(&Self::NAME_IN_X(name_)))
#endif

#ifndef Xdef_varx
#define Xdef_varx(name_)	it->def_var(Xid(name_),&Self::NAME_IN_X(name_))
#endif

#define CLASS_IN_X(class_name_) class NAME_IN_X(class_name_) : public class_name_