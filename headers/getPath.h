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