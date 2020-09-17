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
