% Boaru Alexandru, 311CB

function B = GramSchmidt(A)
	% Functia care calculeaza inversa matricii A folosind factorizari Gram-Schmidt
	% Se va inlocui aceasta linie cu descrierea algoritmului de inversare
  % Am folosit algoritmul Gram Schmidt modificat pentru a gasi matricile Q si R
  % Pentru a gasi inversa matrici A am calculat n sisteme de ecuatii de tipul A*B=I
  % B- inversa matricii A, I- matricea identitate
  % A=Q*R => Q*R*B=I. Deci R*B=Q^(-1)*I=Q'
  % Sistemul de tip R*B=Q' este un sistem de tip superior triangulizat si l-am rezolvat cu formula generalizata de Back Substitution
	[n,n] = size(A);
  Q = zeros(n);
  R = zeros(n);
  for j = 1:n
    Q(:,j) = A(:,j);
    for i=1 : j-1
      R(i,j) = Q(:,i)'*Q(:,j);
      Q(:,j) = Q(:,j) - R(i,j)*Q(:,i);
    endfor
    R(j,j) = norm(Q(:,j))';
    Q(:,j) = Q(:,j)/R(j,j);
  endfor
  b=Q';
  B = zeros(n);
  for k = 1 : n
    for i = n : -1 : 1
      B(i,k)=(b(i,k)-R(i,i+1:n)*B(i+1:n,k))/R(i,i);
    endfor
  endfor    
      
endfunction
    