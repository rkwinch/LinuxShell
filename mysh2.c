//=====================================================================
//Name:  Robin Ouzts
//Assignment:  HW2 (part:  mysh2.c) - implementing fork, exec, and wait in the basic 
//                   shell in C for HW1
//Class:  CS390
//Date:  03/26/19
//I attest that this work is entirely my own.
//=====================================================================

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#define SIZE  2048

void getInputLine(char* input);
void freeWordsInInput(char** wordsInInput, int size);
void memoryAllocateArrayOfStrings(char ** arrayOfStrings, int size);
void printInput(char** wordsInInput, int numberOfWords);
void parseInputLine(char* input, char** wordsInInput, int* count);
void printPrompt(char* prompt);
int isEcho(char** wordsInInput, int numberOfWords, int* errorFlag);
int isPS1(char** wordsInInput, int numberOfWords, int* errorFlag);
int isCat(char** wordsInInput, int numberOfWords,int* errorFlag);
int isCp(char** wordsInInput, int numberOfWords, int* errorFlag);
int isRm(char** wordsInInput, int numberOfWords, int* errorFlag);
int isRmdir(char** wordsInInput, int numberOfWords, int* errorFlag);
int isMkdir(char** wordsInInput, int numberOfWords, int* errorFlag);
int isPrintHelp(char** wordsInInput, int numberOfWords, int *errorFlag);
void echoCommand(char** wordsInInput, int numberOfWords);
void catCommand(char** wordsInInput, int numberOfWords);
void PS1Command(char** wordsInInput, char* prompt);
void cpCommand(char** wordsInInput);
void rmCommand(char** wordsInInput);
void rmdirCommand(char** wordsInInput);
void mkdirCommand(char** wordsInInput);
void printHelpCommand();
int exec(char** wordsInInput, int numberOfWords);



int main(int argc, char** argv, char** env)
{
	char inputLine[SIZE] = "";
	char prompt[SIZE] = "$";
	char* wordsInInput[SIZE];
	int numberOfWords = 0;
	memoryAllocateArrayOfStrings(wordsInInput, SIZE);
	printPrompt(prompt);
	getInputLine(inputLine);
	parseInputLine(inputLine, wordsInInput, &numberOfWords);
	while(strcmp(wordsInInput[0], "exit") != 0)
	{
		int errorFlag = 0; // if a valid command entered but 
		// error occurred, such as incorrect
		// number of arguments, success = 0, failure = -1
		if(numberOfWords == 0) // if no arguments given, do nothing 
		{
		}
		else if(isEcho(wordsInInput, numberOfWords, &errorFlag))
		{
			echoCommand(wordsInInput, numberOfWords);
		}
		else if(isCat(wordsInInput, numberOfWords, &errorFlag))
		{
			catCommand(wordsInInput, numberOfWords);
		}
		else if(isPS1(wordsInInput, numberOfWords, &errorFlag))
		{
			PS1Command(wordsInInput, prompt);
		}
		else if(isCp(wordsInInput, numberOfWords, &errorFlag))
		{
			cpCommand(wordsInInput);
		}
		else if(isRm(wordsInInput, numberOfWords, &errorFlag))
		{
			rmCommand(wordsInInput);
		}
		else if(isRmdir(wordsInInput, numberOfWords, &errorFlag))
		{
			rmdirCommand(wordsInInput);
		}
		else if(isMkdir(wordsInInput, numberOfWords, &errorFlag))
		{
			mkdirCommand(wordsInInput);
		}
		else if(isPrintHelp(wordsInInput, numberOfWords, &errorFlag))
		{
			printHelpCommand();
		}
		else
		{
			exec(wordsInInput, numberOfWords);
		}
		printPrompt(prompt);
		getInputLine(inputLine);
		parseInputLine(inputLine, wordsInInput, &numberOfWords);
	}
		freeWordsInInput(wordsInInput, SIZE);
	return 0;
}

// function which gets a line input by the user 
void getInputLine(char* input)
{
	strcpy(input, "");
	size_t size = SIZE; // size_t because getline fxn requires &(size_t)
	getline((char**)&input, &size, stdin);
	input[strlen(input) - 1] = '\0'; // replaces \n from input with null term.
}

// freeing memory that was allocated for each element in wordsInInput
void freeWordsInInput(char** wordsInInput, int size)
{
	for(int i = 0; i < size; i++)
	{
		free(wordsInInput[i]);
	}
}
// function parses inputLine by spaces
void parseInputLine(char* input, char** wordsInInput, int* count)
{
	*count = 0;
	for (int i = 0; i < SIZE; i++) 
	{
		strcpy(wordsInInput[i], "");
	}
	char *pch = strtok(input, " ");
	while (pch != NULL) 
	{
		strcpy(wordsInInput[*count], pch);
		(*count)++;
		pch = strtok(NULL, " ");
	}
} // end of parse fxn

