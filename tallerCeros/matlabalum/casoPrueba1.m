%% --------- CASO PRUEBA 1 ------------------
% a,b= parametros de ecuacion de la recta
% y  = puntos en R^{3 x n} donde se ubican las rocas de kriptonita
% C  = cota del valor de aturdimiento 
% F  = funcion a encontrar la raiz
% derivada_F = derivada de F
%
close all  %cierra todas las ventanas
clear variables  %elimina variables del entorno

%% CASO 1
%
% Definiciones 

%parametros de la recta
a = [3;1;1];
b = [1;1;-0.5];

% coordenadas de las naves estelares
y = [ [2;3;-0.5] , [2;0.5;0.5] ];   

% cota para el nivel de aturdimiento
C = 1;

% Definicion de Funciones

 F = @(x,a,b,y,C) A(x, a,b,y) - C % **COMPLETAR**
 derivada_F = @(x,a,b,y,C) derA(x, a,b,y)  % **COMPLETAR**

 F_de_t = @(t) F(t,a,b,y,C);
 derivada_F_de_t = @(t) derivada_F(t,a,b,y,C);

 
% Grafico de la funcion de aturdimiento

%graficador(y,a,b)


%% NEWTON

disp('NEWTON-RAPHSON')

x0 = 0.7; % ** COMPLETAR ** 
tol = 0.00000001; 
maxnumit = 40;

[xfinal,seq_x_NR] = NewtonR(F_de_t, derivada_F_de_t, tol, maxnumit, x0);
t = [1:10]/10
g_de_t = @(t) t - F_de_t(t)/derivada_F_de_t(t);
g_de_t(0.5)
aux = []
ts = []
for k = 7 : 10,
    t = k/10;
    ts = [ts, t];
    derivada_de_g = Estimacion_derivada(g_de_t,t)
    fprintf(1,'derivada de g: %f\n',derivada_de_g);
    aux = [aux, derivada_de_g];
end
plot(ts, aux);

seq_x_NR

%plot([1:length(seq_x_NR)], abs(seq_x_NR-xfinal))

%% BISECCION

disp('BISECCION')

x0 = 0.3; % ** COMPLETAR **
x1 = 0.9; % ** COMPLETAR **
tol = 0.00000001; 
maxnumit = 40;

[xfinal,seq_x_Bisec] = Biseccion(F_de_t, tol, maxnumit, x0, x1);

seq_x_Bisec

%plot([1:length(seq_x_Bisec)], abs(seq_x_Bisec-xfinal))