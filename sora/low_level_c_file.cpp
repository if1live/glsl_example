// Ŭnicode please
#include "stdafx.h"
#include "low_level_c_file.h"
#include "filesystem.h"

namespace sora {;
LowLevelCFile::LowLevelCFile(const std::string &filepath)
	: file_(NULL), 
	buffer_(NULL) 
{
	strcpy(filepath_, filepath.c_str());
}
LowLevelCFile::LowLevelCFile(const char *filepath)
	: file_(NULL), buffer_(NULL) 
{
	strcpy(filepath_, filepath);
}
LowLevelCFile::~LowLevelCFile() 
{
	if (IsOpened()) {
		Close();
	}
}

bool LowLevelCFile::Open(const char *mode) 
{
	if (file_ != NULL) {
		return false;
	}
	file_ = fopen(filepath_, mode);
	if (file_ == NULL) {
		return false;
	} else {
		return true;
	}
}

bool LowLevelCFile::Close() 
{
	if (buffer_ != NULL) {
		free(buffer_);
		buffer_ = NULL;
	}
	if (file_ != NULL) {
		fclose(file_);
		file_ = NULL;
		return true;
	} else {
		return false;
	}
}

bool LowLevelCFile::IsOpened() const 
{
	if (file_ == NULL) {
		return false;
	} else {
		return true;
	}
}

int LowLevelCFile::Read(void *buf, int size) 
{
	if (file_ == NULL) {
		return -1;
	}
	return fread(buf, size, 1, file_);
}

int LowLevelCFile::Seek(int offset, int origin) 
{
	if (file_ == NULL) {
		return -1;
	}
	return fseek(file_, offset, origin);
}

int LowLevelCFile::Write(const void *buf, int size) 
{
	if (file_ == NULL) {
		return -1;
	}
	return fwrite(buf, size, 1, file_);
}

int LowLevelCFile::GetLength() const 
{
	if (file_ == NULL) {
		return 0;
	}
	return Filesystem::GetFileSize(file_);
}

int LowLevelCFile::GetRemainLength() const 
{
	if (file_ == NULL) {
		return 0;
	}
	int length = GetLength();
	int curr_pos = ftell(file_);
	return length - curr_pos;
}

const void *LowLevelCFile::GetBuffer() 
{
	if (buffer_ == NULL) {
		int length = GetLength();
		// 1바이트 더 할당해서 \0로 넣어놓자. 안정성 확보를 위한 목적
		// 나중에 마지막 널을 넣는지 안넣는지는 옵션으로 빼는게 나을듯
		buffer_ = malloc(length + 1);

		long curr_pos = ftell(file_);
		fseek(file_, 0, SEEK_SET);
		fread(buffer_, length, 1, file_);
		fseek(file_, curr_pos, SEEK_SET);

		((char*)buffer_)[length] = '\0';
	}
	return buffer_;
}

}	// namespace sora