// allocate memory in each element in array of char*
void memoryAllocateArrayOfStrings(char ** arrayOfStrings, int size)
{
	for(int i = 0; i < size; i++)
	{
		arrayOfStrings[i] = (char*)malloc(sizeof(char) * size);
	}
} // end of MemoryAllocateWordsInInput fxn

// print input from user
void printInput(char** wordsInInput, int numberOfWords)
{
	for(int i = 0; i < numberOfWords; i++)
	{
		if(i != (numberOfWords - 1))
		{
			printf(" ");
		}
	}
	printf("\n");
} // end of printInput fxn


// determines if command is "echo" and is valid command
int isEcho(char** wordsInInput, int numberOfWords, int* errorFlag)
{
	if(strcmp(wordsInInput[0], "echo") == 0 && numberOfWords == 1)
	{
		printf("Incorrect echo command.  Too few arguments.\n");
		*errorFlag = 1;
		return 0;
	}
	if((strcmp(wordsInInput[0], "echo") == 0))
	{
		return 1;
	}
	return 0;
} // end of isEcho fxn


// prints string input to screen
void echoCommand(char** wordsInInput, int numberOfWords)
{
	if(strcmp(wordsInInput[1], "-n") == 0)
	{
		for(int i = 2; i < numberOfWords; i++)
		{
			printf("%s", wordsInInput[i]);
			if(i != (numberOfWords - 1))
			{
				printf(" ");
			}
		}
	}
	else
	{
		for(int i = 1; i < numberOfWords; i++)
		{
			printf("%s", wordsInInput[i]);
			if(i != (numberOfWords - 1))
			{
				printf(" ");
			}
		}
		printf("\n");

	}
} // end of echoCommand fxn

// prints contents of a file to the screen
void catCommand(char** wordsInInput, int numberOfWords)
{
	size_t size = SIZE; // size_t because getline fxn requires &(size_t)
	char* input = NULL;	
	if(numberOfWords > 2)
	{
		printf("Incorrect cat command.  Too many arguments.\n");
		return;
	}
	FILE *filePtr;
	filePtr = fopen(wordsInInput[1], "r+");
	if(filePtr == NULL)
	{
		printf("%s\n", wordsInInput[1]);
		printf("The file did not open correctly.\n");
		return;
	}
	while(getline((char**)&input, &size, filePtr) != -1)
	{
		printf("%s", input);
	}
} // end of catCommand fxn

// checks if command is "cat" and is valid
int isCat(char** wordsInInput, int numberOfWords, int* errorFlag)
{
	if((strcmp(wordsInInput[0], "cat") == 0) && numberOfWords > 2)
	{
		*errorFlag = 1;
		printf("Incorrect cat command.  Too many arguments.\n");
		return 0;
	}
	if((strcmp(wordsInInput[0], "cat") == 0) && numberOfWords < 2)
	{
		*errorFlag = 1;
		printf("Incorrect cat command.  Too few arguments.\n");
		return 0;
	}
	if(strcmp(wordsInInput[0], "cat") == 0)
	{
		return 1;
	}
	return 0;
} // end of isCat fxn

// checks if command is "PS1" and is valid
int isPS1(char** wordsInInput, int numberOfWords, int *errorFlag)
{
	if((strcmp(wordsInInput[0], "PS1") == 0) && numberOfWords < 2)
	{
		*errorFlag = 1;
		printf("Incorrect PS1 command.  Too few arguments.\n");
		return 0;
	}
	if((strcmp(wordsInInput[0], "PS1") == 0) && numberOfWords > 2)
	{
		*errorFlag = 1;
		printf("Incorrect PS1 command.  Too many arguments.\n");
		return 0;
	}
	if(strcmp(wordsInInput[0], "PS1") == 0)
	{
		return 1;
	}
	return 0;
} // end of isPS1 fxn

// prints current prompt, default is "$"
void printPrompt(char* prompt)
{
	// Add space for convenience
	printf("%s ", prompt);
} // end of printPrompt fxn

// changes prompt to desired string (no spaces)
void PS1Command(char** wordsInInput, char* prompt)
{
	strcpy(prompt, wordsInInput[1]);
} // end of PS1Command fxn

// checks if command is "cp" and is valid
int isCp(char** wordsInInput, int numberOfWords, int* errorFlag)
{
	if((strcmp(wordsInInput[0], "cp") == 0) && numberOfWords > 3)
	{
		*errorFlag = 1;
		printf("Incorrect cp command.  Too many arguments.\n");
		return 0;
	}
	if((strcmp(wordsInInput[0], "cp") == 0) && numberOfWords < 3)
	{
		*errorFlag = 1;
		printf("Incorrect cp command.  Too few arguments.\n");
		return 0;
	}
	if((strcmp(wordsInInput[0], "cp") == 0))
	{
		return 1;
	}
	return 0;
} // end of isCp fxn

