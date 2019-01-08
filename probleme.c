#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "probleme.h"


void afficherProbleme(prob_t prob) {
    unsigned int i, j;

	printf("\n");
	if (prob.typeOpt)
		printf("Maximiser z = ");
	else
		printf("Minimiser z = ");

	for (j = 0; j < prob.nVar; j++) {
		if (prob.fonc[j] < 0)
			printf("- ");
		else
			printf("+ ");
		printf("%6.2lf ", fabs(prob.fonc[j]));
		printf("x%d ", j + 1);
	}
	printf("\n\n");
	printf("Sous les contraintes\n\n");
	for (i = 0; i < prob.nCont; i++) {
		for (j = 0; j < prob.nVar; j++) {
			if (prob.cont[i][j] < 0)
				printf("- ");
			else
				printf("+ ");
			printf("%6.2lf ", fabs(prob.cont[i][j]));
			printf("x%d ", j + 1);
		}
		if (prob.signeCont[i] == 0)
			printf("<= ");
		else if (prob.signeCont[i] == 1)
			printf(">= ");
		if (prob.valCont[i] < 0)
			printf("- ");
		else
			printf("+ ");
		printf("%6.2lf\n", fabs(prob.valCont[i]));
	}
	printf("\n");
	for (j = 0; j < prob.nVar; j++) {
		printf("x%d >= 0", j + 1);
		if (j < prob.nVar - 1)
			printf(", ");
	}
	printf("\n");
}

int lireProbleme(char *nomFichier, prob_t *prob) {
    /*Lit les donnees d'un probleme a partir d'un fichier specifie*/
    /*et les place dans la structure prob*/

    FILE *fichier;
	char chTemp[20], signe[3];
    unsigned int i, j;

	if ((fichier = fopen(nomFichier, "r"))) {
		fscanf(fichier, "%s", chTemp);
        fscanf(fichier, "%ud", &prob->nVar);
		fscanf(fichier, "%s", chTemp);
        fscanf(fichier, "%ud", &prob->nCont);
		fscanf(fichier, "%s", chTemp);
		if (!strcmp(chTemp, "max"))
            prob->typeOpt = 1;
		else
            prob->typeOpt = 0;
		prob->fonc = (double*) malloc(sizeof(double) * prob->nVar);
		for (j = 0; j < prob->nVar; j++) {
			fscanf(fichier, "%s", signe);
            fscanf(fichier, "%lf", &prob->fonc[j]);
			if (!strcmp(signe, "-"))
				prob->fonc[j] = -prob->fonc[j];
			fscanf(fichier, "%s", chTemp);
		}
		prob->cont = (double**) malloc(sizeof(double*) * prob->nCont);
		prob->signeCont = (int*) malloc(sizeof(int) * prob->nCont);
		prob->valCont = (double*) malloc(sizeof(double) * prob->nCont);
		for (i = 0; i < prob->nCont; i++) {
			prob->cont[i] = (double*) malloc(sizeof(double) * prob->nVar);
			for (j = 0; j < prob->nVar; j++) {
				fscanf(fichier, "%s", signe);
				fscanf(fichier, "%lf", &prob->cont[i][j]);
				if (!strcmp(signe, "-"))
					prob->cont[i][j] = -prob->cont[i][j];
				fscanf(fichier, "%s", chTemp);
			}
			fscanf(fichier, "%s", signe);
			if (!strcmp(signe, "<="))
				prob->signeCont[i] = 0;
			else if (!strcmp(signe, ">="))
				prob->signeCont[i] = 1;
			fscanf(fichier, "%lf", &prob->valCont[i]);
		}
		fclose(fichier);
	} else {
        printf("Probleme lecture fichier\n");
        return -1;
    }

    return 0;
}

void libererMemoireProbleme(prob_t prob) {
	int i;

	if (prob.fonc != NULL)
		free(prob.fonc);
	for (i = 0; i < prob.nCont; i++)
		if (prob.cont[i] != NULL)
			free(prob.cont[i]);
	if (prob.cont != NULL)
		free(prob.cont);
	if (prob.signeCont != NULL)
		free(prob.signeCont);
	if (prob.valCont != NULL)
		free(prob.valCont);
}

int *heuristique(prob_t *prob, int *heuris) {
    double **pivot = initMatPivot(prob);
	afficherMatrice(prob, pivot);

    int colPivot = selectionnerColPivot(prob, pivot);
    int lignePivot;

    while (colPivot != -1) {
		lignePivot = selectionnerLignePivot(prob, pivot, colPivot);
		heuris[colPivot + 1] = (int) (pivot[lignePivot][prob->nVar + prob->nCont] / pivot[lignePivot][colPivot]);

		for (unsigned int i = 0; i < prob->nCont; ++i) {
			pivot[i][prob->nVar + prob->nCont] -= heuris[colPivot + 1] * pivot[i][colPivot];
        }

        pivot[prob->nCont][colPivot] = 0;

        colPivot = selectionnerColPivot(prob, pivot);
    }

	afficherMatrice(prob, pivot);
	heuris[0] = -pivot[prob->nCont][prob->nVar + prob->nCont];

	return heuris;

}

void affichage(int *heuris) {
	printf("Z = %d\n", heuris[0]);
	printf("X1 = %d, X2 = %d, X3 = %d \n", heuris[1], heuris[2], heuris[3]);
}