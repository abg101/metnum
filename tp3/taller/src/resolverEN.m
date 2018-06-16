function x = resolverEN(A, b)    
    
    cant_cols = size(A,2);

    if (rank(A) < cant_cols)
        error('A no tiene rango columna completo')
    end

    L = chol(A'*A, 'lower');
    x = (L*L')\(A'*b);
    
end
