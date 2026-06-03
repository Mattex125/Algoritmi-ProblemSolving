#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//bitmap data structure, IA generated
void setBit(uint64_t *bm, long int p){
    bm[p / 64] |= (1ULL << (p % 64));}

void clearBit(uint64_t *bm, long int p){
    bm[p / 64] &= ~(1ULL << (p % 64));}

int testBit(uint64_t *bm, long int p){
    return (bm[p / 64] >> (p % 64)) & 1ULL;}

/*practically, per every uint64 has 64 bits, we map the 100000bits into words of 64 bits
word = p / 64;
bit  = p % 64;
*/

//Hackers Delight implementation 2 search nearest next 1 in bitmap
long int findNextStation(uint64_t *bm, long int current, long int L) { //-->returns next station
    long int next = current + 1;
    if (next > L) return L;

    long int idx = next / 64;
    long int shift = next % 64;

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
long int getspacing(uint64_t *bm, long int L) {//bitmap and limit
    long int min_dist = L;
    long int curr = 0;
    
    while (curr < L) {
        long int next = findNextStation(bm, curr, L);
        long int dist = next - curr;
        if (dist < min_dist) //update min dist
            min_dist = dist;
        curr = next;
    }
    return min_dist;
}

int IsFeasableWithD(uint64_t *bitmap, long int L, long int controlled, int M, long int D, long int removed[]){
    //copy bitmap (less than 30Mb)
    //IA generated fast memcopy
    size_t words = (L / 64) + 1;
    size_t bytes = words * sizeof(uint64_t);
    uint64_t *bm = NULL; // tmp bitmap (aligned)
    if (posix_memalign((void**)&bm, 64, bytes) != 0) {
        printf("allocation failed\n"); /*allocation failed*/
    }
    memcpy(bm, bitmap, bytes);

    int removedCount=0;
    // togli le stazioni a dist D or less than controllo
    long int start = controlled - D;
    if (start < 0) start = 0; //start isnt negative
    long int end = controlled + D; //dont go out of bounds
    if (end > L) end = L;
    for (long int h = start; h <= end; h++) { //while distub to controll interval
        if (h == controlled) continue;
        if (testBit(bm, h)) {
            if (removedCount > M) {
                free(bm);
                return -1; // 
            }
            clearBit(bm, h);
            removed[removedCount++] = h;
        }
    }
    //assicuro D per il primo
    long int h=findNextStation(bm,0,D);
    while(h!=D){//lookup until the first is safe
        if(removedCount > M){
            free(bm);
            return -1; // not feasable
        }
        else{
            if(h!=controlled){//if is not bit controlled
                clearBit(bm,h);
                removed[removedCount++]=h;
            }
            else{  //cant remove controlled (btw should be already covered up)
                free (bm);
                return -1; // not feasable
            }
        }
        h=findNextStation(bm,0,D); //lookup if there are other ones
    }
    //policy: if s1 and s2 are to near we remove s1
    long int prev=findNextStation(bm,0,L);
    long int next;
    while(prev!=L){
        next=findNextStation(bm,prev,L);
        if(next != controlled && prev!=controlled && next-prev<D){//not controllo involved and bad dist
            if(removedCount >= M){
                free(bm);
                return -1; // not feasable
            }
            clearBit(bm,prev);
            removed[removedCount++]=prev;
        }
        prev=next;
    }
    free(bm);
    return removedCount;
}


int main() {
    long int L;
    int N, M;
    long int controllo;
    printf("Inserisci L, N, M, Stazione Controllo:\n");
    scanf("%ld %d %d %ld", &L, &N, &M, &controllo);

    // Alloca Bitmap (L/64 + 1 parole da 64 bit)
    uint64_t *bitmap = calloc((L / 64) + 1, sizeof(uint64_t));
    
    //stazioni presenti default
    setBit(bitmap, 0);
    setBit(bitmap, L); 
    setBit(bitmap, controllo);

    long int s; //reading stations
    int n_rem = 0;
    for (int i = 0; i < N; i++) {
        scanf("%ld", &s);
        setBit(bitmap, s);
        if (s != 0 && s != L && s != controllo) n_rem++;
    }

    long int low=0,high=L+1,mid=L/2;
    long int *removed=malloc(M*sizeof(long int));
    int feasable;
    //binary search for optimizing feasability
    while(high-low>1){
        mid=low + (high - low) / 2;
        feasable=IsFeasableWithD(bitmap,L,controllo,M,mid,removed);
        if(feasable != -1) // not feasable
            low=mid;
        else
            high=mid;
    }
    //one last testing if i got to optimal via a unfeasable before
    feasable=IsFeasableWithD(bitmap,L,controllo,M,low,removed);
    // Output richiesto 
    printf("Massimo valore spaziatura: %ld\n", low);
    printf("Stazioni chiuse: ");
    
    //if not feasable
    if (feasable <= 0) {
        printf("0");
    } else {
        for (int i = 0; i < feasable; i++) {
            printf("%ld ", removed[i]);
        }
    }
    printf("\n");
    free(bitmap); free(removed);
    return 0;
}
