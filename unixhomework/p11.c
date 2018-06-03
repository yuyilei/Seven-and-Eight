#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>

int main() {
    pid_t pid ;
    int fd1[2], fd2[2] ;
    pid = fork() ;
    pipe(fd1) ;
    pipe(fd2) ;
  	if(pid == -1) {
		perror("fork error");
		return 0 ;
	}

    int flag = 0 ;
	int num1 = 1 ;
    int num2 = 2 ;
    int signal = 3 ;
    while ( num1 < 11 && num2 < 11 ) {
        if ( pid == 0 ) {                     //  子进程

            //if ( flag == 0 ) {
            //  close(fd2[0]) ;
            //   write(fd2[1],&signal,sizeof(int)) ;                  // 写入fd2，让子进程输出
            //    close(fd2[1]) ;
            //     printf("Init in child\n") ;
            //    flag = 1 ;
            //}
            // else {
            // close(fd1[1]) ;
            // close(fd2[0]) ;
            //printf("In child process: %d\n",num2) ;
            int now, res ;
            res = read(fd1[0],&now,sizeof(int)) ;                  // 从fd1中读
            if ( res > 0 ) {
                close(fd1[0]) ;
                printf("In child process: %d\n",num2) ;
                num2 += 2 ;
                write(fd2[1],&signal,sizeof(int)) ;               // 写入fd2，提示父进程输出数字
                close(fd2[1]) ;
            }
            else {
                printf("!err: read %d in child process\n",res) ;
                num2 += 2 ;
            }
        }
        else {                                //  父进程
            if ( flag == 0 ) {                                       // 初始化，让子进程输出
               // close(fd1[0]) ;
                write(fd1[1],&signal,sizeof(int)) ;                  // 写入fd1，让 子进程输出
                close(fd1[1]) ;
                printf("Init in father\n") ;
                flag = 1 ;
            }
            else {
                int now, res ;
                //close(fd2[1]) ;
                //close(fd1[0]) ;
                //printf("In father process: %d\n",num1) ;
                res = read(fd2[0],&now,sizeof(int)) ;              // 从fd2 中读
                if ( res > 0 ) {
                    close(fd2[0]) ;
                    printf("In father process: %d\n",num1) ;
                    num1 += 2 ;
                    write(fd1[1],&signal,sizeof(int)) ;           // 写入fd1，提示子进程输出数字
                    close(fd1[1]) ;
                }
                else {
                    printf("!err: read %d in father process\n",res) ;
                    num1 += 2 ;
                }
            }
        }
    }
}
