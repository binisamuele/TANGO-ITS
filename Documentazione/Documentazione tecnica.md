DOCUMENTAZIONE TECNICA PROGETTO “GESTIONE SCUOLA” 
TEAM: MISSION IMPOSSIBLE
1. Introduzione
1.1 Scopo del Documento
Questo è un documento di specifica dei requisiti per un nuovo programma di gestione di una scuola. Il nuovo programma dà la possibilità di visualizzare, aggiungere, modificare e cancellare sia singole classi che studenti, insegnanti, valutazioni degli studenti e materie, con la possibilità di calcolare la media delle valutazioni. Il  documento in oggetto, descrive lo scopo, gli obiettivi e le finalità del nuovo programma. Oltre a descrivere i requisiti non funzionali, il medesimo modella i requisiti funzionali con casi d'uso, diagrammi di interazione e modelli di classi. Infine, lo stesso  è destinato a guidare la progettazione e l'implementazione del programma target in un linguaggio orientato agli oggetti.

1.2 Background
La richiesta di sviluppare un nuovo programma per la gestione di una scuola è stata motivata dalla necessità di affrontare diverse problematiche che non erano state precedentemente gestite in modo adeguato. Prima dell'implementazione di questo nuovo software, non esisteva un sistema dedicato specificamente alla gestione scolastica, il che ha portato a inefficienze e difficoltà nella gestione di varie attività chiave. I principali problemi riscontrati erano:

•	Gestione degli studenti e degli insegnanti: Non esisteva un sistema centralizzato per registrare e mantenere le informazioni degli studenti e degli insegnanti. La gestione dei dettagli personali, delle iscrizioni alle classi e delle informazioni di contatto avveniva manualmente o attraverso strumenti non integrati, rendendo difficile l'accesso rapido e l'aggiornamento accurato delle informazioni.

•	Gestione delle valutazioni degli studenti: La registrazione, l'archiviazione e il calcolo delle valutazioni degli studenti erano un processo complesso e suscettibile di errori. Non esisteva un sistema centralizzato per registrare le valutazioni per ogni studente, materia e periodo scolastico, rendendo difficile monitorare il progresso degli studenti e generare report accurati.

•	Gestione delle valutazioni: La raccolta, l'organizzazione e l'analisi delle valutazioni degli studenti richiedevano un notevole sforzo manuale. Non era disponibile un sistema automatizzato per calcolare la media delle valutazioni e generare statistiche utili per gli insegnanti e l'amministrazione scolastica.

Il nuovo programma di gestione scolastica è stato sviluppato per affrontare queste problematiche e fornire una soluzione integrata, efficiente e affidabile per la gestione di studenti, insegnanti e valutazioni.

1.3 Stakeholder
1.3.1 Utenti
I principali stakeholder per questo progetto includono:
1.	Responsabile scolastico: Il responsabile scolastico è responsabile della supervisione generale del sistema di gestione della scuola. Ha interesse nel garantire che il programma soddisfi le esigenze e i requisiti della scuola.

2.	Insegnanti: Gli insegnanti utilizzano il programma per registrare le valutazioni degli studenti e gestire le informazioni delle classi e delle materie. Hanno interesse che il  sistema sia facile da usare, efficiente e in grado di gestire correttamente i dati degli studenti.

3.	Studenti: Gli studenti sono i principali soggetti delle valutazioni e delle informazioni registrate nel sistema. Hanno interesse che il sistema  sia accurato, sicuro e che consenta loro di visualizzare le proprie valutazioni e altre informazioni pertinenti.
1.4 Panoramica del Documento
Il resto del documento fornisce le specifiche dettagliate del nuovo sistema. È organizzato come segue:

Sezione 2: Requisiti funzionali

Sezione 3: Requisiti non funzionali

Sezione 4: Diagrammi UML

Sezione 5: Analisi del codice
Sezione 6: Testing del codice
Sezione 7: Appendice

2. Requisiti Funzionali
Sono stati identificati i seguenti requisiti funzionali per il software.
1.	Visualizzazione delle classi:
•	Il software deve consentire la visualizzazione delle classi presenti nella scuola, mostrando l'ID e il nome di ciascuna classe.

