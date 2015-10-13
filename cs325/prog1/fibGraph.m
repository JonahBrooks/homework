function [fib1Array, fib2Array, fib3Array, stack1, stack2, stack3] = fibGraph( n )
% fibGraph graphs the time efficiency of various algorithms for
% Fibonacci Numbers.

    stack1=java.util.Stack();
    stack2=java.util.Stack();
    stack3=java.util.Stack();
    fib1Array = 0.0:1.0:n;
    fib2Array = 0.0:1.0:n;
    fib3Array = 0.0:1.0:n;
    for i = 0:n
        tic;
        for j = 0:100
            fib1(i);
        end
        fib1Array(i+1) = toc/100;
        stack1.push(fib1Array(i+1));
        
        tic;
        for j = 0:10000
            fib2(i);
        end
        fib2Array(i+1) = toc/10000;
        stack2.push(fib2Array(i+1));
        
        tic;
        for j = 0:10000
            fib3(i);
        end
        fib3Array(i+1) = toc/10000;
        stack3.push(fib3Array(i+1));
    end
    
    x = 0:1:n;
    
    subplot(2,1,1);
    plot(x,fib2Array,x,fib3Array,x,fib1Array);
    legend('fib2 (Iterative)','fib3 (Stack)', 'fib1 (Recursive)');
    xlabel('Value of n');
    ylabel('Time taken (s)');
    title('Time to calculate the nth Fibonnaci Number (all methods)');

    
    subplot(2,1,2);
    plot(x,fib2Array,x,fib3Array);
    xlabel('Value of n');
    ylabel('Time taken (s)');
    title('Only iterative full array (blue) and modulus (green) methods');

    
    figure();
    subplot(2,1,1);
    semilogy(x,fib2Array,x,fib3Array,x,fib1Array);
    legend('fib2 (Iterative)','fib3 (Stack)', 'fib1 (Recursive)');
    xlabel('Value of n');
    ylabel('Time taken (s)');
    title('Time to calculate the nth Fibonnaci Number (all methods) - Semilog');

    
    subplot(2,1,2);
    semilogy(x,fib2Array,x,fib3Array);
    xlabel('Value of n');
    ylabel('Time taken (s)');
    title('Only iterative full array (blue) and modulus (green) methods - Semilog');


end

