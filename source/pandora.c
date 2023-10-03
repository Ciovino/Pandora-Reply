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
    int idx;
    int staminaPersa;
    int turniPerRecuperoStamina;
    int staminaRecuperata;
    int sconfitto; // = -1;
    int turniFrammenti;
    int *frammentiPerTurno;
} DEMON;

int *CalcolaOrdine(
    const PLAYER Pandora, const DEMON *Demoni, const int DimDemoni,
    const int TurniTotali);
int CalcolaFrammenti(
    PLAYER Pandora, DEMON *Demoni, int DimDemoni, int TurniTotali,
    int *OrdineDemoni);

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

        demoni[i].idx = i;
        demoni[i].sconfitto = -1;
        demoni[i].frammentiPerTurno =
            malloc(demoni[i].turniFrammenti * sizeof(int));

        for (int j = 0; j < demoni[i].turniFrammenti; j++)
            fscanf(inputFile, "%d%*c", &demoni[i].frammentiPerTurno[j]);
    }

    fclose(inputFile);

    // Output
    int *ordineFinale =
        CalcolaOrdine(pandora, demoni, totaleDemoni, turniTotali);

    FILE *outputFile = fopen("output.txt", "w");

    if (outputFile == NULL) {
        fprintf(stderr, "Impossibile creare il file '%s'\n", "output.txt");
        return -1;
    }

    for (int i = 0; i < totaleDemoni; i++)
        fprintf(outputFile, "%d\n", ordineFinale[i]);

    fclose(outputFile);

    // Punteggio
    fprintf(
        stdout, "Punteggio totale: %d frammenti recuperati\n",
        CalcolaFrammenti(
            pandora, demoni, totaleDemoni, turniTotali, ordineFinale));

    for (int i = 0; i < totaleDemoni; i++)
        free(demoni[i].frammentiPerTurno);
    free(demoni);
    free(ordineFinale);

    return 0;
}

int MinoreDi_Demon(DEMON A, DEMON B) { return A.staminaPersa < B.staminaPersa; }

DEMON *OrdinaDemoni(DEMON *Demoni, int DimDemoni)
{
    int *daIgnorare = calloc(DimDemoni, sizeof(int));
    DEMON *copia = malloc(DimDemoni * sizeof(*copia));

    for (int count = 0; count < DimDemoni; count++) {
        int min = -1;

        for (int i = 0; i < DimDemoni; i++) {
            if (daIgnorare[i]) continue;

            if (min == -1 || MinoreDi_Demon(Demoni[i], Demoni[min])) min = i;
        }

        daIgnorare[min] = 1;
        copia[count] = Demoni[min];
    }

    free(daIgnorare);
    return copia;
}

int *CalcolaOrdine(
    const PLAYER Pandora, const DEMON *Demoni, const int DimDemoni,
    const int TurniTotali)
{
    // Setup: Ordina i demoni per stamina consumata
    DEMON *CopiaDemoni = OrdinaDemoni(Demoni, DimDemoni);

    int *ordineFinale = malloc(DimDemoni * sizeof(int));

    for (int i = 0; i < DimDemoni; i++)
        ordineFinale[i] = CopiaDemoni[i].idx;

    free(CopiaDemoni);
    return ordineFinale;
}

// Turno ==> Recupero Stamina -> Attacco Demoni -> Prendi Frammenti
int CalcolaFrammenti(
    PLAYER Pandora, DEMON *Demoni, int DimDemoni, int TurniTotali,
    int *OrdineDemoni)
{
    FILE *log = fopen("log.txt", "w");
    if (log == NULL) {
        fprintf(stderr, "Impossibile aprire il file '%s'\n", "log.txt");
        log = stdout;
    }

    int demoneDaSconfiggiere = 0;
    int *demoniSconfitti =
        calloc(DimDemoni, sizeof(int)); // 0 non sconfitto, 1 sconfitto

    for (int turno = 0; turno < TurniTotali; turno++) {
        fprintf(
            log, "\nTurno %d. Pandora: Stamina %d, Frammenti %d\n", turno + 1,
            Pandora.stamina, Pandora.frammentiTotali);

        // Recupero Stamina
        for (int i = 0; i < DimDemoni; i++)
            if (demoniSconfitti[i]) {
                int turniPassati = turno - Demoni[i].sconfitto;

                if (turniPassati == Demoni[i].turniPerRecuperoStamina) {
                    fprintf(
                        log, "\tRecupero stamina %d\n",
                        Demoni[i].staminaRecuperata);
                    Pandora.stamina += Demoni[i].staminaRecuperata;

                    if (Pandora.stamina >= Pandora.maxStamina)
                        Pandora.stamina = Pandora.maxStamina;
                }
            }

        // Attacco Demoni
        if (demoneDaSconfiggiere < DimDemoni) {
            int prossimoAttacco = OrdineDemoni[demoneDaSconfiggiere];

            if (Pandora.stamina >= Demoni[prossimoAttacco].staminaPersa) {
                // Attacco riuscito
                fprintf(
                    log, "\tAttacco su demone %d riuscito\n", prossimoAttacco);

                Pandora.stamina -= Demoni[prossimoAttacco].staminaPersa;
                Demoni[prossimoAttacco].sconfitto = turno;
                demoniSconfitti[prossimoAttacco] = 1;
                demoneDaSconfiggiere++;

                fprintf(
                    log, "\tStamina persa: %d\n",
                    Demoni[prossimoAttacco].staminaPersa);
            }
        }

        // Prendi Frammenti
        for (int i = 0; i < DimDemoni; i++) {
            if (demoniSconfitti[i]) {
                int turniPassati = turno - Demoni[i].sconfitto;

                if (turniPassati < Demoni[i].turniFrammenti) {
                    fprintf(
                        log, "\tPrendi %d frammenti dal demone %d\n",
                        Demoni[i].frammentiPerTurno[turniPassati], i);

                    Pandora.frammentiTotali +=
                        Demoni[i].frammentiPerTurno[turniPassati];
                }
            }
        }
    }

    return Pandora.frammentiTotali;
}