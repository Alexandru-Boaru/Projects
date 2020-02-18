BOARU ALEXANDRU
331CB

TEMA 1 EGC - MY FLAPPY BIRD

0. PENTRU INCEPUT
	Inainte de a ma apuca sa implementez mecanicile si sa desenez elementele din scena. Am pregatit fisierele Tema1.h si Tema1.cpp
plecand de la fisierele Laborator3 pentru crearea scenei, setarea camerei si pentru metodele care lucreaza pe frame-uri si event-uri.
Pe langa acestea, am folosit Laborator2 ca ajutor pentru a-mi defini functia CreateMesh() care se ocupa de crearea meshelor folosind
bufferele VAO, VBO si IBO. De asemenea, am creat un fisier Transform2D.h in care mi-am definit matricile pentru transformarile de
translatie, scalare si rotatie (inspirat din Laborator3). Am adaugat mai tarziu si matriecea de visualizare pentru transformarea
poarta-fereastra (inspirat din Laborator3_Vis2D), modificat pentru a centra scena in fereastra.

1. MECANICA PASARII
	birdPosX, birdPosY	- pozitia pasarii in scena
	birdAngle			- unghiul de inclinare a pasarii
	wingAngle			- factor de scalare al aripilor
	sign				- directia de miscare a aripilor
	birdCenter			- coordonatele centrului pasarii
	birdRadius			- raza cercului din jurul pasarii
	timeForJump, startTimeForJump	- timpul in care pasarea se ridica si valoarea ei de start
	gravity, startGravity			- gravitatia care afecteaza caderea pasarii si valoarea sa de start		
	
	Functionalitatea pasarii este in felul urmator:

		- ea in mod normal se afla in picaj; este afectata de matricea de translatie si se indreapta spre partea de jos a ecranului,
		in acelasi timp se aplica o rotatie care face ca pasarea sa se incline in jos cu birdAngle grade fata de birdCenter. Picajul
		este afectat de gravity (valoarea care se mareste dupa fiecare frame)
		
		- daca se apasa tasta SPACE, atunci timeForJump este setat la startTimeForJump si scade dupa fiecare frame. Ridicarea este de
		scurta durata, dar viteza cu care se ridica este mai puternica decat cea cu care pica initial (de asemenea gravity este resetat).
		In acest timp se inclina in sus cu birdAngle grade.

		- daca pasarea trece vreuna din marginile scenei atunci se incheie jocul si se afiseaza punctajul obtinut

		- aceste mecanici sunt implementate in metoda BirdMotion

2. DESENAREA PASARII
	Pentru a desena pasarea in scene m-am folosit de programul GIMP pentru a ma ghida. Am schitat o pasare, am marcat puncte care joaca
rolul de vertecsi si am unit acesti vertecsi, obtinand triughiurile care impreuna creaza pasarea. Folosind sistemul de coordonate din
GIMP mi-am definit vertecsii in program. Initial era un singur obiect cu o singure culoarea. Ulterior am spart forma in mai multe
piese (corp, cap, cioc, aripi) pentru a le colora diferit si pentru a anima separat aripile.

3. DEFINIREA DREPTUNGHIURILOR
	Obstacolele sunt create din cate doua dreptunghiuri suprapuse, cu un spatiu de dimensiune fixa intre ele prin care poate trece pasarea.
Mi-am definit o clasa Rectangle2D pentru a crea informatia pentru meshe (vertecsi si indici) si pentru a controla mecanica obstacolelor.
	name		- numele obiectului
	corner		- coordonata coltului din care porneste dreptunghiul
	width		- latimea dreptunghiului
	height		- inaltimea dreptunghiului
	color		- culoarea dreptunghiului
	vertices	- vertecsi care definesc dreptunghiul
	indices		- indicii care definesc dreptunghiul

	Clasa contine un constructor care primeste care primeste ca parametri un nume, pozitia coltului de inceput al dreptunghiului, latimea,
