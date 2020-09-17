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

