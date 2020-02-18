function  cerinta2()
  figure;
  image='in/images/image1.gif';
  A=imread(image);
  A=double(A);
  [m n]=size(A);
  k=[1:19 20:20:99 100:30:min(m,n)];

  [U S V]=svd(A);
  j=min(n,m);
  
  %image1 grafic 1
  B=zeros(1,j);
  a=size(k,2);
  for i=1:j
    B(i)=S(i,i);
  endfor  
  
  subplot(4,1,1);
  plot(B,'*');
  grid on;
  
  %image1 grafic 2
  s1=0;
  s2=0;
  M=zeros(1,a);
  for i=1:j
      s2=s2+S(i,i);
  endfor
  for e=1:a
    for i=1:k(e)
      s1=s1+S(i,i);
    endfor
    M(e)=s1/s2;
    s1=0;
  endfor
  subplot(4,1,2);
  plot(k,M,'*');
  grid on;
  
  %image1 grafic 3
  s1=0;
  M(:)=0;
  for i1=1:a
    A_k=cerinta1(image,k(i1));
    Ak=double(A_k);
    for i2=1:m
      for i3=1:n
        s1=s1+(A(i2,i3)-Ak(i2,i3))^2;
      endfor
    endfor
    M(i1)=s1/(m*n);
    s1=0;
  endfor
  subplot(4,1,3);
  plot(k,M,'*');
  grid on;

  %image1 grafic 4  
  M(:)=0;
  for i1=1:a
    M(i1)=(m*k(i1)+n*k(i1)+k(i1))/(m*n);
  endfor
  subplot(4,1,4);
  plot(k,M,'*');
  grid on;

  
  figure;
  image='in/images/image2.gif';
  A=imread(image);
  A=double(A);
  [m n]=size(A);
  k=[1:19 20:20:99 100:30:min(m,n)];
  [U S V]=svd(A);
  j=min(n,m);
  
  %image2 grafic 1
  B=zeros(1,j);
  a=size(k,2);
  for i=1:j
    B(i)=S(i,i);
  endfor  
  
  subplot(4,1,1);
  plot(B,'*');
  grid on;
 
  %image2 grafic 2
  s1=0;
  s2=0;
  M=zeros(1,a);
  for i=1:j
      s2=s2+S(i,i);
  endfor
  for e=1:a
    for i=1:k(e)
      s1=s1+S(i,i);
    endfor
    M(e)=s1/s2;
    s1=0;
  endfor
  subplot(4,1,2);
  plot(k,M,'*');
  grid on;
  
  %image2 grafic 3
  s1=0;
  M(:)=0;
  for i1=1:a
    A_k=cerinta1(image,k(i1));
    Ak=double(A_k);
    for i2=1:m
      for i3=1:n
        s1=s1+(A(i2,i3)-Ak(i2,i3))^2;
      endfor
    endfor
    M(i1)=s1/(m*n);
    s1=0;
  endfor
  subplot(4,1,3);
  plot(k,M,'*');
  grid on;

  
  %image2 grafic 4
  M(:)=0;
  for i1=1:a
    M(i1)=(m*k(i1)+n*k(i1)+k(i1))/(m*n);
  endfor
  subplot(4,1,4);
  plot(k,M,'*');
  grid on;


endfunction
    
    