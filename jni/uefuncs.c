#include "includes.h"


#define ISREADABLE(_perm_)		(_perm_ & 1)
#define ISWRITABLE(_perm_)		(_perm_ & 2)	
#define ISEXECUTABLE(_perm_)	(_perm_ & 4)	
#define ISSHARED(_perm_)		(_perm_ & 8)	



const char *gnamesNone = "None";
const char *gnamesByteProperty = "ByteProperty";
const char *gnamesIntProperty = "IntProperty";
const int gnamesSpacing = 11;					// excluding null terminator

//
// Find Gnames Array in memory using Gnames pattern
//   for the entries None and ByteProperty
// @param         pid: process id of the processs
// @param   memoryMap: memory maps list from /proc/<pid>/maps
// @param szMemoryMap: number of entries in the memory map
// @return value: 0 if not found, gnames array address if found
//---------------------------------------------------------------
uint64_t FindGnamesArray(int pid, MEMORY_MAP* memoryMap, int szMemoryMap) {

	FILE *fpMem;
	char memFile[50];
	char buff[4096];
	int  szBuff = 4096;

	int bhtNone[256];
	int bhtByteProperty[256];
	int bhtIntProperty[256];

	GenBhtTable(gnamesNone, bhtNone, sizeof(gnamesNone), 256);
	GenBhtTable(gnamesByteProperty, bhtByteProperty, sizeof(gnamesByteProperty), 256);
	GenBhtTable(gnamesIntProperty, bhtIntProperty, sizeof(gnamesIntProperty), 256);


	sprintf(memFile, "/proc/%d/mem", pid);

	fpMem = fopen(memFile, "rb");
	if(!fpMem) LOGe("Failed to open mem: %s", memFile);

	for(int i = 0; i < szMemoryMap; i++) {

		int perm = memoryMap[i].permissions;				// skip shared non readable maps and non writable
		if( !ISWRITABLE(perm) || !ISREADABLE(perm) || ISSHARED(perm)) continue;

		fseek(fpMem, memoryMap[i].base , SEEK_SET);
	
		szBuff = 4096;
		int64_t mapSize = memoryMap[i].end - memoryMap[i].base;
		int64_t searched = 0;

		while(searched < mapSize) {
			szBuff = ((mapSize - searched) > szBuff )? 4096: mapSize - searched;

			fread(buff, szBuff, 1 , fpMem);
			
			int none = BmpPatternMatch(gnamesNone, bhtNone, sizeof(gnamesNone), buff, szBuff);
			if(none < 0) { searched += szBuff; continue; } 

			int byteProp = BmpPatternMatch( gnamesByteProperty, bhtByteProperty, sizeof(gnamesByteProperty), buff, szBuff);
			if(byteProp < 0) { searched += szBuff; continue; }
			
			int intProp  = BmpPatternMatch(gnamesIntProperty, bhtIntProperty, sizeof(gnamesIntProperty), buff, szBuff );
			if(intProp < 0) { searched += szBuff; continue; }

			// This should not even work [id be surprised if it did] [oh yes it does work]
			// need to clean it
			// if we are here we lazily found Gnames array
			// please work on this commparison
			return memoryMap[i].base + searched + none - 8;
		}

		
	}

	return 0;
}

		//int none = BmpPatternMatch(gnamesNone, bhtNone, sizeof(gnamesNone), buff, sizeof(buff));
		//if(none < 0) continue;

		//int byteProp = BmpPatternMatch( gnamesByteProperty, bhtByteProperty, sizeof(gnamesByteProperty), 
		//	buff + sizeof(gnamesNone) + 1 + gnamesSpacing, 
		//	sizeof(buff) - sizeof(gnamesNone) - 1 - gnamesSpacing );
		//if(byteProp < 0) continue;
		
		//int intProp  = BmpPatternMatch(gnamesIntProperty, bhtIntProperty, sizeof(gnamesIntProperty),
		//	buff + sizeof(gnamesIntProperty) + 1 + gnamesSpacing, 
		//	sizeof(buff) - sizeof(gnamesIntProperty) - 1 - gnamesSpacing);
		//if(intProp < 0) continue;

//
// Find libUE4 from memory map
// Faster than memory search for pattern
// @param   memoryMap: memory maps list from /proc/<pid>/maps
// @param szMemoryMap: number of entries in the memory map
// @return value: 0 if not found, libUE4 base address if found
//-------------------------------------------------------------
uint64_t FindLibUE4(MEMORY_MAP* memoryMap, int szMemoryMap) {

	const char *libName = "libUE4.so";
	int szLibName = strlen(libName);

	for(int i = 0; i < szMemoryMap; i++) {

		if ( memoryMap[i].offset ) continue;

		int szPathName = strlen(memoryMap[i].pathname);
		int j = szPathName, k = szLibName;

		while( j>0 && k>0) {
			if(libName[k] != (memoryMap[i].pathname)[j]) break;

		 	j--; k--;
		}

		if ( (szPathName - j) == szLibName) return memoryMap[i].base;
	}

	return 0;
}