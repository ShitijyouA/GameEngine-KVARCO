#pragma once

namespace LoadItem
{
	class CBaseLoadItem
	{
	protected:
		bool	Loaded;

		xtal::String Name;
		xtal::String Path;

	public:

		//�R���X�g���N�^����Xtal�ˑ�
		CBaseLoadItem(xtal::StringPtr name,xtal::StringPtr path)
			:Loaded(false)
		{
			Name=name->c_str();
			Path=path->c_str();
		}

		virtual void Load()=0;

		~CBaseLoadItem() {}
	};

	//Graph���[�f�B���O�p
	class CGraphLoadItem : public CBaseLoadItem
	{
	public:
		CGraphLoadItem(xtal::StringPtr name,xtal::StringPtr path)
			:CBaseLoadItem(name,path)
		{}

		void Load()
		{
			if(!Loaded)
			{
				KVARCO::LoadGraph(Name.c_str(),Path.c_str());
				KVARCO::OutputLog("%s�����[�h",Path.c_str());
				Loaded=true;
			}
		}
	};
}