// copies contents of one file into another that is created
// (doesn't work on directories)
void cpCommand(char** wordsInInput)
{
	size_t size = SIZE; // size_t because getline fxn requires &(size_t)
	char* input = NULL;	
	FILE *fileInPtr;
	FILE *fileOutPtr;
	fileInPtr = fopen(wordsInInput[1], "r");
	fileOutPtr = fopen(wordsInInput[2], "w");
	if(fileInPtr == NULL)
	{
		printf("The file did not open correctly.\n");
		return;
	}
	if(fileOutPtr == NULL)
	{
		printf("The file could not be made.\n");
	}
	while(getline((char**)&input, &size, fileInPtr) != -1)
	{
		fputs(input, fileOutPtr);
	}
	fclose(fileInPtr);
	fclose(fileOutPtr);
} // end of cpCommand fxn

// checks if command is "rm" and is valid
int isRm(char** wordsInInput, int numberOfWords, int *errorFlag)
{
	if(strcmp(wordsInInput[0], "rm") == 0 && numberOfWords >2)
	{
		*errorFlag = 1;
		printf("Incorrect rm command.  Too many arguments.\n");
		return 0;
	}
	if(strcmp(wordsInInput[0], "rm") == 0 && numberOfWords < 2)
	{
		*errorFlag = 1;
		printf("Incorrect rm command.  Too few arguments.\n");
		return 0;
	}
	if(strcmp(wordsInInput[0], "rm") == 0)
	{
		return 1;
	}
	return 0;
} // end of isRm fxn

// removes a file(not a directory)
void rmCommand(char** wordsInInput)
{
	int failure = -1; // unlink will return 0 if success and -1 if failure
	failure = unlink(wordsInInput[1]);
	if(failure)
	{
		printf("rm command did not work properly.\n");
	}
} // end of rmCommand fxn

// checks if command is "rmdir" and is valid
int isRmdir(char** wordsInInput, int numberOfWords, int* errorFlag)
{
	if((strcmp(wordsInInput[0], "rmdir") == 0) && numberOfWords > 2)
	{
		*errorFlag = 1;
		printf("Incorrect rmdir command.  Too many arguments.\n");
		return 0;
	}
	if((strcmp(wordsInInput[0], "rmdir") == 0) && numberOfWords < 2)
	{
		*errorFlag = 1;
		printf("Incorrect rmdir command.  Too few arguments.\n");
		return 0;
	}
	if((strcmp(wordsInInput[0], "rmdir") == 0))
	{
		return 1;
	}
	return 0;
} // end of isRmdir fxn

// removes an empty directory
void rmdirCommand(char** wordsInInput)
{
	int failure = -1;
	failure = rmdir(wordsInInput[1]);
	if(failure)
	{
		printf("rmdir command did not work properly.\n");
	}
} // end of rmdirCommand fxn

// checks if command is "mkdir" and is valid
int isMkdir(char** wordsInInput, int numberOfWords, int* errorFlag)
{
	if((strcmp(wordsInInput[0], "mkdir") == 0) && numberOfWords > 2)
	{
		*errorFlag = 1;
		printf("Incorrect mkdir command.  Too many arguments.\n");
		return 0;
	}
	if((strcmp(wordsInInput[0], "mkdir") == 0) && numberOfWords < 2)
	{
		*errorFlag = 1;
		printf("Incorrect mkdir command.  Too few arguments.\n");
		return 0;
	}
	if((strcmp(wordsInInput[0], "mkdir") == 0))
	{
		return 1;
	}
	return 0;
} // end of isMkdir fxn

// makes an empty directory
void mkdirCommand(char** wordsInInput)
{
	int failure = -1;
	failure = mkdir(wordsInInput[1], 0777);
	if(failure)
	{
		printf("mkdir command did not work properly.\n");
	}
} // end of mkdirCommand fxn

// checks if command is "help" and is valid
int isPrintHelp(char** wordsInInput, int numberOfWords, int *errorFlag)
{
	if((strcmp(wordsInInput[0], "help") == 0) && numberOfWords != 1)
	{
		*errorFlag = 1;
		printf("Try typing \"help\".");
		return 0;
	}
	if((strcmp(wordsInInput[0], "help") == 0))
	{
		return 1;
	}
	return 0;
} // end of isPrintHelp fxn

