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

//singleton pattern
//Xtalにバインドしないclass
#define SINGLETON_PATTERN(_class,_class_ptr)\
private:\
	_class() { ;}\
public:\
	static _class_ptr GetInst();\
	void Release()	{ }

#include "ForXtalBind.h"