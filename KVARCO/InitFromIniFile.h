#pragma once
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

namespace kvarco
{
namespace detail 
{
template<typename DstStruct>
class SimpleIniPropertyWrapper_impl
{
	typedef DstStruct StructType;

	bool exists_that_file_;
	boost::property_tree::ptree init_file_ptree_;
	StructType* dst_struct_inst_;

public:

	template<typename PathType>
	SimpleIniPropertyWrapper_impl(PathType& path,StructType& dst)
	{
		dst_struct_inst_=&dst;
		exists_that_file_=fsys::exists(path);
		
		if(exists_that_file_)
		{
			try
			{
				boost::property_tree::read_ini(path.string().c_str(),init_file_ptree_);
			}
			catch(boost::property_tree::ini_parser::ini_parser_error& e)
			{
				exists_that_file_=false;
			}
		}
	}

	template<typename ValueType,typename MemberType,typename StringType>
	void SetMemberXtype(MemberType StructType::* member_ptr,StringType& data_name)
	{
		if(exists_that_file_)
		{
			boost::optional<ValueType> temp;
			temp=init_file_ptree_.get_optional<ValueType>(data_name);
			if(temp.is_initialized()) dst_struct_inst_->*member_ptr=*temp;
		}
	}

	template<typename MemberType,typename StringType>
	void SetMember(MemberType StructType::* member_ptr,StringType& data_name)
	{
		SetMemberXtype<MemberType,MemberType,StringType>(member_ptr,data_name);
	}

	template<typename ValueType,typename MemberType,typename StringType>
	void SetMemberXtype2(MemberType StructType::* member_ptr,StringType& data_name,MemberType& default_value)
	{
		if(exists_that_file_)
			dst_struct_->*member_ptr=init_file_ptree_.get(data_name,default_value);
	}
	
	template<typename ValueType,typename MemberType,typename StringType>
	void SetMember2(MemberType StructType::* member_ptr,StringType& data_name,MemberType& default_value)
	{
		SetMemberXtype2<MemberType,MemberType,StringType>(member_ptr,data_name,default_value)
	}

	boost::property_tree::ptree GetPropertyTree()
	{
		return init_file_ptree_;
	}
};

/*
//template

typedef SampleClass Class;
#define SET_MEMBER_XTYPE(member_name,data_name)			wrapper.SetMamberXtype(&Class::member_name,#data_name)
#define SET_MEMBER(member_name,data_name,value_type)	wrapper.SetMamber<value_type>(&Class::member_name,#data_name)

*/
}
}
