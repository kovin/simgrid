[A,delimiterOut,numOfHeaders] = importdata('/home/kevin/exactas/simgrid/results/scalability/data_for_memory.txt');
nodes = A.data(:, 1);
cMemory = A.data(:, 2);
cppMemory = A.data(:, 3);
javaMemory = A.data(:, 4);
n=2;

pCMemory = polyfit(nodes, cMemory, n);
fCMemory = polyval(pCMemory, nodes);

pCppMemory = polyfit(nodes, cppMemory, n);
fCppMemory = polyval(pCppMemory, nodes);

pJavaMemory = polyfit(nodes, javaMemory, n);
fJavaMemory = polyval(pJavaMemory, nodes);

plot(nodes,[cMemory, fCMemory, cppMemory, fCppMemory, javaMemory, fJavaMemory]);
xlabel(A.colheaders(1,1));
%legend(A.colheaders(2:3));
legend(['C memory';'C fit with ^2';'Cpp memory';'Cpp fit with ^2';'Java memory';'Java fit with ^2']);
