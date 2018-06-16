function resolverCompleto(arch_img, n, m, k)
    % genero los rayos y armo el sistema
    disp('Generando los rayos aleatoriamente...');
    R = generarRayos(arch_img, n, m, k);
    A = R(:, 2:end);
    b = R(:, 1);
    
    disp('Resolviendo el sistema directo con matlab...');
    tic
    x = A\b;
    t_X = toc;
    X = armarImagen(x, n, m);
    
    disp('Resolviendo el sistema por CM usando ecuaciones normales...');
    tic
    x = resolverEN(A, b);
    t_XEN = toc;
    XEN = armarImagen(x, n, m);

    disp('Resolviendo el sistema por CM usando QR...');
    tic
    x = resolverQR(A, b);
    t_XQR = toc;
    XQR = armarImagen(x, n, m);
    
    % muestro las imagenes    

    allPlots = findall(0, 'Type', 'figure', 'FileName', []);
    delete(allPlots);

    I = imread(arch_img);
    colormap(gray(256));
    subplot(2,2,1), image(I);
    title('Original')
    subplot(2,2,2), image(X);
    
    title(sprintf('Reconstruccion Matlab: %d segundos', t_X))
    subplot(2,2,3), image(XEN);
    title(sprintf('Reconstruccion EN: %d segundos', t_XEN))
    subplot(2,2,4), image(XQR);
    title(sprintf('Reconstruccion QR: %d segundos', t_XQR))
    
    suptitle(sprintf('Resultado utilizando %d rayos', k));
    
end

