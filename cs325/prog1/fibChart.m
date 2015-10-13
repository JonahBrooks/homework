function [ ] = fibChart( n )
% fibChart charts the first 100 Fibonacci Numbers against their index
% and compares that to the estimated values


    [a, fibArray] = fib2(n);
    
    x = 1:n;
    subplot(2,1,1);
    plot(x,fibArray,x,2.^(0.694*x));
    legend('fn','estimated fn');
    xlabel('Value of n');
    ylabel('nth Fibonacci Number');
    title('Fn vs n and Estimated Fn vs n');
    
    subplot(2,1,2);
    semilogy(x,fibArray,x,2.^(0.694*x));
    legend('fn','estimated fn');
    xlabel('Value of n');
    ylabel('nth Fibonacci Number');
    title('Fn vs n and Estimated Fn vs n - Semilog');
    
end

