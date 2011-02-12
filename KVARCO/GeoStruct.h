#pragma once

//矩形を保存するための構造体
template<typename NUM=long>
class tRECT
{
public:
	NUM	left;
	NUM	top;
	NUM	right;
	NUM	bottom;

	tRECT(NUM left_=0,NUM top_=0,NUM right_=0,NUM bottom_=0)
		:left(left_),top(top_),
		right(right_),bottom(bottom_)
	{}
};
typedef tRECT<long>		lRECT;
typedef tRECT<float>	dRECT;
typedef xtal::SmartPtr<lRECT>	lRECTPtr;
typedef xtal::SmartPtr<dRECT>	dRECTPtr;

//座標を保存するための構造体
template<typename NUM=long>
class tPOINT
{
public:
	NUM	x;
	NUM	y;
	tPOINT(NUM x_=0,NUM y_=0)
		:x(x_),y(y_)
	{}
};
typedef tPOINT<long>	lPOINT;
typedef tPOINT<float>	dPOINT;
typedef xtal::SmartPtr<lPOINT>	lPOINTPtr;
typedef xtal::SmartPtr<dPOINT>	dPOINTPtr;

//三角関数の値を保存するための構造体
class TriFunc
{
public:
	float sin;
	float cos;

	TriFunc(float s=0.0,float c=0.0)
		:sin(s),cos(c)
	{}
};