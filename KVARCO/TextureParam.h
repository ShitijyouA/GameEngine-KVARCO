#pragma once
#include "GeoStruct.h"

template<typename ParamType>
struct BasicTextureParam
{
	typedef ParamType		Type;
	typedef tAngle<Type>	AngleType;
	typedef short			AlphaType;

	AngleType	angle_;

	Type		scale_x_;
	Type		scale_y_;

	AlphaType	alpha_;	//alpha_Å∏[0...256)

	bool		trans_;	//ìßñæï`âÊÇ∑ÇÈÇ©Ç«Ç§Ç©

	void AngleNormalize()
		{
			angle_.Normalize();
		}

	BasicTextureParam()
		:angle_(0),scale_x_(1),scale_y_(1),alpha_(0)
		{}
	
	static void bind(xtal::ClassPtr it)
		{
			USE_XDEFZ(BasicTextureParam<Type>);

			Xdef_var(angle_);
			Xdef_var(scale_x_);
			Xdef_var(scale_y_);
			Xdef_var(alpha_);
			Xdef_var(trans_);
		}
};

typedef BasicTextureParam<float> TextureParam;
