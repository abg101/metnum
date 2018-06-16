function x = resolverQR(A, b)

    cant_cols = size(A,2);

    if (rank(A) < cant_cols)
        error('A no tiene rango columna completo')
    end

    [Q,R] = qr(A);
    b_prima = Q'*b;
    b_prima_1 = b_prima(1:cant_cols);
    R_prima = R(1:cant_cols,:);
    x = R_prima \ b_prima_1;
    
end
