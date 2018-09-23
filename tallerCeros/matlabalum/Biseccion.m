function [p, X] = Biseccion( F, tol, CantIt, x0, x1)
%
%ENTRADA
% F      = funcion a la cual encontrar su raiz
% tol    = valor de tolerancia
% CantIt = cantidad maxima de iteraciones 
% x0,x1  = puntos iniciales. Debe cumplirse F(x0)*F(x1)<0.
%
%SALIDA
% p      = valor de la raiz encontrada

seq_x = x0;


iter = 1;

while iter <= CantIt,
    
   % calcular punto medio p entre a y b 
   p = x0 + (x1-x0)/2;
   
   fp = F(p);
   
   if abs(fp) < tol || (x1-x0)/2 < tol,
       fprintf(1,'numero iteraciones: %d\n',iter);
       if nargout==2, X = seq_x; end;
       return;    
   end
   
   seq_x = [seq_x , p];
   
   fprintf(1,'x(%d):%f f(x):%f |x(%d)-x(%d)|:%f\n',...
        iter, p, fp, iter, iter-1, abs(x0-p));

   if F(x0) * F(p) > 0,       
       x0 = p;       
   else       
       x1 = p;       
   end
   
   iter = iter + 1;
       
end



error('no se llego a la raiz luego de la cant max de iteraciones')