2.	Gestione delle classi:
•	Il software deve consentire l'aggiunta di nuove classi, richiedendo l'inserimento di un ID univoco e del nome della classe.
•	Il software deve consentire la modifica delle informazioni di una classe esistente, come il nome della classe.
•	Il software deve consentire la cancellazione di una classe esistente, considerando anche l'eventuale riassegnazione degli studenti ad altre classi.
3.	Visualizzazione degli studenti:
•	Il software deve consentire la visualizzazione degli studenti presenti nella scuola, mostrando l'ID, il nome, il cognome, la data di nascita, l'anno di corso e la classe di appartenenza di ciascuno studente.

4.	Gestione degli studenti:
•	Il software deve consentire l'aggiunta di nuovi studenti, richiedendo l'inserimento di un ID univoco, nome, cognome, data di nascita, anno di corso e la classe di appartenenza.
•	Il software deve consentire la modifica delle informazioni di uno studente esistente, come nome, cognome, data di nascita, anno di corso e classe di appartenenza.
•	Il software deve consentire la cancellazione di uno studente esistente, considerando anche l'eventuale aggiornamento delle valutazioni e delle medie.

5.	Visualizzazione degli insegnanti:
•	Il software deve consentire la visualizzazione degli insegnanti presenti nella scuola, mostrando l'ID, il nome, il cognome, la classe assegnata e la materia insegnata.

6.	Gestione degli insegnanti:
•	Il software deve consentire l'aggiunta di nuovi insegnanti, richiedendo l'inserimento di un ID univoco, nome, cognome, classe assegnata e materia insegnata.
•	Il software deve consentire la modifica delle informazioni di un insegnante esistente, come nome, cognome, classe assegnata e materia insegnata.
•	Il software deve consentire la cancellazione di un insegnante esistente, considerando anche l'eventuale riassegnazione degli insegnanti ad altre classi o materie.

7.	Gestione delle valutazioni:
•	Il software deve consentire l'aggiunta di nuove valutazioni per gli studenti, specificando l'ID dello studente, l'ID della materia e il voto.
•	Il software deve consentire la modifica di una valutazione esistente, consentendo la modifica del voto.
•	Il software deve consentire la cancellazione di una valutazione esistente.

8.	Calcolo delle medie delle valutazioni:
•	Il software deve calcolare la media dei voti di ciascuno studente per ciascuna materia e visualizzarla.
•	Il software deve calcolare la media dei voti di ciascuna materia e visualizzarla.
•	Il software deve aggiornare automaticamente la media delle valutazioni di uno studente o di una materia quando vengono aggiunte, modificate o cancellate le valutazioni corrispondenti.
3. Requisiti Non Funzionali
I requisiti non funzionali identificati per il programma di gestione della scuola includono:
1.	Usabilità: Il sistema deve essere intuitivo e facile da usare per gli utenti finali, tra cui insegnanti, studenti e amministrazione scolastica.

2.	Affidabilità: Il sistema deve essere affidabile e garantire l'integrità dei dati. Deve essere in grado di gestire carichi di lavoro elevati senza compromettere le prestazioni.

3.	Prestazioni: Il sistema deve essere efficiente e reattivo, in grado di gestire un numero elevato di operazioni contemporaneamente senza rallentamenti significativi.

4.	Portabilità: Il sistema deve essere compatibile con diverse piattaforme e ambienti operativi, consentendo l'accesso da diversi dispositivi come computer, tablet o smartphone.

5.	Manutenibilità: Il sistema deve essere progettato in modo modulare e ben strutturato, facilitando la manutenzione, l'estensibilità e l'aggiornamento del software.

6.	Documentazione: Il sistema deve essere corredato da una documentazione completa, chiara e dettagliata che descriva le funzionalità, l'utilizzo e la configurazione del sistema. Fare riferimento al manuale tecnico in appendice
SEZIONE 4: Diagrammi
4.1 DIAGRAMMA CASO D’USO

 

4.2 DIAGRAMMA DI CLASSE
 
4.3 DIAGRAMMA DI SEQUENZA
 

