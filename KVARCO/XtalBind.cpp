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
#include "Font.h"

//通常のクラスバインド
//コンストラクタが引数を取らず、bind関数が定義されているクラスで使える
#define XDEF_CTOR0_USE_BIND_METHOD(class_name_)		\
	XTAL_PREBIND(class_name_)						\
	{												\
		it->def_ctor0<class_name_>();				\
	}												\
	XTAL_BIND(class_name_)							\
	{												\
		class_name_::bind(it);						\
	}

XDEF_CTOR0_USE_BIND_METHOD(CBaseActor)
XDEF_CTOR0_USE_BIND_METHOD(CBaseScene)

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
XDEF_CTOR0_USE_BIND_METHOD(CLayerManager)
XDEF_CTOR0_USE_BIND_METHOD(CScriptManager)
XDEF_CTOR0_USE_BIND_METHOD(CAudioManager)
XDEF_CTOR0_USE_BIND_METHOD(CSceneManager)
XDEF_CTOR0_USE_BIND_METHOD(CActorManager)

//各種構造体のバインド

//CGameBootSetting
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

//LoadItem::CGraphLoadItem
XTAL_PREBIND(LoadItem::CGraphLoadItem)
{
	it->inherit(xtal::cpp_class<LoadItem::CBaseLoadItem>());
	it->def_ctor2<LoadItem::CGraphLoadItem,xtal::StringPtr,xtal::StringPtr>();
}

//lRect
XTAL_PREBIND(lRect)
{
	it->def_ctor4<lRect,long,long,long,long>()
		->param(1,Xid(left_),0)	->param(2,Xid(top_),0)
		->param(3,Xid(right_),0)->param(4,Xid(bottom_),0);
}

XTAL_BIND(lRect)
{
	Xdef_var(left);
	Xdef_var(top);
	Xdef_var(right);
	Xdef_var(bottom);
}

//lPoint
XTAL_PREBIND(lPoint)
{
	it->def_ctor2<lPoint,long,long>()
		->param(1,Xid(x_),0)->param(2,Xid(y_),0);
}

XTAL_BIND(lPoint)
{
	Xdef_var(x);
	Xdef_var(y);
}

//dRect
XTAL_PREBIND(dRect)
{
	it->def_ctor4<dRect,float,float,float,float>()
		->param(1,Xid(left_),0.0)->param(2,Xid(top_),0.0)
		->param(3,Xid(right_),0.0)->param(4,Xid(bottom),0.0);
}

XTAL_BIND(dRect)
{
	Xdef_var(left);
	Xdef_var(top);
	Xdef_var(right);
	Xdef_var(bottom);
}

//dPoint
XTAL_PREBIND(dPoint)
{
	it->def_ctor2<dPoint,float,float>()
		->param(1,Xid(x_),0.0)->param(2,Xid(y_),0.0);
}

XTAL_BIND(dPoint)
{
	Xdef_var(x);
	Xdef_var(y);
}

//TriFunc
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

//CFont
XTAL_PREBIND(NAME_IN_X(CFont))
{
	it->def_ctor4<NAME_IN_X(CFont),xtal::StringPtr,xtal::StringPtr,lPointPtrX,lSizePtrX>();
}

XTAL_BIND(NAME_IN_X(CFont))
{
	NAME_IN_X(CFont)::bind(it);
}

//CBGM_Item
XTAL_PREBIND(CBGM_Item)
{
	it->inherit(xtal::cpp_class<CAudioItem>());
	it->def_ctor3<CBGM_Item,xtal::StringPtr,DWORD,bool>();
}

//CSE_Item
XTAL_PREBIND(CSE_Item)
{
	it->inherit(xtal::cpp_class<CAudioItem>());
	it->def_ctor1<CSE_Item,xtal::StringPtr>();
}

#define XTAL_BIND_CLASS(_class) xtal::lib()->def(Xid(_class),xtal::cpp_class<_class>())
void CGame::bind()
{
	KVARCO::bind();

	XTAL_BIND_CLASS(CGameBootSetting);
	xtal::lib()->def(Xid(GraphLoadItem),xtal::cpp_class<LoadItem::CGraphLoadItem>());
	XTAL_BIND_CLASS(lRect);
	XTAL_BIND_CLASS(lPoint);
	XTAL_BIND_CLASS(lSize);
	XTAL_BIND_CLASS(dRect);
	XTAL_BIND_CLASS(dPoint);
	XTAL_BIND_CLASS(dSize);
	XTAL_BIND_CLASS(TriFunc);
	XTAL_BIND_CLASS(CColPolygon);
	XTAL_BIND_CLASS(CBaseScene);
	XTAL_BIND_CLASS(CBGM_Item);
	XTAL_BIND_CLASS(CSE_Item);
	XTAL_BIND_CLASS(NAME_IN_X(CFont));

	//namespace Actor
	xtal::ClassPtr Actors=xtal::xnew<xtal::Class>();
	Actors->set_singleton();
	Actors->def(Xid(BaseActor),xtal::cpp_class<CBaseActor>());
	xtal::global()->def(Xid(Actor),Actors);	//全部libに定義

	//singleton KeyCode
	//GE_KeyCodeのバインド
	xtal::ClassPtr KeyCode=xtal::xnew<xtal::Class>();
	KeyCode->set_singleton();
	KVARCO::GE_KeyCode_bind(KeyCode);
	xtal::lib()->def(Xid(KeyCode),KeyCode);

	//Singletonのバインド
#define XTAL_BIND_SINGLETON(_class,_name)\
	XTAL_BIND_CLASS(_class);\
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