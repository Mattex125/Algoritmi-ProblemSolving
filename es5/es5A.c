#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

typedef struct Candidato {
	int id;
	int a;
	int k;
} Candidato;

int calcola_max(int a, int b) { return (a > b) ? a : b; }

long double permutations(int C, int P) {
	long double perm = 1.0L;

	if (P < 0 || P > C) return 0.0L;
	if (P == 0 || P == C) return 1.0L;

	if (P > C - P) P = C - P;
	for (int i = 1; i <= P; i++) {
		perm = perm * (long double)(C - P + i) / (long double)i;
	}
	return perm;
}

int calcola_tentativi(int C, int P, long double target) { //compute n try's to be 99% sure to have seen the best
	long double combinazioni = permutations(C, P);
	long double fallimento_singolo;
	long double fallimento_totale;
	int tentativi;

	if (combinazioni <= 1.0L) return 1;

	fallimento_singolo = 1.0L - (1.0L / combinazioni);
	fallimento_totale = 1.0L;
	tentativi = 0;

	while (1.0L - fallimento_totale < target) {
		fallimento_totale = fallimento_totale * fallimento_singolo;
		tentativi++;
		if (tentativi > 100000000) break;
	}
	return tentativi;
}

void genera_team_random(int C, int P, int choices[]) {
	int *pool;
	int tmp;
	int j;

	pool = malloc((C + 1) * sizeof(int));
	for (int i = 1; i <= C; i++)
		pool[i] = i;

	for (int i = 1; i <= P; i++) {
		j = i + (rand() % (C - i + 1));
		tmp = pool[i];
		pool[i] = pool[j];
		pool[j] = tmp;
		choices[i - 1] = pool[i];
	}

	free(pool);
}

int valuta_team(Candidato candidati[], int choices[], int P) {
	int maxk = INT_MIN;
	int mina = INT_MAX;

	for (int i = 0; i < P; i++) {
		int idx = choices[i];
		maxk = calcola_max(maxk, candidati[idx].k);
		if (candidati[idx].a < mina) mina = candidati[idx].a;
	}

	return mina - maxk;
}

int main() {
	int C, P;
	scanf("%d %d", &C, &P);

	Candidato *candidati = malloc((C + 1) * sizeof(Candidato));
	//count from 1
	candidati[0].id = NULL;
	candidati[0].a = NULL;
	candidati[0].k = NULL;

	for (int i = 1; i <= C; i++) {
		candidati[i].id = i;
		scanf("%d %d", &candidati[i].a, &candidati[i].k);
	}

	if (P <= 0 || P > C) {
		printf("no possible sol\n");
		free(candidati);
		return 0;
	}

	srand((unsigned int)time(NULL));//set seed

	long double target = 0.9999L;
	int tentativi = calcola_tentativi(C, P, target);
	if (tentativi < 1) tentativi = 1;

	int *choices = malloc(P * sizeof(int));
	int *best_choices = malloc(P * sizeof(int));
	int best = -INT_MAX;

	for (int t = 0; t < tentativi; t++) {//try and save the best
		genera_team_random(C, P, choices);

		int qualita = valuta_team(candidati, choices, P);
		if (qualita > best) {
			best = qualita;
			for (int i = 0; i < P; i++) {
				best_choices[i] = choices[i];
			}
		}
	}

	printf("Qualità massima del team: %d\n", best);
	printf("Componenti del team scelti (ID): ");
	for (int i = 0; i < P; i++) {
		printf("%d ", candidati[best_choices[i]].id);
	}
	printf("\n");

	free(choices);
	free(best_choices);
	free(candidati);
	return 0;
}
