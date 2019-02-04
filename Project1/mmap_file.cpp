#include "mmap_file.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

MemoryMappedFile::MemoryMappedFile() :
	_fileDescriptor(-1),
	_mappedFile(NULL),
	_fileSize(0)
{
}


MemoryMappedFile::MemoryMappedFile(std::string filename) :
	_fileDescriptor(-1),
	_mappedFile(NULL),
	_fileSize(0)
{
	openFile(filename);
}


bool MemoryMappedFile::openFile(std::string filename) {
	if (_fileDescriptor != -1) {
		return false;
	}

	struct stat st;
	if (stat(filename.c_str(), &st) == -1)
		return false;

	// Open the file
	_fileDescriptor = open(filename.c_str(), O_RDONLY);
	if (_fileDescriptor == -1) {
		return false;
	}

	_fileSize = st.st_size;

	_mappedFile = static_cast<unsigned char*>(mmap(0, _fileSize, PROT_READ, MAP_SHARED, _fileDescriptor, 0));
	if (_mappedFile == MAP_FAILED) {
		close(_fileDescriptor);
		_fileDescriptor = -1;
		return false;
	}

	return true;
}


MemoryMappedFile::~MemoryMappedFile()
{
	if (_fileDescriptor != -1) {
		munmap(_mappedFile, _fileSize);
		close(_fileDescriptor);
	}
}
