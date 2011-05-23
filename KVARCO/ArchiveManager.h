#pragma once

#if 1

struct Archive
{
	fsys::path archive_abs_path_;		//archive file's path
	fsys::path root_folder_abs_path_;	//root-folder path in archive
};


class ArchiveManager
{
	boost::unordered_map<>;

public:
	void Exists();
	void ArchiveExists();

	void DecompToMemmory();

	template<typename Type>
	boost::shared_ptr<Type> DecompToMemmory();

	bool DecompToFile();	//return : ê¨å˜ÇµÇΩÇ©
};

#endif