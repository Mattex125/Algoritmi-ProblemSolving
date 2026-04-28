#include <stdio.h>
#include <stdlib.h>

/*
  Versione in stile "tuo codice" della Soluzione B.
  Idee principali (come descritto):
  - trasformiamo il problema in uno zaino: cerchiamo un sottoinsieme
    di pause con somma s tale che totalpauses - T <= s <= T.
  - se esiste, dividiamo i lavoratori in due pile A e B (A = sottoinsieme)
    e mettiamo i lavoratori di ogni pila in pausa uno dopo l'altro,
    quindi in ogni pila c'è al massimo 1 lavoratore in pausa.
  - se la verifica fallisce => nessuna soluzione.
*/

//IA generated zaino
int knapsack_fill(int pauses[], int T, int n, int *prev, int *picked){
    int *reachable;

    reachable = calloc(T + 1, sizeof(int));

    for (int s = 0; s <= T; s++){
        prev[s] = -1;
        picked[s] = -1;
    }

    reachable[0] = 1;

    for (int i = 1; i <= n; i++){
        int d;
        d = pauses[i];
        for (int s = T; s >= d; s--){
            if (reachable[s - d] == 1 && reachable[s] == 0){
                reachable[s] = 1;
                prev[s] = s - d;
                picked[s] = i;
            }
        }
    }

    free(reachable);

    return 0;
}

/* buildAstack: esegue il backtracing a partire da 'found' e
   ritorna un array inA allocato (inA[i]=1 se i appartiene ad A).
   Restituisce NULL in caso di errore (backtracing invalido). */
int *buildAstack(int n, int prev[], int picked[], int found){
    int *inA;
    int i;
    int trace;
    int w;

    inA = calloc(n + 1, sizeof(int));

    trace = found;
    while (trace > 0){
        w = picked[trace];
        if (w < 1 || w > n){
            free(inA);
            return NULL;
        }
        inA[w] = 1;
        trace = prev[trace];
    }

    return inA;
}

int solve(int pauses[], int T, int n, int *whenpausestarts){
    int totalpauses=0; /* somma di tutte le pause */

    for (int i = 1; i <= n; i++){
        whenpausestarts[i] = -1;
        if (pauses[i] > T) //a pause is bigger than worktime
            return -1;
        totalpauses = totalpauses + pauses[i];
    }
    if (totalpauses > 2*T) //if is imp to fit all
        return -1;

    int *prev= malloc(sizeof(int) * (T + 1));   /* prev[s] = somma precedente nel backtracking */
    int *picked= malloc(sizeof(int) * (T + 1)); /* picked[s] = lavoratore scelto per arrivare a s */

    knapsack_fill(pauses, T, n, prev, picked);

    /* Cerco una somma valida per la pila A:
       deve stare in [totalpauses - T, T], cosi anche la pila B <= T */
    int lower; /* limite inferiore valido per la somma della pila A */

    lower = totalpauses - T;
    if (lower < 0){
        lower = 0;
    }

    int found = -1; //is there a good Astack valid for B too?

    for (int i = T; i >= lower; i--){
        if (i == 0){
            found = 0;
            break;
        }
        if (picked[i] != -1){
            found = i;
            break;
        }
    }

    if (found == -1){
        free(prev);
        free(picked);
        return -1;
    }

    /* Backtracing: uso la funzione buildAstack per ottenere inA */
    int *inA = buildAstack(n, prev, picked, found);

    //costruisco pause
    int timeA=0; /* tempo cumulato usato nella pila A */
    int timeB=0; /* tempo cumulato usato nella pila B */

    for (int i = 1; i <= n; i++){
        if (inA[i] == 1){
            whenpausestarts[i] = timeA;
            timeA = timeA + pauses[i];
        }
    }
    for (int i = 1; i <= n; i++){
        if (inA[i] == 0){
            whenpausestarts[i] = timeB;
            timeB = timeB + pauses[i];
        }
    }
    free(prev);
    free(picked);
    free(inA);
    return 1;
}

int main(){
    int T, N;
    if (scanf("%d %d", &T, &N) != 2) return 0;
    int *pauses = malloc(sizeof(int) * (N + 1));
    int *whenpausestarts = malloc(sizeof(int) * (N + 1));
    for (int i = 1; i <= N; i++) 
        scanf("%d", &pauses[i]);
    int sol = solve(pauses, T, N, whenpausestarts);
    if (sol == 1){
        for (int i = 1; i <= N; i++) 
            printf("%d ", whenpausestarts[i]);
        printf("\n");
    } else {
        printf("no possible sol");
    }
    free(pauses); free(whenpausestarts);
    return 0;
}