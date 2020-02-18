% Boaru Alexandru, 311CB

Tema 1 MN - PageRank

Am lucrat funtctiile in ordinea data din enunt.

ITERATIVE:
Am deschis fisierul nume pentru citirea datelor. Am salvat in nr numarul de pagini, si am folosind functiile fgetl() si str2num, pentru
a salva datele de fiecare rand sub forma unui vector m. Acest vector a fost folosit pentru prelucrarea matricii A si a vectorului L.
Cu ajutorul lui A si al lui L am realizat matricea M, folosind functia data in ecuatie (M(i,j)=1/L(j) daca A(j,i)==1 ; 0 daca A(j,i)==0 ).
La final am folosit formula de recurenta pentru R, cat timp |R(t+1)-R(t)|>eps. Functia intoarce R(t).

GRAMSCHMIDT:
Am folosit algoritmul Gram Schmidt modificat pentru a gasi matricile Q si R. Pentru a gasi inversa matrici A am calculat n sisteme 
de ecuatii de tipul A*B=I (B- inversa matricii A, I- matricea identitate). A=Q*R => Q*R*B=I. Deci R*B=Q^(-1)*I=trans(Q). Sistemul
de tip R*B=trans(Q) este un sistem de tip superior triangular si l-am rezolvat cu formula generalizata de Back Substitution.

ALGEBRAIC:
Prima parte a functiei este similara cu ITERATIVE. Dupa ce am calculat matricea M, am calculat inversa matricii expresiei (I-d*M),
folosind GRAMSCHMIDT. Am aflat R pentru cazul in care t->infinit folosind formula data in cerinta.

APARTENENTA:
Stiind ca functia u(x) este continua, am folosit sistemul de ecuatii:
  - a*val1 + b = 0
  - a*val2 + b = 1
pentru a gasi a si b din functie. Dupa aceasta am formulat functia u(x) ceruta.

PAGERANK:
Am folosit ITERATIVE si ALGEBRAIC pentru a afla R1 si R2. Folosind fisierul, am memorat in nr numarul de pagini si in val1 si val2, 
valorile necesare pentru APARTENENTA. Am creat o matrice R3 de 3 coloane si nr randuri. Pe fiecare rand am salvat:
   - Pe prima coloana, indicele paginii
   - Pe a doua coloana, valoarea data de Apartenenta
   - Pe a treia coloana, valoarea din R2 a paginii respective
Am sortat randurile lui R3 cu functia sortrows() in functie de valorile de pe a doua coloana. In cazul in care exista pagini cu 
valori egale (daca au valoarea 1 sau 0) am sortat si in functie de valorile de pe a treia coloana, provenite de la R2. Am creat
un fisiser cu numele provenit din nume si concatenat cu ".out". Am folosit acest fisier pentru a printa R1 R2 si R3, folosind fprintf().

TESTARE:
Am folosit foarte mult Trial & Error. Am testat fiecare functie in parte folosind grafurile date. In unele cazuri am folosit si alte 
matrici pentru a testa diferite calcule, mai ales la matricile Q si R si inversa din functia GRAMSCHMIDT.
Am modificat functiile astfel incat sa-mi arate si alte matrici sau valori de interes (precum A, M, Q, R, nr), insa am inlaturat acele
modificari dupa ce am vazut ca totul mergea bine. Am modificat la un moment dat functia ITERATIVE astfel incat sa-mi afiseze starile 
intermediare ale lui R dupa fiecare utilizare a formulei de recurenta. 
