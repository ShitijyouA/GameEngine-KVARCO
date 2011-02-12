#include "pch.h"
#include "Game.h"

#include "Utillity.h"
#include "Warning.h"
#include "Layer2.h"
#include "BaseScene.h"
#include "Collision.h"
#include "ActorManager.h"
#include "SceneManager.h"
#include "AudioManager.h"
#include "Bezier.h"

//通常のクラスバインド

XTAL_PREBIND(CBaseActor)
{
	it->def_ctor0<CBaseActor>();
}

XTAL_BIND(CBaseActor)
{
	CBaseActor::bind(it);
}

XTAL_PREBIND(CBaseScene)
{
	it->def_ctor0<CBaseScene>();
}

XTAL_BIND(CBaseScene)
{
	CBaseScene::bind(it);
}

XTAL_PREBIND(CColPolygon)
{
	it->def_ctor1<CColPolygon,DWORD>()->param(1,Xid(poly_num),4);
}

XTAL_BIND(CColPolygon)
{
	Xdef_method(SetBox);
	Xdef_method(SetPolygon);
	Xdef_method(SetPolygon2);
	Xdef_method(Movep);
	Xdef_method(Move);
	Xdef_method(Check);
	Xdef_method(Cancel);
	Xdef_method(DrawCollision);
}

XTAL_PREBIND(CBezier)
{
	it->def_ctor3<CBezier,xtal::ArrayPtr,float,DWORD>()
		->param(3,Xid(max_),1024);
}

XTAL_BIND(CBezier)
{
	CBezier::bind(it);
}

//マネージャ系(Singleton)
XTAL_PREBIND(CActorManager)
{
	it->def_ctor0<CActorManager>();
}

XTAL_PREBIND(CLayerManager)
{
	it->def_ctor0<CLayerManager>();
}

XTAL_PREBIND(CScriptManager)
{
	it->def_ctor0<CScriptManager>();
}

XTAL_PREBIND(CSceneManager)
{
	it->def_ctor0<CSceneManager>();
}

XTAL_PREBIND(CAudioManager)
{
	it->def_ctor0<CAudioManager>();
}

XTAL_BIND(CLayerManager)
{
	CLayerManager::bind(it);
}

XTAL_BIND(CScriptManager)
{
	CScriptManager::bind(it);
}

XTAL_BIND(CSceneManager)
{
	CSceneManager::bind(it);
}

XTAL_BIND(CActorManager)
{
	CActorManager::bind(it);
}

XTAL_BIND(CAudioManager)
{
	CAudioManager::bind(it);
}

//各種構造体のバインド
XTAL_PREBIND(CGameBootSetting)
{
	it->def_ctor0<CGameBootSetting>();
}

XTAL_BIND(CGameBootSetting)
{
	Xdef_var(Title);
	Xdef_var(UseIcon);
	Xdef_var(FullScreen);

	Xdef_var(WndWidth);
	Xdef_var(WndHeight);

	Xdef_var(RockFPS);
	Xdef_var(AlwaysRun);

	Xdef_var(UpPG_Priority);
}

XTAL_PREBIND(LoadItem::CGraphLoadItem)
{
	it->inherit(xtal::cpp_class<LoadItem::CBaseLoadItem>());
	it->def_ctor2<LoadItem::CGraphLoadItem,xtal::StringPtr,xtal::StringPtr>();
}

XTAL_PREBIND(lRECT)
{
	it->def_ctor4<lRECT,long,long,long,long>()
		->param(1,Xid(left_),0)	->param(2,Xid(top_),0)
		->param(3,Xid(right_),0)->param(4,Xid(bottom_),0);
}

XTAL_BIND(lRECT)
{
	Xdef_var(left);
	Xdef_var(top);
	Xdef_var(right);
	Xdef_var(bottom);
}

XTAL_PREBIND(lPOINT)
{
	it->def_ctor2<lPOINT,long,long>()
		->param(1,Xid(x_),0)->param(2,Xid(y_),0);
}

XTAL_BIND(lPOINT)
{
	Xdef_var(x);
	Xdef_var(y);
}

