#pragma once

//Set%_Name()関数を作る。構造体は不可。実態なし
#ifndef SETTER_
#define SETTER_(_Type,_Name) void Set##_Name(const _Type x);
#endif
//Get%_Name()関数を作る。構造体は不可。実態なし
#ifndef GETTER_
#define GETTER_(_Type,_Name) _Type Get##_Name() const;
#endif

//SETTERとGETTERのまとめ。実態なし
#ifndef SET_GET_
#define SET_GET_(_Type,_Name)\
	SETTER_(_Type,_Name);\
	GETTER_(_Type,_Name);
#endif

//Set%_Name()関数を作る。構造体は不可
#ifndef SETTER
#define SETTER(_Type,_Name) void Set##_Name(const _Type x) { _Name=x; }
#endif
//Get%_Name()関数を作る。構造体は不可
#ifndef GETTER
#define GETTER(_Type,_Name) _Type Get##_Name() const { return _Name; }
#endif

//SETTERとGETTERのまとめ
#ifndef SET_GET
#define SET_GET(_Type,_Name)\
	SETTER(_Type,_Name)\
	GETTER(_Type,_Name)
#endif

//Xtalバインド用マクロ
#define USE_XDEFZ(class_)	typedef class_ Self
//あとはXtal標準のXdef_fun,Xdef_method_Xdef_varを使用

//singleton pattern
//Xtalにバインドしないclass
#define SINGLETON_PATTERN(_class,_class_ptr)\
private:\
	_class() { ;}\
public:\
	static _class_ptr GetInst();\
	void Release()	{ }

//Xtalにバインドするclass
//Release()は各自で用意する
//friendは使用するXtalによって違うので注意
//コンパイルエラーが起きた場合はこのfriend宣言を全て削除し、
//コンパイルエラーを参考に書きなおす
#define SINGLETON_PATTERNX(_class,_class_ptr)\
private:\
	friend xtal::SmartPtr<_class> xtal::xnew();\
	friend class xtal::SmartPtr<_class>;\
	friend struct xtal::XNew<_class>;\
	friend struct xtal::XXNew<_class,3>;\
	_class() { ;}\
public:\
	static _class_ptr GetInst();

//XtalとC++をできるだけ分離するためのバインド用マクロ

#define NAME_IN_X(name_)	name_##XTAL
#define FOR_XTAL_NAMESPACE	ForXtal
#define BEGIN_FOR_XTAL		namespace FOR_XTAL_NAMESPACE

#ifndef Xdef_funx
#define Xdef_funx(name_)	xtal::lib()->def(Xid(name_),xtal::fun(&NAME_IN_X(name_)))
#endif

#ifndef Xdef_methodx
#define Xdef_methodx(name_) it->def(Xid(name_),xtal::method(&Self::NAME_IN_X(name_)))
#endif

#ifndef Xdef_varx
#define Xdef_varx(name_)	it->def_var(Xid(name_),&Self::NAME_IN_X(name_))
#endif