function PSNR = psnr(img, original)
    MSE = @(img, orig) sum(sum((double(img) - double(orig)).^2)) / (size(orig,1)*size(orig,2));
    PSNR = 10*log10(255*255/MSE(img, original));
end