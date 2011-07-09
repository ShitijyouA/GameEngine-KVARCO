#pragma once
#include "Config.h"

//矩形を表す構造体
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
typedef tRect<float>	fRect;
typedef xtal::SmartPtr<lRect>	lRectPtrX;
typedef xtal::SmartPtr<fRect>	fRectPtrX;

//座標を表す構造体
template<typename NUM=long>
struct tPoint
{
	NUM	x;
	NUM	y;
	tPoint(NUM x_=0,NUM y_=0)
		:x(x_),y(y_)
	{}
};
typedef tPoint<long>	lPoint;
typedef tPoint<float>	fPoint;
typedef xtal::SmartPtr<lPoint>	lPointPtrX;
typedef xtal::SmartPtr<fPoint>	fPointPtrX;

//三角関数の値を保存するための構造体
struct TriFunc
{
	float sin;
	float cos;

	TriFunc(float s=0.0,float c=0.0)
		:sin(s),cos(c)
	{}
};

//サイズ(縦横)を表す構造体
template<typename NUM=long>
struct tSize
{
	NUM width_;
	NUM height_;

	tSize(NUM w=0,NUM h=0)
		:width_(w),height_(h)
	{}
};

typedef tSize<long>		lSize;
typedef tSize<float>	dSize;
typedef xtal::SmartPtr<lSize>	lSizePtrX;
typedef xtal::SmartPtr<dSize>	dSizePtrX;

//degree_を表す構造体
template<typename NUM=float>
class tdegree_
{
	typedef NUM Type;

	static const Type DegreeToRadian;
	static const Type RadianToDegree;

	Type degree_;
public:

	tdegree_()
		:degree_(0.0)
	{}

	Type SetRadian(const Type rad)
		{
			degree_=rad*RadianToDegree;
		}

	Type SetDegree(const Type deg)
		{
			degree_=deg;
		}

	Type GetAsDegree() const
		{
			return degree_;
		}

	Type GetAsRadian() const
		{
			return degree_*DegreeToRadian;
		}

	Type GetAngle()
		{
		#ifdef USE_RADIAN_AS_DEFAULT
			return GetAsRadian();
		#else
			return GetAsDegree();
		#endif
		}
};

typedef tDegree<float>			Degree;
typedef xtal::SmartPtr<Degree>	DegreePtrX;

//Radianを表す構造体
template<typename NUM=float>
class tRadian
{
	typedef NUM Type;

	static const Type DegreeToRadian;
	static const Type RadianToDegree;

	Type Radian;
public:

	tRadian()
		:Radian(0.0)
	{}

	Type SetRadian(const Type rad)
		{
			Radian=rad;
		}

	Type Setdegree_(const Type deg)
		{
			Radian=deg*DegreeToRadian;
		}

	Type GetAsDegree() const
		{
			return Radian*RadianToDegree;
		}

	Type GetAsRadian() const
		{
			return Radian;
		}

	Type GetAngle()
		{
		#ifdef USE_RADIAN_AS_DEFAULT
			return GetAsRadian();
		#else
			return GetAsDegree();
		#endif
		}
};

typedef tRadian<float>			Radian;
typedef xtal::SmartPtr<Radian>	RadianPtrX;
