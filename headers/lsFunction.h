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
