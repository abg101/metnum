function [] = Mostrar(img, titulo)
    img = cast(img, 'uint8');
	
    if nargin >= 2,
        figure('name', titulo);
        %title(titulo);
    else
        figure
    end
    
    imshow(img);
end