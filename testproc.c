#include "kernel/types.h"
#include "user/user.h"

int main(void)
{
    struct procinfo info;
    
    // Call getprocinfo with the address of info struct
    if(getprocinfo(&info) < 0) {
        printf("getprocinfo failed\n");
        exit(1);
    }
    
    printf("getprocinfo succeeded\n");
    printf("PID: %d\n", info.pid);
    printf("State: %d\n", info.state);
    printf("Priority: %d\n", info.priority);
    printf("Queue: %d\n", info.queue);
    printf("Ticks: %d\n", info.ticks);
    printf("Total ticks: %d\n", info.totalticks);
    
    exit(0);
}
