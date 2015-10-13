function [  ] = time( fgh )
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
    A = rand(1, fgh);
    B = zeros(1,100);
    
    for i = 1:100
        tic;
        RandomizedSelect(A, fgh, ceil(rand(1)*fgh));
        B(i) = toc;
    end
    
    disp(min(B));
    disp(mean(B));
    disp(max(B));

end

