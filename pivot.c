#include "pivot.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <float.h>

/**
 * Nombre de ligne dans une matrice : nombre de contrainte.
 * Nombre de colonne matrice = nombre variable
 * @param prob
 * @return Le pointeur de ma matrice pivot.
 */
double **initMatPivot(prob_t *prob) {
    double **matrice = NULL;
    unsigned int nbColonne = prob->nVar + prob->nCont + 1;
    checkErrorNull(matrice = (double **) calloc((prob->nCont + 1), sizeof(double *)));
    for (unsigned int i = 0; i < prob->nCont + 1; ++i) {
        checkErrorNull(matrice[i] = (double *) calloc(nbColonne, sizeof(double)));
        matrice[i][nbColonne - 1] = prob->valCont[i];
        for (unsigned int j = 0; j < prob->nVar; ++j) {
            if (i < prob->nCont)
                matrice[i][j] = prob->cont[i][j];
            if (i == prob->nCont)
                matrice[prob->nCont][j] = prob->fonc[j];
        }
    }
    for (unsigned int i = 0, j = prob->nVar; i < prob->nCont && j < (prob->nVar + prob->nCont); ++i, ++j) {
        matrice[i][j] = 1;
    }
    return matrice;
}

/**
 * Affiche la matrice dans prob.
 * @param prob Contient la matrice pivot à afficher.
 * @param matrice
 */
void afficherMatrice(prob_t *prob, double **matrice) {
    for (unsigned int i = 0; i < prob->nCont + 1; ++i) {
        for (unsigned int j = 0; j < (prob->nVar + prob->nCont + 1); ++j) {
            printf("%2.4lf\t", matrice[i][j]);
        }
        printf("\n");
    }
}

/**
 * Permet de sélectionner la variable d'entrée hors-base pour le simplexe.
 * @param prob
 * @param matrice
 * @return L'indice de la colonne avec le coefficient le plus grand.
 */
int selectionnerColPivot(prob_t *prob, double **matrice) {
	unsigned int nbColonne = prob->nVar + prob->nCont + 1;
    int j = -1;
    double max = 0;
    for (unsigned int i = 0; i < nbColonne; ++i) {
        if (matrice[prob->nCont][i] > max) {
            max = matrice[prob->nCont][i];
            j = i;
        }
    }

    return j;
}

/**
 * Permet de trouver l'indice de la ligne pivot de la matrice
 * @param prob
 * @param matrice
 * @param nColPivot
 * @return
 */
int selectionnerLignePivot(prob_t *prob, double **matrice, int nColPivot) {
	unsigned int nLignePivot = 0, colonneMax = prob->nVar + prob->nCont;
	double rapport = 0, min = DBL_MAX;
    for (unsigned int i = 0; i < prob->nCont; ++i) {
        rapport = matrice[i][colonneMax] / matrice[i][nColPivot];
        if (min > rapport) {
            min = rapport;
            nLignePivot = i;
        }
    }
    return nLignePivot;
}

/**
 * Va diviser la ligne pivot par le pivot. Ce qui fait que le pivot vaudra donc 1
 * @param prob
 * @param matrice
 * @param lignePivot
 * @param colPivot
 */
void diviserLignePivot(prob_t *prob, double **matrice, int lignePivot, int colPivot) {
    double pivot = matrice[lignePivot][colPivot];
    unsigned int nbColonne = prob->nVar + prob->nCont + 1;
    for (unsigned int i = 0; i < nbColonne; ++i) {
        matrice[lignePivot][i] = matrice[lignePivot][i] / pivot;
    }
}

/**
 * Mise à zéro de la colonne du pivot.
 * @param prob
 * @param matrice
 * @param lignePivot
 * @param colPivot
 */
void miseAZeroColPivot(prob_t *prob, double **matrice, unsigned int lignePivot, unsigned int colPivot) {
    unsigned int nbColonne = prob->nVar + prob->nCont + 1;
    //Mise à zéro de la colonne pivot sauf le pivot lui-même
    for (unsigned int i = 0; i < prob->nCont + 1; ++i) {
        if (i != lignePivot)
            matrice[i][colPivot] = 0;
    }
    for (unsigned int k = 0; k < prob->nCont + 1; ++k)
        for (unsigned int j = 0; j < nbColonne; ++j) {
            if (k != lignePivot && j != colPivot)
                matrice[k][j] -= (matrice[k][colPivot] / matrice[lignePivot][colPivot]) * matrice[lignePivot][j];
        }
}

/**
 * Libère la mémoire allouée dynamiquement pour la matrice.
 * @param prob
 * @param matrice
 */
void libererMatrice(prob_t *prob, double ***matrice) {
    for (unsigned int i = 0; i < prob->nCont + 1; ++i) {
        free(((*matrice)[i]));
    }
    free(*matrice);
    *matrice = NULL;
}

/**************************************/
