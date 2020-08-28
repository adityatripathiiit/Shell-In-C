// Including header files 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/wait.h>
#include<fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <libgen.h>

//  Defining constants 

#define MAXSIZE 1100
#define ARGMAX 20
#define WHITE "\x1B[0m"
#define GREEN "\x1b[92m"
#define BLUE "\x1b[94m"
#define RED "\x1b[41m"
#define RED_TEXT "\x1b[31m"

// Function definations  

void clear (); 
void initMessage();
void getPath();
int  selector(); 
void getInput(); 
void cdFunction();
void mkdirFunction();
void exitFunction();
int  rmFunction(); 
void lsFunction(); 
void printContent();
void cpFunction();
void mvFunction(); 
void catFunction(); 
void touchFunction(); 
void chmodFunction(); 
void grepFunction(); 
void inbuiltFunction(); 
void killChild();

// Variable definations 

char cwd[MAXSIZE]; // variable to store the current path
char* argval[ARGMAX]; // argument values/vector  array 
int argcount = 0;     // argument count variable 
int isBackground = 0; // isBackground variable to maintain the state of program
char* input;          // Store the input from user 
int exitFlag = 0;     // exit flag, to exit from the terminal 
int rc ;              // To store rc value of child and terminate if required 
int running = 1;      // variable to handle while loop exits upon signals 


int main (){
    // checking for signal Interrupt and calling killChild function if true
    signal(SIGINT,killChild); 
    // calling clear function to clear the terminal 
    clear(); 
    // Printing initial message on the terminal 
    initMessage(7);

    // getting the current path
    getPath(cwd,0);

    // while we do not exit the terminal
    while(exitFlag != 1){
        
        isBackground = 0 ; 

        
        printf("%s%s%s $ ", GREEN,cwd,WHITE); // printing the current path in the terminal
        getInput(); // function to get the input from the user 
        int selectorFlag = selector(); 
        // If enterted command not any command in the selector
        if(selectorFlag == 0 && *argval[0]!= '\0') {
            inbuiltFunction(); 
        }

    }

}


// Kill any process if its not the parent, i.e the shell itself by sending signal interupt 
// Send signal interupt using ctrl+c
void killChild(){
    if(rc!=0){
        kill(rc, SIGINT);
        rc = 0;
    }
}

// function to handle while loop exits on signal 
void handleWhile(){
    running = 0; 
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
    else perror("Unable to getcwd() : ");

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
        if(strcmp(argval[argcount-1],"&")==0) // if the last argument is &
        {   
            isBackground = 1; //run in in Background
            return;  // return to the caller
        }
    }
    free(input); // freeing the assigned memory
}

// Function to compare the input commands 

