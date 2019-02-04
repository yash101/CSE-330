#pragma once

#include <string>

class MemoryMappedFile
{
private:
	int _fileDescriptor;
	unsigned char* _mappedFile;
	size_t _fileSize;
public:
	MemoryMappedFile();
	MemoryMappedFile(std::string filename);
	bool openFile(std::string filename);
	inline size_t getFileSize() { return _fileSize;  }
	inline operator unsigned char*() { return _mappedFile;  }
	~MemoryMappedFile();
};
