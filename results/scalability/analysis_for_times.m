[A,delimiterOut,numOfHeaders] = importdata('/home/kevin/exactas/simgrid/results/scalability/data_for_times.txt');
nodes = A.data(:, 1);
cTimes = A.data(:, 2);
cppTimes = A.data(:, 3);
javaTimes = A.data(:, 4);
n=2;

pCTimes = polyfit(nodes, cTimes, n);
fCTimes = polyval(pCTimes, nodes);

pCppTimes = polyfit(nodes, cppTimes, n);
fCppTimes = polyval(pCppTimes, nodes);

pJavaTimes = polyfit(nodes, javaTimes, n);
fJavaTimes = polyval(pJavaTimes, nodes);

plot(nodes,[cTimes, fCTimes, cppTimes, fCppTimes, javaTimes, fJavaTimes]);
xlabel(A.colheaders(1,1));
%legend(A.colheaders(2:3));
legend(['C times';'C fit with ^2';'Cpp times';'Cpp fit with ^2';'Java times';'Java fit with ^2']);
