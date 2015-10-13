% Jonah Brooks, MATLAB tutorial

tut1 = [ 1 2 3 4 5; 5 4 3 2 1 ]
tut2 = [ 9 8 7 6 5; 5 6 7 8 9 ]

size (tut1)
size (tut2)

all = 1:1:100
evens = 2:2:1001

sum (all)
all*ones(1,numel(all))'
sum (evens)
evens*ones(1,numel(evens))'

tut1 .* tut2


x = -2*pi:0.1:2*pi
y = sin(x).*cos(x).^2
plot(x,y)

hold on
plot(x,cos(x),'--')
hold off