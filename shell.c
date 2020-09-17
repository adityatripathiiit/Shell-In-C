#include "headers/custom.h"


int main (){
    // checking for signal Interrupt and calling killChild function if true
    signal(SIGINT,killChild); 
    // calling clear function to clear the terminal 
    clear(); 
    // Printing initial message on the terminal 
    initMessage(7);

    // getting the current path
    getPath(cwd,0);

    // while we do not exit the terminal
    while(exitFlag != 1){
        
        isBackground = 0 ; 

        printf("%s%s%s $ ", GREEN,cwd,WHITE); // printing the current path in the terminal
        getInput(); // function to get the input from the user 
        int selectorFlag = selector(); 
        // If enterted command not any command in the selector
        if(selectorFlag == 0 && *argv[0]!= '\0') {
            inbuiltFunction(); 
        }

    }

}
