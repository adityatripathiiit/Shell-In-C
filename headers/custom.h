// Including header files 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/wait.h>
#include<fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <libgen.h>



// Including Custom header files 
#include "./initMessage.h"
#include "./constant.h"
#include "./getInput.h"
#include "./clear.h"
#include "./getPath.h"
#include "./printContent.h"
#include "./lsFunction.h"
#include "./touchFunction.h"
#include "./grepFunction.h"
#include "./cdFunction.h"
#include "./cpFunction.h"
#include "./catFunction.h"
#include "./chmodFunction.h"
#include "./exitFunction.h"
#include "./killChild.h"
#include "./mkdirFunction.h"
#include "./mvFunction.h"
#include "./rmFunction.h"
#include "./selector.h"
#include "./inbuiltFunction.h"