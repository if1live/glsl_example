// Ŭnicode please
#include "stdafx.h"
#include "filesystem.h"

// for filesystem
#include <Windows.h>
#include <io.h>
#include <tchar.h>
#include <direct.h>
#define tell _tell
#define lseek _lseek


namespace sora {;
std::string app_root_path;
std::string doc_root_path;

int InitFileSystem();
int init_filesystem = InitFileSystem();

int InitFileSystem() 
{
	/*
	// 윈도우에서 실행프로그램이 있는 경로 얻기
	// 이것을 이용해서 경로 변경후 파일을 열자
	TCHAR path[MAX_PATH];
	::GetModuleFileName(0, path, _MAX_PATH);
	TCHAR* p = _tcsrchr(path, '\\');
	path[p - path] = 0;
	doc_root_path = path;
	app_root_path = path;
	chdir(app_root_path.c_str());
	return 1;
	*/
	TCHAR path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, path);
	doc_root_path = path;
	app_root_path = path;
	_chdir(app_root_path.c_str());
	return 1;
}

int Filesystem::GetFileSize(int fd) 
{
	if (fd == -1) {
		return -1;
	}
	// library하고 꼬여서 자구 죽네... 다른 방법으로 얻어야될듯
	// struct stat s;
	// int result = fstat(fd, &s);
	// return s.st_size;
	int curr_pos = _tell(fd);
	_lseek(fd, 0, SEEK_END);
	int length = tell(fd);
	lseek(fd, curr_pos, SEEK_SET);
	return length;
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

	char last_ch = app_root_path[app_root_path.size()-1];
	if (last_ch == '/' || last_ch == '\\') {
		return app_root_path + filename;
	} else {
		return app_root_path + PATH_SEPARATOR + filename;
	}
}
}