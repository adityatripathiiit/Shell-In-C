// Function to compare the input commands 

int selector(){

        // The value of this flag is set to 1, if the entered command 
        // is from the custom commands that are implemented
        // If so, then they are executed, otherwise it checks inbuilt binaries
        int selectorFlag = 0 ;

        // strcmp return 0 if strings/characters are a match
        if(strcmp(argv[0],"exit")==0){
                selectorFlag = 1;
                exitFunction();
            }
        else if(strcmp(argv[0],"ls")==0 && !isBackground){
            selectorFlag = 1;
            if(argcount == 2) lsFunction(argv[1]); 

            else for(int i= 1; i<argcount-1; i++) lsFunction(argv[i]);
        }
        else if(strcmp(argv[0],"grep")==0 && !isBackground){
            selectorFlag = 1;
            if(argcount <3) printf("Error: cannot grep insufficient parameters \n");
            else {
                if(*argv[1] == '\0') return 1;
                for(int i= 2; i<ARGMAX; i++){
                    if(strcmp(argv[1], "\"\"")==0 ||strcmp(argv[1], "\"")==0 ){
                        printf("Nothing is matched \n");
                        return 1 ; 
                    }
                    else if( i == 2 &&*argv[i] == '\0' && *argv[1] != '\0' ) grepFunction(argv[1], argv[i]) ;
                    else if( *argv[i] != '\0' && *argv[1] != '\0' ) grepFunction(argv[1], argv[i]);
                    else break ; 
                }
            }
        }
        else if(strcmp(argv[0],"cat")==0 && !isBackground){
            selectorFlag = 1;
            for(int i= 1; i<ARGMAX; i++){
                if( *argv[i] != '\0' )catFunction(argv[i]);
                else break ; 
            }        
        }
        else if(strcmp(argv[0],"mv")==0 && !isBackground){
            selectorFlag = 1;
            if(argcount <=3) printf("Error: cannot copy insufficient parameters \n");
            else for(int i=1; i<argcount-2; i++) mvFunction(argv[i], argv[argcount-2]);
        }
        else if(strcmp(argv[0],"cp")==0 && !isBackground){
            selectorFlag = 1;
            
            if(argcount <=3) printf("Error: cannot copy insufficient parameters \n");
            else if(strcmp(argv[1], "-r") ==0) {
                for(int i=2; i<argcount-2; i++) cpFunction(argv[i], argv[argcount-2]);
            }
            else{
                for(int i=1; i<argcount-2; i++) cpFunction(argv[i], argv[argcount-2]);
            }
        }
        else if(strcmp(argv[0],"cd")==0 && !isBackground){            
            selectorFlag = 1;
            cdFunction(argv[1]);
        }

        else if(strcmp(argv[0],"pwd")==0 && !isBackground){
            selectorFlag = 1;
            getPath(cwd,1);
        }
        else if(strcmp(argv[0],"rm")==0 && !isBackground){
            selectorFlag = 1;
            if(strcmp(argv[1], "-r") ==0) rmFunction(argv[2],argv[1]);
            else rmFunction(argv[1],"");
        }
        else if(strcmp(argv[0],"chmod")==0 && !isBackground){
            selectorFlag = 1;
            for(int i= 2; i<ARGMAX; i++){
                if( *argv[i] != '\0' )chmodFunction(argv[1], argv[i]);
                else break ; 
            }
            
        }
        else if(strcmp(argv[0],"mkdir")==0 && !isBackground){
            selectorFlag = 1;
            mkdirFunction(argv[1]);
        }
        else if(strcmp(argv[0],"touch")==0 && !isBackground){
            selectorFlag = 1;
            for(int i= 1; i<ARGMAX; i++){
                if( *argv[i] != '\0' )touchFunction(argv[i]);
                else break ; 
            }
        }

        return selectorFlag; 
}

