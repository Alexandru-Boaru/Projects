function cerinta5()

  figure;
  image='in/images/image1.gif';
  A=imread(image);
  [m n]=size(A);
  k=[1:19 20:20:99 100:30:min(m,n)];

  %image1 grafic 1
  [Ak S] = cerinta3(image, k);
  D=diag(S);
  j=min(m,n);
  a=size(k,2);
  
  subplot(4,1,1);
  plot(D,'*');
  grid on;
  
  %image1 grafic 2
  s1=0;
  s2=0;
  M=zeros(1,a);
  S1=double(S);
  for i=1:j
      s2=s2+S1(i,i);
  endfor
  for i1=1:a
    for i2=1:k(i1)
      s1=s1+S1(i2,i2);
    endfor
    M(i1)=s1/s2;
    s1=0;
  endfor
  subplot(4,1,2);
  plot(k,M,'*');
  grid on;
  
  %image1 grafic 3
  s1=0;
  M(:)=0;
  A1=double(A);
  for i1=1:a
    A_k=cerinta3(image,k(i1));
    Ak=double(A_k);
    for i2=1:m
      for i3=1:n
        s1=s1+(A1(i2,i3)-Ak(i2,i3))^2;
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
    M(i1)=(2*k(i1)+1)/n;
    M(i1)
  endfor
  subplot(4,1,4);
  plot(k,M,'*');
  grid on;

  
  figure;
  image='in/images/image2.gif';
  A=imread(image);
  [m n]=size(A);
  k=[1:19 20:20:99 100:30:min(m,n)];
  
  %image2 grafic 1
  [Ak S] = cerinta3(image, k);
  D=diag(S);
  j=min(m,n);
  a=size(k,2);
  
  subplot(4,1,1);
  plot(D,'*');
  grid on;
  
  %image2 grafic 2  
  s1=0;
  s2=0;
  M=zeros(1,a);
  S1=double(S);
  for i=1:j
      s2=s2+S1(i,i);
  endfor
  for i1=1:a
    for i2=1:k(i1)
      s1=s1+S1(i2,i2);
    endfor
    M(i1)=s1/s2;
    s1=0;
  endfor
  subplot(4,1,2);
  plot(k,M,'*');
  grid on;

  %image2 grafic 3
  s1=0;
  M(:)=0;
  A1=double(A);
  for i1=1:a
    A_k=cerinta3(image,k(i1));
    Ak=double(A_k);
    for i2=1:m
      for i3=1:n
        s1=s1+(A1(i2,i3)-Ak(i2,i3))^2;
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
    M(i1)=(2*k(i1)+1)/n;
    M(i1)
  endfor
  subplot(4,1,4);
  plot(k,M,'*');
  grid on;
  
endfunction