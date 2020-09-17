void mkdirFunction(char* folderName){
    int answer = mkdir(folderName, 0777);// all appropriate permissions
    if(answer !=-1) printf(" successfully created directory %s %s %s \n", BLUE,folderName ,WHITE);
    else perror(" Error: Cannot make directory ");
}
