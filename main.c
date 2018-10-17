#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Nombre d'argument insuffisant.\n");
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}