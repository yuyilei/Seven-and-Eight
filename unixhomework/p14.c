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

void TELL_CHILD(char signal) {
    /***
     * 父进程唤醒子进程
     */
    if (write(pfd1[1], &signal, 1) != 1) {
        fprintf(stderr, "write error");
    }
    fflush(stdout);
}

char WAIT_PARENT(void) {
    /***
     * 子进程等待父进程通知
     */
    char c;
    while (read(pfd1[0], &c, 1) != 1) {
        fprintf(stderr, "read error");
    }

    if (c != 'p' && c != 'q' ) {
        fprintf(stderr, "WAIT_PARENT: incorrect data");
        exit(1);
    }
    return c;
}

int generate_random() {
    srand((unsigned int)time(NULL));
    int num = rand() % 100 + 1;
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
        int num = generate_random();
        int num2 = 0;
        while ( num != num2 ) {
            num2 = WAIT_CHILD();                                  // 获取子进程中猜的数字
            if ( num2 > num ) {
                printf("In father process: 猜大了!\n") ;
                TELL_CHILD('p');                                 // p 表示继续猜
            }
            else if ( num2 < num ) {
                printf("In father process: 猜小了!\n") ;
                TELL_CHILD('p');                                 // p 表示继续猜
            }
            else {
                printf("In father process: 猜对了!\n") ;
                TELL_CHILD('q');                                 // 写入q, 停止猜
            }
        }
        printf("In fahter process: 随机生成的数字是:%d\n",num) ;
    }
    else {
        // 子进程
        int num2;
        char flag;
        do {
            printf("In child process: 输入一个1到100的数\n") ;
            scanf("%d",&num2) ;
            TELL_PARENT(num2) ;                                // 把猜的数字写入管道
            flag = WAIT_PARENT();
        } while ( flag == 'p' ) ;
    }
    return 0;
}
