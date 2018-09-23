% Aplica un muestreo a una imagen y la retorna
%
% img    = imagen fuente de 1 o 3 canales.
% factor = un entero >= 2 para zoom o una fracción 1/n con n entero para
%          subsampling
% method = función de resampling
% IMG    = imagen con el muestreo aplicado, del mismo tipo de datos que img
%
function IMG = Muestreo(img, factor, zoomMethod)
    
    iterations = 1; % para hacer mas iteraciones, por simplicidad no lo usamos
    
    channels = size(img,3);
    classOfElements = class(img);
    img = cast(img, 'double');
    
    if channels == 3,
        for c=1:size(img,3),
            imgZoom = img(:,:,c);
            for iter=1:iterations,
                imgZoom = zoomMethod(imgZoom,factor);
            end
            IMG(:,:,c) = imgZoom;
        end
    elseif channels == 1
        for iter=1:iterations,
            img = zoomMethod(img,factor);
        end
        IMG = img;
    end
    
    IMG = cast(IMG, classOfElements);
end