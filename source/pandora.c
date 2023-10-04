#include <math.h>
#include <stdio.h>  // fprintf, fscanf, fopen, fclose
#include <stdlib.h> // malloc, calloc, free

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
    int frammentiTotali;
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
        demoni[i].frammentiTotali = 0;
        demoni[i].frammentiPerTurno =
            malloc(demoni[i].turniFrammenti * sizeof(int));

        for (int j = 0; j < demoni[i].turniFrammenti; j++) {
            fscanf(inputFile, "%d%*c", &demoni[i].frammentiPerTurno[j]);
            demoni[i].frammentiTotali += demoni[i].frammentiPerTurno[j];
        }
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

DEMON *OrdinaDemoni(const DEMON *Demoni, int DimDemoni)
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

// 1/(1+Q*e^-Bx)^2
double LogisticCurve(double input, double Q, double B)
{
    return 1.0 / (pow(1 + Q * exp(-B * input), 2));
}

int FrammentiPerNTurni(DEMON Demone, int NTurni)
{
    if (NTurni >= Demone.turniFrammenti) return Demone.frammentiTotali;

    int frammenti = 0;
    for (int i = 0; i < NTurni; i++)
        frammenti += Demone.frammentiPerTurno[i];

    return frammenti;
}

double CalcolaPunteggio(DEMON Demone, int TurniUtili)
{
    double recuperoMedio =
        (double)(Demone.staminaRecuperata - Demone.staminaPersa) /
        Demone.turniPerRecuperoStamina;

    int frammentiPersi = FrammentiPerNTurni(Demone, TurniUtili) -
                         FrammentiPerNTurni(Demone, TurniUtili - 1);

    return LogisticCurve(recuperoMedio, 2.35, 1.0) *
           (1.0 - LogisticCurve(frammentiPersi, 6.3, 0.5));
}

int *CalcolaOrdine(
    const PLAYER Pandora, const DEMON *Demoni, const int DimDemoni,
    const int TurniTotali)
{
    enum STATISTICHE_DEMONE {
        DA_IGNORARE = -3,
        DA_SCEGLIERE = -2,
        PUNTEGGIO = -1,
        AGGIORNA_STAMINA = 0
    };

    struct INFO_STAMINA {
        int corrente, massima, accumulata;
    };

    // Setup: Ordina i demoni per stamina consumata
    DEMON *CopiaDemoni = OrdinaDemoni(Demoni, DimDemoni);

    struct INFO_STAMINA stamina = {Pandora.stamina, Pandora.maxStamina, 0};
    int *ordineFinale = malloc(DimDemoni * sizeof(int)), idx = 0,
        *statisticheDemone = malloc(DimDemoni * sizeof(int)),
        prossimoDemone = 0, ultimoDemone = 0;
    double *punteggi = malloc(DimDemoni * sizeof(double));

    for (int i = 0; i < DimDemoni; i++)
        statisticheDemone[i] = DA_SCEGLIERE;

    for (int turno = 1; turno < TurniTotali; turno++) {
        // Aggiorna stamina
        for (int i = 0; i < DimDemoni; i++) {
            if (statisticheDemone[i] == AGGIORNA_STAMINA) {
                statisticheDemone[i] = DA_IGNORARE;
                stamina.corrente += CopiaDemoni[i].staminaRecuperata;
                if (stamina.corrente > stamina.massima)
                    stamina.corrente = stamina.massima;
            }
        }

        // Aggiungi demoni alla lista 'punteggi'
        int listaAggiornata = 0;
        while (!listaAggiornata) {
            if (CopiaDemoni[ultimoDemone].staminaPersa + stamina.accumulata >
                stamina.corrente) {
                listaAggiornata = 1;
            } else {
                stamina.accumulata += CopiaDemoni[ultimoDemone].staminaPersa;
                statisticheDemone[ultimoDemone] = -1;
                ultimoDemone++;
            }
        }

        // Aggiorna i punteggi
        for (int i = prossimoDemone; i < ultimoDemone; i++) {
            if (statisticheDemone[i] == PUNTEGGIO) {
                punteggi[i] =
                    CalcolaPunteggio(CopiaDemoni[i], TurniTotali - turno + 1);
            }
        }

        // Ammazza il demone col punteggio più alto
        int max = -1;
        double punteggioMax = 0.0;
        for (int i = prossimoDemone; i < ultimoDemone; i++) {
            if (statisticheDemone[i] == PUNTEGGIO &&
                punteggi[i] > punteggioMax) {
                max = i;
                punteggioMax = punteggi[i];
            }
        }

        // Nessun demone scelto
        if (max != -1) {
            ordineFinale[idx++] = CopiaDemoni[max].idx;
            statisticheDemone[max] = CopiaDemoni[max].turniPerRecuperoStamina;
            stamina.accumulata -= CopiaDemoni[max].staminaPersa;
            stamina.corrente -= CopiaDemoni[max].staminaPersa;
        }

        // Decrementa le statistiche
        for (int i = 0; i < DimDemoni; i++)
            if (statisticheDemone[i] > 0) statisticheDemone[i]--;

        // Sposta Prossimo Demone
        while (statisticheDemone[prossimoDemone] == DA_IGNORARE)
            prossimoDemone++;
    }

    if (idx != DimDemoni) {
        for (int i = 0; i < DimDemoni; i++) {
            if (statisticheDemone[i] == DA_SCEGLIERE ||
                statisticheDemone[i] == PUNTEGGIO) {
                ordineFinale[idx++] = CopiaDemoni[i].idx;
            }
        }
    }

    free(statisticheDemone);
    free(punteggi);
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

    fprintf(log, "Ordine di attacco: { ");
    for (int i = 0; i < DimDemoni; i++)
        fprintf(log, "%d ", OrdineDemoni[i]);
    fprintf(log, "}\n");

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

    fprintf(log, "\nTotale: %d frammenti raccolti\n", Pandora.frammentiTotali);

    if (log != stdout) fclose(log);
    return Pandora.frammentiTotali;
}