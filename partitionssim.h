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

void readPartitionsFile(vector<vector<int > > &partitions,ifstream &partitionsFile,int &Nnodes,int &Npartitions){

  cout << "Reading partitions file " << flush;  

  string line;  
  string buf;

  // Count number of nodes and boot partitions
  getline(partitionsFile,line);
    Nnodes++; // First line corresponds to first node
  istringstream read(line);
  while(read >> buf)
      Npartitions++;

  cout << "with " << Npartitions << " partitions " << flush;

  // Count remaining nodes
  while(getline(partitionsFile,line))
    Nnodes++;
  cout << "of " << Nnodes << " nodes..." << flush;

  partitions = vector<vector<int> >(Npartitions,vector<int>(Nnodes));

  // Restart from beginning of file
  partitionsFile.clear();
  partitionsFile.seekg(0, ios::beg);

  // Read partitions data    
  int nodeNr = 0;
  while(getline(partitionsFile,line)){
    istringstream read(line);
    int i = 0;
    while(read >> buf){
      partitions[i][nodeNr] = atoi(buf.c_str()); 
      i++;
    }
    nodeNr++;
  }
  partitionsFile.close();
  cout << "done!" << endl;

}

void calcWJaccard(vector<vector<int > > &partitions,ofstream &outFile,int &Nnodes,int &Npartitions){

  vector<unordered_map<int,int> > clusterSizes(Npartitions);
  for(int i=0;i<Npartitions;i++){
    for(int k=0;k<Nnodes;k++){
      clusterSizes[i][partitions[i][k]]++;
    }
  }

  cout << "Calculating Jaccard similarities..." << flush; 

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
        sim += 1.0*Ncommon/Nnodes*Ncommon/Ntotal;
      }
      outFile << i+1 << " " << j+1 << " " << sim << endl;
    }
  }
  
  cout << "done!" << endl;

}
