#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXNAME 11

int main(){
    int npartecipanti;
    scanf("%d",&npartecipanti);
    char names[npartecipanti][MAXNAME+1];//array of pointers to names
    char newname[MAXNAME+1];
    char foundederror=0;
    int i=0;
    for (; i<npartecipanti && foundederror==0; i++){
        scanf("%s",newname);
        if (i!=0){
            int lastnamelast=strlen(names[i-1]);
            if(newname[0]!=names[i-1][lastnamelast-1]) //first letter must be same last one
            foundederror=1;
        }
        for (int j=i-1; j>=0 && foundederror==0; j--){
            if(strcmp(newname,names[j])==0)//already seen
                foundederror=1;
        }
        //if no error i can add
        strcpy(names[i], newname);
    }
    if (foundederror==1)
        printf("Error in %d name\n",i);
    else
        printf("nice brainstorm!\n");
}