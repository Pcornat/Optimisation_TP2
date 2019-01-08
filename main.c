#include <stdio.h>
#include <stdlib.h>
#include "probleme.h"


int main(int argc, char* argv[]) {
	prob_t prob = {0, 0, 0, NULL, NULL, NULL, NULL};
	if (argc != 2) {
		fprintf(stderr, "Nombre d'argument insuffisant.\n");
		exit(EXIT_FAILURE);
	}


	lireProbleme(argv[1], &prob);
	int *heuris = (int *) malloc(sizeof(int) * (prob.nVar + 1));

	heuristique(&prob, heuris);
	affichage(heuris);
	return EXIT_SUCCESS;
}