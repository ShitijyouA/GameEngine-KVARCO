#include "pch.h"
#include "Game.h"

#include "InitFromIniFile.h"

GameBootSetting kvarco::InitFromIniFile(const fsys::path& path)
{
	GameBootSetting res;
	kvarco::detail::SimpleIniPropertyWrapper_impl<GameBootSetting> wrapper(path,res);

#define SET_MEMBER(member_name,data_name)\
	wrapper.SetMember(&GameBootSetting::member_name,#data_name)

	SET_MEMBER		(Title,			WindowSetting.Title			);
	SET_MEMBER		(UseIcon,		WindowSetting.UseIcon		);
	SET_MEMBER		(FullScreen,	WindowSetting.FullScreen	);
	SET_MEMBER		(WndWidth,		WindowSetting.WndWidth		);
	SET_MEMBER		(WndHeight,		WindowSetting.WndHeight		);

	SET_MEMBER		(RockFPS,		SystemSetting.RockFPS		);
	SET_MEMBER		(AlwaysRun,		SystemSetting.AlwaysRun		);
	SET_MEMBER		(UpPG_Priority,	SystemSetting.UpPG_Priority	);
	SET_MEMBER		(BGM_Volume,	ScriptSetting.BGM_Volume	);
	SET_MEMBER		(SE_Volume,		ScriptSetting.SE_Volume		);

	SET_MEMBER		(LoadFileList,	ScriptSetting.LoadFileList	);

	return res;

#undef SET_MEMBER
#undef SET_MEMBER_XTYPE
}
