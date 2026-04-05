#include <stdio.h>
#include <stdlib.h>

//ia generated
int choose_max(int a, int b, int c, int *value) {
    if (a >= b && a >= c) {
        *value = a;
        return 1;
    } else if (b >= a && b >= c) {
        *value = b;
        return 2;
    } else {
        *value = c;
        return 3;
    }
}
//ia generated
int get_abc(int A, int B, int C, int whatihavechosen) {
    if (whatihavechosen == 1)
        return A;
    else if (whatihavechosen == 2)
        return B;
    else
        return C;
}

int solveT(int T,int A,int B, int C, int prev[],int *dy){ //dy[] is max task in T time
    int atask=-1,btask=-1,ctask=-1;
    //if(T==0){
    //    return 0;
    //}
    if(dy[T]!=-2)//standard not seen value
        return dy[T];
    if(T-A>=0)
        atask=solveT(T-A,A,B,C,prev,dy);
    if(T-B>=0)
        btask=solveT(T-B,A,B,C,prev,dy);
    if(T-C>=0)
        ctask=solveT(T-C,A,B,C,prev,dy);
    int moretaskdone;
    int whatihavechoosen=choose_max(atask,btask,ctask,&moretaskdone);
    //other base cases
    if (moretaskdone==-1) //i cannot choose
        return dy[T]=-1;
    prev[T]=T-get_abc(A,B,C,whatihavechoosen);//the T after my choice
    return dy[T]=moretaskdone+1; //i count one more decision tree
}

int solve(int T,int A,int B,int C,int *prev){//returns time used
    int *dy=malloc(sizeof(int)*(T+1));
    dy[0]=0;
    for(int i=1; i<=T; i++)
        dy[i]=-2; //standard not set value
    for(int i=1;i<=T;i++) //DYNAMIC PROG
        solveT(i,A,B,C,prev,dy);
    int result;
    if (dy[T]!=-1) //if i can maximize time used i return it
        result = T;
    else{
        while(dy[T]==-1){//while cannot min time lost i still search
            T--;
        }
        if (T!=0)//i found a good sol
            result=T;
        else
            result=-1;//cannot do anything
    }
    free(dy);
    return result;
}

void RebuildSol(int prev[],int finalt,int a,int b,int c){
    int ntimesused[]={0,0,0,0};
    int diff;
    while(finalt>0){
        diff=finalt-prev[finalt];//i took away last choise from T
        if(diff==a)
            ntimesused[1]++;
        if(diff==b)
            ntimesused[2]++;
        if(diff==c)
            ntimesused[3]++;
        finalt=prev[finalt];//new T is the T from prev, the new remaining time
        }
    printf("Optimal is done via choosing:\n");
    char letter='a';
    for (int i=1; i<=3; i++){
        printf("%c %d times\n",letter,ntimesused[i]);
        letter++;}
}


int main(){
    int T;
    scanf("%d",&T);
    int A,B,C;
    scanf("%d %d %d",&A,&B,&C);
    int *prev=calloc(T+1,sizeof(int));
    int result=solve(T,A,B,C,prev);
    if (result==-1){
        printf("nothing to do, IMP\n");
        free(prev);
        return -1;
    }
    printf("remaining T: %d\n",T-result);
    RebuildSol(prev,result,A,B,C);
    free(prev);
    return 0;
}