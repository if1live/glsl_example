// Ŭnicode please
#include "stdafx.h"

#include <io.h>
#include <fcntl.h>

#include "memory_file.h"
#include "filesystem.h"

namespace sora {;

MemoryFile::MemoryFile(const char *filepath)
	: start(NULL),
	end(NULL),
	curr(NULL),
	data(NULL) 
{
	strcpy(filepath_, filepath);
}
MemoryFile::MemoryFile(const std::string &filepath)
	: start(NULL),
	end(NULL),
	curr(NULL),
	data(NULL) 
{
	strcpy(filepath_, filepath.c_str());
}

MemoryFile::~MemoryFile() 
{
	Close();
}

void MemoryFile::Close() 
{
	if(data != NULL) {
		free(data);
		start = NULL;
		curr = NULL;
		end = NULL;
		data = NULL;
	}
}

int MemoryFile::Read(void *buf, int size) 
{
	int curr_pos = curr - start;
	int length = GetLength();
	int remain = length - curr_pos;
	if (remain < size) {
		memcpy((unsigned char*)buf, curr, remain);
		curr += remain;
		return remain;
	} else {
		memcpy((unsigned char*)buf, curr, size);
		curr += size;
		return size;
	}
}

bool MemoryFile::Open() 
{
	int flag = O_RDONLY;
	flag |= O_BINARY;
	int fd = _open(filepath_, flag);
	SR_ASSERT(fd != -1 && "file is not exist");
	if (fd == -1) {
		return false;
	}

	int length = Filesystem::GetFileSize(fd);
	start = static_cast<unsigned char*>(malloc(length + 1)); 
	data = start;
	_read(fd, start, length);

	curr = start;
	end = curr + length;
	*end = '\0';

	_close(fd);
	return true;
}
}	// namespace sora