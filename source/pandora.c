#include <stdio.h> // fprintf(), fopen(), fclose()

int main(int argc, char **argv)
{
    fprintf(stdout, "Pandora - Reply\n");

    // File di input passato da linea di comando
    if (argc != 2) {
        fprintf(stderr, "Errore: Input mancante da linea di comando\n");
        return -1;
    }

    FILE *inputFile = fopen(argv[1], "r");
    if (inputFile == NULL) {
        fprintf(stderr, "File '%s' non trovato\n", argv[1]);
        return -2;
    }

    fclose(inputFile);

    return 0;
}