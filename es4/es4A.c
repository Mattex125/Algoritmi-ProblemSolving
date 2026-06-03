#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

//bitmap data structure, IA generated
void setBit(uint64_t *bm, int p){
    bm[p / 64] |= (1ULL << (p % 64));}

void clearBit(uint64_t *bm, int p){
    bm[p / 64] &= ~(1ULL << (p % 64));}

int testBit(uint64_t *bm, int p){
    return (bm[p / 64] >> (p % 64)) & 1ULL;}

/*practically, per every uint64 has 64 bits, we map the 100000bits into words of 64 bits
word = p / 64;
bit  = p % 64;
*/

//Hackers Delight implementation 2 search nearest next 1 in bitmap
int findNextStation(uint64_t *bm, int current, int L) { //-->returns next station
    int next = current + 1;
    if (next > L) return L;

    int idx = next / 64;
    int shift = next % 64;

    // Crea una maschera per ignorare i bit precedenti alla posizione 'next' nel primo word
    uint64_t mask = (~0ULL) << shift;
    uint64_t word = bm[idx] & mask;

    while (word == 0) {
        idx++;
        if (idx > L / 64) return L;
        word = bm[idx];
    }

    // __builtin_ctzll usa l'istruzione hardware per contare gli zeri a destra (trailing zeros)
    // È l'implementazione moderna ed efficiente descritta in Hacker's Delight
    return (idx * 64) + __builtin_ctzll(word);
}

//gets min distance in total bitmap
int getspacing(uint64_t *bm, int L) {//bitmap and limit
    int min_dist = L;
    int curr = 0;
    
    while (curr < L) {
        int next = findNextStation(bm, curr, L);
        int dist = next - curr;
        if (dist < min_dist) //update min dist
            min_dist = dist;
        curr = next;
    }
    return min_dist;
}

// Backtracking
void brute_force(uint64_t *bm, int L, int M, int controllo, int curr, int count, int *cur_rem, int *best_rem, int *max_s) {
    int next_st = findNextStation(bm, curr, L);

    //base case, we are in L
    if (next_st >= L) {
        int s = getspacing(bm, L);
        if (s > *max_s) {
            *max_s = s;
            for (int i = 0; i < count; i++) best_rem[i] = cur_rem[i];
        }
        return;
    }

    // REC1: non togliamo next_st
    brute_force(bm, L, M, controllo, next_st, count, cur_rem, best_rem, max_s);

    //Proviamo a rimuovere next_st
    if (count < M && next_st != controllo) { //can M and not controllo
        clearBit(bm, next_st);          // La rimuovo dalla bitmap
        cur_rem[count] = next_st;       // La segno nell'array temporaneo
        
        //REC2
        brute_force(bm, L, M, controllo, next_st, count + 1, cur_rem, best_rem, max_s);
        
        setBit(bm, next_st);            // Backtrack: la ripristino per gli altri rami
    }
}

int main() {
    int L, N, M, controllo;
    printf("Inserisci L, N, M, Stazione Controllo:\n");
    scanf("%d %d %d %d", &L, &N, &M, &controllo);

    // Alloca Bitmap (L/64 + 1 parole da 64 bit)
    uint64_t *bitmap = calloc((L / 64) + 1, sizeof(uint64_t));
    
    //stazioni presenti default
    setBit(bitmap, 0);
    setBit(bitmap, L); 
    setBit(bitmap, controllo);

    int s; //reading stations
    int n_rem = 0;
    for (int i = 0; i < N; i++) {
        scanf("%d", &s);
        setBit(bitmap, s);
        if (s != 0 && s != L && s != controllo) n_rem++;
    }


    int *best_removed = malloc(M * sizeof(int));
    int *current_removed = malloc(M * sizeof(int));
    int max_spacing = -1;

    for (int i = 0; i < M; i++) best_removed[i] = -1;




    brute_force(bitmap, L, M, controllo, 0, 0, current_removed, best_removed, &max_spacing);

    // Output
    printf("Massimo valore spaziatura: %d\n", max_spacing);
    printf("Stazioni chiuse: ");
    for (int i = 0; i < M; i++) {
        if (best_removed[i] != -1) printf("%d ", best_removed[i]);
    }
    printf("\n");

    free(bitmap); free(best_removed); free(current_removed);
    return 0;
}


