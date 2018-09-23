% Hace un resampling de la imagen con el algoritmo Splines.
%
% img    = matriz de tipo double (de 1 canal)
% factor = un entero >= 2 para zoom o una fracción 1/n con n entero para
%          subsampling
%
% IMG    = imagen con el factor aplicado. Los elementos son de tipo double.
%
function [IMG] = Splines(img,factor)
    if factor < 1,
        % Para subsampling usamos el de matlab
        IMG = imresize(uint8(img), factor, 'bicubic');
        return
    end

	step = 1/factor;
	
	[filas, cols] = size(img);
    imgAux = zeros(filas, round(cols*factor));
    IMG = zeros(round(filas*factor), round(cols*factor));
    
    % Resample por filas (para cada fila, interpolar los valores)
	x = 1:cols;
	xx = 1:step:(cols+1-step);
    for i = 1:filas,
		imgAux(i,:) = spline(x,[0 img(i,:) 0],xx);	
    end
    
    % Resample por columnas (para cada columna, interpolar los valores)
	y = 1:filas;
	yy = 1:step:(filas+1-step);
    for j = 1:round(factor*cols),
		IMG(:,j) = (spline(y,[0 imgAux(:,j)' 0],yy))';	
    end
end