SEZIONE 5: Analisi del codice
5.1 Scrittura del codice sorgente
Quest’applicazione è stata sviluppata utilizzando il linguaggio di programmazione C#. 
Il codice sorgente è organizzato in diverse classi all'interno del namespace "Gestione". La classe principale è denominata "Program" e contiene il metodo "Main" che rappresenta il punto di ingresso dell'applicazione.
Il codice è strutturato in maniera modulare, suddividendo le funzionalità di gestione in metodi separati. Ogni metodo gestisce un aspetto specifico, come la gestione delle classi, degli studenti, degli insegnanti, delle valutazioni e delle materie. Questa suddivisione consente una migliore leggibilità e manutenibilità del codice.
L'applicazione interagisce con l'utente attraverso un menu a selezione multipla, consentendo di scegliere le diverse opzioni di gestione. Ogni opzione del menu corrisponde a un metodo specifico che implementa la funzionalità desiderata. Inoltre, l'applicazione include anche la possibilità di caricare i dati da un file JSON e di salvarli in modo persistente.
Il codice fa ampio uso di strutture dati come liste per memorizzare le diverse entità, quali classi, studenti, insegnanti, valutazioni e materie. Queste liste vengono aggiornate e manipolate in base alle azioni dell'utente.
Inoltre, il codice include anche controlli di input per garantire che l'utente fornisca valori validi durante l'inserimento dei dati, ad esempio controllando che gli ID siano numeri interi e che i voti siano compresi entro un limite specifico - non è possibile inserire valutazioni superiori al 100 -. 
Infine, l'applicazione include anche la funzionalità di calcolo delle medie dei voti sia per gli studenti che per le valutazioni associate a una determinata materia.
5.2 JSON
Durante lo sviluppo del nostro software, il team ha integrato diverse librerie e plugin per ottenere funzionalità avanzate e un'efficiente gestione dei dati. Tra queste, uno degli elementi fondamentali è stato Newtonsoft.Json, comunemente noto come JSON.NET.

Newtonsoft.Json è una libreria ampiamente adottata nell'ecosistema .NET per la manipolazione e la gestione dei dati JSON. Grazie a questa libreria, siamo riusciti a semplificare notevolmente il processo di conversione degli oggetti .NET da e verso il formato JSON.

Una delle principali sfide affrontate durante il progetto era garantire la persistenza dei dati in un formato leggibile e interoperabile. Sfruttando Newtonsoft.Json, siamo riusciti a convertire agevolmente gli oggetti complessi del nostro software in una rappresentazione JSON e viceversa. Questo ci ha consentito di salvare i dati su supporti di archiviazione o scambiarli con altre applicazioni in modo semplice ed efficace.

In conclusione, l'inclusione del plugin Newtonsoft.Json nel nostro software ha svolto un ruolo cruciale nella gestione dei dati JSON. Grazie alle sue potenti funzionalità e alla sua facilità d'uso, siamo riusciti a garantire la persistenza dei dati in modo affidabile e a semplificare la comunicazione con altre applicazioni. L'utilizzo di questa libreria si è dimostrato una scelta strategica che ha contribuito al successo generale del nostro progetto
Sezione 6: Testing del codice

6.1 Fase di Verifica:
Durante la fase di verifica, sono stati considerati i requisiti utente e i requisiti di sistema per garantire un'analisi accurata per la progettazione dell'applicazione.

Requisiti Utente/Analisi dei Requisiti

Nella fase di analisi dei requisiti utente, sono stati identificati i seguenti requisiti per l'applicazione di gestione della scuola:

Gestione degli studenti:
Inserimento di nuovi studenti nel sistema con dati personali, come nome, cognome, data di nascita, ecc.
Modifica delle informazioni degli studenti esistenti.
Visualizzazione dei dettagli degli studenti.

Gestione delle classi:
Creazione di nuove classi con un nome e un insegnante responsabile.
Aggiunta e rimozione degli studenti da una classe.
Visualizzazione delle classi e dei relativi studenti.

Gestione dei voti delle materie:
Assegnazione dei voti agli studenti per ciascuna materia.
Modifica dei voti assegnati in precedenza.
Visualizzazione dei voti degli studenti.