int selector(){

        // The value of this flag is set to 1, if the entered command 
        // is from the custom commands that are implemented
        // If so, then they are executed, otherwise it checks inbuilt binaries
        int selectorFlag = 0 ;

        // strcmp return 0 if strings/characters are a match
        if(strcmp(argval[0],"exit")==0){
                selectorFlag = 1;
                exitFunction();
            }
        else if(strcmp(argval[0],"ls")==0 && !isBackground){
            selectorFlag = 1;
            if(argcount == 2) lsFunction(argval[1]); 

            else for(int i= 1; i<argcount-1; i++) lsFunction(argval[i]);
        }
        else if(strcmp(argval[0],"grep")==0 && !isBackground){
            selectorFlag = 1;
            if(*argval[1] == '\0') return 1;
            for(int i= 2; i<ARGMAX; i++){
                if(strcmp(argval[1], "\"\"")==0 ||strcmp(argval[1], "\"")==0 ){
                    printf("Nothing is matched \n");
                    return 1 ; 
                }
                else if( i == 2 &&*argval[i] == '\0' && *argval[1] != '\0' ) grepFunction(argval[1], argval[i]) ;
                else if( *argval[i] != '\0' && *argval[1] != '\0' ) grepFunction(argval[1], argval[i]);
                else break ; 
            }
        }
        else if(strcmp(argval[0],"cat")==0 && !isBackground){
            selectorFlag = 1;
            for(int i= 1; i<ARGMAX; i++){
                if( *argval[i] != '\0' )catFunction(argval[i]);
                else break ; 
            }        
        }
        else if(strcmp(argval[0],"mv")==0 && !isBackground){
            selectorFlag = 1;
            if(argcount <=3) printf("Error: cannot copy insufficient parameters \n");
            else for(int i=1; i<argcount-2; i++) mvFunction(argval[i], argval[argcount-2]);
        }
        else if(strcmp(argval[0],"cp")==0 && !isBackground){
            selectorFlag = 1;
            if(argcount <=3) printf("Error: cannot copy insufficient parameters \n");
            else for(int i=1; i<argcount-2; i++) cpFunction(argval[i], argval[argcount-2]);
        }
        else if(strcmp(argval[0],"cd")==0 && !isBackground){            
            selectorFlag = 1;
            cdFunction(argval[1]);
        }

        else if(strcmp(argval[0],"pwd")==0 && !isBackground){
            selectorFlag = 1;
            getPath(cwd,1);
        }
        else if(strcmp(argval[0],"rm")==0 && !isBackground){
            selectorFlag = 1;
            if(strcmp(argval[1], "-r") ==0) rmFunction(argval[2],argval[1]);
            else rmFunction(argval[1],"");
        }
        else if(strcmp(argval[0],"chmod")==0 && !isBackground){
            selectorFlag = 1;
            for(int i= 2; i<ARGMAX; i++){
                if( *argval[i] != '\0' )chmodFunction(argval[1], argval[i]);
                else break ; 
            }
            
        }
        else if(strcmp(argval[0],"mkdir")==0 && !isBackground){
            selectorFlag = 1;
            mkdirFunction(argval[1]);
        }
        else if(strcmp(argval[0],"touch")==0 && !isBackground){
            selectorFlag = 1;
            for(int i= 1; i<ARGMAX; i++){
                if( *argval[i] != '\0' )touchFunction(argval[i]);
                else break ; 
            }
        }

        return selectorFlag; 
}


// Print the contents of the directory
void printContent(struct dirent* name)
{
    // regular file type
    if(name->d_type == DT_REG) printf("%s%s    ",BLUE, name->d_name);

    // a directory type
    else if(name->d_type == DT_DIR) printf("%s%s/    ",GREEN, name->d_name);

    // unknown file type
    else printf("%s%s    ",BLUE, name->d_name );                             
}



void lsFunction(char* folderName){
    
    int i=0;
    struct dirent **items;
    int n ; // number of items in the list +2 because of the 2 null byte string terminator entries 
            // items[0]->d_name == ".\0" and items[1]->d_name == "..\0"
    
    // If no arguments are given scan the content of the current directory
    if(folderName == NULL || *folderName == '\0') n = scandir(".", &items, 0, alphasort); // alphasort will sort alphabatically, 
                                                                                          // otherwise files would be in the order they were created
    else n = scandir(folderName, &items, 0, alphasort);

    if (n >= 0){
        printf("%s Total %d objects in this directory\n",BLUE,n-2);
        
        for(i = 2; i < n; i++ ){
            printContent(items[i]);
            if(i%7==0) printf("\n");   // just insrting new line character at every 7th index to fit the names in the CMD and pretiffy
        }
        printf("\n");
    }
    else{
        perror ("Error in ls ");
    }
}

// helper function to copy one file to another

