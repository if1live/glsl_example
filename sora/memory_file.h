// Ŭnicode please
#include "filesystem.h"

namespace sora {;
class MemoryFile {
public:
	MemoryFile(const char *filepath);
	MemoryFile(const std::string &filepath);
	~MemoryFile();

	bool Open();
	bool IsOpened() const { return (data != NULL); }
	void Close();
	int GetLength() const { return end - start; }
	int Read(void *buf, int size);

	const char *filepath() const { return filepath_; }

public:
	// data
	unsigned char *start;
	unsigned char *end;
	unsigned char *curr;
	void *data;

	char filepath_[kMaxPathLength];
};

}	// namespace sora

