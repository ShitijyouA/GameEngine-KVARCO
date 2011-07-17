#pragma once
#include "TextureManager.h"

namespace LoadItem
{
	class BaseLoadItem
	{
	protected:
		volatile bool	loaded_;

		volatile const	xtal::String name_;
		volatile const	xtal::String path_;

	public:
		//コンストラクタだけXtal依存
		BaseLoadItem(xtal::StringPtr name,xtal::StringPtr path)
			:loaded_(false),name_(name->c_str()),path_(path->c_str())
		{}

		virtual void Load()=0;

		~BaseLoadItem() {}
	};

	//Graphローディング用
	class GraphLoadItem : public BaseLoadItem
	{
	public:
		GraphLoadItem(xtal::StringPtr name,xtal::StringPtr path)
			:BaseLoadItem(name,path)
		{}

		void Load()
		{
			if(loaded_) return;
	
			TextureManager::TextureNameType	name_ptr=const_cast<xtal::String*>(&name_);
			TextureManager::PathType		path_ptr=const_cast<xtal::String*>(&path_);

			TextureManager::GetInst()->LoadX(path_ptr,name_ptr);
			kvarco::OutputLog("%sをロード",path_ptr->c_str());
			loaded_=true;
		}
	};
}