#pragma once

namespace LoadItem
{
	class BaseLoadItem
	{
	protected:
		bool	Loaded;

		xtal::String Name;
		xtal::String Path;

	public:

		//コンストラクタだけXtal依存
		BaseLoadItem(xtal::StringPtr name,xtal::StringPtr path)
			:Loaded(false)
		{
			Name=name->c_str();
			Path=path->c_str();
		}

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
			if(!Loaded)
			{
				kvarco::LoadGraph(Name.c_str(),Path.c_str());
				kvarco::OutputLog("%sをロード",Path.c_str());
				Loaded=true;
			}
		}
	};
}