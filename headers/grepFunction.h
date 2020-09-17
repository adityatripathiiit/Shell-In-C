
// function to handle while loop exits on signal 
void handleWhile(){
    running = 0; 
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
