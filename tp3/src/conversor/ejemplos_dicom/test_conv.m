%%
archin = '1.2.826.0.1.3680043.2.656.1.138.35.dcm';
archout = 'convertida.pgm';

[Iin, mask] = dicom2pgm(archin,archout);
figure;
imshow(Iin,[]);
colorbar;

%% leo la imagen convertida y aplico la mascara para mejorar la visualizacion
% ver test_vis.m para ver ejemplos de visualizacion
Iout = imread(archout);
Iout = double(Iout);
Iout(~mask) = nan;

figure;
min_val = -2000; % es el minimo de la imagen de entrada
max_val = max(Iout(:));
imshow(Iout,[min_val max_val])
colorbar;
