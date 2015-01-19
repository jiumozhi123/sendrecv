#include <stdio.h>
#include"MD5.h"
#include<string.h>

int main(int argc, char* argv[])
{
	char md5_sum1[MD5_LEN + 1];
        char md5_sum2[MD5_LEN + 1];
        printf("Waiting for calculating MD5 of Input File......\n");
	if(!CalcFileMD5("../Input/1G.img",md5_sum1))
		puts("Error occured!");
	else
		printf("Success! Input MD5 sum is :%s\n",md5_sum1);
        printf("Waiting for calculating MD5 of Output File......\n");
        if(!CalcFileMD5("../Output/1G.img",md5_sum2))
                puts("Error occured!");
        else
                printf("Success!Output MD5 sum is :%s\n",md5_sum2);
        if(strcmp(md5_sum1,md5_sum2)==0)
                printf("A correct transmission!\n");
        else
                printf("An incorrect transmission!\n");
}
