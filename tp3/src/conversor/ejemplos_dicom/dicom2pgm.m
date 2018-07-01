function [I,mask] = dicom2pgm(filein,fileout)
% Convierte una imagen en formato DICOM a PGM de 16 bit por pixel
% Devuelve la imagen leide por dicomread y una mascara que indica con 1 o
% con 0 si hay informacion en la imagen leida

[~,~,ext] = fileparts(filein);
assert(strcmpi(ext,'.dcm'));

[~,~,ext] = fileparts(fileout);
assert(strcmpi(ext,'.pgm'));

I = dicomread(filein);
I2 = uint16(I);
% los valores negativos indican que no hay informacion
mask = I >= 0;
% los pongo en 0 para poder visualizarlos
I2(~mask) = 0; 
imwrite(I2,fileout);

end
