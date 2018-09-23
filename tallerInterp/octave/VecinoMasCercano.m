% Hace un resampling de la imagen con el algoritmo de MNN.
% El vecino más cercano de (x,y) es floor(x,y).
%
% img    = matriz de tipo double (de 1 canal)
% factor = un entero >= 2 para zoom o una fracción 1/n con n entero para
%          subsampling
%
% IMG    = imagen con el factor aplicado. Los elementos son de tipo double.
%
function [IMG] = VecinoMasCercano(img,factor)
	step = 1/factor;

	[filas, cols] = size(img);
        
	% Resample por filas (para cada fila, interpolar los valores)
    xx = floor(1:step:(cols+1-step));
    imgAux(:,:) = img(:,xx);

	% Resample por columnas (para cada columna, interpolar los valores)
    yy = floor(1:step:(filas+1-step));
	IMG(:,:) = imgAux(yy,:);
end