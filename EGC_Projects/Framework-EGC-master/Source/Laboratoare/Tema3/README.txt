BOARU ALEXANDRU
331CB
EGC TEMA-3

Pentru implementarea acestei teme am folosit:
	-imaginea de heightmap din cerinta de pe ocw.cs.pub.ro
	-Blender pentru crearea unui model 3D al viermelui
	-laboratoarele 7, 8 si 9 ca surse principale de informatie
	-transformarile 3D implementate inspirate dupa cele din laboratorului 4
	-camera impreuna cu deplasarea ei inspirate dupa laboratorul 5

Afisare HeightMap:
	-Am creat functia CreateFlatGround() care genereaza o mesha a unui plan paralela cu XOZ cu lungimea si latimea date ca argumente
	-Am introdus coordonatele x si z pe post de coordonate de texturare (folosite mai tarziu pentru a introduce o textura repetitiva pe suprafata meshei)
	-Am incarcat imaginea ca textura cu numele "heightmap"
	-Am creat un shader pentru heightmap (alcatuit din HMVertexShader si HMFragmentShader)
	-In HMVertexShader calculez inaltimea corespunzatoare coordonatelor x,z si o folosesc pe post de coordonata y impreuna cu un factor de scalare;
	-De asemenea, calculez normalele ce urmeaza a fi folosite pentru iluminare
	-In HMFragmentShader aplic o textura de pamant care se repeta, folosind coordonatele de texturare create anterior
	-Se combina cu o nuanta de alb-negru in functie de inaltimea acelui punct din plan
	-Se calculeaza iluminarea globala
	-La final am adaugat la rezultat si doua lumini spot care lumineaza rosu (jucatorul 1) si albastru (jucatorul 2)

Deformare HeightMap:
	-In init folosesc stbi_load pentru a incarca informatia texturii in buffer-ul heigthPixels
	-Am creat functia CreateHole care primeste ca argumente coordonatele x si z in care a avut loc contatcul dintre teren si proiectil
	-Caut punctul de pe plan care corespunde acestor si date si scad din valorile acestui punct si ale vecinilor sai formand un crater (mai adanc in interior)
	-Buffer-ul de informatii este incarcat in textura heightmap facand bind la textura si folosind functia glTexImage2D

Vierme si textura:
	-Folosind software-ul de modelare 3D, Blender, am creat o mesha care reprezinta un vierme din joc
	-Tot cu ajutorul lui Blender am creat si o textura pentru vierme pentru a colora pielea, ochelarii si arma acesteia
	-Mesha a fost incarcata sub denumirea de "worm" si textura a fost incarcata sub denumirea de "worm_skin"

Afisare viermi:
	-Am creat WormShader folosit pentru viermi si pentru alte obiecte
	-Acest shader are mai multe moduri de randare:
		-0: foloseste numai culoarea data ca argument;
		-1: aplica o textura si lumina
		-2: aplica 2 texturi si lumina (nefolosit in final)
		-3: aplica o textura fara lumina
	-Pentru viermi am folosit modul 1, folosind textura "worm_skin"
	-Am adaugat si surse de lumini spot de culoare rosie si albastra in shader
	-Sursele de lumina urmaresc viermii
	-Folosind buffer-ul heightPixels, caut inaltimea corespunzatoare punctului in care se afla viermele in momentul curent si translateaza viermele la inaltimea respectiva
	-Daca zona de teren pe care sta este deformata, atunci viermele se va translata la noua inaltime

Camera:
	-Camera urmareste viermele curent din perspectiva third person, iar cand acesta lanseaza un proiectil, informatia despre pozitia si camerei si 
directia camerei sunt salvate in oldCameraPosition si in oldCameraUp. Camera se muta deasupra proiectilului si il urmareste pana face contact cu
terenul. Dupa un delay de 2 secunde camera se muta in perspectiva third person a celuilalt jucator

Gameplay:
	-Jucatorul curent poate directiona viermele miscand mouse-ul in timp de butonul drept al mouse-ului e apasat.
	-Viermele se poate deplasa cu un numar limitat de pasi in orice directie folosind sagetile
	-Pentru a lansa proiectilul se apasa butonul stang al mouse-ului
	-Cat timp proiectilul este in aer se blocheaza orice alta actiune
	-Proiectilul este translatat in directia data de vectorul forward al camerei + un unghi de offset
	-Proiectilul se deplaseaza cu bolta
	-Contactul cu terenul se verifica prin compararea coordonatei y a proiectilului fata de inaltimea terenului corespunzatoare pozitiei (x,z)
	-In momentul in care inaltimea proiectilului este mai mica decat cea a terenului, se formeaza un crater in jurul acelui punct de contact
	-Daca in zona de contact exista un vierme, atunci se scad 10 puncte HP ale acelui vierme
	-In momentul in care unul din viermi ramane fara HP, celalalt jucator este declarat castigator

UI:
	-Am adaugat niste bare pentru a reprezenta numarul de puncte HP al jucatorului curent si pentru a arata cati pasi mai se poate misca
	-Am adaugat o mesha a unei sageti transparente (facuta tot in blender) pentru a da o idee utilizatorului directia in care va fi lansat proiectilul

Fundal:
	-Am creat mesha a unei sfere dar cu fetele inversate, astfel incat sa pot afisa textura sferei in interior-ul acesteia
	-Am facut acest lucru pentru ca am activat culling-face pentru a nu bloca vederea camerei din orice unghi ar fi pozitionata
	-Am randat sfera cu textura unui cer instelat