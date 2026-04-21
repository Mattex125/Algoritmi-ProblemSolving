#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int *state;
    int value;
} MemoEntry;

typedef struct {
    MemoEntry *entries;
    int size;
    int cap;
} MemoBucket;

static int statesEqual(const int *a, const int *b, int n) {
    for (int i = 1; i <= n; i++) {
        if (a[i] != b[i]) {
            return 0;
        }
    }
    return 1;
}

static int memoGet(MemoBucket memo[], int t, int whenpausestarts[], int n, int *outValue) {
    MemoBucket *bucket = &memo[t];
    for (int i = 0; i < bucket->size; i++) {
        if (statesEqual(bucket->entries[i].state, whenpausestarts, n)) {
            *outValue = bucket->entries[i].value;
            return 1;
        }
    }
    return 0;
}

static void memoPut(MemoBucket memo[], int t, int whenpausestarts[], int n, int value) {
    MemoBucket *bucket = &memo[t];
    for (int i = 0; i < bucket->size; i++) {
        if (statesEqual(bucket->entries[i].state, whenpausestarts, n)) {
            bucket->entries[i].value = value;
            return;
        }
    }

    if (bucket->size == bucket->cap) {
        int newCap = (bucket->cap == 0) ? 8 : bucket->cap * 2;
        MemoEntry *newEntries = realloc(bucket->entries, sizeof(MemoEntry) * newCap);
        if (newEntries == NULL) {
            return;
        }
        bucket->entries = newEntries;
        bucket->cap = newCap;
    }

    bucket->entries[bucket->size].state = malloc(sizeof(int) * (n + 1));
    if (bucket->entries[bucket->size].state == NULL) {
        return;
    }
    memcpy(bucket->entries[bucket->size].state, whenpausestarts, sizeof(int) * (n + 1));
    bucket->entries[bucket->size].value = value;
    bucket->size++;
}

static void freeMemo(MemoBucket memo[], int tmax) {
    for (int t = 0; t <= tmax; t++) {
        for (int i = 0; i < memo[t].size; i++) {
            free(memo[t].entries[i].state);
        }
        free(memo[t].entries);
    }
    free(memo);
}

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

int solveT(int pauses[], int ttoal, int tpassed, int n, int *whenpausestarts, MemoBucket memo[]){

    int cachedValue;
    if (memoGet(memo, tpassed, whenpausestarts, n, &cachedValue)) {
        return cachedValue;
    }

    int CurrentInPause = countCurrentInPause(pauses, tpassed, n, whenpausestarts);
    if(CurrentInPause>2) {//kill bad branches
        memoPut(memo, tpassed, whenpausestarts, n, -1);
        return -1;
    }

    if(ttoal-tpassed!=0){ //rec fase, gotta study
//nessuno in pausa
        if (solveT(pauses, ttoal, tpassed+1, n, whenpausestarts, memo) != -1) {
            memoPut(memo, tpassed, whenpausestarts, n, 1);
            return 1;
        }
//1 in pausa
        for(int i=1;i<=n;i++){//try all perms
            if (whenpausestarts[i] == -1) {
                // scelta
                whenpausestarts[i] = tpassed;
                if (solveT(pauses,ttoal,tpassed+1,n,whenpausestarts,memo)!=-1) {//if i got it solved
                    memoPut(memo, tpassed, whenpausestarts, n, 1);
                    return 1;
                }
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

                    if(solveT(pauses,ttoal,tpassed+1,n,whenpausestarts,memo)!=-1) {
                        memoPut(memo, tpassed, whenpausestarts, n, 1);
                        return 1;
                    }

                    whenpausestarts[i]=-1;
                    whenpausestarts[j]=-1;
                }
            }
        }
    }
    memoPut(memo, tpassed, whenpausestarts, n, -1);
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
        memoPut(memo, tpassed, whenpausestarts, n, result);
        return result;
    }
}

int solve(int pauses[],int t,int n, int* whenpausestarts){
    for(int i=1; i<=n; i++)
        whenpausestarts[i]=-1;
    MemoBucket *memo = calloc(t + 1, sizeof(MemoBucket));
    int result = solveT(pauses, t, 0, n, whenpausestarts, memo);
    freeMemo(memo, t);
    return result;
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