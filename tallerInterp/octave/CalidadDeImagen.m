function [maeVal, mseVal, psnrVal, mssimVal] = CalidadDeImagen(img, original, titulo),
    MSE = @(img, orig) sum(sum((double(img) - double(orig)).^2)) / (size(orig,1)*size(orig,2));
    MAE = @(img, orig) sum(sum(abs(double(img) - double(orig)))) / (size(orig,1)*size(orig,2));
    
    if nargin < 3,
        titulo = '';
    end
    
    maeVal = MAE(img,original);
    mseVal = MSE(img,original);
    psnrVal = psnr(img, original);
    %mssimVal = ssim(img, original);

    if length(titulo) > 0,
        fprintf([titulo, '\n']);
    end
    
    fprintf('---------------------------------------------------------\n');
    fprintf(['EAM  = ', num2str(maeVal),...
           '\nECM  = (', num2str(sqrt(mseVal)), ')^2' ,...
           '\nPSNR = ', num2str(psnrVal), '\n\n' ]);
    
    %fprintf(['MAE       = ', num2str(maeVal),...
    %       '\nsqrt(MSE) = ', num2str(sqrt(mseVal)),...
    %       '\nPSNR      = ', num2str(psnrVal),...
    %       '\nMSSIM     = ', num2str(mssimVal), '\n\n' ]);
end