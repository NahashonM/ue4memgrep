
#include "includes.h"


int main(int argc, const char**argv) {

	void* parsedArgs;
	GetArguments(argc, argv, &parsedArgs);

	int pid, numMaps;
	MEMORY_MAP memoryMaps[4096];

	pid =  FindProcessByName(processName, strlen(processName) );
	numMaps = GetProcessMemoryMaps(pid, memoryMaps);

	uint64_t libUE4 = FindLibUE4(memoryMaps, numMaps);
	uint64_t gnamesArr = FindGnamesArray(pid, memoryMaps, numMaps);

	printf("LibUE4 Address = %llx \n", libUE4);
	printf("Gnames Address = %llx \n", gnamesArr);


	printf("pid = %d\n", pid );
	printf("numMaps = %d\n", numMaps );

	printf("%d\n", ((int*)parsedArgs)[0]);
}

