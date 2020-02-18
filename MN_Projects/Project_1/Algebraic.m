% Boaru Alexandru, 311CB

function R = Algebraic(nume, d)
	% Functia care calculeaza vectorul PageRank folosind varianta algebrica de calcul.
	% Intrari: 
	%	-> nume: numele fisierului in care se scrie;
	%	-> d: probabilitatea ca un anumit utilizator sa continue navigarea la o pagina urmatoare.
	% Iesiri:
	%	-> R: vectorul de PageRank-uri acordat pentru fiecare pagina.
  
  % Am deschis fisierul nume pentru citirea datelor
  [fid, msg]=fopen(nume,"r","native");
  
  % nr retine numarul de pagini
  nr=str2num(fgetl(fid));
  A=zeros(nr,nr);
  M=zeros(nr,nr);
  L=zeros(nr,1);
  R(1:nr,1)=1/nr;
  l=ones(nr,1);
  I=eye(nr);
  
  % Pentru fiecare pagina citesc cate un rand de date salvat sub forma vectorului m
  % L retine numarul de vecini pentru fiecare pagina. In cazul in care unul din vecini este pagina curenta, scad o unitate din L(i)
  for i=1 : nr
    m=str2num(fgetl(fid));
    L(i)=m(2);
    for j=1 : m(2);
      if(i!=m(2+j))
        A(i,m(2+j))=1;
      else
        L(i)--;
      endif
    endfor
  endfor
  
  % Crearea matricii M
  for i=1 : nr
    for j=1 : nr
      if(A(j,i)==1)
        M(i,j)=1/L(j);
      endif
    endfor
  endfor
  
  % Am aflat inversa matricii B=I-d*M folosind GramSchmidt 
  B=I-d*M;
  B1=GramSchmidt(B);
  B2=inv(B);
  
  % Am aflat R in cazul in care t->infinit
  R=B1*((1-d)/nr)*l;
endfunction