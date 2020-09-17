
void cdFunction(char * newPath){    
    int result = chdir(newPath); // call change directory to the path supplied 
    if(result == 0) getPath(cwd,0); // If successful, update the cwd variable with the new path
    else perror("Error: Can't change directory"); 
}
