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
#define XDEF_CTOR0_USE_BIND_METHOD(class_name_)	\
	XTAL_PREBIND(class_name_)					\
	{											\
		it->def_ctor0<class_name_>();			\
	}											\
	XTAL_BIND(class_name_)						\
	{											\
		class_name_::bind(it);					\
	}

XDEF_CTOR0_USE_BIND_METHOD(BaseActor)
XDEF_CTOR0_USE_BIND_METHOD(BaseScene)

XTAL_PREBIND(ColPolygon)
{
	it->def_ctor1<ColPolygon,DWORD>()->param(1,Xid(poly_num),4);
}

XTAL_BIND(ColPolygon)
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

XTAL_PREBIND(Bezier)
{
	it->def_ctor3<Bezier,xtal::ArrayPtr,float,DWORD>()
		->param(3,Xid(max_),1024);
}

XTAL_BIND(Bezier)
{
	Bezier::bind(it);
}

//マネージャ系(Singleton)
XDEF_CTOR0_USE_BIND_METHOD(LayerManager)
XDEF_CTOR0_USE_BIND_METHOD(ScriptManager)
XDEF_CTOR0_USE_BIND_METHOD(AudioManager)
XDEF_CTOR0_USE_BIND_METHOD(SceneManager)
XDEF_CTOR0_USE_BIND_METHOD(ActorManager)
XDEF_CTOR0_USE_BIND_METHOD(ArchiveManager)

//各種構造体のバインド

//GameBootSetting
XTAL_PREBIND(GameBootSetting)
{
	it->def_ctor0<GameBootSetting>();
}

XTAL_BIND(GameBootSetting)
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

//LoadItem::GraphLoadItem
XTAL_PREBIND(LoadItem::GraphLoadItem)
{
	it->inherit(xtal::cpp_class<LoadItem::BaseLoadItem>());
	it->def_ctor2<LoadItem::GraphLoadItem,xtal::StringPtr,xtal::StringPtr>();
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

//fRect
XTAL_PREBIND(fRect)
{
	it->def_ctor4<fRect,float,float,float,float>()
		->param(1,Xid(left_),0.0)->param(2,Xid(top_),0.0)
		->param(3,Xid(right_),0.0)->param(4,Xid(bottom),0.0);
}

XTAL_BIND(fRect)
{
	Xdef_var(left);
	Xdef_var(top);
	Xdef_var(right);
	Xdef_var(bottom);
}

//fPoint
XTAL_PREBIND(fPoint)
{
	it->def_ctor2<fPoint,float,float>()
		->param(1,Xid(x_),0.0)->param(2,Xid(y_),0.0);
}

XTAL_BIND(fPoint)
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

//Font
XTAL_PREBIND(NAME_IN_X(Font))
{
	it->def_ctor4<NAME_IN_X(Font),xtal::StringPtr,xtal::StringPtr,lPointPtrX,lSizePtrX>();
}

XTAL_BIND(NAME_IN_X(Font))
{
	NAME_IN_X(Font)::bind(it);
}

//BGM_Item
XTAL_PREBIND(BGM_Item)
{
	it->inherit(xtal::cpp_class<AudioItem>());
	it->def_ctor3<BGM_Item,xtal::StringPtr,DWORD,bool>();
}

//SE_Item
XTAL_PREBIND(SE_Item)
{
	it->inherit(xtal::cpp_class<AudioItem>());
	it->def_ctor1<SE_Item,xtal::StringPtr>();
}

#define XTAL_BIND_CLASS(_class) xtal::lib()->def(Xid(_class),xtal::cpp_class<_class>())
void Game::bind()
{
	kvarco::bind();

	XTAL_BIND_CLASS(GameBootSetting);
	xtal::lib()->def(Xid(GraphLoadItem),xtal::cpp_class<LoadItem::GraphLoadItem>());
	XTAL_BIND_CLASS(lRect);
	XTAL_BIND_CLASS(lPoint);
	XTAL_BIND_CLASS(lSize);
	XTAL_BIND_CLASS(fRect);
	XTAL_BIND_CLASS(fPoint);
	XTAL_BIND_CLASS(dSize);
	XTAL_BIND_CLASS(TriFunc);
	XTAL_BIND_CLASS(ColPolygon);
	XTAL_BIND_CLASS(BaseScene);
	XTAL_BIND_CLASS(BGM_Item);
	XTAL_BIND_CLASS(SE_Item);
	XTAL_BIND_CLASS(NAME_IN_X(Font));

	//namespace Actor
	xtal::ClassPtr Actors=xtal::xnew<xtal::Class>();
	Actors->set_singleton();
	Actors->def(Xid(BaseActor),xtal::cpp_class<BaseActor>());
	xtal::global()->def(Xid(Actor),Actors);	//全部libに定義

	//singleton KeyCode
	//GE_KeyCodeのバインド
	xtal::ClassPtr KeyCode=xtal::xnew<xtal::Class>();
	KeyCode->set_singleton();
	kvarco::GE_KeyCode_bind(KeyCode);
	xtal::lib()->def(Xid(KeyCode),KeyCode);

	//Singletonのバインド
#define XTAL_BIND_SINGLETON(_class,_name)\
	XTAL_BIND_CLASS(_class);\
	xtal::lib()->def(Xid(_name),\
	xtal::SmartPtr<_class>(_class::GetInst(),xtal::undeleter));

	//ActorManager
	XTAL_BIND_SINGLETON(ActorManager,ActorMngr)

	//LayerManager
	XTAL_BIND_SINGLETON(LayerManager,LayerMngr)

	//ScriptManager
	XTAL_BIND_SINGLETON(ScriptManager,ScriptMngr)

	//SceneManager
	XTAL_BIND_SINGLETON(SceneManager,SceneMngr)

	//AudioManager
	XTAL_BIND_SINGLETON(AudioManager,AudioMngr)

	//ArchiveManager
	XTAL_BIND_SINGLETON(ArchiveManager,ArchiveMngr)

#undef XTAL_BIND_SINGLETON
}