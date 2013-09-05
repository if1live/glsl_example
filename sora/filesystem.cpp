// Ŭnicode please
#include "stdafx.h"
#include "filesystem.h"

namespace sora {;
std::string app_root_path;
std::string doc_root_path;

int InitFileSystem();
int init_filesystem = InitFileSystem();

int InitFileSystem() 
{
    doc_root_path = "";
	app_root_path = "";
	return 1;
}

int Filesystem::GetFileSize(FILE *file) 
{
	int curr_pos = ftell(file);
	fseek(file, 0, SEEK_END);
	int length = ftell(file);
	fseek(file, curr_pos, SEEK_SET);
	return length;
}

std::string Filesystem::GetExtension(const std::string &str) 
{
	using std::string;
	// 경로 쪼개느거는 2개다 동시 지원할수 있도록함
	// /. \\를 하나로 합치면 되겠지
	string filename = str;
	char *data = (char*)filename.c_str();
	for (size_t i = 0 ; i < filename.size() ; i++) {
		if (data[i] == '\\') {
			data[i] = '/';
		}
	}

	size_t comma_found = filename.rfind(".");
	size_t separator_found = filename.rfind('/');

	if (comma_found == string::npos) {
		//.을 못찾은 경우, 아마도 확장자 없다
		return string("");
	}

	if (comma_found != string::npos && separator_found == string::npos) {
		string name = filename.substr(comma_found+1);
		return name;
	}

	if (comma_found > separator_found) {
		string name = filename.substr(comma_found+1);
		return name;
	} else {
		return string("");
	}
}

std::string Filesystem::GetAppPath(const std::string &str) 
{
	std::string filename = str;
	for(size_t i = 0 ; i < filename.length() ; ++i) {
		if(filename[i] == '/') {
			filename[i] = PATH_SEPARATOR;
		}
	}
    return filename;
}
}