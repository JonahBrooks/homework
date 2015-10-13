function [ kth ] = SillySelect( array, k )
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here

    for i = 1:k
        [kth,minin] = min(array);
        array(minin) = [];
    end

end