void cpFunctionHelper(char* file1, char* file2){
    if(argcount >3 && strlen(file1) > 0 && strlen(file2) > 0){
        FILE *f1, *f2;  // defining file pointers 
        f1 = fopen(file1, "r+"); // opening file1 in read mode
                                 // Using r+ lets us differntiate 
                                //  if we are performing copying on a folder or a file 
        if(f1 == NULL){
            perror("Error while copying File 1"); 
            return ; 
        }
        f2 = fopen(file2,"w+"); 
        // checking if the file is opened/created correctly
        if(f2 == NULL){
            perror("Error while copying to File 2 "); 
            fclose(f1);
            return;
        }
        // Checking if read and write access is there for the files 
        if( access(file1,R_OK)!= 0 ||access(file2,W_OK)!=0 ){
            perror("Error while accessing files ");  
            return ;
        }

        char wfile ;
        while((wfile = getc(f1))!= EOF){
            putc(wfile,f2); 
        }
        fclose(f1);
        fclose(f2); 

    }
    else printf("Error: cannot copy insufficient parameters \n");
}

// function to copy one file to another

void cpFunction(char* file1, char* newPath){
    if(argcount >3 && strlen(file1) > 0 && strlen(newPath) > 0){
        struct stat statusFile;
        struct stat statusPath; 
        int resultFile = stat(file1,&statusFile);
        int resultPath = stat(newPath,&statusPath);

        
        if(resultFile !=0 ){ 
            perror("Error: Cannot perform stat on the file ");
            return ; 
        }
        if((statusFile.st_mode & S_IFMT ) == S_IFREG) {
            if(resultPath !=0 || (resultPath == 0 && ((statusPath.st_mode&S_IFMT )== S_IFREG))) 
                cpFunctionHelper(file1,newPath); 
            else{
                char temp[MAXSIZE]; 
                char* fileName = basename(file1); 
                strcpy(temp,newPath);
                strcat(temp, "/");
                strcat(temp, fileName); 
                cpFunctionHelper(file1,temp); 
            }
        }
        else if((statusFile.st_mode & S_IFMT ) == S_IFDIR){
            if(resultPath !=0){ 
                perror("Error: Cant perform cp");
                return ;
            }
            if((statusPath.st_mode&S_IFMT )== S_IFREG){
                 perror("Error: cant copy a directory to a file") ; 
                 return ; 
            }
            if((statusPath.st_mode&S_IFMT )== S_IFDIR){
                char oldPath[MAXSIZE] ; 
                strcpy(oldPath, cwd); 
                cdFunction(newPath); 
                char* folderName = basename(file1); 

                int answer = mkdir(folderName, 0777);// all appropriate permissions
                cdFunction(oldPath); 
                if(answer !=-1) printf(" successfully created directory %s %s %s \n", BLUE,folderName ,WHITE);
                else {
                    perror(" Error: Cannot make directory ");
                    return; 
                }
                
                char temp[MAXSIZE];
                strcpy(temp,newPath);
                strcat(temp, "/");
                strcat(temp, folderName); 
                int i=0;
                struct dirent **items;
                int n = scandir(file1, &items, 0,alphasort);
                if (n >= 0){
                    for(i = 2; i < n; i++ ){
                        if(items[i]->d_type == DT_REG){
                            char name1[MAXSIZE];
                            char name2[MAXSIZE]; 
                            strcpy(name1, file1); 
                            strcat(name1, "/");
                            strcat(name1, items[i]->d_name); 

                            strcpy(name2, temp); 
                            strcat(name2, "/");
                            strcat(name2, items[i]->d_name); 
                            cpFunctionHelper(name1,name2); 
                        }
                    }
                }
                else{
                    perror ("Nothing to copy");
                }

            }
            else{
                printf("Error: cant perform cp") ;
                exit(1); 
            }
    }

    }
    else printf("Error: cannot copy insufficient parameters \n");
    
}

