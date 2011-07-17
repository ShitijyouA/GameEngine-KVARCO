#pragma once
#include "Config.h"

//矩形を表す構造体
template<typename Num=long>
class tRect
{
public:
	typedef Num Type;
	Type	left_;
	Type	top_;
	Type	right_;
	Type	bottom_;

	tRect(Type left=0,Type top=0,Type right=0,Type bottom=0)
		:left_(left),top_(top),
		right_(right),bottom_(bottom)
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

//角度を表す構造体
template<typename NUM=float>
class tAngle
{
public:
	typedef NUM				Type;

protected:
	typedef tAngle<Type>	ThisType;

	const Type DEGREE_TO_RADIAN;
	const Type RADIAN_TO_DEGREE;

	friend class ThisType;
	//内部ではradian
	Type angle_;

public:
	tAngle(Type angle=0.0)
		:DEGREE_TO_RADIAN(0.017453292519943),RADIAN_TO_DEGREE(57.29577951308232)
	{
#ifdef USE_RADIAN_AS_DEFAULT
			SetAsRadian(angle);
#else
			SetAsDegree(angle);
#endif
	}
	
	void SetAsRadian(const Type rad)
		{
			angle_=rad;
		}

	void SetAsDegree(const Type deg)
		{
			angle_=deg*DEGREE_TO_RADIAN;
		}

	Type GetAsDegree() const
		{
			return angle_*RADIAN_TO_DEGREE;
		}

	Type GetAsRadian() const
		{
			return angle_;
		}

	virtual Type GetAngle()
		{
#ifdef USE_RADIAN_AS_DEFAULT
			return GetAsRadian();
#else
			return GetAsDegree();
#endif
		}

	void Normalize()
		{
			const Type pi2=math::constants::pi<Type>()*2;

			// return if normalized
			Type rad=GetAsRadian();
			if(rad>=0.0 && rad<pi2) return;

			int div=static_cast<int>(rad/pi2);
			rad-=div*pi2;

			if(rad<0.0) rad+=pi2;

			SetAsRadian(rad);
		}

	template<typename AngleRawType> void operator=(const tAngle<AngleRawType>& angle)  { angle_=angle.angle_; }
};

typedef tAngle<float>			Angle;
typedef xtal::SmartPtr<Angle>	AnglePtrX;

template<typename Num=float>
struct tRadian
	:public tAngle<Num>
{
	typedef tAngle<Type>	AngleType;
	typedef tRadian<Type>	ThisType;

	tRadian(Type radian=0.0)
		:tAngle<Type>()
		{
			SetAsRadian(radian);
		}

	template<typename AngleRawType> void operator+=(const tAngle<AngleRawType>& angle) { angle_+=angle.GetAsRadian(); }
	template<typename AngleRawType> void operator-=(const tAngle<AngleRawType>& angle) { angle_-=angle.GetAsRadian(); }
	template<typename AngleRawType> void operator*=(const tAngle<AngleRawType>& angle) { angle_*=angle.GetAsRadian(); }
	template<typename AngleRawType> void operator/=(const tAngle<AngleRawType>& angle) { angle_/=angle.GetAsRadian(); }
};
typedef tRadian<float>			Radian;
typedef xtal::SmartPtr<Radian>	RadianPtrX;


template<typename Num=float>
struct tDegree
	:public tAngle<Num>
{
	typedef tAngle<Type>	AngleType;
	typedef tDegree<Type>	ThisType;

	tDegree(Type degree=0.0)
		:tAngle<Type>()
		{
			SetAsDegree(degree);
		}

	template<typename AngleRawType> void operator+=(const tAngle<AngleRawType>& angle) { angle_+=angle.GetAsDegree(); }
	template<typename AngleRawType> void operator-=(const tAngle<AngleRawType>& angle) { angle_-=angle.GetAsDegree(); }
	template<typename AngleRawType> void operator*=(const tAngle<AngleRawType>& angle) { angle_*=angle.GetAsDegree(); }
	template<typename AngleRawType> void operator/=(const tAngle<AngleRawType>& angle) { angle_/=angle.GetAsDegree(); }
};
typedef tDegree<float>			Degree;
typedef xtal::SmartPtr<Degree>	DegreePtrX;
