#pragma once

#include "types.h"

double **initMatPivot(prob_t *);

void libererMatrice(prob_t *, double ***);

void afficherMatrice(prob_t *, double **);

int selectionnerColPivot(prob_t *, double **);

int selectionnerLignePivot(prob_t *, double **, int);

void diviserLignePivot(prob_t *, double **, int, int);

void miseAZeroColPivot(prob_t *, double **, unsigned int, unsigned int);