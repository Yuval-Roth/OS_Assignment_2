#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int is_prime(int n){
    if(n < 2){
        return 0;
    }
    for(int i = 2; i * i <= n; i++){
        if(n % i == 0){
            return 0;
        }
    }
    return 1;
}

int main(int argc, char *argv[]){

    int pid, num, i = 0, data , n=3;
    int cd1 = channel_create(), cd2 = channel_create();

    if(argc == 2){
        n = atoi(argv[1]); // number of checkers to fork
    } 
    
    if(cd1 < 0 || cd2 < 0){
        printf("Failed to create channel\n");
        exit(1);
    }

    // fork 1 producer
    if(fork() == 0){
        i = 2;
        while((num = channel_put(cd1, i)) >= 0){
            i++;
        }
        exit(0);
    }

    // fork n checkers
    while((pid = fork()) > 0 && n > 0){n--;}

    if(pid == 0){
        while(channel_take(cd1,&data) >= 0){
            if(is_prime(data)){
                if(channel_put(cd2, data) < 0){
                    channel_destroy(cd1);
                    exit(0);
                }
            }
        }
        exit(0);
    } 

    while((num = channel_take(cd2, &data)) >= 0 && i < 100){
        i++;
        printf("%d\n", data);
    }

    channel_destroy(cd2);
    exit(0);
}
    