inaltimea si culoarea dreptunghiului. Constructorul foloseste aceste informatii pentru a crea informatiile necesare la crearea meshei necesare

4. MECANICA OBSTACOLELOR
	Tot in clasa Rectangle2D am definit cateva variabile si metode in plus pentru informatia despre pozitionarea obstacolelor in scena si
pentru mecanica acestora

	sign		- decide daca gaura este mobila (0 - imobila, 1 - mobila in sus, -1 - mobila in jos)
	point		- coordonata folosita in identificarea pozitiei obstacolului
					point.x semnifica pozitia coltului din stanga jos a obstacolului in scena
					point.y semnifica inaltimea dreptunghiului de jos si totodata coordonata de la care porneste gaura din obstacol
	counted		- bool care verifica daca obstacolul a punctat jucatorul
	checked		- bool care verifica daca obstacolul a generat un nou obbstacol

	Exista de asemenea un constructor care primeste ca argumente un vec2 care defineste pozitia obstacolului si a gaurii, o culoare si
un semn care folosit pentru a decide daca gaura e mobila sau nu

	Este de asemenea implementata o metoda checkCollision() care verifica coliziunea intre pasare si obstacol (colizune intre cerc si cele 
doua dreptunghiuri ale obstacolului). 

	In Tema1 am definit cateva variabile pentru a tine evidenta si pentru a controla toate obstacolele existente

	rectList		- lista de obstacole existente in lista
	rectangleColor	- lista de culori posibile ale obstacolelor
	height			- inaltimea initiala a unui dreptunghi
	width			- latimea unui dreptunghi
	hole			- largimea unei gauri din obstacol
	offset			- punctul fata de origine de care trebuie sa treaca un obstacol pentru a genera altul
	obstacleSpeed	- viteza obstacolelor

	Functionalitatea obstacolelor este definita in metoda PipeMotion():

		- daca lista rectList este goala (inainte de inceperea jocului sau la resetare), atunci se genereaza un obiect Rectangle2D care retine
pozitia, culoarea si sensul de miscare al gaurii (daca e cazul)
		
		- inaltimea gaurii se alege in mod random folosind metoda NewRandomPipe() astfel incat gaura sa fie la distanta de 5% din inaltime fata
		de marginile de jos si de sus ale ecranului
		
		- pentru fiecare obstacol din lista (aflat in scena), se verifica daca apare vreo coliziune intre cercul din jurul pasarii si vreunul 
din cele doua dreptunghiuri ale obstacolului

		- se plaseaza obstacolele in scena
			dreptunghiul de jos este scalat astfel incat inaltimea acestuia sa fie egala cu point.y si este translatat pe axa OX pana la valoarea point.x
			dreptunghiul de sus este scalat astfel incat inaltimea acestuia sa fie egala cu height - point.y - hole, este translatat pe axa OX pana
				la valoarea point.x si este translatat pe OY pana la point.y + hole

		- se aplica translatia in stanga a obstacolului folosind obstacleSpeed si translatia gaurii pe axa OY (se schimba sensul in momentul in care gaura
		atinge limitele in care poate exista)

		- in momentul in care un obstacol trece de offset, atunci se adauga un obstacol nou in lista care porneste din partea dreapta a ecranului 
			(se schimba valoarea checked in true pentru ca sa se stie ca acest obstacol a generat deja un obstacol)
			(obstacolul are o sansa de 20% sa fie cu gaura mobila)

		- in momentul in care pasarea depaseste un obstacol, punctajul este incrementat; daca se trece de un anumit numar de puncte atunci 
		obstacleSpeed se mareste
			(se schimba valoarea counted in true pentru ca sa se stie ca acest obstacol a punctat deja jucatorul)

		- daca obstacolul cel mai din stanga a iesit complet din ecran (scena afisata) atunci acesta este sters din lista

