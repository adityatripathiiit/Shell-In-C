//  Defining constants 

#define MAXSIZE 1100
#define ARGMAX 20
#define WHITE "\x1B[0m"
#define GREEN "\x1b[92m"
#define BLUE "\x1b[94m"
#define RED "\x1b[41m"
#define RED_TEXT "\x1b[31m"


// Variable definations 

char cwd[MAXSIZE];    // variable to store the current path
char* argv[ARGMAX];   // argument values/vector  array 
int argcount = 0;     // argument count variable 
int isBackground = 0; // isBackground variable to maintain the state of program
char* input;          // Store the input from user 
int exitFlag = 0;     // exit flag, to exit from the terminal 
int rc ;              // To store rc value of child and terminate if required 
int running = 1;      // variable to handle while loop exits upon signals 
