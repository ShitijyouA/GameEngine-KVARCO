#include "DecryptedZip.h"
#include "DecryptFilter.h"

namespace boost
{
	namespace filesystem3
	{
		size_t hash_value(const path& data)
		{
			size_t h=boost::hash_value(data.native());
			return h;
		}
	}
}

namespace kvarco
{
namespace crypted_zip
{

template<typename StreamType>
void DecryptedZip::ReadTerminalHeader(StreamType& stream)
{
	const long header_offset_from_end=-static_cast<long>(sizeof(terminal_)+ZIP_HEADER.size());
	//ios::seek(stream,header_offset_from_end,std::ios::end);
	stream.seekg(header_offset_from_end,std::ios::end);

	header::TerminalHeader::HeaderType* readed_header
		=new header::TerminalHeader::HeaderType[ZIP_HEADER.size()+1];

	const DWORD read_bytes=sizeof(readed_header[0])*(ZIP_HEADER.size());
	stream.read(reinterpret_cast<char*>(readed_header),read_bytes);
	readed_header[ZIP_HEADER.size()]=0;

	stream.read(reinterpret_cast<char*>(&terminal_),sizeof(terminal_));
	terminal_.header_=readed_header;
}

template<typename StreamType>
void DecryptedZip::ReadCentralHeader(StreamType& stream)
{
	//ios::seek(stream,terminal_.central_headers_pos_,std::ios::beg);
	stream.seekg(terminal_.central_headers_pos_,std::ios::beg);

	for(DWORD i=0; i<terminal_.file_num_; ++i)
	{
		header::CentralHeader tmp;

		stream.read(reinterpret_cast<char*>(&tmp),sizeof(tmp));

		tmp.name_=new header::CentralHeader::NameCharType[tmp.name_len_+1];
		const DWORD read_bytes=sizeof(tmp.name_[0])*(tmp.name_len_+1);
		stream.read(reinterpret_cast<char*>(tmp.name_),read_bytes);

		centrals_.insert(std::make_pair(tmp.name_,tmp));
	}
}

DecryptedZip::DecryptedZip(const fsys::path& source,DWORD key)
	: archive_name_(source.stem().generic_string().c_str()),archive_path_(source)
{
	//open archive file by ipc::file_mapping
	ipc::file_mapping file_map(source.string().c_str(),ipc::read_only);
	ipc::mapped_region file_map_region(file_map,ipc::read_only);

	ipc::ibufferstream source_stream
	(
		reinterpret_cast<char*>(file_map_region.get_address()),
		file_map_region.get_size(),
		std::ios::in | std::ios::binary
	);

	//read headers
	ReadTerminalHeader(source_stream);
	ReadCentralHeader(source_stream);

	source_stream.seekg(0,std::ios::beg);
}

DecryptedZip::~DecryptedZip()
{
	BOOST_FOREACH(CentralHeaderMap::value_type& it,centrals_)
	{
		delete[] it.second.name_;
		it.second.name_=NULL;
	}
}

DecryptedZip::CentralHeaderMap::iterator DecryptedZip::GetIteratorToCentralHeader(fsys::path& file)
{
	// file
	CentralHeaderMap::iterator i=centrals_.find(file);
	if(i!=centrals_.end()) return i;

	// archive_name_ / file
	// maybe,somebady need...?
	fsys::path new_path=archive_name_/file;
	CentralHeaderMap::iterator ii=centrals_.find(new_path);
	if(ii!=centrals_.end()) return ii;

	return centrals_.end();
}

bool DecryptedZip::Exists(fsys::path& file)
{
	return GetIteratorToCentralHeader(file)!=centrals_.end();
}

DecryptedZip::ArchivedFilePtr DecryptedZip::GetFile(fsys::path& file)
{
	CentralHeaderMap::iterator cheader_ite=GetIteratorToCentralHeader(file);

	//the file does not exist
	if(cheader_ite==centrals_.end()) return ArchivedFilePtr(new ArchivedFile(archive_name_,false,terminal_.is_crypted_));

	//the file exists
	CentralHeaderMapValueType& cheader=*cheader_ite;
	ipc::file_mapping file_map(archive_path_.string().c_str(),ipc::read_only);

	return ArchivedFilePtr
		(
			new ArchivedFile_Exist
			(
				archive_name_		,
				archive_name_/file	,
				cheader.second		,
				terminal_.is_crypted_,
				file_map
			)
		);
}

//DecryptedZip::ArchivedFile
boost::intmax_t DecryptedZip::ArchivedFile_Exist::RawSize()
{
	return file_info_.raw_size_;
}

boost::intmax_t DecryptedZip::ArchivedFile_Exist::CompedSize()
{
	return file_info_.comped_size_;
}

DecryptedZip::ArchivedFile_Exist::filtering_istream_ptr
	DecryptedZip::ArchivedFile_Exist::MakeInputStream()
{
	ipc_ibufferstream_ptr tmp_source_stream
	(
		new ipc::ibufferstream
		(
			reinterpret_cast<char*>(file_view_.get_address()),
			file_view_.get_size(),
			std::ios::in | std::ios::binary
		)
	);
	source_stream.swap(tmp_source_stream);

	DecompresserType decomp;
	//init crypt filter and open temp stream,crypt
	//open temp stream
	filtering_istream_ptr tmp_istream(new ios::filtering_istream);

	tmp_istream->push(decomp);
	if(is_crypted_)
	{
		dices_.InitByKey(file_info_.key_);
		detail::DecryptFilter cfilter(dices_);

		tmp_istream->push(cfilter);
	}
	tmp_istream->push(*source_stream);

	return boost::move(tmp_istream);
}

bool DecryptedZip::ArchivedFile_Exist::OutToFile(const fsys::path& dst)
{
	std::ofstream out_stream(dst.string().c_str(),std::ios::out | std::ios::binary);
	if(!out_stream.is_open()) return false;

	filtering_istream_ptr tmp_istream=MakeInputStream();
	std::copy
	(
		std::istreambuf_iterator<char>	(tmp_istream->rdbuf()),
		std::istreambuf_iterator<char>	(),
		std::ostream_iterator<char>		(out_stream)
	);

	return true;
}

bool DecryptedZip::ArchivedFile_Exist::OutToMemory(BYTE* dst,DWORD dst_size)
{
	if(dst_size<file_info_.raw_size_) return false;

	filtering_istream_ptr tmp_istream=MakeInputStream();

	std::istreambuf_iterator<char> begin_ite(*tmp_istream);
	std::istreambuf_iterator<char> end_ite;	

	for(DWORD i=0; begin_ite!=end_ite && i<dst_size; ++i)
	{	
		dst[i]=*begin_ite;
	}

	return true;
}

} //namespace crypted_zip
} //namespace kvarco