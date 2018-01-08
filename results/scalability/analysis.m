[A,delimiterOut,numOfHeaders] = importdata('/home/kevin/exactas/simgrid/results/scalability/data_for_analysis.txt');
nodes = A.data(:, 1);
cRSS = A.data(:, 2);
javaRSS = A.data(:, 3);
n=2;


pCRSS = polyfit(nodes, cRSS,n);
fCRSS = polyval(pCRSS, nodes);

pJavaRSS = polyfit(nodes, javaRSS, n);
fJavaRSS = polyval(pJavaRSS, nodes);

plot(nodes,[cRSS, fCRSS, javaRSS, fJavaRSS]);
xlabel(A.colheaders(1,1));
%legend(A.colheaders(2:3));
legend(['C';'C fit with ^2';'Java';'Java fit with ^2']);
