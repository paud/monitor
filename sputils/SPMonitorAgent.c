#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <windows.h>
#include "bson.h"
#include "hooking.h"
#include "memory.h"
#include "misc.h"
#include "native.h"
#include "ntapi.h"
#include "log.h"
#include "pipe.h"
#include "symbol.h"
#include "utf8.h"
#include "inirw.h"
#include "SPUtils.h"


export int apinameToIndex(string aname){    
    int i=6; //jmup g_hooks before 6 others
    hook_t *h;
    for (hook_t *h = sig_hooks(); h->funcname != NULL; h++)
    {
        if(strcmp(h->funcname,aname)==0){
            return i;
        }
        i++;     
    }
    return -1;
}