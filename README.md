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

## Record
*00-example.txt* : 117 frammenti; 04/10/2023
*01-the-cloud-abyss.txt*: 82074 frammenti; 04/10/2023
*02-iot-island-of-terror.txt*: 5594738 frammenti; 04/10/2023
*03-etheryum.txt*: 0 frammenti; 04/10/2023
*04-the-desert-of-autonomous-machines.txt*: 5226692 frammenti; 04/10/2023
*05-androids-armageddon.txt*: 2438340 frammenti; 04/10/2023