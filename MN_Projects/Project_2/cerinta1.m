function A_k = cerinta1 (image, k)
  A=imread(image);
  A=double(A);
  [m n]=size(A);
  [U S V]=svd(A);
  A_k=U(:,1:k)*S(1:k,1:k)*(V(:,1:k))';
  A_k=uint8(A_k);
endfunction