6.2 Requisiti di Sistema:
I requisiti di sistema per l'applicazione di gestione della scuola sono stati identificati come segue:

Archiviazione dei dati:

I dati degli studenti, delle classi e dei voti devono essere salvati in modo persistente per poter essere utilizzati anche dopo un riavvio dell'applicazione.
È stata preferita l'implementazione di un database per garantire la conservazione e l'accesso efficiente ai dati.

6.3 Fase di Validazione:
Durante la fase di validazione, sono stati eseguiti test per garantire che l'applicazione soddisfi i requisiti e funzioni correttamente.

Esecuzione del Test dei Componenti/Test delle Unità

Per la verifica dei componenti dell'applicazione, sono stati sviluppati test delle unità specifici per ogni componente. I test delle unità mirano a verificare il corretto funzionamento di singoli moduli o parti dell'applicazione. Durante l'esecuzione dei test delle unità, sono stati verificati i seguenti aspetti:

•	Verifica dell'inserimento, modifica e visualizzazione degli studenti.
•	Verifica della creazione, aggiunta e visualizzazione delle classi.
•	Verifica dell'assegnazione, modifica e visualizzazione dei voti delle materie.

Esecuzione del Test di Accettazione/Accettazione dell'Utente
Durante il test di accettazione si è valutato se si soddisfano le aspettative e i requisiti definiti. Durante questo test, sono stati verificati i seguenti aspetti:
Verifica che l'applicazione consenta l'inserimento, la modifica e la visualizzazione corretta degli studenti, delle classi e dei voti.
Verifica che l'applicazione offra un'interfaccia utente intuitiva e di facile utilizzo.
Verifica che l'applicazione risponda in modo adeguato alle esigenze dell'utente e soddisfi i requisiti specifici della scuola.
6.4 Esecuzione dei test
I seguenti test sono stati eseguiti sulla classe "TestClasse" nel namespace "TestGestione". La classe contiene metodi per la gestione delle classi, inclusi i metodi per visualizzare, aggiungere, modificare e cancellare classi.

Test del metodo "VisualizzaClasse_DisplayClass":
1)	Scopo: Verificare se il metodo "VisualizzaClassi" visualizza correttamente le informazioni delle classi.
2)	Descrizione:
Si crea una lista di oggetti "Classe" con tre elementi di test. Si richiama il metodo "VisualizzaClassi" passando la lista di classi di test. Si verifica che l'output contenga le informazioni corrette per ogni classe nella lista di test.
3)	Esito atteso: Il test è considerato superato se l'output contiene le informazioni corrette per tutte le classi nella lista di test.

 

Test del metodo "AggiungiClasse_AddClass":
1)	Scopo: Verificare se il metodo "AggiungiClasse" aggiunge correttamente una nuova classe alla lista.
2)	Descrizione: Si crea una lista vuota di oggetti "Classe". Si simula l'input dell'utente fornendo un ID e un nome di classe di test. Si richiama il metodo "AggiungiClasse" passando la lista di classi vuota. Si verifica che la lista di classi contenga esattamente un elemento, corrispondente all'ID e al nome forniti.
3)	Esito atteso: Il test è considerato superato se la lista di classi contiene esattamente un elemento con l'ID e il nome corretti.

 

Test del metodo "ModificaClasse_ChangeClass":
1)	Scopo: Verificare se il metodo "ModificaClasse" modifica correttamente il nome di una classe esistente.
2)	Descrizione: Si crea una lista di oggetti "Classe" con un elemento di test. Si simula l'input dell'utente fornendo l'ID della classe di test e un nuovo nome di classe. Si richiama il metodo "ModificaClasse" passando la lista di classi di test. Si verifica che il nome della classe nella lista sia stato modificato correttamente.
3)	Esito atteso: Il test è considerato superato se il nome della classe nella lista è stato modificato correttamente.

 
Test del metodo "CancellaClasse_DeleteClass":
1)	Scopo: Verificare se il metodo "CancellaClasse" cancella correttamente una classe esistente dalla lista.
2)	Descrizione: Si crea una lista di oggetti "Classe" con un elemento di test. Si simula l'input dell'utente fornendo l'ID della classe di test. Si richiama il metodo "CancellaClasse" passando la lista di classi di test. Si verifica che la classe sia stata correttamente cancellata dalla lista.
3)	Esito atteso: Il test è considerato superato se la classe è stata correttamente cancellata dalla lista.
 
