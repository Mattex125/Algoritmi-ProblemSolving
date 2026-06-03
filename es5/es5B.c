#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct Candidato{
    int id;
    int a; // affidabilità
    int k; // criticità
} Candidato;

//qsort compare
int confronta_affidabilita(const void *c1, const void *c2) {
    Candidato *cand1 = (Candidato *)c1;
    Candidato *cand2 = (Candidato *)c2;
    return cand2->a - cand1->a; 
}

int calcola_max(int a, int b) { return (a > b) ? a : b; }
int calcola_min(int a, int b) { return (a < b) ? a : b; }

int main() {
    int C, P;
    scanf("%d %d", &C, &P);

    Candidato *candidati = malloc((C + 1) * sizeof(Candidato));
    //count from 1

    for (int i = 1; i <= C; i++) {
        candidati[i].id = i;
        scanf("%d %d", &candidati[i].a, &candidati[i].k);
    }

    if (P <= 0 || P > C) {
        printf("no possible sol\n");
        free(candidati);
        return 0;
    }

    // 1. sorting candidates via qsort, first) bestA last) worseA
    qsort(&candidati[1], C, sizeof(Candidato), confronta_affidabilita);

    // 2. init dp[p][i]:
    // p = numero di elementi nel team (fino a P)
    // i = considerando i primi i candidati
    int **dp = (int **)malloc(P * sizeof(int *));
    for (int i = 0; i < P; i++) {
        dp[i] = (int *)malloc((C + 1) * sizeof(int));
    }

    // init row 1 --> no people to choice
    for (int i = 0; i <= C; i++) {
        dp[0][i] = 0; 
    }
    
    // 3.dp start
    for (int p = 1; p < P; p++) { //for al increasing n-people teams
        for (int i = 0; i <= C; i++) { //init to IMP
            dp[p][i] = INT_MAX;
        }
        
        // per every i candidate
        for (int i = p; i <= C; i++) {
            int k_escludi_i = dp[p][i - 1]; // dont take i cand
            
            // take i candidate
            // la criticità di i e la criticità del miglior gruppo di dimensione p-1
            int k_includi_i = INT_MAX;
            if (dp[p - 1][i - 1] != INT_MAX) {//if before was possible
                k_includi_i = calcola_max(dp[p - 1][i - 1], candidati[i].k); //magg criticità
            }
            
            // the best is less k
            dp[p][i] = calcola_min(k_escludi_i, k_includi_i);
        }
    }

    // 4 max the
    int best = -INT_MAX;
    int ultimo_membro_idx = -1;

    //take best k until the i-1 person with total-1 people
    for (int i = P; i <= C; i++) {
        if (dp[P - 1][i - 1] != INT_MAX) {
            // Take i member
            int criticita_team = calcola_max(dp[P - 1][i - 1], candidati[i].k);
            
            // compute quality
            int qualita_attuale = candidati[i].a - criticita_team;
            
            //save best
            if (qualita_attuale > best) {
                best = qualita_attuale;
                ultimo_membro_idx = i;
            }
        }
    }

    // 5. Backtracking for people in squad (IA generated)
    int *team_scelto = (int *)malloc(P * sizeof(int));
    int team_size = 0;
    
    // get last member
    team_scelto[team_size++] = candidati[ultimo_membro_idx].id;
    
    // IA genereated rebuild (same than knapsack)
    int curr_p = P - 1;
    int curr_i = ultimo_membro_idx - 1;
    
    while (curr_p > 0) {
        // Se il valore è uguale alla cella precedente, significa che il candidato curr_i è stato escluso
        if (curr_i > curr_p && dp[curr_p][curr_i] == dp[curr_p][curr_i - 1]) {
            curr_i--;
        } else {
            // Altrimenti, il candidato curr_i è stato incluso
            team_scelto[team_size++] = candidati[curr_i].id;
            curr_p--;
            curr_i--;
        }
    }

    //result
    printf("Qualità massima del team: %d\n", best);
    printf("Componenti del team scelti (ID): ");
    for (int i = 0; i < P; i++) {
        printf("%d ", team_scelto[i]);
    }
    printf("\n");

    // Pulizia della memoria
    for (int i = 0; i < P; i++) free(dp[i]);
    free(dp);
    free(team_scelto);
    free(candidati);

    return 0;
}