// prints all commands, what their requirements are, and a brief 
// description of each
void printHelpCommand()
{
	printf("NAME\n    cat - prints file to screen\nSYNOPSIS\n    cat  <filename>\n");
	printf("DESCRIPTION\n    prints contents of a file (not directory) to screen\n\n");
	printf("NAME\n    cp - copies a file\nSYNOPSIS\n    cp  <SOURCE>  <DESTINATION>\n");
	printf("DESCRIPTION\n    copies SOURCE to DESTINATION (a file, not directory)\n\n");
	printf("NAME\n    echo - prints to screen\nSYNOPSIS\n    echo  <string>\n");
	printf("DESCRIPTION\n    prints string input to screen\n\n");
	printf("NAME\n    rm - removes a file\nSYNOPSIS\n    rm  <filename>\n");
	printf("DESCRIPTION\n    removes a file(not a directory) from the file system\n\n");
	printf("NAME\n    mkdir - creates a directory\nSYNOPSIS\n    mkdir  <directory name>\n");
	printf("DESCRIPTION\n    creates an empty directory\n\n");
	printf("NAME\n    rmdir - removes a directory\nSYNOPSIS\n    rmdir  <directory name>\n");
	printf("DESCRIPTION\n    removes an empty directory from the file system\n\n");
	printf("NAME\n    exit - exits shell\nSYNOPSIS\n    exit\n");
	printf("DESCRIPTION\n    exits the current shell\n\n");
	printf("NAME\n    help - prints help information\nSYNOPSIS\n    help\n");
	printf("DESCRIPTION\n    prints list of commands, how they are used, and a brief description\n");
	printf("    to the screen\n");
} // end of printHelpCommand fxn

//implements command called 'exec' to use 'ls' command or run another process
int exec(char** wordsInInput, int numberOfWords)
{
	int status;
	pid_t pid;
	char* env = NULL;
	char* path = NULL;
	char** arguments = (char**) malloc(sizeof(char*) * (numberOfWords + 1)); // allocating for each element
	for(int i = 0; i < (numberOfWords); i++) // allocating for space within each element with null term.
	{
		arguments[i] = (char*) malloc(sizeof(char) * (strlen(wordsInInput[i]) + 1));
		strcpy(arguments[i], wordsInInput[i]); // may as well copy the elements in this loop too
	}
	arguments[numberOfWords] = NULL; // making last element NULL to use in execv fxn
	//---------------------------------------------------------------------------------------
	// handling if path is not an absolute path
	if((arguments[0][0] != '/') && (arguments[0][0] != '.'))// if path is not an absolute path but a simple program name
	{
		env = (char*) malloc(sizeof(int) * strlen(getenv("PATH"))); // allocating mem for env
		strcpy(env, getenv("PATH"));
		char *pch = strtok(env, ":");
		while (pch != NULL) 
		{
			// allocate memory for path to accomodate size of path token and program name
			// in addition to one char space for '/' and null term
			path = (char*) malloc((sizeof(char) * strlen(pch)) + (sizeof(char) * (strlen(arguments[0]))) + 2);
			strcpy(path, pch); // copying path token first
			strcat(path, "/"); // adding a '/' to the end before adding the file name
			strcat(path, arguments[0]); // concatenating file name to end of path token
			if(!access(path, X_OK)) // checking if file exists. access(...) will return 0 if 
				//file is found and -1 if an error occurred, so checking if file was found 
		//X_OK as mode to see if file exists using access fxn in unistd.h 
			{
				break;
			}
			pch = strtok(NULL, ":");
			if(path)
			{
				free(path);
				path = NULL;
			}
		}
		if(pch == NULL) // command not found. return -1 for failure
		{
			printf("Command not found.\n");
			if(env)
			{
				free(env);
				env = NULL;
			}
			if(path)
			{
				free(path);
				path = NULL;
			}
			for(int i = 0; i < (numberOfWords + 1); i++)
			{
				free(arguments[i]);
			}
			free(arguments);
			return -1;
		}
		free (arguments[0]); // freeing first element since I want to rewrite it with path
		arguments[0] = (char*) malloc(sizeof(char*) * strlen(path));
		strcpy(arguments[0], path); // making first element in arguments the absolute path
		// which will making calling execv, later, more uniform
	}
	//----------------------------------------------------------------------------------------

	pid = fork();
	if(pid == 0)
	{
		execv(arguments[0], arguments);
	}
	else if(pid > 0)
	{
		wait(&status); // waiting for child to finish
	}
	else
	{
		printf("Error.  Negative pid.\n"); // if fork didn't work correctly
		if(env)
		{
			free(env);
			env = NULL;
		}
		if(path)
		{
			free(path);
			path = NULL;
		}
		for(int i = 0; i < numberOfWords; i++)
		{
			free(arguments[i]);
		}
		free(arguments);
		return -1; // for failure here
	}
	if(env)
	{
		free(env);
		env = NULL;
	}
	if(path)
	{
		free(path);
		path = NULL;
	}
	for(int i = 0; i < numberOfWords; i++)
	{
		free(arguments[i]);
	}
	free(arguments);
	return 0; // for success on completing exec for found command
}

