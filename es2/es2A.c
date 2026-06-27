#include <stdio.h>
#include <stdlib.h>
/*
I can say that i create a decision tree, count nodes into it and then i get the one with maxnodes, nodes cost abc vals
*/

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

int solveT(int T,int A,int B, int C, int prev[]){//max ntask
    int atask=-1,btask=-1,ctask=-1;
    if(T==0){
        return 0;
    }
    if(T-A>=0)
        atask=solveT(T-A,A,B,C,prev);
    if(T-B>=0)
        btask=solveT(T-B,A,B,C,prev);
    if(T-C>=0)
        ctask=solveT(T-C,A,B,C,prev);
    int moretaskdone;
    int whatihavechoosen=choose_max(atask,btask,ctask,&moretaskdone);
    if (moretaskdone==-1) //i cannot choose
        return -1;
    int TremainingAfterChoice=T-get_abc(A,B,C,whatihavechoosen); //T-what i have used
    prev[T]=TremainingAfterChoice; //so i can see what's next to choose
    return moretaskdone+1; //new branch counted
}

int solve(int T,int A,int B,int C,int *prev){//returns time used
    int result=solveT(T,A,B,C,prev); //try for t
    if (result==-1){//if cannot do it in T time i have to look for less
        int i=T-1;
        for (;i>0 &&result==-1;i--){ //i try until i get sol
            result=solveT(i,A,B,C,prev);
        }
        if (i!=0) //if there's good sol
            return ++i;  // return the time that was actually used
        else
            return -1; //no sol 
    }
    else
        return T;
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
    if(result==-1){
        printf("nothing to do, IMP\n");
        free(prev);
        return -1;
    }
    printf("remaining T: %d\n",T-result);
    RebuildSol(prev,result,A,B,C);
    free(prev);
    return 0;
}