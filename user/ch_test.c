#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(){
    int cd = channel_create();
    if (cd < 0) {
        printf("Failed to create channel\n");
        exit(1);
    }
    if (fork() == 0) {

        if (channel_put(cd, 42) < 0) {

            printf("Failed to put data in channel\n");
            exit(1);
        }
        channel_put(cd, 43); // Sleeps until cleared
        // Handle error
        channel_destroy(cd);
        // Handle error
        } 
        else {
            int data, returned;
            if (channel_take(cd, &data) < 0) { // 42
                printf("Failed to take data from channel\n");
                exit(1);
            }
            returned = channel_take(cd, &data); // 43
            printf("returned: %d\n", returned);
            if(returned >= 0){
                printf("Received data: %d\n", data);
            }
            // Handle error
            returned = channel_take(cd, &data); // Sleep until child destroys channel
            printf("returned: %d\n", returned);
            if(returned >= 0){
                printf("Received data: %d\n", data);
            }
            // Handle error
    }
    exit(0);
}