XTAL_PREBIND(dRECT)
{
	it->def_ctor4<dRECT,float,float,float,float>()
		->param(1,Xid(left_),0.0)->param(2,Xid(top_),0.0)
		->param(3,Xid(right_),0.0)->param(4,Xid(bottom),0.0);
}

XTAL_BIND(dRECT)
{
	Xdef_var(left);
	Xdef_var(top);
	Xdef_var(right);
	Xdef_var(bottom);
}

XTAL_PREBIND(dPOINT)
{
	it->def_ctor2<dPOINT,float,float>()
		->param(1,Xid(x_),0.0)->param(2,Xid(y_),0.0);
}

XTAL_BIND(dPOINT)
{
	Xdef_var(x);
	Xdef_var(y);
}

XTAL_PREBIND(TriFunc)
{
	it->def_ctor2<TriFunc,float,float>()
		->param(1,Xid(s),0.0)->param(2,Xid(c),0.0);
}

XTAL_BIND(TriFunc)
{
	Xdef_var(sin);
	Xdef_var(cos);
}

XTAL_PREBIND(CBGM_Item)
{
	it->inherit(xtal::cpp_class<CAudioItem>());
	it->def_ctor3<CBGM_Item,xtal::StringPtr,DWORD,bool>();
}

XTAL_PREBIND(CSE_Item)
{
	it->inherit(xtal::cpp_class<CAudioItem>());
	it->def_ctor1<CSE_Item,xtal::StringPtr>();
}

#define XTAL_BIND_CLASS_DEFNAME(_class) xtal::lib()->def(Xid(_class),xtal::cpp_class<_class>())
void CGame::bind()
{
	KVARCO::bind();

	XTAL_BIND_CLASS_DEFNAME(CGameBootSetting);
	xtal::lib()->def(Xid(GraphLoadItem),xtal::cpp_class<LoadItem::CGraphLoadItem>());
	XTAL_BIND_CLASS_DEFNAME(lRECT);
	XTAL_BIND_CLASS_DEFNAME(lPOINT);
	XTAL_BIND_CLASS_DEFNAME(dRECT);
	XTAL_BIND_CLASS_DEFNAME(dPOINT);
	XTAL_BIND_CLASS_DEFNAME(TriFunc);
	XTAL_BIND_CLASS_DEFNAME(CColPolygon);
	XTAL_BIND_CLASS_DEFNAME(CBaseScene);
	XTAL_BIND_CLASS_DEFNAME(CBGM_Item);
	XTAL_BIND_CLASS_DEFNAME(CSE_Item);

	//namespace Actor
	xtal::ClassPtr Actors=xtal::xnew<xtal::Class>();
	Actors->set_singleton();
	Actors->def(Xid(BaseActor),xtal::cpp_class<CBaseActor>());
	xtal::lib()->def(Xid(Actor),Actors);	//全部libに定義

	//singleton KeyCode
	//GE_KeyCodeのバインド
	xtal::ClassPtr KeyCode=xtal::xnew<xtal::Class>();
	KeyCode->set_singleton();
	KVARCO::GE_KeyCode_bind(KeyCode);
	xtal::lib()->def(Xid(KeyCode),KeyCode);

	//Singletonのバインド
#define XTAL_BIND_SINGLETON(_class,_name)\
	XTAL_BIND_CLASS_DEFNAME(_class);\
	xtal::lib()->def(Xid(_name),\
	xtal::SmartPtr<_class>(_class::GetInst(),xtal::undeleter));

	//CActorManager
	XTAL_BIND_SINGLETON(CActorManager,ActorMngr)

	//CLayerManager
	XTAL_BIND_SINGLETON(CLayerManager,LayerMngr)

	//CScriptManager
	XTAL_BIND_SINGLETON(CScriptManager,ScriptMngr)

	//CSceneManager
	XTAL_BIND_SINGLETON(CSceneManager,SceneMngr)

	//CAudioManager
	XTAL_BIND_SINGLETON(CAudioManager,AudioMngr)

#undef XTAL_BIND_SINGLETON
}