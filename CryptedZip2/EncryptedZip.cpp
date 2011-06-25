#include "EncryptedZip.h"
#include "CryptingFilter.h"
const std::string kvarco::crypted_zip::ZIP_HEADER="kvcz";

namespace kvarco
{
namespace crypted_zip
{

template<typename LoggerType>
void EncryptedZip_impl<LoggerType>::AddPackFile(const fsys::path& source)
{
	if(!fsys::exists(source)) return;

	parent_dir_=fsys::absolute(source);

	if(fsys::is_directory(source))
		InsertAllFilePath(source);
	else
		input_file_path_list_.push_back(source.filename());

	MakeCentralHeaderList();
}

template<typename LoggerType>
void EncryptedZip_impl<LoggerType>::InsertAllFilePath(const fsys::path& dir)
{
	logger_("*at begin of EncryptedZip_impl<LoggerType>_impl::InsertAllFilePath*");
	fsys::directory_iterator end;
	for(fsys::directory_iterator di(dir); di!=end; ++di)
	{
		if(!fsys::is_directory(*di))
		{
			input_file_path_list_.push_back(fsys::absolute(di->path()));
			logger_(fsys::absolute(di->path()).string());
		}
		else
			InsertAllFilePath(di->path());
	}
	logger_("*at end of EncryptedZip_impl<LoggerType>_impl::InsertAllFilePath*");
}

template<typename LoggerType>
void EncryptedZip_impl<LoggerType>::MakeRltPathList(detail::PathList& rls_path_list)
{
	logger_("*at begin of EncryptedZip_impl<LoggerType>_impl::MakeRltPathList*");
	for(detail::PathList::iterator i=input_file_path_list_.begin(); i!=input_file_path_list_.end(); ++i)
	{
		if(fsys::is_directory(*i))
		{
			i=input_file_path_list_.erase(i);
			continue;
		}
		
		//through same path range
		fsys::path::iterator ii,iii;
		for(ii=(*i).begin(),iii=parent_dir_.begin(); (*ii)==(*iii); ++ii,++iii);

		//copy different path range
		fsys::path rlt_path;
		for(--ii; ii!=(*i).end(); ++ii) rlt_path/=*ii;

		rls_path_list.push_back(rlt_path);
		logger_(rlt_path.string());
	}
	logger_("*at end of EncryptedZip_impl<LoggerType>_impl::MakeRltPathList*");
}

template<typename LoggerType>
void EncryptedZip_impl<LoggerType>::MakeCentralHeaderList()
{
	detail::PathList rls_path_list;
	MakeRltPathList(rls_path_list);
	for(
		detail::PathList::iterator i=rls_path_list.begin(),ii=input_file_path_list_.begin();
		i!=rls_path_list.end() && ii!=input_file_path_list_.end();
		++i,++ii
	)
	{
		header::CentralHeader tmp;

		tmp.data_pos_	=0;
		tmp.raw_size_	=fsys::file_size(*ii);

		if(do_encrypt_)	tmp.key_=dices_.RollDice<DWORD>();
		else			tmp.key_=0;

		tmp.name_len_	=(*i).generic_wstring().size();
		tmp.name_		=new header::CentralHeader::NameCharType[tmp.name_len_+1];
		::wcscpy_s(tmp.name_,tmp.name_len_+1,(*i).c_str());

		central_header_list_.push_back(std::make_pair(*ii,tmp));
	}

	input_file_path_list_.clear();
}

template<typename LoggerType>
template<typename StreamType>
void EncryptedZip_impl<LoggerType>::CryptingAndZipping_impl(const fsys::path& path,StreamType& dst_stream)
{
	if(!fsys::exists(path)) return;

	logger_("zipping : "+path.string());
	std::ifstream src_stream(path.string().c_str(),std::ios::in | std::ios::binary);
	std::copy
	(
		std::istream_iterator<char>(src_stream),
		std::istream_iterator<char>(),
		std::ostream_iterator<char>(dst_stream)
	);
}

namespace
{

template<typename StreamType>
void WriteCentralHeader(StreamType& dst_stream,const header::PathAndCentralHeader& it)
{
	dst_stream.write(reinterpret_cast<const char*>(&it.second),sizeof(it.second));
	dst_stream.write
	(
		reinterpret_cast<const char*>(it.second.name_),
		sizeof(header::CentralHeader::NameCharType)*(it.second.name_len_+1)
	);
}

template<typename StreamType>
void WriteTerminalHeader(StreamType& dst_stream,const header::TerminalHeader& it)
{
	dst_stream<<ZIP_HEADER;
	dst_stream.write(reinterpret_cast<const char*>(&it.header_),sizeof(it));
}

}

template<typename LoggerType>
template<typename DstStreamType>
bool EncryptedZip_impl<LoggerType>::WriteToTemp(header::PathAndCentralHeader& it,DstStreamType& dst_stream)
{
	CompresserType comp(compress_params_);
	//open new temp file
	std::ofstream fsink("tmp",std::ios::out | std::ios::binary);
	if(!fsink.is_open()) return false;

	//init crypt filter and open temp stream,crypt
	//open temp stream
	ios::filtering_ostream tmp_ostream;

	tmp_ostream.push(comp);
	if(!password_.empty())
	{
		typedef detail::CryptingFilterTypeByStreamType<ios::filtering_ostream>::type filter_type;
		arcfour::Arcfour arcfour(password_);

		dices_.InitByKey(it.second.key_);
		filter_type::type iv=filter_type::InitialBlock(dices_);
		filter_type cfilter(iv,arcfour);

		tmp_ostream.push(cfilter);
	}
	tmp_ostream.push(fsink);

	//do compress and crypt
	logger_("zipping : "+it.first.string());
	std::ifstream src_stream(it.first.string().c_str(),std::ios::in | std::ios::binary);

	ios::copy(src_stream,tmp_ostream);

	return true;
}

template<typename LoggerType>
template<typename DstStreamType>
bool EncryptedZip_impl<LoggerType>::WriteToDst(header::PathAndCentralHeader& it,DstStreamType& dst_stream)
{
	//open temp file
	std::ifstream tmp_istream("tmp",std::ios::in | std::ios::binary);
	if(!tmp_istream.is_open()) return false;

	//copy to dst file
	it.second.data_pos_		=static_cast<boost::intmax_t>(dst_stream.tellp());
	std::copy
	(
		std::istreambuf_iterator<char>	(tmp_istream.rdbuf()),
		std::istreambuf_iterator<char>	(),
		std::ostream_iterator<char>		(dst_stream)
	);
	it.second.comped_size_	=static_cast<boost::intmax_t>(dst_stream.tellp())-it.second.data_pos_;

	return true;
}

template<typename LoggerType>
template<typename DstStreamType>
bool EncryptedZip_impl<LoggerType>::CompToDst(header::PathAndCentralHeader& it,DstStreamType& dst_stream)
{
	logger_("*at begin of EncryptedZip_impl<LoggerType>_impl::CompToTemp*");

	//compress and write to temp file
	if(!WriteToTemp(it,dst_stream))	return false;

	//write to dst
	if(!WriteToDst(it,dst_stream))	return false;

	logger_("*at end of EncryptedZip_impl<LoggerType>_impl::CompToTemp*");
	return true;
}

template<typename LoggerType>
bool EncryptedZip_impl<LoggerType>::ZipToFile(const fsys::path& dst)
{
	bool success=true;

	//if the file exists,remove
	if(fsys::exists(dst)) fsys::remove(dst);
	
	//open the file as append mode
	std::fstream dst_stream(dst.string().c_str(),std::ios::in | std::ios::out | std::ios::binary | std::ios::app);
	
	//write tmp files
	BOOST_FOREACH(header::PathAndCentralHeader& it,central_header_list_)
	{
		success&=CompToDst(it,dst_stream);
	}

	if(fsys::exists("tmp")) fsys::remove("tmp");

	boost::intmax_t tmp_central_headers_pos=dst_stream.tellp();

	//write CentralHeaders
	BOOST_FOREACH(header::PathAndCentralHeader& it,central_header_list_)
	{
		WriteCentralHeader(dst_stream,it);
	}

	//make TerminalHeader
	header::TerminalHeader t_header;
	t_header.file_num_				=central_header_list_.size();
	t_header.central_headers_pos_	=tmp_central_headers_pos;

	//write TerminalHeader
	WriteTerminalHeader(dst_stream,t_header);
	
	//success or failed
	if(!success) fsys::remove(dst);
	return success;
}

template<typename LoggerType>
EncryptedZip_impl<LoggerType>::~EncryptedZip_impl()
{
	BOOST_FOREACH(header::PathAndCentralHeader& it,central_header_list_)
	{
		delete[] it.second.name_;
		it.second.name_=NULL;
	}
}

//
template class EncryptedZip_impl<logger::NullLogger>;
template class EncryptedZip_impl<logger::StdLogger>;

} //namespace kvarco
} //namespace crypted_zip
