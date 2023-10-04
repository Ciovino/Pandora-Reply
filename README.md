# Reply Challenge 2022

## Traccia della sfida
Pandora deve sconfiggere dei demoni e raccogliere più frammenti d'ombra possibili.
Per poter combattere, Pandora comincia con una certa quantità $S_{i}$ di *stamina*, che viene consumata in un combattimento e recuperata dopo un po' di tempo, fino ad una quantità massima $S_{max}$. Stamina in più viene persa.
Ci sono un totale di $T$ *turni*. In ogni turno puoi decidere se attaccare un demone dalla lista $D$ di *demoni*.
Ogni demone è definito da:
- $S_{c}$: stamina necessaria per iniziare lo scontro (e sconfiggiere il demone);
- $T_{r}$: numero di turni necessari per recuperare la stamina;
- $S_{r}$: quantità di stamina recuperata dopo $T_{r}$ turni;
- $N_{a}$: numero di turni in cui si possono guadagnare dei frammenti per aver sconfitto il demone;
- Lista di $N_{a}$ elementi, dove $\forall a_{i}, i\in [0,N_{a}):a_{i}\in \mathbb{N}$. Ogni $a_{i}$ rappresenta il numero di frammenti guadagnati all'$i-esimo$ turno dopo la battaglia.

Bisogna trovare l'ordine in cui combattere i demoni.
Pandora:
- Non può sconfiggere più di un demone nello stesso turno;
- Può solo combattere in demone se ha abbastanza stamina;
- Non può combattere contro lo stesso demone più di una volta;
- Dopo aver sconfitto un demone, recupera la stamina dopo un certo numero di turni;
- La stamina deve essere sempre inclusa nell'intervallo $[0,S_{max}]$;
- Per il calcolo dei frammenti raccolti, viene processata la lista di demoni che decidi di affrontare nell'ordine in cui è presentata. 

Se Pandora non ha abbastanza stamina per combattere, il sistema aspetta finché non recuperi abbastanza stamina o il numero totale di turni sono passati;
- Ad ogni turno, prima recuperi la stamina, poi affronti i demoni, poi collezioni i frammenti;
- Ad ogni turno, se possibile, recuperi i frammenti che hai ottenuto da ogni demone sconfitto;
- Ad ogni turno, se possibile, recuperi tutta la stamina che hai ottenuto dai demoni sconfitti;
- Puoi collezionare frammenti e combattere demoni per un massimo di $T$ turni.
Il punteggio totale è pari alla somma dei frammenti collezionati alla fine dei $T$ turni.

## Input
L'input è un file di testo ASCII. Ogni riga del file di input è separata da un singolo '\n' carattere. Se una riga contiene più dati, ogni valore è separato da un singolo spazio.
La prima riga è composta da:
- $S_{i}$, quantità di stamina con cui Pandora comincia;
- $S_{max}$, massima quantità di stamina accumulabile;
- $T$, numero di turni;
- $D$, numero di demoni.
Seguono $D$ righe, ognuna descrive un demone. Ogni riga contiene:
- $S_{c}$, quantità di stamina necessaria per sconfiggere il demone;
- $T_{r}$, il numero di turni da aspettare prima di poter recuperare la stamina;
- $S_{r}$, quantità di stamina che viene recuperata dopo $T_{r}$ turni;
- $N_{a}$, numero di turni in cui si possono guadagnare frammenti dopo aver sconfitto il demone;
- $N_{a}$ numeri interi $a_{i}, i\in [0,N_{a})$, numero di frammenti guadagnati dell'$i-esimo$ turno dopo la battaglia.
  
## Output
L'output deve essere un file di testo ASCII. Ogni riga del file di output è separata da un singolo '\n' carattere. Ogni riga deve contenere l'indice di un demone. I demoni vengono fronteggiati nell'ordine scritto.
Il file di output è valido se:
- Contiene sono numeri e caratteri '\n';
- $\forall i, i\in [0,D)$;
- Ogni indice è presente nel file una volta sola.
  
