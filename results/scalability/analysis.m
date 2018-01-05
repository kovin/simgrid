[A,delimiterOut,numOfHeaders] = importdata('/home/kevin/exactas/simgrid/results/scalability/data_for_analysis.txt')
nodes = A.data(:, 1)
cRSS = A.data(:, 2)
javaRSS = A.data(:, 3)
plot(nodes,[cRSS, javaRSS])
xlabel(A.colheaders(1,1));
legend(A.colheaders(2:3))