//  get input containing spaces and tabs and store it in argv 
// character array 
void getInput()
{
    fflush(stdout); // clear all previous buffers if any
    input = NULL;
    // size_t variable type, which is a special type of integer.
    // That’s required by the getline() function
    size_t size = 0;
    getline(&input,&size,stdin);

    argcount = 0;
    isBackground = 0;

    while(argcount < ARGMAX-1 && (argv[argcount] = strsep(&input, " \t\n")) != NULL)
    {
        // do not consider "" as a parameter
        if(sizeof(argv[argcount])==0)
        {
            free(argv[argcount]);
        }
        else argcount++;  // increasing the count of the index which store the ith argument
        if(strcmp(argv[argcount-1],"&")==0) // if the last argument is &
        {   
            isBackground = 1; //run in in Background
            return;  // return to the caller
        }
    }
    free(input); // freeing the assigned memory
}
