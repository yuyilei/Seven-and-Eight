#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

static int pfd1[2], pfd2[2]; // 管道端口文件描述符数组

void TELL_WAIT(void) {
    /***
     * 进程间同步初始化
     * 创建2个管道pfd1, pfd2
     * -- 父进程写pfd1[1], 读pfd2[0]
     * -- 子进程写pfd2[1], 读pfd1[0]
     */
    if (pipe(pfd1) < 0 || pipe(pfd2) < 0) {
        fprintf(stderr, "create pipe error");
    }
}

void TELL_PARENT(int num2) {
    /***
     * 子进程唤醒父进程
     */
    if (write(pfd2[1], &num2, sizeof(int))  != sizeof(int) ) {
         fprintf(stderr, "write error ");
    }
    fflush(stdout);
}

int WAIT_CHILD(void) {
    /***
     * 父进程等待子进程通知
     */
    int res;
    while (read(pfd2[0], &res, sizeof(int)) <= 0 ) {
        fprintf(stderr, "read error");
    }
    return res;
}

void TELL_CHILD(int signals[2]) {
    /***
     * 父进程唤醒子进程
     */
    if (write(pfd1[1], signals, 2*sizeof(int)) != 2*sizeof(int) ) {
        fprintf(stderr, "write error");
    }
    fflush(stdout);
}

void WAIT_PARENT(int signals[2]) {
    /***
     * 子进程等待父进程通知
     */
    char c;
    while (read(pfd1[0], signals, 2*sizeof(int)) != 2*sizeof(int)) {
        fprintf(stderr, "read error");
    }

    if (signals[0] != 0 && signals[0] != 1 && signals[0] != 2  ) {
        fprintf(stderr, "WAIT_PARENT: incorrect data");
        exit(1);
    }
}


int generate_random(int s, int e) {
    /*
     * 生成s到e的随机数
     */
    srand((unsigned int)time(NULL));
    int num = rand() % ( e - s + 1 ) + s;
    return num ;
}

int main(int argc, char** argv) {
    pid_t pid;

    TELL_WAIT();

    // fork 创建进程
    if ((pid = fork()) < 0) {
        fprintf(stderr, "fork error");
    }

    else if (pid > 0) {
        // 父进程
        int num = generate_random(1,100);
        int num2 = 0;
        while ( num != num2 ) {
            num2 = WAIT_CHILD();                                  // 获取子进程中猜的数字
            int signals[2] = {0} ;
            if ( num2 > num ) {
                printf("In father process: 猜大了!\n") ;
                signals[0] = 1 ;
            }
            else if ( num2 < num ) {
                printf("In father process: 猜小了!\n") ;
                signals[0] = 2 ;
            }
            else {
                printf("In father process: 猜对了!\n") ;
                signals[0] = 0 ;
            }
            signals[1] = num2 ;
            // signal[0] 表示猜大了还是猜小了，signals[1] 是上一轮的猜的数
            TELL_CHILD(signals);
        }
        printf("In fahter process: 随机生成的数字是:%d\n",num) ;
    }
    else {
        // 子进程
        int num2;
        int signals[2];
        int s = 1, e = 100;
        do {
            printf("In child process: 猜一个%d到%d的数\n",s,e) ;
			int sleep_time = generate_random(1,3) ;
            sleep(sleep_time) ;
            num2 = generate_random(s,e) ;                       // 生成随机数
            printf("In child process: 猜这个数为%d\n",num2) ;
            TELL_PARENT(num2) ;                                // 把猜的数字写入管道
            WAIT_PARENT(signals);
            // 根据上一轮猜的结果改变范围的大小
            if ( signals[0] == 1 )
                e = signals[1] - 1 ;
            else if ( signals[0] == 2 )
                s = signals[1] + 1 ;
        } while ( signals[0] != 0 ) ;
    }
    return 0;
}
