#ifndef __FILEMGR
#define __FILEMGR

#define SECTION3(line, a, b, c) ((line)[0] == (a) && (line)[1] == (b) && (line)[2] == (c))
#define SECTION4(line, a, b, c, d) ((line)[0] == (a) && (line)[1] == (b) && (line)[2] == (c) && (line)[3] == (d))

class CFileMgr
{
public:
	static inline FILE*		OpenFile(const char* path, const char* mode)
				{ return fopen(path, mode); }
	static inline  int		CloseFile(FILE* stream)
				{ return fclose(stream); }
	static inline bool		ReadLine(FILE* stream, char* str, int num)
				{ return fgets(str, num, stream) != nullptr; }
	static inline size_t	Read(FILE* stream, void* buf, size_t len)
				{ return fread(buf, 1, len, stream); }
	static inline size_t	Write(FILE* stream, const char* ptr, size_t len)
				{ return fwrite(ptr, 1, len, stream); }
	static inline bool		Seek(FILE* stream, long pos, int from)
				{ return fseek(stream, pos, from) != 0; }
};

// Thought they could fall into the same file...
class CFileLoader
{
public:
	static const char*			LoadLine(FILE* hFile);
};

#endif