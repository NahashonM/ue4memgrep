#include "includes.h"


bool IsInt(const char* str, int argc){
	for(int i=0; i < argc; i++) 
		if ( !isdigit(str[i]) ) 
			return false;

	return true;
}


//	/proc/<pid>/status
int FindProcessByName(const char* processName, int szProcessName ) {

	DIR *pProc = opendir("/proc");					// Open /proc dir
	if(!pProc) { LOGe("Reading %s failed", "/proc"); exit(0); }

	int pid = 0;
	struct dirent *ep;								// enum /proc files
	while((ep = readdir(pProc)) ){
		if (IsInt(ep->d_name, strlen(ep->d_name))){	// Only interested in processes

			FILE *fp;
			char path[260];
			int szFileName = 0, curr_char = 0;
			sprintf(path, "/proc/%s/status", ep->d_name);

			fp = fopen(path, "r");
			if(!fp) { LOGe("Reading %s failed", path ); continue; }

			curr_char = getc(fp);
			while( (curr_char != '\n') && (curr_char != EOF) ) {	// read name into buffer
				path[szFileName] = curr_char;
				szFileName++;
				curr_char = getc(fp);
			}
			path[szFileName] = '\0';

			bool match = true;										// Compare names 
			for(int i = szProcessName, j = szFileName; i>=0 && j>=0; i--, j--) {
				if (processName[i] != path[j]) {					// not perfect but does for now
					match = false;
					break;
				}
			}

			if (match) {											// add multiple matches here
				pid = atoi(ep->d_name);								// for now we want only one
				LOG("found process %s pid %d\n", path, pid );	
				fclose(fp);
				break;
			}

			fclose(fp);
		}
	}

	closedir(pProc);
	return pid;
}



// /proc/3650/mem
// /proc/3650/maps
// /proc/3650/pagemap
// get memory maps
int GetProcessMemoryMaps(int pid, MEMORY_MAP* memoryMaps) {
	if (!memoryMaps)
		LOGe("Null pointer passed to function","");

	FILE *fpMaps;
	char memMapsPath[50], buffer[260];
	int numMaps, curr_char, line_size;

	sprintf(memMapsPath, "/proc/%d/maps", pid);

	fpMaps = fopen(memMapsPath, "r");
	if(!fpMaps) { LOGe("Reading %s failed", memMapsPath ); exit(0); }

	numMaps = line_size=0;
	curr_char = getc(fpMaps);

	while( curr_char != EOF ) {			// start reading the mapping file line by line
		if (curr_char == '\n') {
			buffer[line_size] = '\0';	// do something after reading line

			//-------------------------------------------------------------------------
			char read = 0, write = 0, execute = 0, sharing = 0;

			sscanf( buffer, "%lx-%lx %c%c%c%c %lx %x:%x %ld        %s", 
				&memoryMaps[numMaps].base,
				&memoryMaps[numMaps].end,
				&read, &write, &execute, &sharing,
				&memoryMaps[numMaps].offset,
				&memoryMaps[numMaps].major_dev,
				&memoryMaps[numMaps].minor_dev,
				&memoryMaps[numMaps].inode,
				&memoryMaps[numMaps].pathname );

			memoryMaps[numMaps].permissions = (read * 1) ||
											  (write * 2) || 
											  (execute * 4) || 
											  ((sharing == 's') * 8);

			numMaps++;

			//-------------------------------------------------------------------------
			line_size = 0;				//reset and start reading next line
			curr_char = getc(fpMaps);
			continue;
		}

		buffer[line_size] = curr_char;
		line_size++;

		curr_char = getc(fpMaps);
	}

	fclose(fpMaps);
	return numMaps;
}

