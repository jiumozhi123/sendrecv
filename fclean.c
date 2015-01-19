#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

void main()
{
    if(access("../Input/1G.img",F_OK)==0)
        {
        printf("Input/1G.img is found!\n");
        if(remove("../Input/1G.img")==0)
            printf("Delete Input/1G.img!\n");
        else
            printf("Delete Input/1G.img failed!\n");
        }
    else
        printf("Input/1G.img is never found!\n");
    if(rmdir("../Input")==0)
        printf("Delete Input folder!\n");
    else
        printf("Delete Input folder failed!\n");
    if(access("../Output/1G.img",F_OK)==0)
        {
        printf("Output/1G.img is found!\n");
        if(remove("../Output/1G.img")==0)
            printf("Delete Output/1G.img!\n");
        else
            printf("Delete Output/1G.img failed!\n");
        }
    else
        printf("Output/1G.img is never found!\n");
    if(rmdir("../Output")==0)
        printf("Delete Output folder!\n");
    else
        printf("Delete Output folder failed!\n");
}
