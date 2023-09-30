#include <stdio.h>  // fprintf, fscanf, fopen, fclose
#include <stdlib.h> // malloc, free

#define MAX_TURNI               1000000
#define MAX_DEMONI              100000
#define MAX_STAMINA             100000
#define MAX_FRAMMENTI_TURNI     100000
#define MAX_FRAMMENTI_PER_TURNO 10000

typedef struct {
    int stamina;
    int maxStamina;
    int frammentiTotali;
} PLAYER;

typedef struct {
    int staminaPersa;
    int turniPerRecuperoStamina;
    int staminaRecuperata;
    int turniFrammenti;
    int *frammentiPerTurno;
} DEMON;

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

    // File trovato ed aperto
    PLAYER pandora = {0, 0, 0};
    DEMON *demoni;
    int turniTotali, totaleDemoni;

    // Pandora
    fscanf(
        inputFile, "%d %d %d %d%*c", &pandora.stamina, &pandora.maxStamina,
        &turniTotali, &totaleDemoni);

    // Demoni
    demoni = malloc(totaleDemoni * sizeof(*demoni));
    for (int i = 0; i < totaleDemoni; i++) {
        fscanf(
            inputFile, "%d %d %d %d%*c", &demoni[i].staminaPersa,
            &demoni[i].turniPerRecuperoStamina, &demoni[i].staminaRecuperata,
            &demoni[i].turniFrammenti);

        demoni[i].frammentiPerTurno =
            malloc(demoni[i].turniFrammenti * sizeof(int));

        for (int j = 0; j < demoni[i].turniFrammenti; j++)
            fscanf(inputFile, "%d%*c", &demoni[i].frammentiPerTurno[j]);
    }

    fclose(inputFile);

    // Test Output
    fprintf(stdout, "Numero turni: %d\n", turniTotali);
    fprintf(
        stdout,
        "Pandora:\n\tStamina: %d\n\tStamina massima: %d\n\tFrammenti totali: "
        "%d\n",
        pandora.stamina, pandora.maxStamina, pandora.frammentiTotali);
    fprintf(stdout, "Numero totale demoni: %d\n", totaleDemoni);

    for (int i = 0; i < totaleDemoni; i++) {
        fprintf(
            stdout,
            "Demone %d:\n\tStamina persa:%d\n\tTurni per recuperare la "
            "stamina:%d\n\tStamina recuperata:%d\n\tTurni per fraggmenti:%d\n",
            i + 1, demoni[i].staminaPersa, demoni[i].turniPerRecuperoStamina,
            demoni[i].staminaRecuperata, demoni[i].turniFrammenti);

        free(demoni[i].frammentiPerTurno);
    }

    free(demoni);

    return 0;
}