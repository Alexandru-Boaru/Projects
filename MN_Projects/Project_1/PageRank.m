% Nume, Prenume, Grupa

function [R1 R2] = PageRank(nume, d, eps)
	% Calculeaza indicii PageRank pentru cele 3 cerinte
	% Scrie fisierul de iesire nume.out
  
  % Aplic Iterative si Algebraic pentru aflarea lui R1 si R2
	R1=Iterative(nume, d, eps);
  R2=Algebraic(nume, d);
  
  % Folosesc fisierul nume pentru a memora nr (numar de pagini) val1 si val2 (pentru Apartenenta)
  [fid, msg]=fopen(nume,"r","native");
  nr=str2num(fgetl(fid));
  for i=1 : nr
    m=str2num(fgetl(fid));
  endfor
  val1=str2num(fgetl(fid));
  val2=str2num(fgetl(fid));
  fclose(fid);
  
  % Am creat o matrice R3 de 3 coloane si nr randuri
  R3=zeros(nr,3);
  
  % Pe fiecare rand am salvat:
  %   - Pe prima coloana indicele paginii
  %   - Pe a doua coloana valoarea data de Apartenenta
  %   - Pe a treia coloana valoarea din R2 a paginii respective
  for i = 1 : nr
    R3(i,1)=i;
    R3(i,2)=Apartenenta(R2(i,1),val1,val2);
    R3(i,3)=R2(i,1);
  endfor
  
  % Am sortat randurile lui R3 cu functia sortrows() 
  % in functie de valorile de pe a doua coloana. In cazul in care
  % exista pagini cu valori egale (daca au valoarea 1 sau 0) am sortat 
  % si in functie de valorile de pe a treia coloana
  R3=sortrows(R3,[2 3]);
  
  % Am creat un fisiser cu numele provenit din nume si concatenat cu ".out"
  % Am folosit acest fisier pentru a printa R1 R2 si R3, folosind fprintf()
  nume1=strcat(nume,".out");
  [fid, msg]=fopen(nume1,"w","native");

  fprintf(fid,"%d\n",nr);
  
  for i=1 : nr
    fprintf(fid,"%.6f\n",R1(i,1));
  endfor
  fprintf(fid,"\n");
  for i=1 : nr
    fprintf(fid,"%.6f\n",R2(i,1));
  endfor
  fprintf(fid,"\n");
  for i=nr : -1 : 1
    fprintf(fid,"%d %d %.6f\n",nr-i+1,R3(i,1),R3(i,2));
  endfor
  fclose(fid);
endfunction