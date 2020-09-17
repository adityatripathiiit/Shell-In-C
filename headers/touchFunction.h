void touchFunction(char* file1){
    if(argcount >1 && strlen(file1) > 0){
        FILE *f1; 

        // creating or updating access time of a file using append mode
        f1 = fopen(file1, "a");
        if(f1 == NULL){
            perror("Error while creating File 1"); 
            return ; 
        }

        // Checking if the file is accessible for read
        if( access(file1,R_OK)!= 0 ||access(file1,F_OK)!=0 ){   
            perror("Error while accessing files ");  
            return ;
        }

        fclose(f1);
    }
    else printf("Error: cannot create file insufficient parameters\n") ;
}

