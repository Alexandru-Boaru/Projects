# ============================== README PROIECT PA ==============================
# ================================== BYTECRAFT ==================================

## 1.   INSTRUCTIUNI DE COMPILARE:

    Codul sursa se poate compila folosind doar comanda "make" si se pot sterge fisierele create folosind comanda "make clean".
    
    Fisierul Makefile compileaza codul sursa pentru engine-ul jocului (folder-ul hlt), instructiunile bot-ului (fisierul MyBot.java) si dependintele de care MyBot are nevoie (folder-ul myBot).
    
    Comanda "make clean" sterge fisierele .class generate anterior si errorlog-urile existente in folderul principal.



## 2.    DETALII DESPRE STRUCTURA PROIECTULUI:

    Proiectul contine: un pachet cu toate fisierele sursa pentru engine-ul jocului (hlt), clasa principala MyBot.java care contine codul sursa pentru apelul strategiilor implementate si pachetul care le implementeaza, numit "myBot". In acest director se afla clasa care se ocupa de investirea resurselor in nave si dropoff-uri (HaliteSpender.java) si clasa care se ocupa de miscarea barcilor pe harta (Movement.java).

 
## 3.    DETALII DESPRE ABORDAREA ALGORITMICA:

## ============================== Etapa 1 ==============================

        Implementarea botului s-a bazat pe cerintele primei etape, si anume de a strange minimul de halite cerut pentru fiecare test.     

        Primii pasi pe care i-am facut ca echipa a fost sa testam toate functionalitatile oferite de API-ul jocului si sa observam ce strategii am putea sa aplicam.

        Am cautat strategiile finalistilor competitiei Halite III si am observat anumite comportamente ale botilor, spre exemplu:

    1)  Botii lor se concentrau la inceput pe crearea a cat mai multe nave ramanand de regula cu mai 
    putin de 1000 halite in fiecare tura;
    2)  Navele mai intai minau tot ce era in apropierea shipyard-ului si apoi isi extindeau zona de cautarea avand o 
    predispozitie in a se indrepta spre zone cu mult halite;
    3)  Dropoff-urile erau create foarte rar ceea ce inseamna ca pentru finalistii competitiei
    crearea unui dropoff era un big deal;
    4) In ultimele ture toate navele se indreptau spre un dropoff sa descarce cat halite mai aveau
    si tot atunci se cumula practic scorul final al botului;
    
        Toti membrii echipei au participat la proiectarea si imbunatatirea botului.
        Acest bot a fost proiectat folosind limbajul Java.
        Proiectarea botului a fost facuta pas cu pas, adaugand functionalitati noi, corectand bug-uri si modificand unii parametrii.
    
###    Functionalitate:
    
####    Spawn nave:
    
    - In primele 200 de ture, se creaza cat mai multe nave in limita halite-ului din storage.

####    Mutarea navelor:
    
    - O nava colecteaza halite-ul dintr-un spatiu pana cand ramane mai putin de 100 halite. Astfel se evita plata de tranzitie prea mare in acel spatiu la intoarcere (se va plati cel mult 10 halite).

    - Nava cauta local (in nord/sud/est/vest) zona cu cel mai mult halite.
    
    Aceasta tranzitie se realizeaza daca sunt indeplinite conditiile:
            -spatiul este liber
            -nava are destul halite sa avanseze
            -nava iese cu un castig mai mare de halite in acel spatiu decat halite-ul consumat pe tranzitie
            -nu s-a intrat inca in ultimele 20 de ture
            -daca nu este indeplinita una din conditii, nava ramane pe loc
            
    Nave pline si dropoff-uri:
    
    - Odata ce o nava a atins plinul (1000 de halite) aceasta incepe sa se indrepte spre baza/dropoff

    - Pentru a retine faptul ca nava a atins plinul, se salveaza intr-o lista id-ul navei si se verifica in fiecare tura daca id-ul navei se afla inca in lista. Cat timp id-ul e in lista, nava se duce spre baza

    - Odata ce nava a trecut prin baza/dropoff, id-ul este scos din lista

    - Daca nava se afla la o distanta mai mare de 20 de mutari de baza/dropoff, acea nava se converteste in dropoff

####    Final de joc:
    
    - In ultimele 20 de ture, toate navele se indreapta spre cea mai apropiata baza sau dropoff. Odata ce o barca intra in baza/dropoff aceasta nu va mai face nicio actiune.
    - Am permis coliziunea navelor intre ele in dropoff, avand in vedere ca se termina jocul curand.
    

## ============================== Etapa 2 ==============================

    - Pentru aceasta etapa nu a mai fost nevoie de nicio modificare a codului deoarece acesta a fost proiectat sa castige batalii si impotriva altor boti.
    - Dupa rularea checkerului pentru Etapa a 2 a, botul echipei noastre reuseste sa invinga in 5 runde, iar in 2 runde reuseste sa scoata un punctaj de 0.81/0.90.


## ============================== Etapa 3 ==============================

    - Pentru aceasta etapa nu am mai modificat bot-ul din cauza aglomerarilor testelor si a altor teme.
    - In ciuda a celor zise mai sus, bot-ul nostru scoate pe rand locurile: 1st, 1st, 2nd, 2nd, 3rd, 4th, 4th, 2nd,
    acumuland in final 0.29/0.36 puncte.


## ============================== Etapa 4 ==============================

    - Si pentru aceasta etapa, botul a ramas cel din etapa I



##    SURSE DE INSPIRATIE:

    - In principal ne-am inspirat din modul in care botii finalistilor din concursul Halite III se comportau. Pana la ora actuala nu am folosit un algoritm gandit de ei pentru a ne construi botul.

    - Am importat o librarie cu Algebra Liniara pentru operarea cu matrici. Aceasta se numeste Linear Algebra for Java (la4j)
    Codul sursa se gaseste pe pagina de git:
    https://github.com/vkostyukov/la4j/releases

    JavaDoc-ul se gaseste pe:
    http://la4j.org/apidocs/

    Pagina principala a librariei este:
    http://la4j.org

    




##    RESPONSABILITATI:
    
    - alexandru.boaru: proiectarea codului principal
    - giorgiana.balan: testarea si corectarea eventualelor erori
    - andrei.vlad0412: liderul echipei (coordonarea activitatilor + administrare repo git)
    - adrian.neata0506: cercetare si refactorizarea codului