----------
I seguenti test sono stati eseguiti sulla classe "TestStudente" nel namespace "TestGestione". La classe contiene metodi per la gestione degli studenti, inclusi i metodi per visualizzare, aggiungere, modificare e cancellare studenti.
Test del metodo "VisualizzaStudenti_DisplayStudents":
1)	Scopo: Verificare se il metodo "VisualizzaStudenti" visualizza correttamente le informazioni degli studenti.
2)	Descrizione: Si crea una lista di oggetti "Studente" con un elemento di test. Si richiama il metodo "VisualizzaStudenti" passando la lista di studenti di test. Si verifica che l'output contenga le informazioni corrette per lo studente nella lista di test.
3)	Esito atteso: Il test è considerato superato se l'output contiene le informazioni corrette per lo studente nella lista di test.

 

Test del metodo "AggiungiStudente_AddStudent":
1)	Scopo: Verificare se il metodo "AggiungiStudente" aggiunge correttamente un nuovo studente alla lista.
2)	Descrizione: Si crea una lista vuota di oggetti "Studente". Si simula l'input dell'utente fornendo un ID, nome, cognome, data di nascita, anno di corso e classe di test. Si richiama il metodo "AggiungiStudente" passando la lista di studenti vuota. Si verifica che la lista di studenti contenga esattamente un elemento, corrispondente ai dati forniti.
3)	Esito atteso: Il test è considerato superato se la lista di studenti contiene esattamente un elemento con i dati corretti.

 

Test del metodo "ModificaStudente_ChangeStudent":
1)	Scopo: Verificare se il metodo "ModificaStudente" modifica correttamente i dati di uno studente esistente.
2)	Descrizione: Si crea una lista di oggetti "Studente" con un elemento di test. Si simula l'input dell'utente fornendo l'ID dello studente di test e nuovi dati per nome, cognome, data di nascita, anno di corso e classe. Si richiama il metodo "ModificaStudente" passando la lista di studenti di test. Si verifica che i dati dello studente nella lista siano stati modificati correttamente.
3)	Esito atteso: Il test è considerato superato se i dati dello studente nella lista sono stati modificati correttamente.

 
Test del metodo "CancellaStudente_RemoveStudent":
1)	Scopo: Verificare se il metodo "CancellaStudente" cancella correttamente uno studente esistente dalla lista.
2)	Descrizione: Si crea una lista di oggetti "Studente" con un elemento di test. Si simula l'input dell'utente fornendo l'ID dello studente di test. Si richiama il metodo "CancellaStudente" passando la lista di studenti di test. Si verifica che lo studente sia stato correttamente cancellato dalla lista.
3)	Esito atteso: Il test è considerato superato se lo studente è stato correttamente cancellato dalla lista.
 
-----------------
I seguenti test sono stati eseguiti sulla classe "TestValutazione" nel namespace "TestGestione". La classe contiene metodi per la gestione delle valutazioni degli studenti, inclusi i metodi per visualizzare, aggiungere, modificare e cancellare le valutazioni.

Test del metodo "VisualizzaVoto_DisplayGrades":
1)	Scopo: Verificare se il metodo "VisualizzaVoto" visualizza correttamente le informazioni delle valutazioni.
2)	Descrizione: Si crea una lista di oggetti "Valutazione" con alcuni elementi di test. Si simula l'output della console. Si richiama il metodo "VisualizzaVoto" passando la lista di valutazioni di test. Si verifica che l'output contenga le informazioni corrette per le valutazioni nella lista di test.
3)	Esito atteso: Il test è considerato superato se l'output contiene le informazioni corrette per le valutazioni nella lista di test.

 

