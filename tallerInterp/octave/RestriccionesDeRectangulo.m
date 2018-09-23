% Toma y devuelve una posición xywh = [xmin ymin width height] del
% rectángulo. Le aplica restricciones.
% xZoom es el zoom que se quiere aplicar
% imgSize es el tamaño de la imagen base
function xywh = RestriccionesDeRectangulo(xywh, xZoom, imgSize)

    % Redondear
    xywh = round(xywh);

    % No se puede salir de la imagen
    x = xywh(1); y = xywh(2); w = xywh(3); h = xywh(4);
    if x < 1,
        x = 1;
    else
        if x > imgSize(2),
            x = imgSize(2);
        end
    end
    if y < 1,
        y = 1;
    else
        if y > imgSize(1),
            y = imgSize(1);
        end
    end
    if x+w-1 < 1,
        w = 2-x;
    else
        if x+w-1 > imgSize(2),
            w = imgSize(2) + 1 - x;
        end
    end
    if y+h-1 < 1,
        h = 2-y;
    else
        if y+h-1 > imgSize(1),
            h = imgSize(1) + 1 - y;
        end
    end
    xywh = [x y w h];
    %borderize = makeConstrainToRectFcn('imrect',get(axis,'XLim'), get(axis,'YLim'));
    %xywh = borderize(xywh);
    
    % Alto y ancho deben ser múltiplos del xZoom que se realizará
    xywh = [xywh(1) xywh(2) (xywh(3)-mod(xywh(3),xZoom)) (xywh(4)-mod(xywh(4),xZoom))];
end