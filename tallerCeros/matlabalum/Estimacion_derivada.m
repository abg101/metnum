function der = Estimacion_derivada(F, x)
% function der = Estimacion_derivada(F, x)
%
%  Se calcula la derivada la funcion F en un punto x
%

h = sqrt(eps)*x;
xph = x + h;
dx = xph - x;
der = (F(xph) - F(x)) ./ dx;
