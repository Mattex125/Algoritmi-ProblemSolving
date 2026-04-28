#include <stdio.h>
#include <stdlib.h>

int countCurrentInPause(int pauses[], int t, int n, int *whenpausestarts){
    int currentInPause = 0;
    for (int i = 1; i <= n; i++) {
        if (whenpausestarts[i] != -1 &&
            t >= whenpausestarts[i] &&
            t < whenpausestarts[i] + pauses[i]) {
            currentInPause++;
        }
    }
    return currentInPause;
}

int solveT(int pauses[], int ttoal, int tpassed, int n, int*whenpausestarts){

    int CurrentInPause = countCurrentInPause(pauses, tpassed, n, whenpausestarts);
    if(CurrentInPause>2)//kill bad branches
    return -1;

    if(ttoal-tpassed!=0){ //rec fase, gotta study
//nessuno in pausa
        if (solveT(pauses, ttoal, tpassed+1, n, whenpausestarts) != -1)
            return 1;
//1 in pausa
        for(int i=1;i<=n;i++){//try all perms
            if (whenpausestarts[i] == -1) {
                // scelta
                whenpausestarts[i] = tpassed;
                if (countCurrentInPause(pauses, tpassed, n, whenpausestarts) <= 2 &&
                    solveT(pauses,ttoal,tpassed+1,n,whenpausestarts)!=-1)//if i got it solved
                    return 1;
                // UNDO my scelta (so i can do other choices)
                whenpausestarts[i] = -1;
            }
        }
//2 in pausa
        for(int i=1;i<=n;i++){
        if(whenpausestarts[i]==-1){
            for(int j=i+1;j<=n;j++){
                if(whenpausestarts[j]==-1){
                    whenpausestarts[i]=tpassed;
                    whenpausestarts[j]=tpassed;

                    if(countCurrentInPause(pauses, tpassed, n, whenpausestarts) <= 2 &&
                       solveT(pauses,ttoal,tpassed+1,n,whenpausestarts)!=-1)
                        return 1;

                    whenpausestarts[i]=-1;
                    whenpausestarts[j]=-1;
                }
            }
        }
    }
    return -1; //all choices done, nothing when good...
    }
    else //base case, all time consumed
    {
        int result=-1;
        for(int i=1; i<=n; i++){
            if(whenpausestarts[i] == -1){ //if someone didnt have a timeslot -->no sol
                return result = -1;
            }
        } 
        int CurrentInPause = countCurrentInPause(pauses, ttoal, n, whenpausestarts);//all time slots finisced
        if(CurrentInPause == 0){
            result = 1;
        }
        return result;
    }
}

int solve(int pauses[],int t,int n, int* whenpausestarts){
    for(int i=1; i<=n; i++)
        whenpausestarts[i]=-1;
    return solveT(pauses,t,0,n,whenpausestarts);
}

int main(){
    int T,N;
    scanf("%d %d",&T,&N);
    int *pauses=malloc(sizeof(int) * (N + 1));
    for (int i=1; i<=N; i++)
        scanf("%d",&pauses[i]);
    int *whenpausestarts=malloc(sizeof(int) * (N + 1));
    int sol = solve(pauses,T,N,whenpausestarts);
    if(sol==1){
        for(int i=1; i<=N; i++)
            printf("%d ", whenpausestarts[i]);
        printf("\n");
    }
    else{
        printf("no possible sol");
    }

    free(pauses);
    free(whenpausestarts);
}