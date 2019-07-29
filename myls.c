//=============================================================================================
//Name:  Robin Ouzts
//Assignment:  HW2 (part: myls.c) - implementing 'ls' command with and without '-a' option
//                                  for use in mysh.c
//
//Class:  CS390
//Date:  03/26/19
//I attest that this work is entirely my own.
//=============================================================================================

#include <stdio.h>
#include <dirent.h>
#include <string.h>


int main(int argc, const char* argv[])
{

	DIR *dirPtr = opendir("."); // getting pointer to current directory
	struct dirent* practice = readdir(dirPtr); // giving readdir fxn pointer to curr directory
	// to be used for dirent struct, which represents
	// a directory entry in the current directory
	if(argc == 2 && strcmp(argv[1], "-a") == 0)
	{
		while(practice != NULL) // readdir will return NULL when there are no more dirent structs in the 
			// current directory
			// loop over all direct structures in the current directory
		{
			printf("%s\n", practice->d_name);
			practice = readdir(dirPtr);
		}
	}
	else if(argc == 1) // so no '-a' option. Therefore, don't show hidden files
	{
		while(practice != NULL)
		{
			if(practice->d_name[0] == '.') // if the file is a hidden file, don't show it 
				// for ls with no '-a' option
			{
				practice = readdir(dirPtr); // point to next entry but don't print the current one and continue
				continue;
			}
			printf("%s\n", practice->d_name); // if not a hidden file, show the filename, which is a POSIX
			// standard to be d_name
			practice = readdir(dirPtr); // point to next direct struct
		}
	}

	return 0;
}

