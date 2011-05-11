#include "pch.h"
#include "Game.h"

#include "InitFromIniFile.h"

GameBootSetting kvarco::InitFromIniFile(const fsys::path& path)
{
	GameBootSetting res;
	kvarco::detail::SimpleIniPropertyWrapper_impl<GameBootSetting> wrapper(path,res);

#define SET_MEMBER(member_name,data_name)\
	wrapper.SetMember(&GameBootSetting::member_name,#data_name)

#define SET_MEMBER_XTYPE(member_name,data_name,value_type)\
	wrapper.SetMemberXtype<value_type>(&GameBootSetting::member_name,#data_name)

	boost::optional<std::string> temp;

	//TitleÇÕoperator>>ÇéùÇΩÇ»Ç¢ÇÃÇ≈ì¡ï 
	temp=wrapper.GetPropertyTree().get_optional<std::string>("WindowSetting.Title");
	if(temp.is_initialized()) res.Title=temp->c_str();

	//
	SET_MEMBER		(UseIcon,		WindowSetting.UseIcon		);
	SET_MEMBER		(FullScreen,	WindowSetting.FullScreen	);
	SET_MEMBER		(WndWidth,		WindowSetting.WndWidth		);
	SET_MEMBER		(WndHeight,		WindowSetting.WndHeight		);

	SET_MEMBER		(RockFPS,		SystemSetting.RockFPS		);
	SET_MEMBER		(AlwaysRun,		SystemSetting.AlwaysRun		);
	SET_MEMBER		(UpPG_Priority,	SystemSetting.UpPG_Priority	);

	SET_MEMBER		(LoadFileList,	ScriptSetting.LoadFileList	);

	return res;

#undef SET_MEMBER
#undef SET_MEMBER_XTYPE
}