5. CONTROLUL JOCULUI SI MESAJE IN CONSOLA

		startGame			- defineste starea de PLAY (true) si de PAUSE (false) a jocului
		gameOver			- defineste starea de GAME OVER a jocului
		score, highScore	- punctajul intermediar si punctajul maxim obtinut

		- cand jocul (aplicatia) este pornit, el se afla intr-o stare PAUSE

		- se afiseaza mesajul "NEW GAME" si punctajul 0

		- pentru a incepe jocul se apasa tasta SPACE sau P (se afla in stare PLAY)

		- pentru a trece din stare PLAY in PAUSE si invers se apasa tasta P

		- pentru a faca pasarea sa se ridice se apasa SPACE

		- pentru fiecare obstacol obtinut, scorul este incrementat si afisat in consola

		- daca pasarea intra in coliziune un obstacol sau daca atinge vreuna din margini ale ecranului, jocul intra in starea GAME OVER
		
		- in starea GAME OVER se afiseasa mesajul "GAME OVER" in consola, dupa care se afiseaza "FINAL SCORE: " si scorul final obtinut

		- in cazul in care scorul final obtinut este mai mare decat cel mai mare scor obtinut pana in prezent atunci se afiseaza
		"NEW HIGHSCORE: " si noul highScore

		- pentru a reporni jocul se apasa tasta R care reseteaza scene la forma initiala (se poate apasa R in orice stare se afla jocul)

6. CARACTERISTICI IN PLUS
	
	a) CERC AJUTATOR:

		- deoarece pasarea nu are forma rotunda (in ciuda faptului ca se verifica o coliziune de tip cerc) am creat un cerc ajutator pentru
		a identifica exact care zona pasarii care nu trebuie sa se atinga de obstacole

		- pentru a activa / dezactiva cercul se apasa tasta E

	b) PASARE DETALIATA SI ANIMATIE ARIPI:

		- am incercat sa desenez o pasare care sa aiba cat de cat forma unui tucan
		
		- forma a fost descompusa in mai multe piese colorate diferit (corp si aripi = negru, cap = alb, cioc = portocaliu)

		- aripile sunt animate astfel incat sa dea senzatia unei pasari care bate din aripi pentru a se mentine in aer

		- se foloseste wingAngle pe post de factor de scalare de la 1 pana la -1 (cu alte cuvinte imaginea aripilor este oglindita)

	c) OBSTACOLE DE MAI MULTE CULORI:

		- am creat mai multe meshe, fieacare cu o alta nuanta de rosu

		- culoarea urmatorului obstacol este aleasa aleator

	d) SCENA CU DETALII:

		- am adaugat un disc galben pe post de soare, un dreptunghi verde pe post de sol si am schimbat culoarea spatiului in culoarea cerului albastru

	e) CRESTEREA DIFICULTATII:

		- am mentionat mai devreme faptul odata ce scorul acumuleaza un anumit numar de puncte, viteza obstacolelor creste (in cazul meu odata 10 puncte)

	f) SCALARE SCENA SI ADAUGARE MASCA NEAGRA

		- pentru a pastra proportiile din scena in momentul in care se modifica dimensiunea ferestrei, am folosit o transformare de vizualizare
		(transformare poarta-fereastra) pe fiecare obiect randat in scena

		- am apelat metoda SetOrthographic a camerei folosind noile valori ale ferestrei care parametrii ai event-ului OnWindowResize() 

		- scena iese centrata in raport cu marginile ferestrei

		- am mascat zonele care nu fac parte din scena redata in mod normal cu niste dreptunghiuri negre folosind metoda AddBlackScreen()

		- daca factorul de scalare minim este pe axa OY, atunci se adauga doua dreptunghiuri in stanga si dreapta scenei

		- daca factorul de scalare minim este pe axa OX, atunci se adauga doua dreptunghiuri peste si sub scena

		- dimensiunile dreptunghiurilor sunt calculate in functie de dimensiunile logice definite de mine (1280 si 720) si de dimensiunile
		ferestrei reale, astfel incat ocupa atat spatiu cat este necesar (nu mai mult si nu mai putin decat necesar)
