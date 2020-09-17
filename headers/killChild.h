// Kill any process if its not the parent, i.e the shell itself by sending signal interupt 
// Send signal interupt using ctrl+c
void killChild(){
    if(rc!=0){
        kill(rc, SIGINT);
        rc = 0;
    }
}