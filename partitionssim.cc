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

  cout << "Version: February 17, 2017." << endl;
  cout << "Command: ";
  cout << argv[0];
  for(int i=1;i<argc; i++)
    cout << " " << argv[i];
  cout << endl;

  // Parse command input
  const string CALL_SYNTAX = "Call: ./partitionssim partitionsfile outfile [--skiplines N] [-h]\n";
  if( argc == 1 ){
    cout << CALL_SYNTAX;
    exit(-1);
  }

  int argNr = 1;
  string partitionsFileName = "noname";
  string outFileName = "noname";
  int Nskiplines = 0;

  while(argNr < argc){
    if(to_string(argv[argNr]) == "-h"){
      cout << CALL_SYNTAX;
      cout << "partitionmatrix: Each column corresponds to a partition and each row corresponds to a node id." << endl;
      cout << "outfile: partitionID1 partitionID2 similarity" << endl;
      cout << "--skiplines N: Skip N lines in partitionmatrix before reading data in" << endl;
      cout << "-h: This help" << endl;
      exit(-1);
    }
    else if(to_string(argv[argNr]) == "--skiplines"){
      argNr++;
      Nskiplines = atoi(argv[argNr]);
      argNr++;
    }
    else{

      if(argv[argNr][0] == '-'){
        cout << "Unknown command: " << to_string(argv[argNr]) << endl;
        cout << CALL_SYNTAX;
        exit(-1);
      }

      if(partitionsFileName == "noname")
        partitionsFileName = string(argv[argNr]);
      else
        outFileName = string(argv[argNr]);

      argNr++;

    }
  }

  if(partitionsFileName != "noname" && outFileName != "noname"){
    cout << "Will read data from " << partitionsFileName;
    if(Nskiplines > 0)
      cout << ", skipping " << Nskiplines << " lines";
    cout << ", and write results to " << outFileName << endl;
  }
  else{
    if(partitionsFileName == "noname")
      cout << "Missing partitionmatrix" << endl;
    if(outFileName == "noname")
      cout << "Missing outfile" << endl;
    cout << CALL_SYNTAX;
    exit(-1);
  }


  vector<vector<int > > partitions;
  int Nnodes = 0;
  int Npartitions = 0;

  readPartitionsFile(partitions,partitionsFileName,Nnodes,Npartitions,Nskiplines);
  calcWJaccard(partitions,outFileName,Nnodes,Npartitions);
  
}
  