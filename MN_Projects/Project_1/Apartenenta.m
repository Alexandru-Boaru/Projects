% Nume, Prenume, Grupa

function y = Apartenenta(x, val1, val2)
	% Functia care primeste ca parametrii x, val1, val2 si care calculeaza valoarea functiei membru in punctul x.
	% Stim ca 0 <= x <= 1
  
  % Stiind ca functia este continua, am folosit sistemul de ecuatii:
  % - a*val1 + b = 0
  % - a*val2 + b = 1
  % pentru a gasi a si b din functie
  
  a=1/(val2-val1);
  b=-a*val1;
  
  % functia u(x)
  if x<val1
    y=0;
  endif
  if x>val2
    y=1;
  endif
  if x>=val1 && x<=val2
    y=a*x+b;
  endif
endfunction
	