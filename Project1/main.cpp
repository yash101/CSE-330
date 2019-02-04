#include "regex_parser_single.h"
#include "mmap_file.h"
#include <cstdio>

void help(int argc, char** argv) {
	printf("%s <pattern> <list of text files>\n", argv[0]);
}

int main(int argc, char** argv)
{
	MemoryMappedFile file("/home/chaos/install_x11_windows.md");
	for (size_t i = 0; i < file.getFileSize(); i++) {
		printf("%c", file[i]);
	}
	getchar();
}