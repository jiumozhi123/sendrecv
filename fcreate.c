#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>

void main()
{
    if(mkdir("../Input",0777)==0)
        printf("Input folder create succeed!\n");
    else
        printf("Input folder create fail!\n");
    if(mkdir("../Output",0777)==0)
        printf("Output folder create succeed!\n");
    else
        printf("Output folder create fail!\n");
}
