
#include "includes.h"


#define	ARG_PID		0

#define FREE_ARGS_AND_HELP_EXIT(_mem_, _args_)		{ free(_mem_); Help(_args_); }

//(*parsedArgs)[ARG_PID] = (char*)argv[i];
//#define ARG_VALUE(_argdef_, _value_, _argstore_)	(*parsedArgs)[ARG_PID] = (char*)argv[i];




// Here
//
int GetArguments(int argc, const char** argv, void** parsedArgs) {

	if (!argv || !parsedArgs) {
		printf("Null arguments parsed to GetArguments");
		exit(0);
	}

	if (argc <= 1)	Help(argv);

	*parsedArgs = malloc(4);

	// parse arguments
	int i = 1;
	for(; i < argc; i++ ){

		if (argv[i][0] != '-') FREE_ARGS_AND_HELP_EXIT(*parsedArgs, argv);

		switch((char)*(argv[i] + 1)) {
			case 'P':
			case 'p':
				{
					i++;
					if ( i >= argc ) FREE_ARGS_AND_HELP_EXIT(*parsedArgs, argv);
					
					(*(int**)parsedArgs)[ARG_PID] = atoi(argv[i]);
				}
				break;

			default:
				Help(argv);
		}


	}

	return 0;
}


//
//--------------------------------
void Help(const char** argv) {
	printf("\n\nUsage: %s [<flag> <value>]\n", argv[0] );
	printf("   ex: %s -p 4560 -t 100\n", argv[0] );
	printf("\n--------Flags--------\n");
	printf("    -h          : print this help message\n");
	printf("    -p <pid>    : pid of the game process\n");

	exit(0);
}