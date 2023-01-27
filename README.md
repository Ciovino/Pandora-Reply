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
- Per il calcolo dei frammenti raccolti, viene processata la lista di demoni che decidi di affrontare nell'ordine in cui è presentata. Se Pandora non ha abbastanza stamina per combattere, il sistema aspetta finché non recuperi abbastanza stamina o il numero totale di turni sono passati;
- Ad ogni turno, prima recuperi la stamina, poi affronti i demoni, poi collezioni i frammenti;
- Ad ogni turno, se possibile, recuperi i frammenti che hai ottenuto da ogni demone sconfitto;
- Ad ogni turno, se possibile, recuperi tutta la stamina che hai ottenuto dai demoni sconfitti;
- Puoi collezionare frammenti e combattere demoni per un massimo di $T$ turni.
Il punteggio totale è pari alla somma dei frammenti collezionati alla fine dei $T$ turni.