#include "CryptedZip.h"
#include "CryptFilter.h"
const std::string kvarco::crypted_zip::ZIP_HEADER="kvcz";

namespace kvarco
{
namespace crypted_zip
{

EncryptedZip::EncryptedZip(const fsys::path& source,bool do_encrypt,ios::zlib_params params)
	: zipping_params_(params),do_encrypt_(do_encrypt)
{
	AddPackFile(source);
}

void EncryptedZip::AddPackFile(const fsys::path& source)
{
	if(!fsys::exists(source)) return;

	if(fsys::is_directory(source))
	{
		InsertAllFilePath(source);
	}
	else
		input_file_path_list_.push_back(source.filename());
}

void EncryptedZip::InsertAllFilePath(const fsys::path& dir)
{
	parent_dir_=fsys::absolute(dir);

	fsys::directory_iterator end;
	for(fsys::directory_iterator di(dir); di!=end; ++di)
	{
		if(!fsys::is_directory(*di))
			input_file_path_list_.push_back(fsys::absolute(di->path()));
		else
			InsertAllFilePath(di->path());
	}
}

void EncryptedZip::MakeRltPathList(detail::PathList& rls_path_list)
{
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
	}
}

void EncryptedZip::MakeCentralHeaderList()
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

		if(do_encrypt_)	tmp.key_=dices_.DiceDWORD();
		else			tmp.key_=0;

		tmp.name_len_	=(*i).wstring().size();

		tmp.name_		=new header::CentralHeader::NameCharType[tmp.name_len_+1];
		::wcscpy_s(tmp.name_,tmp.name_len_+1,(*i).c_str());

		central_header_list_.push_back(std::make_pair(*ii,tmp));
	}
}

template<typename StreamType>
void EncryptedZip::CryptingAndZipping_impl(const fsys::path& path,StreamType& dst_stream)
{
	if(!fsys::exists(path)) return;

	//file open
	ipc::file_mapping file_map(path.string().c_str(),ipc::read_only);
	ipc::mapped_region file_map_region(file_map,ipc::read_only);

	boost::iterator_range<const char*>
		mapped_iterator_range(detail::make_mapped_range<const char>(file_map_region));

	std::ostream_iterator<char> ofs_iter(dst_stream);

	//copy
	range::copy(mapped_iterator_range,ofs_iter);
}

namespace
{

template<typename StreamType>
void WriteCentralHeader(StreamType& dst_stream,const header::PathAndCentralHeader& it)
{
	dst_stream.write(reinterpret_cast<const char*>(&it.second),sizeof(it.second));
	dst_stream.write(
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

template<typename DstStreamType>
bool EncryptedZip::CompToTemp(header::PathAndCentralHeader& it,DstStreamType& dst_stream)
{
	//compress and write to temp file
	{
		static ios::zlib_compressor comp(zipping_params_);
		
		//open new temp file
		ios::file_sink fsink("tmp",std::ios::out | std::ios::binary);
		if(!fsink.is_open()) return false;

		//init crypt filter and open temp stream,crypt
		if(do_encrypt_)
		{
			dices_.InitByKey(it.second.key_);
			detail::CryptFilter cfilter(dices_);

			//open temp stream
			ios::filtering_ostream tmp_ostream(comp | cfilter | fsink);

			//do compress and crypt
			CryptingAndZipping_impl(it.first,tmp_ostream);
		}
		else
		{
			detail::NoCryptFilter cfilter;

			//open temp stream
			ios::filtering_ostream tmp_ostream(comp | cfilter | fsink);

			//do compress and crypt
			CryptingAndZipping_impl(it.first,tmp_ostream);
		}
	}

	//write to dst
	{
		//open temp file
		std::fstream tmp_istream("tmp",std::ios::in | std::ios::binary);
		if(!tmp_istream.is_open()) return false;

		//copy to dst file
		it.second.data_pos_	=static_cast<boost::intmax_t>(dst_stream.tellp());

		std::copy
		(
			std::istream_iterator<char>(tmp_istream),
			std::istream_iterator<char>(),
			std::ostream_iterator<char>(dst_stream)
		);

		it.second.size_		=static_cast<boost::intmax_t>(dst_stream.tellp())-it.second.data_pos_;
	}

	return true;
}

bool EncryptedZip::OutToFile(const fsys::path& dst)
{
	bool success=true;

	//
	MakeCentralHeaderList();

	//if the file exists,remove
	if(fsys::exists(dst)) fsys::remove(dst);
	
	//open the file as append mode
	std::fstream dst_stream(dst.string().c_str(),std::ios::in | std::ios::out | std::ios::binary | std::ios::app);
	
	//write tmp files
	BOOST_FOREACH(header::PathAndCentralHeader& it,central_header_list_)
	{
		success&=CompToTemp(it,dst_stream);
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

EncryptedZip::~EncryptedZip()
{
	BOOST_FOREACH(header::PathAndCentralHeader& it,central_header_list_)
	{
		delete[] it.second.name_;
		it.second.name_=NULL;
	}
}

} //namespace kvarco
} //namespace crypted_zip
