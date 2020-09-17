
// Function to run inbuilt binaries and background processes 
void inbuiltFunction(){
    // Creating a child processes 
    rc = fork();
    if(rc<0) {
        fprintf(stderr, "failed to create child process");
        exit(1); 
    }
    else if (rc == 0 ){
        // child process
        (argv[argcount-1]) = NULL;  // As the argument array must be NULL terminated for execvp
        //  If we want to run the process in background
        if(isBackground == 1){
            // The background process 
            isBackground = 0 ; 
            // Select the command which was passed 
            int selectorFlag = selector(); 
            //  If the command that was to be run background is not in the custom commands we implemented
            // run it using execvp as usual
            if(selectorFlag == 0) {
                if (execvp(argv[0],argv) <0){
                    fprintf(stderr, "%s : No such file or command \n", argv[0]);
                    exit(1); 
                } 
            }
            exit(1); 
        }
        // else just run the command using execvp 
        else if (execvp(argv[0],argv) <0){
            fprintf(stderr, "%s : No such file or command \n", argv[0]);
            exit(1); 
        } 

    }
    else {
        
        int status; 
        // parent should wait for the child process to get over and then continue the original path
        // iff the user does not want to run the process in background. Parent process need not be waiting in this case.
        if(isBackground == 0) waitpid(rc,&status, 0);
        else printf("Process running in background, with PID = %d\n",rc);
        
    }
    
}

