// some functionality is placed here to avoid code clutter

#include <iostream>
#include <fstream>

#include <vector>
#include <string>

#include <algorithm>

using namespace std;

const int ARRAYSIZE = 5000;

typedef vector <double>      adjMatrixL;
typedef vector <adjMatrixL>  adjMatrix;

typedef vector <string>      namesL;

struct n2 { string name; double val;};
inline bool operator<(const n2 & x, const n2 & y) {return x.val > y.val;} //hack
typedef vector <n2>         workL;

adjMatrix load(string filename, namesL & n)
{
  adjMatrix data;
  char buff = 'O';
  string * ts;
  
  ifstream infile, nfile;

  infile.open(filename.c_str());
  if (infile.fail())
  {
   cout << "Could not open file. Using \"simple\" " << endl;
   return load("simple", n);
  }

 while (!infile.eof())
 {
  adjMatrixL * line = new adjMatrixL;

  infile.get(buff);
  while (buff != '\n')
  {
   if       (buff == '0') (*line).push_back(0);
   else if  (buff == '1') (*line).push_back(1);
   infile.get(buff);
  }

  data.push_back(*line);
 }

 data.pop_back();

 if (data.size() != data[0].size())
 {
  cout << "ERROR:    INPUT IS NOT SQUARE\n";
  cout << "COULD YIELD UNEXPECTED RESULTS\n";
  cout << data.size() << " rows and " << data[0].size() << " cols\n";
 } 

 infile.close();

 nfile.open((filename + "Names").c_str() );
 if (nfile.fail())
 {
  cout << "Could not open " << (filename + "Names").c_str()
     << ". Using \"A B C D ...\" " << endl;
  char cc[1];
  
  for (int y=0; y < data.size(); y++)
  {
   cc[0] = char( int('A') + y );
   ts = new string(cc);
   n.push_back(*ts);
  }
 } 
 else
 {
  string line;
  
  while (getline(nfile,line,'\n'))
  {
   n.push_back (line);
  }
 }

 nfile.close(); 

 return data;
}

void print(adjMatrix in) 
{
  for (int y = 0; y < in.size(); y++)
  { for (int x = 0; x < in[y].size(); x++)
    { 
     cout << in[y][x];
    }
    cout << endl; 
  }
}

void print(adjMatrixL in) 
{
 for (int y = 0; y < in.size(); y++)
 {
   cout << in[y] << "\t";
 }
}

void print(workL in) 
{
 for (int y = 0; y < in.size(); y++)
 {
   cout << in[y].name << endl;
 }
}

template <class T>
void vec2_2_array1 (const vector < vector <T> > & in, T out[ARRAYSIZE])
{
 for (int y = 0; y < in.size(); y++)
 {
  for (int x = 0; x < in[y].size(); x++)
  {
   int z = ( y*in.size() + x);
   out[z] = in[y][x];
  } 
 }
}

template <class T>
void vec1_2_array1 (const vector <T> & in, T out[ARRAYSIZE])
{
 for (int y = 0; y < in.size(); y++)
 {  out[y] = in[y]; }
}

template <class T>
void print (T in[ARRAYSIZE], int sizex = ARRAYSIZE)
{
 for (int x = 0; x < sizex; x++)
 {   cout << in[x] << "\t";  }
}

template <class T>
void print (T in[ARRAYSIZE], int sizex, int sizey)
{
 for (int y = 0; y < sizey; y++)
 {
  for (int x = 0; x < sizex; x++)
  {
   cout << in[y*sizey + x];
  } 
  cout << endl; 
 }
}

template <class T>
void array1_cp_array1 (T in[ARRAYSIZE], T out[ARRAYSIZE], int n)
{
 for (int y = 0; y < n; y++)
 {
  out[y] = in[y];
 }
}

template <class T> void normalize(T a[ARRAYSIZE], int size)
{
  T sigma_squares(0);
 
  //normalize the authority values as in Kleinberg[98]
  for (int x = 0; x < size; x++)
  {
   sigma_squares += (a[x]) * (a[x]);
  }
  for (int x = 0; x < size; x++)
  {
   a[x] = a[x]*a[x] / sigma_squares;
  }
        
}