void grepFunction(char* pattern, char* file1){
    if(argcount >1 && strlen(file1) > 0){
        FILE *f1; 
        f1 = fopen(file1, "r+");
        if(f1 == NULL){
            perror("grep: Error while opening File 1"); 
            return ; 
        }

        if( access(file1,R_OK)!= 0 ||access(file1,F_OK)!=0 ){
            perror("grep: Error while accessing files ");  
            return ;
        }

        char *line = NULL;
        size_t len = 0;
        ssize_t read;

        while ((read = getline(&line, &len, f1)) != -1) {
            if(strstr(line,pattern) != NULL) printf("%s", line); 
        }
        printf("\n");
        fclose(f1);
    }
    else {
        char *line = NULL;
        size_t len = 0;
        ssize_t read;
        running = 1;
        signal(SIGINT, handleWhile); 
        printf("%s Please enter the lines to be matched : \n%s ", BLUE,WHITE );
        while ((read = getline(&line, &len, stdin)) != -1 ) {
            if(running == 0) return ; 
            if(strstr(line,pattern) != NULL) printf("%s %s %s ",GREEN, line, WHITE); 
            else printf("%s Match not found, press ctrl+c to exit or continue entering lines %s \n ", RED_TEXT, WHITE); 
        }
        printf("\n");
    }
}


void catFunction(char* file1){
    if(argcount >1 && strlen(file1) > 0){
        FILE *f1; 
        f1 = fopen(file1, "r+");
        if(f1 == NULL){
            perror("Error while opening File 1"); 
            return ; 
        }

        if( access(file1,R_OK)!= 0 ||access(file1,F_OK)!=0 ){
            perror("Error while accessing files ");  
            return ;
        }

        char rfile ;
        while((rfile = getc(f1))!= EOF){
            printf("%c", rfile); 
        }
        printf("\n");
        fclose(f1);
    }
    else printf("Error: cannot open file insufficient parameters\n") ;
}


void touchFunction(char* file1){
    if(argcount >1 && strlen(file1) > 0){
        FILE *f1; 

        // creating or updating access time of a file using append mode
        f1 = fopen(file1, "a");
        if(f1 == NULL){
            perror("Error while creating File 1"); 
            return ; 
        }

        // Checking if the file is accessible for read
        if( access(file1,R_OK)!= 0 ||access(file1,F_OK)!=0 ){   
            perror("Error while accessing files ");  
            return ;
        }

        fclose(f1);
    }
    else printf("Error: cannot create file insufficient parameters\n") ;
}


void chmodFunction(char* mode, char* fileFolder){
    int temp = atoi(mode); 
    int unit = temp%10; temp = temp/10; 
    int tens = temp%10; temp = temp/10; 
    int hundred = temp%10; temp = temp/10; 
    if((atoi(mode)>777 || atoi(mode) < 0 || unit>7 || tens>7|| hundred >7)) {
        printf("chmod: Invalid mode value %d \n", atoi(mode)) ; 
        return ; 
    }    
    int m = strtol(mode,0,8);  // Convert the mode  value to ocatal 
    
    char newPath[MAXSIZE];  // newPath variable to store the complete supplied path
    strcpy(newPath,cwd);    // concatention operation 
    strcat(newPath,"/");
    strcat(newPath, fileFolder);
    int result = chmod(newPath,m); // set the path value 
    if(result<0) perror("unable to chmod") ; 
    else printf("%s Successfully changed permissions for %s %s \n", GREEN,fileFolder, WHITE);
}

void cdFunction(char * newPath){    
    int result = chdir(newPath); // call change directory to the path supplied 
    if(result == 0) getPath(cwd,0); // If successful, update the cwd variable with the new path
    else perror("Error: Can't change directory"); 
}


void mkdirFunction(char* folderName){
    int answer = mkdir(folderName, 0777);// all appropriate permissions
    if(answer !=-1) printf(" successfully created directory %s %s %s \n", BLUE,folderName ,WHITE);
    else perror(" Error: Cannot make directory ");
}


