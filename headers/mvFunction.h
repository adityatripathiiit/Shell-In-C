
void mvFunction(char* file1, char* newPath){
    
    if(argcount >3 && strlen(file1) > 0 ){    
        struct stat statusFile;                
        struct stat statusPath; 
        int resultFile = stat(file1,&statusFile);
        int resultPath = stat(newPath,&statusPath);

        char* fileName = basename(file1);  
        char temp[MAXSIZE]; 
        if(resultFile !=0 ){
             perror("Error: Cannot perform stat on the file ");
             return; 
        } 
        if((statusFile.st_mode&S_IFMT )== S_IFDIR) {
            printf("Error: Cannot perform mv on directory\n") ;
            return; 
        }
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
