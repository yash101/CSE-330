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
	MemoryMappedFile(std::string filename, const unsigned int mode);
	~MemoryMappedFile();

	bool openFile(std::string filename, const unsigned int mode);
	inline size_t getFileSize() { return _fileSize;  }
	inline operator unsigned char*() { return _mappedFile;  }
	//inline unsigned char& operator[](size_t pos) {
	//	if (pos < getFileSize()) return _mappedFile[pos]; else { std::exception e; throw e; }
	//}
	
	const static unsigned int MODE_READ = 0x1;
	const static unsigned int MODE_WRITE = 0x2;
	const static unsigned int MODE_EXEC = 0x4;
};
