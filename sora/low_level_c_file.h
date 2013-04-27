// Ŭnicode please

#include "filesystem.h"

namespace sora {;

class ReadonlyCFile;
class WriteonlyCFile;
class LowLevelCFile;

class LowLevelCFile {
public:
	LowLevelCFile(const std::string &filepath);
	LowLevelCFile(const char *filepath);
	~LowLevelCFile();

	bool Open(const char *mode);
	bool Close();
	bool IsOpened() const;

	int Read(void *buf, int size);
	int Seek(int offset, int origin);
	int Write(const void *buf, int size);

	const void *GetBuffer();
	int GetLength() const;
	int GetRemainLength() const;
	const char *filepath() const { return filepath_; }

private:
	FILE *file_;
	char filepath_[kMaxPathLength];
	void *buffer_;
};

class ReadonlyCFile {
public:
	ReadonlyCFile(const std::string &filepath) : file_(filepath) {}
	ReadonlyCFile(const char *filepath) : file_(filepath) {}
	~ReadonlyCFile() { file_.Close(); }

	bool Open() { return file_.Open("rb"); }
	bool Close() { return file_.Close(); }
	bool IsOpened() const { return file_.IsOpened(); }

	int Read(void *buf, int size) { return file_.Read(buf, size); }
	int Seek(int offset, int origin) { return file_.Seek(offset, origin); }
	const void *GetBuffer() { return file_.GetBuffer(); }

	int GetLength() const { return file_.GetLength(); }
	int GetRemainLength() const { return file_.GetRemainLength(); }
	const char *filepath() const { return file_.filepath(); }

private:
	LowLevelCFile file_;
};

class WriteonlyCFile {
public:
	WriteonlyCFile(const std::string &filepath) : file_(filepath) {}
	WriteonlyCFile(const char *filepath) : file_(filepath) {}
	~WriteonlyCFile() { file_.Close(); }

	bool Open() { return file_.Open("w"); }
	bool Close() { return file_.Close(); }
	bool IsOpened() const { return file_.IsOpened(); }
	int Write(const void *buf, int size) { return file_.Write(buf, size); }
	int GetLength() const { return file_.GetLength(); }
	int GetRemainLength() const { return file_.GetRemainLength(); }
	const char *filepath() const { return file_.filepath(); }

private:
	LowLevelCFile file_;
};

}
