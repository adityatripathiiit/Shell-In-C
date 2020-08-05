#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAXSIZE 1100
#define ARGMAX 10
#define WHITE "\x1B[0m"
#define GREEN "\x1b[92m"
#define BLUE "\x1b[94m"
#define RED "\x1b[41m"

void clear (); 
void initMessage();
void getPath();
void getInput(); 
void cdFunction();
void mkdirFunction();
void exitFunction();
void rmFunction(); 
void lsFunction(); 

char cwd[MAXSIZE];
char* argval[ARGMAX]; // our local argc, argv
int argcount = 0,isBackground = 0;
char* input; 
char* input_copy;
int exitFlag = 0; 

int main (){
    clear();
    initMessage(7);
    getPath(cwd,0);
    while(exitFlag != 1){
        isBackground = 0 ; 

        printf("%s%s%s $ ", GREEN,cwd,WHITE);
        getInput();

        // strcmp return 0 if strings/characters are a match
        if(strcmp(argval[0],"exit")==0){
            exitFunction();
        }
        else if(strcmp(argval[0],"ls")==0 && !isBackground){
            lsFunction(argval[1]);
        }
        // else if(strcmp(argval[0],"grep")==0 && !isBackground){
        //     grepFunction();
        // }
        // else if(strcmp(argval[0],"cat")==0 && !isBackground){
        //     catFunction();
        // }
        // else if(strcmp(argval[0],"mv")==0 && !isBackground){
        //     mvFunction();
        // }
        // else if(strcmp(argval[0],"cp")==0 && !isBackground){
        //     cpFunction();
        // }
        else if(strcmp(argval[0],"cd")==0 && !isBackground){            
            cdFunction(argval[1]);
        }

        else if(strcmp(argval[0],"pwd")==0 && !isBackground){
            getPath(cwd,1);
        }
        else if(strcmp(argval[0],"rm")==0 && !isBackground){
            rmFunction(argval[1]);
        }
        // else if(strcmp(argval[0],"chmod")==0 && !isBackground){
        //     chmodFunction();
        // }
        else if(strcmp(argval[0],"mkdir")==0 && !isBackground){
            mkdirFunction(argval[1]);
        }
        // else if(strcmp(argval[0],"touch")==0 && !isBackground){
        //     touchFunction();
        // }

        

    }

}


    

void clear(){

 // Method 1  : https://stackoverflow.com/questions/2347770/how-do-you-clear-the-console-screen-in-c
//   const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
//   write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);

  // Method 2:  Takes more time than the regex method
    system("clear"); 
}

//function to print the very initial message
void initMessage(int n) 
{ 
    for (int i = 0; i < n; i++) { 
        for (int j = 0; j <= n / 2; j++) { 
            if ((j == 0 || j == n / 2) && i != 0 || i == 0 && j != 0 && j != n / 2 || i == n / 2) 
                printf("*"); 
            else
               printf(" "); 
        } 
        if(i == n/2) printf(" DITYA's Shell ");
        printf("\n"); 
    } 
} 

// function to get the users current path 
// and update the string with it
void getPath(char* cwd,int flag)
{
    char temp[MAXSIZE];
    //using getcwd function to get current directory path
    char* path = getcwd(temp, sizeof(temp));
    if(path != NULL){
        strcpy(cwd,temp);
        // check if pwd is to be printed
        if(flag == 1){  
            printf("%s\n",cwd);
        }
    }
    // using perror function to print error in the console
    else perror("---- Unable to getcwd() : ");

}

//  get input containing spaces and tabs and store it in argval 
// character array 
void getInput()
{
    fflush(stdout); // clear all previous buffers if any
    input = NULL;
    // size_t variable type, which is a special type of integer.
    // That’s required by the getline() function
    size_t size = 0;
    getline(&input,&size,stdin);

    // Copy into another string if we need to run special executables

    input_copy = (char *)malloc(strlen(input) * sizeof(char));
    strncpy(input_copy,input,strlen(input));
    argcount = 0;
    isBackground = 0;

    while(argcount < ARGMAX-1 && (argval[argcount] = strsep(&input, " \t\n")) != NULL)
    {
        // do not consider "" as a parameter
        if(sizeof(argval[argcount])==0)
        {
            free(argval[argcount]);
        }
        else argcount++;  // increasing the count of the index which store the ith argument
        if(strcmp(argval[argcount-1],"&")==0)
        {
            isBackground = 1; //run in in Background
            return;  // return to the caller
        }
    }
    free(input);
}

void lsFunction(char* folderName){
    if(folderName) printf("SSS \n");
    else printf("Salsa \n");
}

void cdFunction(char * newPath){    
    int result = chdir(newPath);
    if(result == 0) getPath(cwd,0);
    else perror(" Error: Can't change directory"); 
}


void mkdirFunction(char* folderName){
    int answer = mkdir(folderName, 0777);// all appropriate permissions
    if(answer !=-1) printf(" successfully created directory %s %s %s \n", BLUE,folderName ,WHITE);
    else perror(" Error: Cannot make directory ");
}

void rmFunction(char* folderName){
    int answer = rmdir(folderName);
    if(answer !=-1) printf(" successfully deleted directory %s %s %s \n", BLUE,folderName ,WHITE);
    else perror(" Error: Cannot delete directory ");
}


// function to initiate an exit from the shell
void exitFunction(){
    exitFlag = 1;
    printf("%s It was nice having you! Bye! %s \n", RED, WHITE); 
}