% Hace un resampling de la imagen con el algoritmo Bilineal.
%
% img    = matriz de tipo double (de 1 canal)
% factor = un entero >= 2 para zoom o una fracción 1/n con n entero para
%          subsampling
%
% IMG    = imagen con el factor aplicado. Los elementos son de tipo double.
%
function [IMG]= Bilineal(img,factor)
    if factor < 1,
        % Para subsampling usamos el de matlab
        IMG = imresize(uint8(img), factor, 'bilinear');
        return
    end

	step = 1/factor;

	[filas, cols] = size(img);
    
    imgAux = zeros(filas+1, round(cols*factor));
    IMG = zeros(round(filas*factor), round(cols*factor));
	
    % Resample por filas (para cada fila, interporar los valores)
	x = 1:(cols+1);
    img(:,cols+1) = img(:,cols-1); % para los bordes
	xx = 1:step:(cols+1-step);
    
    for i=1:filas,
        imgAux(i,:) = interp1(x,img(i,:), xx, 'linear'); 
    end
    imgAux(filas+1,:) = imgAux(filas-1,:);
    
    % Resample por columnas (para cada columna, interpolar los valores)
    y = 1:(filas+1);
    yy = 1:step:(filas+1-step);
    for j=1:round(cols*factor),
        IMG(:,j) = interp1(y, imgAux(:,j)', yy, 'linear')';
    end
end
