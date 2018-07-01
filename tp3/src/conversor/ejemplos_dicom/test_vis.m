%% Levanto una imagen dicom y la muestro
%archin = '1.2.826.0.1.3680043.2.656.1.138.35.dcm';
archin = '1.2.826.0.1.3680043.2.656.1.138.35.dcm';
I = dicomread(archin);
% dicomread devuelve una imagen con 16 bit por pixel usando int16
% Por ende, los valores de los pixeles estan fuera del rango [0 255]
% y los valores negativos indican que no hay informacion.
% Para visualizar directamente esta imagen de 16 bits, hay que convertir
% los valores al rango [0 255] usando imshow

figure;
% Probar con distintos valores para entender que esta pasando
min_val = min(I(:));
max_val = max(I(:));
imshow(I,[min_val max_val])
colorbar;
%% al usar [] se toman el minimo y maximo por defecto
figure;
imshow(I,[])
colorbar;

%% Los valores negativos indican que no hay informacion
mask = I < 0;
% en 1 estan los negativos y en 0 los que, niego la mascara con ~
Imask = uint8(~mask)*255;
figure;
imshow(Imask);
title('Mascara con informacion: blanco hay info, negro no')

%% Uso de la mascara
I2 = double(I);
I2(mask) = nan; % Not a number
% los valores NaN se grafican en negro con imshow

figure;
% Probar con distintos valores para entender que esta pasando
min_val = -2000;
max_val = max(I2(:));
imshow(I2,[min_val max_val])
colorbar;

%% Tambien se puede graficar con imagesc que escala los valores automaticamente
figure;
imagesc(I);
colormap(gray); % probar distintos colormaps
colorbar;
axis image;
