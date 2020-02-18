function [Ak S] = cerinta3(image, k)
  A=imread(image);
  A=double(A);
  [m n]=size(A);
  M=zeros(m,1);
  %PAS 1
  s=0;
  for i=1:m
    for j=1:n

      s=s+A(i,j);
    endfor
    M(i)=s/n;
    s=0;
  endfor
  
  %PAS 2
  H=ones(1,n);
  A=A-(M*H);
    
  %PAS 3
  Z=A'/sqrt(n-1);
  
  %PAS 4
  [U S V]=svd(Z);
  
  %PAS 5
  W=V(:,1:k);
  
  %PAS 6
  Y=W'*A;
  
  %PAS 7
  Ak=(W*Y)+(M*H);
  Ak=uint8(Ak);
endfunction
  