#ifndef _TYPES_H
#define _TYPES_H

typedef struct prob_type {
    unsigned int nVar;      /*nombre de variables*/
    unsigned int nCont;     /*nombre de contraintes*/
    unsigned int typeOpt;   /*type d'optimisation : 0 si min et 1 si max*/
    double *fonc;            /*fonction objectif, coefficients des xj*/
    double **cont;        /*contraintes, coefficients des aij*/
    int *signeCont;        /*signe de chaque contrainte, 0 si <=, 1 si >= */
    double *valCont;        /*valeur associee a chaque contrainte : bi*/
} prob_t;

#endif
