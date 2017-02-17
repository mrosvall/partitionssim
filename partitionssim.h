#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <random>
#include <algorithm>
#include <unordered_map>
#include <map>
#include <set>
using namespace std;
#ifdef _OPENMP
#include <omp.h>
#include <stdio.h>
#else
  #define omp_get_thread_num() 0
  #define omp_get_max_threads() 1
#endif


// ofstream with higher precision to avoid truncation errors
struct my_ofstream : ofstream {
  explicit my_ofstream(streamsize prec = 15)
  {
    this->precision(prec);
  }
};

unsigned stou(char *s);

struct pairhash {
public:
  template <typename T, typename U>
  size_t operator()(const pair<T, U> &x) const
  {
    return x.first*31 + x.second;
  }
};

template <class T>
inline std::string to_string (const T& t){
  std::stringstream ss;
  ss << t;
  return ss.str();
}

void find_and_replace(string &str,string searchString,string replaceString){

  string::size_type pos = 0;
  while ( (pos = str.find(searchString, pos)) != string::npos ) {
    str.replace( pos, searchString.size(), replaceString );
    pos++;
  }
  
}

vector<string> tokenize(const string& str,string& delimiters)
{

  vector<string> tokens;

  // skip delimiters at beginning.
  string::size_type lastPos = str.find_first_not_of(delimiters, 0);

  // find first "non-delimiter".
  string::size_type pos = str.find_first_of(delimiters, lastPos);

  while (string::npos != pos || string::npos != lastPos){

    // found a token, add it to the vector.
    tokens.push_back(str.substr(lastPos, pos - lastPos));

    // skip delimiters.  Note the "not_of"
    lastPos = str.find_first_not_of(delimiters, pos);

    // find next "non-delimiter"
    pos = str.find_first_of(delimiters, lastPos);

  }

  return tokens;
}

void readPartitionsFile(vector<vector<int > > &partitions,string partitionsFileName,int &Nnodes,int &Npartitions,int Nskiplines){

  cout << "Reading partitions file " << flush;  

  ifstream partitionsFile;
  partitionsFile.open(partitionsFileName.c_str());
  if(!partitionsFile.is_open()){
    cout << "Could not open " << partitionsFileName << ", no such file." << endl;
    exit(-1);
  }

  string line;  
  string buf;
  int lineNr = 0;

  // Count number of nodes and boot partitions
  while(getline(partitionsFile,line)){
    lineNr++;
    if(lineNr > Nskiplines)
      break;
  }
  Nnodes++; // First line corresponds to first node

  istringstream read(line);
  while(read >> buf)
      Npartitions++;

  cout << "with " << Npartitions << " partitions " << flush;

  // Count remaining nodes
  while(getline(partitionsFile,line))
    Nnodes++;
  if(partitionsFile.bad())
    cout << "Error while reading file" << endl;

  cout << "of " << Nnodes << " nodes..." << flush;

  partitions = vector<vector<int> >(Npartitions,vector<int>(Nnodes));

  // Restart from beginning of file
  partitionsFile.clear();
  partitionsFile.seekg(0, ios::beg);

  // Read partitions data    
  int nodeNr = 0;
  lineNr = 0;
  while(getline(partitionsFile,line)){
    lineNr++;
    if(lineNr > Nskiplines){
      istringstream read(line);
      int i = 0;
      while(read >> buf){
        partitions[i][nodeNr] = atoi(buf.c_str()); 
        i++;
      }
      nodeNr++;
    }
  }
  partitionsFile.close();
  cout << "done!" << endl;

}

void calcWJaccard(vector<vector<int > > &partitions,string outFileName,int &Nnodes,int &Npartitions){

  vector<unordered_map<int,int> > clusterSizes(Npartitions);
  for(int i=0;i<Npartitions;i++){
    for(int k=0;k<Nnodes;k++){
      clusterSizes[i][partitions[i][k]]++;
    }
  }
 // 0.528042328042328
  cout << "Calculating Jaccard similarities using " << omp_get_max_threads() << " core(s)..." << flush; 

  vector<stringstream> outputVec(omp_get_max_threads());

  #pragma omp parallel for schedule(dynamic,1)
  for(int i=0;i<Npartitions;i++){
    
    for(int j=i+1;j<Npartitions;j++){
      unordered_map<pair<int,int>,int,pairhash> jointM;
      for(int k=0;k<Nnodes;k++){
        jointM[make_pair(partitions[i][k],partitions[j][k])]++;
      }
      double sim = 0.0;
      for(unordered_map<pair<int,int>,int,pairhash>::iterator it = jointM.begin(); it != jointM.end(); it++){
        int Ncommon = it->second;
        int Ntotal = clusterSizes[i][it->first.first] + clusterSizes[j][it->first.second] - Ncommon;
        sim += 1.0*Ncommon*Ncommon/(Nnodes*Ntotal);
      }

      outputVec[omp_get_thread_num()] << i+1 << " " << j+1 << " " << sim << endl;
      
    }
  }

  my_ofstream outFile;
  outFile.open(outFileName.c_str());
  for(int i=0;i<omp_get_max_threads();i++)
    outFile << outputVec[i].rdbuf();
  
  cout << "done!" << endl;

}
