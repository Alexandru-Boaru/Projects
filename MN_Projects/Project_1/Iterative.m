% Boaru Alexandru, 311CB

function R = Iterative(nume, d, eps)
	% Functia care calculeaza matricea R folosind algoritmul iterativ.
	% Intrari:
	%	-> nume: numele fisierului din care se citeste;
	%	-> d: coeficentul d, adica probabilitatea ca un anumit navigator sa continue navigarea (0.85 in cele mai multe cazuri)
	%	-> eps: eruarea care apare in algoritm.
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
  
  %Cat timp diferenta de eroare este mai mare decat eps, se repeta relatia de recurenta petru R
  
  while 1
    r=R;
    R=d*M*r+((1-d)/nr)*l;
    if(abs(R-r)<eps)
      R=r;
      break;
    endif
  endwhile
  
  %Inchide fisierul nume
  fclose(fid);
endfunction