#include "pch.h"
#include "Game.h"
#include <boost/lexical_cast.hpp>

CGameBootSetting KVARCO::InitFromIniFile(string path)
{
	CGameBootSetting res;
	char tmp[50];

	try
	{
		memset(tmp,0,sizeof(tmp)); GetPrivateProfileString("WindowSetting","Title"			,"Game"	,tmp,sizeof(tmp),path.c_str());
		res.Title			=tmp;
		memset(tmp,0,sizeof(tmp)); GetPrivateProfileString("WindowSetting","UseIcon"		,"1"	,tmp,sizeof(tmp),path.c_str());
		res.UseIcon			=lexical_cast<int>(tmp);
		memset(tmp,0,sizeof(tmp)); GetPrivateProfileString("WindowSetting","FullScreen"		,"1"	,tmp,sizeof(tmp),path.c_str());
		res.FullScreen		=lexical_cast<int>(tmp)!=0;
		memset(tmp,0,sizeof(tmp)); GetPrivateProfileString("WindowSetting","WndWidth"		,"640"	,tmp,sizeof(tmp),path.c_str());
		res.WndWidth		=lexical_cast<int>(tmp);
		memset(tmp,0,sizeof(tmp)); GetPrivateProfileString("WindowSetting","WndHeight"		,"480"	,tmp,sizeof(tmp),path.c_str());
		res.WndHeight		=lexical_cast<int>(tmp);

		memset(tmp,0,sizeof(tmp)); GetPrivateProfileString("SystemSetting","RockFPS"		,"0"	,tmp,sizeof(tmp),path.c_str());
		res.RockFPS			=lexical_cast<int>(tmp)!=0;
		memset(tmp,0,sizeof(tmp)); GetPrivateProfileString("SystemSetting","AlwaysRun"		,"1"	,tmp,sizeof(tmp),path.c_str());
		res.AlwaysRun		=lexical_cast<int>(tmp)!=0;
		memset(tmp,0,sizeof(tmp)); GetPrivateProfileString("SystemSetting","UpPG_Priority"	,"0"	,tmp,sizeof(tmp),path.c_str());
		res.UpPG_Priority	=lexical_cast<int>(tmp)!=0;

		memset(tmp,0,sizeof(tmp)); GetPrivateProfileString("ScriptSetting","LoadFileList"	,"LoadFileList.xtal"	,tmp,sizeof(tmp),path.c_str());
		res.LoadFileList	=tmp;
	}
	catch(bad_lexical_cast e)
	{
		exit(-'b');
	}

	return res;
}