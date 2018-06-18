function [f] = ej2(muestra)
%muestra = (1:200)
n = size(muestra,2)
c = 6.375 * 10.^-3
h = muestra * 0.255
g1 = 1200*muestra
g2 = (55/4)*muestra.^2
g3 = (1/24) * muestra.^3
g = c*(g3 - g2 + g1)
mh = ones(n,1)*h
mg = transpose(ones(n,1)*g)

f = mg + mh

errorColumnas = 0
errorFilas = 0 %???

