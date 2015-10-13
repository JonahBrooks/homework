function [ fn, fibArray ] = fib2( n )
% fib2 implements the second (iterative) algorithm from the book
    fn = -1;
   
    fibArray = zeros(1,n); %indexed at 1...
    if n > 0
        fibArray(1) = 1;
        if n > 1
            fibArray(2) = 1;
        end
    end
    
    for i = 3:n
        fibArray(i) = fibArray(i-1) + fibArray(i-2);
    end
    
    
    if n == 0
        fn = 0;
    else
        fn = fibArray(n);
    end
     
end

