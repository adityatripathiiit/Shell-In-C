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
