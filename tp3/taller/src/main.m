%%
I = leerImagen('../img/tomo.png');
mostrarImagen(I)
hold on

%%
simularRayo(I, 100, 100, 50, 1, 50, 99, 1)
simularRayo(I, 100, 100, 20, 1, 20, 99, 1)
simularRayo(I, 100, 100, 80, 1, 80, 99, 1)

%%
resolverCompleto('../img/tomo.png', 20, 20, 1000) % discretizacion 20x20 

%%
resolverCompleto('../img/tomo.png', 15,15, 4000) % discretizacion 40x40

%%
resolverCompleto('../img/tomo.png', 60, 60, 5000) % discretizacion 60x60

%%
resolverCompleto('../img/tomo.png', 60, 60, 8000) % discretizacion 60x60 

%%
resolverCompleto('../img/tomo.png', 60, 60, 10000) % esto tarda bastante
