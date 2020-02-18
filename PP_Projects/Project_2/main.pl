%predicatul principal care intoarce nodul Root si lista Edges cu muchiile continute in arbore
stp([N,M], Root, Edges) :- 
	minPrioritate(N, R), [Root,_] = R,
	append(R, [0], Raux),
	createTree(M, N, [], [Raux], Edges, _).

%predicatul prin care verifica existenta unui element intr-o lista
member(E, [E|_]).
member(E, [_|Tail]) :- member(E, Tail).

%predicatul prin care verfica existenta unui nod
node(X, Nodes) :- member(X, Nodes).

%predicatul prin care verfica existenta unei muchii
edge(X, Y, C, Edges) :- member([X,Y,C], Edges).
edge(X, Y, C, Edges) :- member([Y,X,C], Edges).

edge(X, Y, Edges) :- member([X, Y], Edges); member([Y, X], Edges).

%predicatul prin care cauta nodul root (cu prioritatea cea mai mica)
minPrioritate([H|T], R) :- minPrioritate(T, H, R).
minPrioritate([], R, R).
minPrioritate([[I,P]|T], [_,P0], R) :- P < P0, minPrioritate(T,[I,P], R),!.
minPrioritate([[_,P]|T], [I0,P0], R) :- P >= P0, minPrioritate(T,[I0,P0], R),!.

%predicatul prin care se construieste arborele FullTree care contine toate muchiile 
%grafului cautat
createTree(_, Nodes, Tree, Visited, FullTree, FullVisited) :- 
	length(Nodes, Ln),length(Visited, Lv),
	Ln == Lv, FullTree = Tree, FullVisited = Visited.

createTree(Edges, Nodes, Tree, Visited, FullTree, FullVisited) :- 
	length(Nodes, Ln), length(Visited, Lv),Ln > Lv,
	listaMin(Edges, Visited, Nodes, M),
	updateTree(M, Nodes, NewTree, NewVisited, Tree, Visited),
	createTree(Edges, Nodes, NewTree, NewVisited, FullTree, FullVisited),!.

%predicat prin care se actualizeaza starea arborelui (adaugandu-se muchii noi)
%si se actualizeaza starea listei de noduri vizitate (adaugandu-se noduri noi)
updateTree([], _, NewTree, NewVisited, NewTree, NewVisited).

updateTree([[Ns,Nd,C]|T], Nodes, NewTree, NewVisited, Aux1, Aux2) :- 
	node([Nd,P], Nodes),
	updateTree(T, Nodes, NewTree, NewVisited, [[Ns,Nd]|Aux1], [[Nd,P,C]|Aux2]),!.


%predicat prin care intoarce o lista cu muchiile care creaza drumuri de cost minim
listaMin(Edges, Visited, Nodes, M) :- 
	listaMuchii(Edges, Visited, L),
	minMuchie(L, Maux),
	filterMinPriority(Maux, Nodes, M, []).

%predicat prin care se elimina caile cu cost egal care duc la acelasi nod 
%(se pastreaza numai calea care are nodul cu prioritatea cea mai mica)
filterMinPriority([], _, Mnew, Mnew).

filterMinPriority([[N1,N2,C]|T], Nodes, Mnew, Acc) :- 
	not(member([_,N2,_], Acc)),
	filterMinPriority(T, Nodes, Mnew, [[N1,N2,C]|Acc]),!.

filterMinPriority([[N1,N2,C]|T], Nodes, Mnew, Acc) :- 
	member([Naux,N2,C], Acc), 
	node([N1,P1], Nodes), node([Naux,P2], Nodes),
	P1 < P2, 
	swap([Naux,N2,C], [N1,N2,C], Acc, NAcc),
	filterMinPriority(T, Nodes, Mnew, NAcc),!.

filterMinPriority([[N1,N2,C]|T], Nodes, Mnew, Acc) :- 
	member([Naux,N2,C], Acc),
	node([N1,P1], Nodes), node([Naux,P2], Nodes),
	P1 > P2,
	filterMinPriority(T, Nodes, Mnew, Acc),!.


%predicat prin care se inlocuieste un element X dintr-o lista L cu un element Y
swap(X, Y, L, R) :- !,swap(X, Y, L, R, []).
swap(_, _, [], R, R) :- !.
swap(X, Y, [X|T], R, Acc) :- !,swap(X, Y, T, R, [Y|Acc]).
swap(X, Y, [Z|T], R, Acc) :- !,swap(X, Y, T, R, [Z|Acc]).


%predicat prin care se cauta muchiile de cost minim. Daca sunt mai multe muchii de cost minim,
%atunci toate vor fi salvate intr-o lista
minMuchie([H|T], M) :- minMuchie(T, M, [H]),!.

minMuchie([], M, M).
minMuchie([[N1,N2,C1]|T1], M, [[_,_,C2]|_]) :- 		C1<C2, minMuchie(T1, M, [[N1,N2,C1]]).
minMuchie([[_,_,C1]|T1], M, [[N3,N4,C2]|T2]) :- 	C1>C2, minMuchie(T1, M, [[N3,N4,C2]|T2]).
minMuchie([[N1,N2,C]|T1], M, [[N3,N4,C]|T2]) :- 	minMuchie(T1,M,[[N1,N2,C],[N3,N4,C]|T2]).

%predicat prin care se genereaza o lista cu toate muchiile care pot fi alese 
listaMuchii(Edges, Visited, L) :-setof([N1,N2,C], costMuchie(N1, N2, C, Edges, Visited), L).

%predicat prin care se creaza o muchie al carui cost este egal cu suma dintre costul 
%drumului pana in X si costul muchiei (X,Y)
costMuchie(X, Y, C, Edges, Visited) :- 
	node([X,_,C1], Visited), edge(X, Y, C2, Edges), not(node([Y,_,_], Visited)), C is C1 + C2.

%predicat pentru bonus: se foloseste de Edges primit de la stp pentru a gasi drumul 
%intre Src si Dst
drum([N,M], Src, Dst, Root, Edges, Path) :- stp([N,M], Root, Edges), path(Src, Dst, Edges, Path).

%predicat prin care returneaza Path-ul dintre Src si Dst
path(Src, Dst, Tree, Path) :-
	searchPath(Src, Dst, [Src], Tree, Paux),
	reverse(Paux, Path).

%predicat recursiv folosit pentru a cauta prin muchiile aflate in Tree un drum intre Src si Dst
searchPath(Src, Dst, Visited, Tree, Path) :-
	edge(Src, Dst, Tree),
	Path = [Dst|Visited],!.

searchPath(Src, Dst, Visited, Tree, Path) :-
	edge(Src, N, Tree),
	N \== Dst,
	not(member(N, Visited)),
	searchPath(N, Dst, [N|Visited], Tree, Path),!.

%predicat prin care se returneaza o lista inversata
reverse(L,Rev) :- reverse(L,Rev,[]).
reverse([],Rev,Rev).
reverse([H|T],Rev,Acc) :- reverse(T,Rev,[H|Acc]).
