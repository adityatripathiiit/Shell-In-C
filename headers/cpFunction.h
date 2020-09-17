
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
