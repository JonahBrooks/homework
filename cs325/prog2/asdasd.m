function [  ] = asdasd( asd )
%UNTITLED4 Summary of this function goes here
%   Detailed explanation goes here

    A = rand(1,asd);
    B = zeros(1,20);
    
    for i = 1:20
        tic;
        SillySelect(A,ceil(rand(1)*asd));
        B(i) = toc;
    end
    
    disp(min(B));
    disp(mean(B));
    disp(max(B));
    

end