void mvFunction(char* file1, char* newPath){
    
    if(argcount >3 && strlen(file1) > 0 ){    
        struct stat statusFile;
        struct stat statusPath; 
        int resultFile = stat(file1,&statusFile);
        int resultPath = stat(newPath,&statusPath);

        char* fileName = basename(file1);  
        char temp[MAXSIZE]; 
        if(resultFile !=0 ) perror("Error: Cannot perform stat on the file "); 
        if((statusFile.st_mode&S_IFMT )== S_IFDIR) perror("Error: Cannot perform mv on directory") ;
        if((statusPath.st_mode&S_IFMT )== S_IFDIR) {
            strcpy(temp,newPath);
            strcat(temp, "/");
            strcat(temp, fileName); 
            rename(file1,temp);  
        }
        else rename(file1,newPath); 

    }
    else printf("Error: cannot copy insufficient parameters \n");
}




// some parts referenced from 
// https://stackoverflow.com/questions/2256945/removing-a-non-empty-directory-programmatically-in-c-or-c/2256974#2256974

int rmFunction(char* fileFolderName,char* flag){
   
    if(strcmp(flag, "-r") ==0) {
        size_t pathLength = strlen(fileFolderName);

        DIR *folder = opendir(fileFolderName); 

        int result = -1;

        if (folder) {
            struct dirent *items; // defining items array, of type directory entry, for removal of files inside directory 

            result = 0;
            while (!result && (items=readdir(folder))) {
                
                char *buf;
                size_t len;
                int itemResult = -1;
                
                // Skipping "."  and "..",as we do not want to recurse on those directories
                if (!strcmp(items->d_name, ".") || !strcmp(items->d_name, ".."))
                    continue;

                len = pathLength + strlen(items->d_name) + 2; // +2 if a folder, and then there will be "."  and ".."
                buf = malloc(len); // assign memory to buf variable

                // just a fancy way to copy the path fileFolderName/files to buf, with maximum number of characters argument
                snprintf(buf, len, "%s/%s", fileFolderName, items->d_name);
                
                if (items->d_type != DT_DIR) itemResult = remove(buf); // If not a directory, remove the file
                else itemResult = rmFunction(buf,flag);  // If the item is a directory recurse and remove the contents. 
                
                free(buf); // freeing the assigned memory 
            
                result = itemResult; 
            }
            closedir(folder);
        }
        if (!result) result = rmdir(fileFolderName);

        if(result !=-1) printf(" successfully deleted directory %s %s %s \n", BLUE,fileFolderName ,WHITE);
        else perror(" Error: Cannot delete directory ");
        return result; 
    }
    else{
        if(remove(fileFolderName) ==0) printf(" successfully deleted file/directory %s %s %s \n", BLUE,fileFolderName ,WHITE);
        else perror(" Error: Cannot delete file/directory ");    
    }
    
}


void inbuiltFunction(){
    rc = fork();
    if(rc<0) {
        fprintf(stderr, "failed to create child process");
        exit(1); 
    }
    else if (rc == 0 ){
        // child process
        (argval[argcount-1]) = NULL;  // As the argument array must be NULL terminated for execvp
        if(isBackground == 1){
            isBackground = 0 ; 
            int selectorFlag = selector(); 
            if(selectorFlag == 0) {
                if (execvp(argval[0],argval) <0){
                    fprintf(stderr, "%s : No such file or command \n", argval[0]);
                    exit(1); 
                } 
            }
            exit(1); 
        }
        else if (execvp(argval[0],argval) <0){
            fprintf(stderr, "%s : No such file or command \n", argval[0]);
            exit(1); 
        } 

    }
    else {
        // parent should wait for the child process to get over and then continue the original path
        int status; 
        if(isBackground == 0) waitpid(rc,&status, 0);
        else printf("Process running in background, with PID = %d\n",rc);
        
    }
    
}

// function to initiate an exit from the shell
void exitFunction(){
    exitFlag = 1;
    printf("%s It was nice having you! Bye! %s \n", RED, WHITE); 
}