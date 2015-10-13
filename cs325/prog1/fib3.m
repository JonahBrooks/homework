function [ fn, fibArray ] = fib3( n )
% fib3 computes the nth Fibonnaci Number using an iterative method
% This method should run in linear time O(n), unlike the previous
% iterative method (fib2), which stored the array and ran in constant time
%
% I would have used a stack, except that Matlab returns the pop value
% every time, which takes a while from spamming the screen...  ...?

    fn = -1;
   
    %fibStack=java.util.Stack();
    
    fibArray = zeros(1,3); %indexed at 1...
    fibArray(1) = 0; % n = 0
    %fibStack.push(0)
    fibArray(2) = 1; % n = 1
    %fibStack.push(1)
    
    for i = 2:n
        fibArray(mod(i,3)+1) = fibArray(mod(i-1,3)+1) + fibArray(mod(i-2,3)+1);
        %fibStack.push(fibArray(mod(i,3)+1));
        %f1 = fibStack.pop();
        %fn = f1 + fibStack.pop();
        %fibStack.push(f1)
        %fibStack.push(fn)
    end
    
    
    fn = fibArray(mod(n,3)+1);
    
end