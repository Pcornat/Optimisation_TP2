#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <glpk.h>

#include "types.h"
#include "pivot.h"


int lireProbleme(const char *, prob_t *);

void afficherProbleme(prob_t);

void libererMemoireProbleme(prob_t);

int *heuristique(prob_t *);
