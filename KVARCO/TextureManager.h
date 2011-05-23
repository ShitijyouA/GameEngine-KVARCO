#pragma once

#if 1

template<typename ParamType>
struct TextureParam
{
	typedef ParamType Type;

	Type angle_;

	Type scale_x_;
	Type scale_y_;

	short alpha_;	//0<=alpha_<256
};

//WarningƒNƒ‰ƒX‚à“‡
class TextureManager
{
	boost::unordered_map<>;
public:
	void Exists();
	void Load();
	void LoadFromMemory();
	
	DWORD GetHandle();
	lRect GetSize();
};

#endif