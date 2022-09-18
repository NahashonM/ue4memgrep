#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <dirent.h>

#define LOG(__msg__,  ...)	{ printf("-----: "); printf(__msg__, __VA_ARGS__); printf("\n"); }
#define LOGe(__msg__, ...)	{ printf("ERROR: [%s:%d:%s] ",__FILE__,__LINE__,__func__ ); printf(__msg__, __VA_ARGS__); printf("\n");}
#define LOGi(__msg__, ...)	{ printf("INFOR: [%s:%d:%s] ",__FILE__,__LINE__,__func__ ); printf(__msg__, __VA_ARGS__); printf("\n");}
#define LOGw(__msg__, ...)	{ printf("ALERT: [%s:%d:%s] ",__FILE__,__LINE__,__func__ ); printf(__msg__, __VA_ARGS__); printf("\n");}


extern const char pattern_gnames[];
extern const char *processName;

typedef struct MEMORY_MAP {
	uint64_t	base;			// starting address in address space
	uint64_t 	end;			// ending address in address space
	uint64_t	offset;			// offset in file for file maps
	uint32_t	permissions;	// access permissions
	uint32_t	major_dev;		// major device number where the file is stored
	uint32_t	minor_dev;		// minor device number where the file is stored
	uint32_t	inode;			// file number for file map
	char		pathname[100];	// name of file | special region indicators
}MEMORY_MAP;

//-------------- Argument Parsing -------------------
int GetArguments(int argc, const char** argv, void** parsedArgs);
void Help(const char** argv);

//-------------- Pattern Matching -------------------
int GenBhtTable(const char* pattern, int* bht, int szPattern, int szAlphabet);
int BmpPatternMatch(const char* pattern, const int* bhtTable, int szPattern, const char* text, int szText);

//-------------- Tools -------------------
bool IsInt(const char* str, int argc);
int FindProcessByName(const char* processName, int szProcessName );
int GetProcessMemoryMaps(int pid, MEMORY_MAP* memoryMaps);


uint64_t FindGnamesArray(int pid, MEMORY_MAP* memoryMap, int szMemoryMap);
uint64_t FindLibUE4(MEMORY_MAP* memoryMap, int szMemoryMap);