#include "partitionssim.h"

using namespace std;
using std::cout;
using std::cin;
using std::endl;

unsigned stou(char *s){
  return strtoul(s,(char **)NULL,10);
}


// Call: trade <seed> <Ntries>
int main(int argc,char *argv[]){

  if( argc != 3){
    cout << "Call: ./partitionssim partitionsfile outfile" << endl;
    exit(-1);
  }
   
  string line;
  string buf;
  istringstream ss;

  // Load rank data
  ifstream partitionsFile(argv[1]);
  ofstream outFile(argv[2]);
  vector<vector<int > > partitions;
  int Nnodes = 0;
  int Npartitions = 0;

  readPartitionsFile(partitions,partitionsFile,Nnodes,Npartitions);
  calcWJaccard(partitions,outFile,Nnodes,Npartitions);
  
}
  