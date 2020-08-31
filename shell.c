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

char cwd[MAXSIZE];    // variable to store the current path
char* argv[ARGMAX];   // argument values/vector  array 
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
        if(selectorFlag == 0 && *argv[0]!= '\0') {
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

//  get input containing spaces and tabs and store it in argv 
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

    while(argcount < ARGMAX-1 && (argv[argcount] = strsep(&input, " \t\n")) != NULL)
    {
        // do not consider "" as a parameter
        if(sizeof(argv[argcount])==0)
        {
            free(argv[argcount]);
        }
        else argcount++;  // increasing the count of the index which store the ith argument
        if(strcmp(argv[argcount-1],"&")==0) // if the last argument is &
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
        if(strcmp(argv[0],"exit")==0){
                selectorFlag = 1;
                exitFunction();
            }
        else if(strcmp(argv[0],"ls")==0 && !isBackground){
            selectorFlag = 1;
            if(argcount == 2) lsFunction(argv[1]); 

            else for(int i= 1; i<argcount-1; i++) lsFunction(argv[i]);
        }
        else if(strcmp(argv[0],"grep")==0 && !isBackground){
            selectorFlag = 1;
            if(argcount <3) printf("Error: cannot grep insufficient parameters \n");
            else {
                if(*argv[1] == '\0') return 1;
                for(int i= 2; i<ARGMAX; i++){
                    if(strcmp(argv[1], "\"\"")==0 ||strcmp(argv[1], "\"")==0 ){
                        printf("Nothing is matched \n");
                        return 1 ; 
                    }
                    else if( i == 2 &&*argv[i] == '\0' && *argv[1] != '\0' ) grepFunction(argv[1], argv[i]) ;
                    else if( *argv[i] != '\0' && *argv[1] != '\0' ) grepFunction(argv[1], argv[i]);
                    else break ; 
                }
            }
        }
        else if(strcmp(argv[0],"cat")==0 && !isBackground){
            selectorFlag = 1;
            for(int i= 1; i<ARGMAX; i++){
                if( *argv[i] != '\0' )catFunction(argv[i]);
                else break ; 
            }        
        }
        else if(strcmp(argv[0],"mv")==0 && !isBackground){
            selectorFlag = 1;
            if(argcount <=3) printf("Error: cannot copy insufficient parameters \n");
            else for(int i=1; i<argcount-2; i++) mvFunction(argv[i], argv[argcount-2]);
        }
        else if(strcmp(argv[0],"cp")==0 && !isBackground){
            selectorFlag = 1;
            
            if(argcount <=3) printf("Error: cannot copy insufficient parameters \n");
            else if(strcmp(argv[1], "-r") ==0) {
                for(int i=2; i<argcount-2; i++) cpFunction(argv[i], argv[argcount-2]);
            }
            else{
                for(int i=1; i<argcount-2; i++) cpFunction(argv[i], argv[argcount-2]);
            }
        }
        else if(strcmp(argv[0],"cd")==0 && !isBackground){            
            selectorFlag = 1;
            cdFunction(argv[1]);
        }

        else if(strcmp(argv[0],"pwd")==0 && !isBackground){
            selectorFlag = 1;
            getPath(cwd,1);
        }
        else if(strcmp(argv[0],"rm")==0 && !isBackground){
            selectorFlag = 1;
            if(strcmp(argv[1], "-r") ==0) rmFunction(argv[2],argv[1]);
            else rmFunction(argv[1],"");
        }
        else if(strcmp(argv[0],"chmod")==0 && !isBackground){
            selectorFlag = 1;
            for(int i= 2; i<ARGMAX; i++){
                if( *argv[i] != '\0' )chmodFunction(argv[1], argv[i]);
                else break ; 
            }
            
        }
        else if(strcmp(argv[0],"mkdir")==0 && !isBackground){
            selectorFlag = 1;
            mkdirFunction(argv[1]);
        }
        else if(strcmp(argv[0],"touch")==0 && !isBackground){
            selectorFlag = 1;
            for(int i= 1; i<ARGMAX; i++){
                if( *argv[i] != '\0' )touchFunction(argv[i]);
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
            if(i%7==0) printf("\n");   // just inserting new line character at every 7th index to fit the names in the CMD and pretiffy
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
            putc(wfile,f2);   // copying file1 to file2
        }
        fclose(f1); // closing the file1 
        fclose(f2); // closing the file2 

    }
    else printf("Error: cannot copy insufficient parameters \n");
}

// function to copy one file/directory to another

void cpFunction(char* file1, char* newPath){
    if(argcount >3 && strlen(file1) > 0 && strlen(newPath) > 0){
        struct stat statusFile;    // struct to store the stat info about the given file 
        struct stat statusPath;  
        int resultFile = stat(file1,&statusFile); 
        int resultPath = stat(newPath,&statusPath);

        // If stat is successfully performed, it returns 0. 
        if(resultFile !=0 ){ 
            perror("Error: Cannot perform stat on the file ");
            return ; 
        }
        // If the file is a regular file, then there are 2 cases, either the given path is 
        // an existent directory or the given path is existent or non-existent file
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
        else if((statusFile.st_mode & S_IFMT ) == S_IFDIR && strcmp(argv[1],"-r") ==0){
            // If cannot perform stat on the path
            if(resultPath !=0){ 
                perror("Error: Cant perform cp");
                return ;
            }
            // If the newPath is a file 
            if((statusPath.st_mode&S_IFMT )== S_IFREG){
                 perror("Error: cant copy a directory to a file") ; 
                 return ; 
            }
            if((statusPath.st_mode&S_IFMT )== S_IFDIR){
                char oldPath[MAXSIZE] ; 
                strcpy(oldPath, cwd);                // Creating a copy of the current path, so we can switch back 
                cdFunction(newPath);                 // changing the directory to the newPath
                char* folderName = basename(file1);  // getting the name of the file

                int answer = mkdir(folderName, 0777);// all appropriate permissions
                cdFunction(oldPath);                 // Switching back to the original path 

                // If directory doesn't already exists, create the directory, otherwise stop cp command
                // The stopping of cp can be easily changed, just implementation specific thing. 
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
                int n = scandir(file1, &items, 0,alphasort); // Traversing the directory 
                if (n >= 0){
                    for(i = 2; i < n; i++ ){
                        //  Copying only the files of a directory 
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
                return ; 
            }
        }
        else {
            printf("Error: cant perform cp on a directory. Try with -r flag \n") ;
            return ; 
        }

    }
    else printf("Error: cannot copy insufficient parameters \n");
    
}

void grepFunction(char* pattern, char* file1){
    //  If file is provided then perform search on the file
    if(argcount >1 && strlen(file1) > 0){
        // Opening a file in read mode and checking basic read access 
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
        // Using getline to read any arbitrary length of a line in a file, without specifiying size before hand.
        // Using inbuilt matching algorthm of C. Can implement KMP also. 
        while ((read = getline(&line, &len, f1)) != -1) {
            if(strstr(line,pattern) != NULL) printf("%s", line); 
        }
        printf("\n");
        fclose(f1);
    }
    // else take input from the user and perform search
    else {
        char *line = NULL;
        size_t len = 0;
        ssize_t read;
        running = 1;
        signal(SIGINT, handleWhile);  // signal to exit the while loop 
        printf("%s Please enter the lines to be matched : \n%s ", BLUE,WHITE );
        while ((read = getline(&line, &len, stdin)) != -1 ) {
            if(running == 0) break ; 
            if(strstr(line,pattern) != NULL) printf("%s %s %s ",GREEN, line, WHITE); 
            else printf("%s Match not found, press ctrl+c to exit or continue entering lines %s \n ", RED_TEXT, WHITE); 
        }
        printf("\n");
    }
}


void catFunction(char* file1){
    if(argcount >1 && strlen(file1) > 0){
        // Opening a file in read mode and checking basic read access 
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
        // Printing all the contents of a file
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
    int temp = atoi(mode); // converting received mode value to int 
    int unit = temp%10; temp = temp/10;  // finding unit place digit 
    int tens = temp%10; temp = temp/10;  // finding tens place digit
    int hundred = temp%10; temp = temp/10;  // finding hundred place digit  

    // If any of this ones, tens or hundred place is not between 0 and 7 (inclusive) 
    // then its not a valid mode. Also, if the total number is not between 0 and 777 
    // the mode is invalid
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
        // There can be 2 cases depending on the newpath 
        // If the newpath is a not an existing directory, then just use rename C system call to move the files 
        // else if the given path is a directory, then move the file inside the directory 
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

// Function to run inbuilt binaries and background processes 
void inbuiltFunction(){
    // Creating a child processes 
    rc = fork();
    if(rc<0) {
        fprintf(stderr, "failed to create child process");
        exit(1); 
    }
    else if (rc == 0 ){
        // child process
        (argv[argcount-1]) = NULL;  // As the argument array must be NULL terminated for execvp
        //  If we want to run the process in background
        if(isBackground == 1){
            // The background process 
            isBackground = 0 ; 
            // Select the command which was passed 
            int selectorFlag = selector(); 
            //  If the command that was to be run background is not in the custom commands we implemented
            // run it using execvp as usual
            if(selectorFlag == 0) {
                if (execvp(argv[0],argv) <0){
                    fprintf(stderr, "%s : No such file or command \n", argv[0]);
                    exit(1); 
                } 
            }
            exit(1); 
        }
        // else just run the command using execvp 
        else if (execvp(argv[0],argv) <0){
            fprintf(stderr, "%s : No such file or command \n", argv[0]);
            exit(1); 
        } 

    }
    else {
        
        int status; 
        // parent should wait for the child process to get over and then continue the original path
        // iff the user does not want to run the process in background. Parent process need not be waiting in this case.
        if(isBackground == 0) waitpid(rc,&status, 0);
        else printf("Process running in background, with PID = %d\n",rc);
        
    }
    
}

// function to initiate an exit from the shell
void exitFunction(){
    exitFlag = 1;
    printf("%s It was nice having you! Bye! %s \n", RED, WHITE); 
}