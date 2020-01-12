/** IZP Project 1
 * Ondrej Sedlacek [xsedla1o], Autumn 2019
 *
 * Text processing
 *  Input: 
 *   List of contacts, each contact entry has 2 lines, name and phone
 *   number, maximum length of line is 100 characters, given as stdin.
 *   
 *   Pattern of numbers representing keystrokes on a telephone, 
 *   given as argument.
 *  
 *  Output: 
 *   List of contacts matching the filter pattern, or a "Not Found" msg.
 *   If no filter is given, all contacts are matched. Outputs to stdout.
 */

#include <stdio.h>
#include <stdbool.h>


// Definitions
#define LIMIT 101 // 100 char limit + 1 terminating char

#define ERR_OK 0
#define ERR_WRONG_INPUT 1
#define ERR_INPUT_TOO_LONG 2
#define ERR_TOO_MANY_ARGS 3


// Declaration of functions
int loadFilter(int argc, char *arg, int *filter, int *filterLen, bool *noFilter);
int loadContact(char *name, char *num);
int readLineToLower(char *str);
bool matchContact(char *name, char *num, int *filter, int filterLen);
bool containsSeeked(char *arr, int *filter, int filterLen);
bool onKey(int key, char c);


/** Main program */
int main(int argc, char **argv){
	// Intialization
	bool noFilter = false;
	bool contactFound = false;
	int filter[LIMIT];
	int filterLen = 0;

	// Loads filter pattern from program args
	if(loadFilter(argc, argv[1], filter, &filterLen, &noFilter) != ERR_OK)
		return 1;

	// Iterates over contacts and decides if they match the pattern
	char name[LIMIT];
	char num[LIMIT];
	int lineStatus; 
	while((lineStatus = loadContact(name, num)) == ERR_OK){
		if(noFilter || matchContact(name, num, filter, filterLen)){
			contactFound = true;
			printf("%s, %s\n", name, num);
		}
	}

	// On reading error
	if(lineStatus != EOF)
		return 1;

	if(!contactFound)
		printf("Not found\n");

	// End
	return 0;
}


/**
 * Function loads filter pattern from args
 * @argc	number of arguments
 * @arg		array containing first argument
 * @filter	output, string for the pattern
 * @filterLen	output, length of filter
 * @noFilter	output, true in case of no user args
 */
int loadFilter(int argc, char *arg, int *filter, int *filterLen, bool *noFilter){
	if(argc == 2){
		while(arg[*filterLen] != '\0'){
			if(arg[*filterLen] > '9' || arg[*filterLen] < '0'){ // Not a Number
				fprintf(stderr, "Error, incorrect filter input.\n");
				return ERR_WRONG_INPUT;
			}
			// Convert char to int
			filter[*filterLen] = arg[*filterLen] - '0'; // Due to ASCII
			(*filterLen)++;
			if(*filterLen == LIMIT){
				fprintf(stderr, "Error, filter input too long.\n");
				return ERR_INPUT_TOO_LONG;
			}
		}
	} else if(argc == 1){ // No filter given
		*noFilter = true;
	} else{
		fprintf(stderr, "Error, too many arguments.\n");
		return ERR_TOO_MANY_ARGS;
	}
	return ERR_OK;
}


/**
 * Function loads contact from stdin using readLineToLower and handles errors.
 * @param name	name will be outputed here,	constraints: none
 * @param num	number will be outputed here,	constraints: [0-9],' ','+'
 */ 
int loadContact(char *name, char *num){
	// Load name
	int lineStatus = readLineToLower(name);
	if(lineStatus == EOF){
		return EOF;
	}
	if (lineStatus == ERR_INPUT_TOO_LONG){
		fprintf(stderr, "Error, contact input too long\n");
		return ERR_INPUT_TOO_LONG;
	}

	// Load number
	lineStatus = readLineToLower(num);
	if(lineStatus == EOF){
		fprintf(stderr, "Error, incomplete contact\n");
		return ERR_WRONG_INPUT;
	}
	if (lineStatus == ERR_INPUT_TOO_LONG){
		fprintf(stderr, "Error, contact input too long\n");
		return ERR_INPUT_TOO_LONG;
	}
	for(int i = 0; num[i] != '\0'; i++){
		if((num[i] > '9' || num[i] < '0') && num[i] != ' ' && num[i] != '+'){
			fprintf(stderr, "Error, given number contains forbidden characters\n");
			return ERR_WRONG_INPUT;
		}
	}
	return ERR_OK;



/**
 * Function reads a line from stdin and makes it lowercase.
 * @param str	output string
 */
int readLineToLower(char *str){
	int i = 0;
	char c = 0;

	while(i <= LIMIT - 1){
		if((c=getchar()) == EOF){
			return EOF;
		} else if (c == '\n'){
			str[i] = '\0';
			return ERR_OK;
		} else {
			if(c >= 'A' && c <= 'Z'){
			 	str[i++] = c + ('a' - 'A'); // Due to ASCII
			}
			else{
				str[i++] = c;
			}
		}
	}
	
	// The line is too long
	str[LIMIT - 1] = '\0';
	return ERR_INPUT_TOO_LONG;
}


/**
 * Function returns if the given contact contains seeked pattern
 * @param name		contact's name
 * @param num		contact's number
 * @param filter	represents pattern of pressed keyboard keys
 * @param filterLen	length of filter array
 * @return Returns true when finds pattern, else false.
 */
bool matchContact(char *name, char *num, int *filter, int filterLen){
	return(containsSeeked(name, filter, filterLen) || containsSeeked(num, filter, filterLen));
}


/**
 * Function returns if the given char array contains seeked pattern
 * @param arr		char array
 * @param filter	represents pattern of pressed keyboard keys
 * @param filterLen	length of filter array
 * @return Returns true when finds pattern, else false.
 */
bool containsSeeked(char *arr, int *filter, int filterLen){
	for(int i = 0; arr[i+1] != '\0'; i++){
		// fi - filter index, skipped - number of skipped chars
		// continues if filter is not completed
		// AND if terminating char in arr was not reached
		for(int fi = 0, skipped = 0;
			fi < filterLen && arr[i+fi+skipped] != '\0';){
			if(onKey(filter[fi], arr[i+fi+skipped])){
				if(fi+1 == filterLen){
					return true;
				}
				fi++;
			} else {
				skipped++;
			}
		}
	}
	return false;
}
/**
 * Function decides if char is on given key.
 * @param key 	[0-9], represents key number.
 * @param c	[a-z],'+'
 * @return Returns if char is on the key.
 */
bool onKey(int key, char c){
	// Each string in keys[] represents letters on key equal to its index
	char* keys[] = {"0+",
	       	"1", "2abc","3def",
		"4ghi","5jkl","6mno",
	       	"7pqrs", "8tuv", "9wxyz"};
	for(int i = 0; keys[key][i] != '\0'; i++){
		if(c == keys[key][i])
			return true;
	}
	return false;
}