Test del metodo "AggiungiValutazione_AddGrade":
1)	Scopo: Verificare se il metodo "AggiungiVoto" aggiunge correttamente una nuova valutazione alla lista.
2)	Descrizione: Si crea una lista vuota di oggetti "Valutazione". Si simula l'input dell'utente fornendo l'ID dello studente, l'ID della materia e il voto di test. Si simula l'input/output della console. Si richiama il metodo "AggiungiVoto" passando la lista di valutazioni vuota. Si verifica che la lista di valutazioni contenga esattamente un elemento, corrispondente ai dati forniti.
3)	Esito atteso: Il test è considerato superato se la lista di valutazioni contiene esattamente un elemento con i dati corretti.
 

Test del metodo "ModificaVoto_ChangeGrade":
1)	Scopo: Verificare se il metodo "ModificaVoto" modifica correttamente il voto di una valutazione esistente.
2)	Descrizione: Si crea una lista di oggetti "Valutazione" con un elemento di test. Si simula l'input dell'utente fornendo l'ID della valutazione di test e il nuovo voto. Si simula l'input/output della console. Si richiama il metodo "ModificaVoto" passando la lista di valutazioni di test. Si verifica che il voto della valutazione nella lista sia stato modificato correttamente.
3)	Esito atteso: Il test è considerato superato se il voto della valutazione nella lista è stato modificato correttamente.

 

Test del metodo "CancellaValutazione_RemoveGrade":
1)	Scopo: Verificare se il metodo "CancellaVoto" cancella correttamente una valutazione esistente dalla lista.
2)	Descrizione: Si crea una lista di oggetti "Valutazione" con un elemento di test. Si simula l'input dell'utente fornendo l'ID della valutazione di test. Si simula l'input/output della console. Si richiama il metodo "CancellaVoto" passando la lista di valutazioni di test. Si verifica che la lista di valutazioni sia vuota dopo la cancellazione.
3)	Esito atteso: Il test è considerato superato se la lista di valutazioni è vuota dopo la cancellazione.
 
Note:
-	I test sono implementati utilizzando il framework di testing "Microsoft.VisualStudio.TestTools.UnitTesting".
-	I test utilizzano la simulazione dell'input e dell'output della console tramite l'utilizzo di "StringReader" e "Console.SetIn" per i test interattivi.
-	I test controllano le asserzioni utilizzando il metodo "Assert" del framework di testing per confrontare i risultati attesi con i risultati ottenuti.
-	Ogni test è indipendente dagli altri e non dipende dall'ordine di esecuzione.

In conclusione, il processo di testing e documentazione per il progetto di gestione di una scuola ha garantito la verifica accurata delle funzionalità dell'applicazione e la conformità ai requisiti utente e di sistema. Seguendo i principi fondamentali e eseguendo test approfonditi, è stato possibile sviluppare un'applicazione affidabile e di successo per la gestione delle attività scolastiche.

SEZIONE 7:
7.1 MANUALE DI UTILIZZO:
L’interfaccia per la gestione della scuola è molto semplice ed intuitiva. Una volta avviato il programma si verrà riportati ad un menù con le seguenti voci:
 
Basta digitare il numero associato all’azione che si vuole eseguire ed il lavoro è presto che fatto.
 
Per ogni gestione sono disponibili le opzioni di: visualizzazione, aggiunta, modifica e rimozione. 
Nel caso soprastante è stata richiesta la visualizzazione delle classi. Ogni classe è associata ad un ID per poterla recuperare facilmente nel database ed, eventualmente, modificarla.

 

Per gli studenti vengono specificati: nome, cognome, data di nascita, anno del corso che sta conseguendo e la classe. Anche gli studenti naturalmente vengono associati ad un ID per rendere la modifica immediata.

 


Per gli insegnanti invece vengono visualizzati: nome, cognome, le classi in cui insegna e la materia.
Anche loro sempre identificati tramite un ID.

 

                                                                                     
Per quanto riguarda invece le valutazioni e le materie:
  


Ogni materia è associata allo stesso ID degli insegnanti, e ogni valutazione ha l’ID dello studente che l’ha preso; in aggiunta è anche presente la media dei voti per ogni materia.

È fondamentale, una volta eseguita qualsiasi modifica, di eseguire l’azione di uscita (digitare 6 dal menù principale) per apportare in modo definitivo le modifiche.

