function [position, croppedImg] = SeleccionarAreaParaHacerZoom(img, xZoom, position)
    if nargin == 2,
        hfigSeleccion = figure('name', 'Seleccionar area');
        imshow(img);

        [x, y, buttons] = ginput (2);

        close(hfigSeleccion);
    end

    % Obtener el cuadradito de la imagen original que le vamos a hacer zoom
    x = round(x);
    y = round(y);
    
    % Obtener el cuadradito de la imagen original que le vamos a hacer zoom
    % El tamanio debe ser multiplo del factor a hacer zoom
    croppedImg = img(y(1) : y(2) - mod(y(2)-y(1)+1,xZoom), x(1):x(2)- mod(x(2)-x(1)+1,xZoom), :);

    position=[x,y];


end
