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


MemoryMappedFile::MemoryMappedFile(std::string filename, const unsigned int mode) :
	_fileDescriptor(-1),
	_mappedFile(NULL),
	_fileSize(0)
{
	openFile(filename, mode);
}


bool MemoryMappedFile::openFile(std::string filename, const unsigned int mode) {
	if (_fileDescriptor != -1) {
		return false;
	}

	struct stat st;
	if (stat(filename.c_str(), &st) == -1)
		return false;

	int open_flags = 0;
	int mmap_flags = 0;
	if (mode & MODE_READ) {
		open_flags |= O_RDONLY;
		mmap_flags |= PROT_READ;
	}
	if (mode & MODE_WRITE) {
		if (open_flags & O_RDONLY)
			open_flags = O_RDWR;
		mmap_flags |= PROT_WRITE;
	}
	if (mode & MODE_EXEC) {
		open_flags |= O_EXCL;
		mmap_flags |= PROT_EXEC;
	}


	// Open the file
	_fileDescriptor = open(filename.c_str(), O_RDONLY);
	if (_fileDescriptor == -1) {
		return false;
	}

	_fileSize = st.st_size;

	_mappedFile = static_cast<unsigned char*>(mmap64(0, _fileSize, PROT_READ, MAP_SHARED, _fileDescriptor, 0));

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
