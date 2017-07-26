clear variables
close all

x = mvnrnd([0 ; 0], [1 .9 ; .9 1], 100);
y = [1 1 ; 1 -1 ; -1 1 ; -1 -1];

size(x)
size(y)

mahalDist = mahal(y, x)

sqEuclidDist = sum((y - repmat(mean(x), 4, 1)).^2, 2)


subplot(2, 1, 1)
plot(x(:,1), x(:,2), 'b.')
hold on
plot(y(:,1), y(:,2), 'ro')


subplot(2, 1, 2)
stem(mahalDist, 'r')
hold on
stem(sqEuclidDist, 'g')