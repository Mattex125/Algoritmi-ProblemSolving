#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int idx;
    int duration;
} WorkerPause;

int cmpDescDuration(const void *a, const void *b) {
    const WorkerPause *wa = (const WorkerPause *)a;
    const WorkerPause *wb = (const WorkerPause *)b;
    if (wb->duration != wa->duration) {
        return wb->duration - wa->duration;
    }
    return wa->idx - wb->idx;
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

int solve(int pauses[],int t,int n, int* whenpausestarts){
    int loadA = 0;
    int loadB = 0;
    WorkerPause *workers = malloc(sizeof(WorkerPause) * (size_t)n);
    int *pileA = malloc(sizeof(int) * (size_t)n);
    int *pileB = malloc(sizeof(int) * (size_t)n);
    int sizeA = 0;
    int sizeB = 0;

    if (workers == NULL || pileA == NULL || pileB == NULL) {
        free(workers);
        free(pileA);
        free(pileB);
        return -1;
    }

    for (int i = 1; i <= n; i++) {
        whenpausestarts[i] = -1;
        if (pauses[i] > t) {
            free(workers);
            free(pileA);
            free(pileB);
            return -1;
        }
        workers[i - 1].idx = i;
        workers[i - 1].duration = pauses[i];
    }

    qsort(workers, (size_t)n, sizeof(WorkerPause), cmpDescDuration);

    for (int i = 0; i < n; i++) {
        int worker = workers[i].idx;
        int d = workers[i].duration;
        int placed = 0;

        if (loadA <= loadB) {
            if (loadA + d <= t) {
                pileA[sizeA++] = worker;
                loadA += d;
                placed = 1;
            } else if (loadB + d <= t) {
                pileB[sizeB++] = worker;
                loadB += d;
                placed = 1;
            }
        } else {
            if (loadB + d <= t) {
                pileB[sizeB++] = worker;
                loadB += d;
                placed = 1;
            } else if (loadA + d <= t) {
                pileA[sizeA++] = worker;
                loadA += d;
                placed = 1;
            }
        }

        if (!placed) {
            free(workers);
            free(pileA);
            free(pileB);
            return -1;
        }
    }

    int timeA = 0;
    int timeB = 0;

    for (int i = 0; i < sizeA; i++) {
        int worker = pileA[i];
        whenpausestarts[worker] = timeA;
        timeA += pauses[worker];
    }

    for (int i = 0; i < sizeB; i++) {
        int worker = pileB[i];
        whenpausestarts[worker] = timeB;
        timeB += pauses[worker];
    }

    if (timeA > t || timeB > t) {
        free(workers);
        free(pileA);
        free(pileB);
        return -1;
    }

    free(workers);
    free(pileA);
    free(pileB);

    return 1;
}

int main(){
    int T,N;
    if (scanf("%d %d", &T, &N) != 2) {
        return 1;
    }
    int *pauses=malloc(sizeof(int) * (N + 1));
    int *whenpausestarts=malloc(sizeof(int) * (N + 1));
    if (pauses == NULL || whenpausestarts == NULL) {
        free(pauses);
        free(whenpausestarts);
        return 1;
    }
    for (int i=1; i<=N; i++) {
        if (scanf("%d", &pauses[i]) != 1) {
            free(pauses);
            free(whenpausestarts);
            return 1;
        }
    }
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