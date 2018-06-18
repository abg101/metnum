% 
% hCuad = cuantos cuadrados negros y blancos de alto
% wCuad = cuantos cuadrados negros y blancos de ancho
% lCuad = tamaño en pixels del lado de cada cuadrado
%
function img = GenerarImagenTablero(hCuad, wCuad, lCuad)    
    img(:,:,1) = (checkerboard(lCuad,hCuad,wCuad) > 0.5).*255;
    img(:,:,2) = img(:,:,1);
    img(:,:,3) = img(:,:,1);
    img = uint8(img);
end
