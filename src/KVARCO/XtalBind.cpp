#include "pch.h"
#include "Game.h"

#include "Utillity.h"
#include "LayerManager.h"
#include "BaseScene.h"
#include "Collision.h"
#include "ActorManager.h"
#include "SceneManager.h"
#include "AudioManager.h"
#include "ArchiveManager.h"
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
	ColPolygon::bind(it);
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
XDEF_CTOR0_USE_BIND_METHOD(TextureManager)

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
	Xdef_var(left_);
	Xdef_var(top_);
	Xdef_var(right_);
	Xdef_var(bottom_);
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
		->param(1,Xid(left),0.0)->param(2,Xid(top),0.0)
		->param(3,Xid(right),0.0)->param(4,Xid(bottom),0.0);
}

XTAL_BIND(fRect)
{
	Xdef_var(left_);
	Xdef_var(top_);
	Xdef_var(right_);
	Xdef_var(bottom_);
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

//lSize
XTAL_PREBIND(lSize)
{
	it->def_ctor2<lSize,long,long>()
		->param(1,Xid(width_),0)->param(2,Xid(height_),0);
}

XTAL_BIND(lSize)
{
	Xdef_var(width_);
	Xdef_var(height_);
}

//fSize
XTAL_PREBIND(fSize)
{
	it->def_ctor2<fSize,float,float>()
		->param(1,Xid(width_),0)->param(2,Xid(height_),0);
}

XTAL_BIND(fSize)
{
	Xdef_var(width_);
	Xdef_var(height_);
}

//Angle
XTAL_PREBIND(Angle)
{
	it->def_ctor1<Angle,float>()->param(1,Xid(radian),0.0f);
}

XTAL_BIND(Angle)
{
	Xdef_method(SetAsRadian);
	Xdef_method(SetAsDegree);
	Xdef_method(GetAsDegree);
	Xdef_method(GetAsRadian);
	Xdef_method(GetAngle);
	Xdef_method(Normalize);
}

//Radian
XTAL_PREBIND(Radian)
{
	it->inherit(xtal::cpp_class<Angle>());
	it->def_ctor1<Radian,float>()->param(1,Xid(radian),0.0f);
}

//Degree
XTAL_PREBIND(Degree)
{
	it->inherit(xtal::cpp_class<Angle>());
	it->def_ctor1<Degree,float>()->param(1,Xid(degree),0.0f);
}

//Font
XTAL_PREBIND(NAME_IN_X(Font))
{
	it->def_ctor4<NAME_IN_X(Font),xtal::StringPtr,lSizePtrX,lPointPtrX,xtal::StringPtr>()
		->param(3,Xid(offset),&Font::StdOffset)
		->param(4,Xid(set_string),Font::StdSetString.c_str());
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

//concept::Texture<TextureInstType>
XTAL_PREBIND(texture::TextureConcept)
{}

//メンバがvirtual関数ばっかりなのでここで
//static関数内でvirtual関数参照できるの……?
XTAL_BIND(texture::TextureConcept)
{
	//Xdef_method(Get);
	//Xdef_method(GetSize);
	//Xdef_method(GetCenter);

	Xdef_method(AsTexture);
	Xdef_method(AsTextureSet);
	Xdef_method(AsAnimation);
}

//Texture
XTAL_PREBIND(texture::Texture)
{
	it->inherit(xtal::cpp_class<texture::TextureConcept>());
}

XTAL_BIND(texture::Texture)
{
	texture::Texture::bind(it);
}

//TextureSet
XTAL_PREBIND(texture::TextureSet)
{
	it->inherit(xtal::cpp_class<texture::TextureConcept>());
}

XTAL_BIND(texture::TextureSet)
{
	texture::TextureSet::bind(it);
}

//Animation
XTAL_PREBIND(texture::Animation)
{
	it->inherit(xtal::cpp_class<texture::TextureConcept>());
}

XTAL_BIND(texture::Animation)
{
	texture::Animation::bind(it);
}

//TextureParam
XTAL_PREBIND(TextureParam)
{
	it->def_ctor0<TextureParam>();
}

XTAL_BIND(TextureParam)
{
	TextureParam::bind(it);
}

//CharParam
XTAL_PREBIND(CharParam)
{
	it->inherit(xtal::cpp_class<TextureParam>());
	it->def_ctor0<CharParam>();
}

XTAL_BIND(CharParam)
{
	CharParam::bind(it);
}

#define Xdef_class_lib(_class)				xtal::lib()->def(Xid(_class),xtal::cpp_class<_class>())
#define Xdef_class_lib_alias(_name,_class)	xtal::lib()->def(Xid(_name),xtal::cpp_class<_class>())
void Game::bind()
{
	kvarco::bind();

	Xdef_class_lib(GameBootSetting);
	Xdef_class_lib_alias(GraphLoadItem,LoadItem::GraphLoadItem);
	
	Xdef_class_lib(lRect);
	Xdef_class_lib(lPoint);
	Xdef_class_lib(lSize);
	
	Xdef_class_lib(fRect);
	Xdef_class_lib(fPoint);
	Xdef_class_lib(fSize);
	
	Xdef_class_lib(TriFunc);
	Xdef_class_lib(ColPolygon);
	Xdef_class_lib(BaseScene);
	Xdef_class_lib(BGM_Item);
	Xdef_class_lib(SE_Item);
	Xdef_class_lib_alias(Font,NAME_IN_X(Font));
	Xdef_class_lib(Angle);
	Xdef_class_lib(Radian);
	Xdef_class_lib(Degree);
	Xdef_class_lib_alias(__TextureConcept__,texture::TextureConcept);
	Xdef_class_lib_alias(__Texture__,texture::Texture);
	Xdef_class_lib_alias(__TextureSet__,texture::TextureSet);
	Xdef_class_lib_alias(__Animation__,texture::Animation);
	Xdef_class_lib(TextureParam);
	Xdef_class_lib_alias(CharParam,CharacterParam);

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
	Xdef_class_lib(_class);\
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

	//TextureManager
	XTAL_BIND_SINGLETON(TextureManager,TextureMngr)

#undef XTAL_BIND_SINGLETON
}
