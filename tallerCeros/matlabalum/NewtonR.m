function [x, seq_x] = NewtonR(F, derivadaF, tol, CantIt, x0)
%
% F         = funcion a la cual encontrar su raiz
% derivadaF = derivada de F
% x0        = punto inicial 
% tol       = valor de tolerancia para |F(x)|
% CantIt    = cantidad maxima de iteraciones 
%

x_ant = x0;
indit = 0;
seq_x = x0;

fprintf(1,'x(%d):%f f(x):%f\n', indit, x0, F(x0) );


while abs(F(x_ant)) > tol && indit < CantIt,
    
    % ** COMPLETAR CON ITERACION DE NEWTON **
    x = x_ant - F(x_ant) / derivadaF(x_ant);
    
    seq_x = [seq_x, x];
 
    indit = indit+1;
    
    fprintf(1,'x(%d):%f f(x):%f x(%d)-x(%d):%f\n',...
        indit, x, F(x), indit, indit-1, x-x_ant);
    
    x_ant = x;
  
    
    
end


fprintf(1,'numero iteraciones: %d\n',indit);

