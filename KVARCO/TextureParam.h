#pragma once

template<typename ParamType>
struct TextureParam
{
	typedef ParamType Type;

	Type angle_;

	Type scale_x_;
	Type scale_y_;

	short alpha_;	//alpha_Å∏[0...256)

	static void bind(xtal::ClassPtr it)
	{
		USE_XDEFZ(TextureParam<Type>)

		Xdef_var(angle_);
		Xdef_var(scale_x_);
		Xdef_var(scale_y_);
		Xdef_var(alpha_);
	}
};

typedef TextureParam<float> TextureParam;