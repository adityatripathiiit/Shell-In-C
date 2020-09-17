# Shell-In-C

#### A basic linux-shell implemented in c. Here are some of the features of the shell. 


* Run inbuilt binaries (like ps, pmap, wget, etc.) with arguments
* Custom implementations of the following functions 
  * ls (Argument is either directory name or no argument at all)
  * grep (requirements are same as those for wgrep given here: [Link](https://github.com/remzi-arpacidusseau/ostep-projects/tree/master/initial-utilities)) 
  * cat (requirements are same as those for wcat given here: [Link](https://github.com/remzi-arpacidusseau/ostep-projects/tree/master/initial-utilities))
  * Mv
  * Cp (along with -r option)
  * Cd
  * Pwd
  * rm (along with -r option)
  * Chmod
  * Mkdir
  * Touch 
* Can run programs in background using & at the end 
* Coloured interface
* Handling Interrupt signals 
## Running the shell

1. `make`
2. `./shell`
