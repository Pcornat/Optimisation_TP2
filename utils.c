//
// Created by postaron on 26/09/18.
//

#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Permet de vérifier si le pointeur et null (utiliser pour les allocations)
 * @param pointer Pointeur à vérifier.
 */
void checkErrorNull(void *pointer) {
    if (pointer == NULL) {
        fprintf(stderr, "Erreur : pointeur null.\n");
        exit(EXIT_FAILURE);
    }
}