// Ŭnicode please
#pragma once

#define PATH_SEPARATOR  '\\'

#include <cstdio>
#include <string>

namespace sora {;

const int kMaxPathLength = 512;

class Filesystem {
public:
	static int GetFileSize(int fd);
	static int GetFileSize(FILE *file);
	static std::string GetExtension(const char *filename) {
		return GetExtension(std::string(filename));
	}
	static std::string GetExtension(const std::string &str);

	static std::string GetAppPath(const std::string &str);
	static std::string GetAppPath(const char *filename) {
		return GetAppPath(std::string(filename));
	}	
};
}
