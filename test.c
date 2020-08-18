#include <stdio.h>
#include<stdlib.h>
#include <unistd.h>


int main () {
    while(1){
        printf("\nThis is a background process which is running\n") ; 
        sleep(5); 
    }
}