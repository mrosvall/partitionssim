Partition similarity

Calculates the weighted Jaccard similarity over all clusters of pairwise partitions


Author:
--------------------------------------------------------
Martin Rosvall

For contact information, see 
http://www.mapequation.org/about


Getting started:
--------------------------------------------------------
In a terminal with the GNU Compiler Collection installed,
just run 'make' in the current directory to compile the
code with the included Makefile.

Call: ./partitionssim partitionsfile outfile  
partitionmatrix: Each column corresponds to a partition and each row corresponds to a node id.   
outfile: partitionID1 partitionID2 similarity


Example:

./partitionssim partitiosnmatrix.txt similarities.txt

partitionsmatrix.txt   
1 1 1 1 1 1 1 1 1 1 1  
1 1 1 1 1 1 1 1 1 1 1  
1 2 1 2 1 2 1 2 1 2 2  
2 2 1 2 1 2 1 2 1 2 2  
2 2 2 2 2 2 2 2 2 2 2  
2 2 2 2 2 2 2 2 2 2 2  
3 2 2 2 2 2 2 2 2 2 2  
3 3 3 3 3 3 3 3 3 3 3  
3 3 3 3 3 3 3 3 3 3 3  

similarities.txt  
1 2 0.528042  
1 3 0.55  
1 4 0.528042  
1 5 0.55  
1 6 0.528042  
1 7 0.55  
1 8 0.528042  
1 9 0.55  
1 10 0.528042  
2 3 0.596825  
2 4 1  
2 5 0.596825  
2 6 1  
2 7 0.596825  
2 8 1  
2 9 0.596825  
2 10 1  
3 4 0.596825  
3 5 1  
3 6 0.596825  
3 7 1  
3 8 0.596825  
3 9 1  
3 10 0.596825  
4 5 0.596825  
4 6 1  
4 7 0.596825  
4 8 1  
4 9 0.596825  
4 10 1  
5 6 0.596825  
5 7 1  
5 8 0.596825  
5 9 1  
5 10 0.596825  
6 7 0.596825  
6 8 1  
6 9 0.596825  
6 10 1  
7 8 0.596825  
7 9 1  
7 10 0.596825  
8 9 0.596825  
8 10 1  
9 10 0.596825.  

