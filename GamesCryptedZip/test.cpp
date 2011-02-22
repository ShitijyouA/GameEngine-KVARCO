﻿#include "pch.h"
#include "CryptedZip.h"
using namespace KVARCO::CryptedZip;

#include <iostream>
using namespace std;

#include <boost/assign.hpp>
using namespace boost::assign;

const path SAMPLE_DIR("..\\GamesCryptedZip\\SampleFiles\\");
const bool DO_CRYPT		=true;

int main()
{
	try
	{
#if 0
		//en
		{
			PathList path_list;
			path_list+=MakePathPair(SAMPLE_DIR/"artificl");

			CEncryptedZip zipper(path_list,5,0xAAAAAAAA,1234567,DO_CRYPT);
			zipper.OutputToFile(SAMPLE_DIR/"sample.kva");
		}
#endif

		//de
		{
			CDecryptedUnZip unzipper(SAMPLE_DIR/"sample.kva",5,0xAAAAAAAA,DO_CRYPT);
			CHeader_op ch=unzipper.FindFile(path("\\artificl\\alphabet.txt"));
			if(!ch) throw bad_exception("couldn't find the file at Zip file");

			unzipper.OutputToFile(SAMPLE_DIR/"sample_out.txt",*ch);
		}
	}
	catch(std::exception& e)
	{
		cout<<e.what()<<endl;
	}

	cout<<"finish!"<<endl;
	getchar();
	return 0;
}