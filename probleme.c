#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "probleme.h"

void afficherProbleme(glp_prob* prob) {
	int i, j;

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

int lireProbleme(const char* nomFichier, glp_prob* prob) {
/*Lit les donnees d'un probleme a partir d'un fichier specifie*/
/*et les place dans la structure prob*/

	FILE* fichier = NULL;
	char chTemp[20], signe[3];
	int i, j, num = 0;
	double numD = 0;

	if ((fichier = fopen(nomFichier, "r"))) {
		fscanf(fichier, "%s", chTemp);
		fscanf(fichier, "%d", &num);
		glp_add_cols(prob, num);
		fscanf(fichier, "%s", chTemp);
		fscanf(fichier, "%d", &num);
		glp_add_rows(prob, num);
		fscanf(fichier, "%s", chTemp);
		if (!strcmp(chTemp, "max"))
			glp_set_obj_dir(prob, GLP_MAX);
		else
			glp_set_obj_dir(prob, GLP_MIN);
		prob->fonc = (double*) malloc(sizeof(double) * prob->nVar);
		/*
		 * Fonction maximiser/minimiser
		 */
		for (j = 0; j < prob->nVar; j++) {
			fscanf(fichier, "%s", signe);
			fscanf(fichier, "%lf", &numD);
			if (!strcmp(signe, "-"))
				prob->fonc[j] = -prob->fonc[j];
			else

			fscanf(fichier, "%s", chTemp);
		}
		/*
		 * Contraintes
		 */
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
		perror("Erreur. Code ");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

void libererMemoireProbleme(glp_prob* prob) {
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
