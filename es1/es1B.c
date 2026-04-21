#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXNAME 11
#define MOD1 1000000009
#define MOD2 998244353
// IA generated
int hash_oaat(const char *str, const int mod) {
    unsigned int hash = 0;
    while (*str) {
        hash += *str;
        hash += hash << 10;
        hash ^= hash >> 6;
        str++;
    }
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    return (int)((hash & 0x7fffffff)%mod);
}

typedef struct Hashbucket{
    int hash2;
    char str[MAXNAME+1];
    struct Hashbucket *next;
}Hashbucket;

int Secondhash(Hashbucket **h,int hash1,int hash2, const char *str){ //0 if alright 1 if collision at second level and identical string
    Hashbucket *lookerbucket=h[hash1];
    while (lookerbucket!=NULL)
    {
        if(hash2==lookerbucket->hash2){ //second hash
            if(strcmp(str,lookerbucket->str)==0) //they are literal identical
                return 1;
        }
        lookerbucket=lookerbucket->next;
    }
    //not found
    Hashbucket *newbucket=malloc(sizeof(Hashbucket));
    //insert elem on list
    newbucket->hash2=hash2;
    strcpy(newbucket->str, str);
    lookerbucket=h[hash1];
    h[hash1]=newbucket;
    newbucket->next=lookerbucket;
    return 0;
}

int Firsthash(Hashbucket **h, char str[]){ //0 if new bucket  1 if collision (duplicate)
    int hash1=hash_oaat(str,MOD1);
    int hash2=hash_oaat(str,MOD2);
    if (h[hash1]==NULL){ //empty bucket
        h[hash1]=malloc(sizeof(Hashbucket));
        h[hash1]->next=NULL;
        h[hash1]->hash2=hash2;
        strcpy(h[hash1]->str, str);
        return 0;
    }
    return Secondhash(h,hash1,hash2,str);
}

int main(){
    int npartecipanti;
    scanf("%d",&npartecipanti);
    char newname[MAXNAME+1];
    char foundederror=0;   
    Hashbucket **h=calloc(MOD1,sizeof(Hashbucket*));
    //first elem is done instandly since i dont have to controll anything
    int i=0;
    scanf("%10s",newname);
    Firsthash(h,newname);
    for (i=1; i<npartecipanti && foundederror==0; i++){
        //get last str char
        char lastbefore=newname[strlen(newname)-1];
        scanf("%10s",newname);
        char firstnew=newname[0];
        if(firstnew!=lastbefore)
            foundederror=1;
        if(Firsthash(h,newname)!=0)
            foundederror=1;
    }
    if (foundederror==1)
        printf("Error in %d name\n",i);
    else
        printf("nice brainstorm!\n");
    //free
    return 0;
}