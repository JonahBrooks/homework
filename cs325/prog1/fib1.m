function [fn] = fib1(n)
% fib1 implements the first (recursive) algorith from the book
    
    fn = -1;
    
    if n == 0
        fn = 0;
    end
    if n == 1
        fn = 1;
    end
    if n > 1
        fn = fib1(n-1) + fib1(n-2);
    end
    
    