## Limiti
- $T \le 1000000$;
- $D \le 100000$;
- La stamina deve essere sempre $S \in [0,100000]$;
- $T_{r} \in [1,T]$;
- $N_{a} \in [0,100000]$;
- $a_{i} \in [0,10000]$.

## Spiegazione Algoritmi
### 04/10/2023 Prima versione
Viene effettuata una simulazione della partita. I demoni sono ordinati prima in ordine crescente rispetto alla quantità di stamina necessaria per iniziare l'attacco.

Ad ogni demone viene associato uno **stato** e un **punteggio**:
- Lo **stato** (salvato in ```statoDemone```) è un intero che indica lo stato del demone nella simulazione:
  - ```-3 DA_IGNORARE``` è un demone già sconfitto di cui è stata anche recuperata la stamina;
  - ```-2 DA_SCEGLIERE``` è un demone ancora da analizzare;
  - ```-1 PUNTEGGIO``` è un demone di cui è necessario aggiornare il suo punteggio;
  - ```0 AGGIORNA_STAMINA``` avvisa che sono passati i turni per poter recuperare la stamina dal demone;
  - qualsiasi numero *>0* indica quanti turni mancano al recupero della stamina.
- Il **punteggio**:
  - E' un valore reale compreso nell'intervallo *$(0, 1)$*; 
  - Viene calcolato a partire da due componenti:
    - *Recupero medio di stamina*: il recupero medio di un demone equivale al rapporto tra recupero totale (stamina recuperata - stamina persa) e il numero di turni necessari per il recupero;
    - *Frammenti persi*: il numero di frammenti che verrebbero persi nel caso in cui il demone non fosse sconfitto in quel turno.
  - Le due componenti sono poi ricalcolate usando la *funzione logistica* $F(x, Q, B)=\frac{1}{(1+Qe^{-Bx})^2}$:
    - Per il *recupero medio di stamina* $Q_{1}=2.35$, $B_{1}=1.0$, mentre per i *frammenti persi* il calcolo è $1 - F(x, Q_{2}, B_{2})$ con $Q_{2} = 6.3$ e $B_{2} = 0.5$.
  - Il **punteggio** finale è $Punteggio = F(RecuperoMedioStamina, Q_{1}, B_{1}) \cdot (1-F(FrammentiPersi, Q_{2}, B_{2}))$


Per ogni turno simulato:
- Viene aggiornata la stamina, se necessario (Se ci sono demoni con lo stato ```AGGIORNA_STAMINA```);
- Viene ispezionata la lista di demoni in ordine, salvando i demoni (Cambiando il loro stato in ```PUNTEGGIO```) che fanno in modo che la stamina totale di tutti i demoni scelti non superi la stamina corrente;
- Viene aggiornato il punteggio dei demoni;
- Viene scelto il demone con il punteggio più alto;
- Il demone viene salvato come *attaccato* (Lo stato del demone diventa il numero di turni per il recupero stamina);
- Viene decrementato lo stato di tutti i demoni con uno stato *>0*.


Se alla fine della simulazione ci sono demoni che non sono stati ancora selezionati, vengono inseriti in coda alla lista finale nell'ordine in cui si trovano.

### Risultati
- *00-example.txt* : 117 frammenti;
- *01-the-cloud-abyss.txt*: 82074 frammenti;
- *02-iot-island-of-terror.txt*: 5594738 frammenti;
- *03-etheryum.txt*: 0 frammenti;
- *04-the-desert-of-autonomous-machines.txt*: 5226692 frammenti;
- *05-androids-armageddon.txt*: 2438340 frammenti;

### Riflessioni
- Il sistema di punteggio non considera il numero effettivo di frammenti che un demone può dare, infatti demoni che danno 0 frammenti (20000+ in *03-etheryum.txt*) restituiscono sempre 0 frammenti persi, andando a favorirli rispetto a chi perde anche 1 frammento.

### Idee
- Migliorare il calcolo del punteggio, facendo pesare il numero di frammenti collezionabili da un demone;
- Migliorare l'ordinamento iniziale. 