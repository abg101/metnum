function [ranking] = pagerank(nombre_archivo)
%% 

%nombre = "test_30_segundos";
fname = nombre_archivo ;%strcat(nombre,".txt");
outname = strcat(fname, ".out");

conectividades = dlmread(fname);
resultado_catedra = dlmread(outname);

paginas = conectividades(1,1);
links = conectividades(2,1);
p = resultado_catedra(1);

conectividades = conectividades(3:end,:);
resultado_catedra = resultado_catedra(2:end);
%% 
W = zeros(paginas,paginas);
for f = 1:size(conectividades,1)
    W(conectividades(f,2),conectividades(f,1)) = 1;
    
end

grados = sum(W,1);
for i = 1:size(grados,2)
    if grados(i) ~= 0
        grados(i) = 1/grados(i);
    end
end    

D = diag(grados);

matriz_a_resolver = eye(paginas)-p*W*D;
b = ones(paginas,1);

res = matriz_a_resolver\b;
res_normalizado = res/norm(res,1);
ranking = res_normalizado;

end
