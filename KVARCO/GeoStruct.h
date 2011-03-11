#pragma once

//矩形を保存するための構造体
template<typename NUM=long>
class tRect
{
public:
	NUM	left;
	NUM	top;
	NUM	right;
	NUM	bottom;

	tRect(NUM left_=0,NUM top_=0,NUM right_=0,NUM bottom_=0)
		:left(left_),top(top_),
		right(right_),bottom(bottom_)
	{}
};
typedef tRect<long>		lRect;
typedef tRect<float>	dRect;
typedef xtal::SmartPtr<lRect>	lRectPtrX;
typedef xtal::SmartPtr<dRect>	dRectPtrX;

//座標を保存するための構造体
template<typename NUM=long>
class tPoint
{
public:
	NUM	x;
	NUM	y;
	tPoint(NUM x_=0,NUM y_=0)
		:x(x_),y(y_)
	{}
};
typedef tPoint<long>	lPoint;
typedef tPoint<float>	dPoint;
typedef xtal::SmartPtr<lPoint>	lPointPtrX;
typedef xtal::SmartPtr<dPoint>	dPointPtrX;

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

//サイズ(縦横)を示す構造体
template<typename NUM=long>
class tSize
{
public:
	NUM width;
	NUM height;

	tSize(NUM w=0,NUM h=0)
		:width(w),height(h)
	{}
};

typedef tSize<long>		lSize;
typedef tSize<float>	dSize;
typedef xtal::SmartPtr<lSize>	lSizePtrX;
typedef xtal::SmartPtr<dSize>	dSizePtrX;