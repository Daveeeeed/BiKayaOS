#include "const.h"
#include "listx.h"
#ifdef TARGET_UMPS
#include "umps/libumps.h"
#include "umps/arch.h"
#endif

#ifdef TARGET_UARM
#include "uarm/libuarm.h"
#include "uarm/arch.h"
#endif

#include "pcb.h"

#define TIME_SLICE 3000

void timer(){
    tprint("timer setted\n");
    setTIMER(3);
}

void scheduler(struct list_head* queue){
    timer();
    tprint("ciao\n");
}