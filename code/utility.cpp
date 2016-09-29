// some functionality is placed here to avoid code clutter

#include <iostream>
#include <fstream>

#include <vector>
#include <string>

#include <algorithm>

using namespace std;

const int  ARRAYSIZE    = 8192;
const bool VERBOSE     = false;
const bool INTERACTIVE = false;

////////////////////////////////////////////////////////////////////////////////
// compatability load
////////////////////////////////////////////////////////////////////////////////
typedef vector <bool> adjMatrixL;
typedef vector <adjMatrixL> adjMatrix;
typedef vector <string> namesL;

bool load(string filename, namesL & n, adjMatrix & out)
{
 char buff = 'O';
 string * ts;
   
 ifstream infile, nfile;

 out.clear();

 infile.open(filename.c_str());
 if (infile.fail() && VERBOSE)
 {
  cout << "Could not open file " << filename << endl;
  return false;
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

  out.push_back(*line);
 }

 out.pop_back();

 if (out.size() != out[0].size() && VERBOSE)
 {
  cout << "ERROR:    INPUT IS NOT SQUARE\n";
  cout << "COULD YIELD UNEXPECTED RESULTS\n";
  cout << out.size() << " rows and " << out[0].size() << " cols\n";
 } 

 infile.close();

 nfile.open((filename + "Names").c_str() );
 if (nfile.fail())
 {
  if (VERBOSE) cout << "Could not open " << (filename + "Names").c_str()
                    << ". Using \"A B C D ...\" " << endl;
  char cc[1];
  
  for (int y=0; y < out.size(); y++)
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
 
 return true; //success
}

////////////////////////////////////////////////////////////////////////////////
// load ascii adjacency matrix 
////////////////////////////////////////////////////////////////////////////////
template <class N, class T>
bool loadA_matrix(string filename, vector <N> & n, vector < vector <T> > & out)
{
 char buff = 'O';
 string * ts;
   
 ifstream infile, nfile;

 out.clear();

 infile.open(filename.c_str());
 if (infile.fail() && VERBOSE)
 {
  cout << "Could not open file " << filename << endl;
  return false;
 }

 while (!infile.eof())
 {
  vector <T> * line = new vector <T>;

  infile.get(buff);
  while (buff != '\n')
  {
   if       (buff == '0') (*line).push_back(T(0));
   else if  (buff == '1') (*line).push_back(T(1));
   infile.get(buff);
  }

  out.push_back(*line);
 }

 out.pop_back();

 if (data.size() != data[0].size() && VERBOSE)
 {
  cout << "ERROR:    INPUT IS NOT SQUARE\n";
  cout << "COULD YIELD UNEXPECTED RESULTS\n";
  cout << data.size() << " rows and " << data[0].size() << " cols\n";
 } 

 infile.close();

 nfile.open((filename + "Names").c_str() );
 if (nfile.fail())
 {
  if (VERBOSE) cout << "Could not open " << (filename + "Names").c_str()
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
 
 return true; //success
}

////////////////////////////////////////////////////////////////////////////////
// load binary adjacency matrix 
////////////////////////////////////////////////////////////////////////////////
template <class N, class T>
void loadB_matrix(string filename, vector <N> & n, T & out)
{
}

////////////////////////////////////////////////////////////////////////////////
// load ascii adjacency list 
////////////////////////////////////////////////////////////////////////////////

template <class N, class T>
void loadA_list(string filename, vector <N> & n, T & out)
{
}

////////////////////////////////////////////////////////////////////////////////
// load a binary adjacency list 
////////////////////////////////////////////////////////////////////////////////

template <class N, class T>
void loadB_list(string filename, vector <N> & n, T & out)
{
}

////////////////////////////////////////////////////////////////////////////////
// generic print routines
////////////////////////////////////////////////////////////////////////////////

template <class T> void print(vector < vector <T> > in) 
{ for (int y = 0; y < in.size(); y++)
 {
   print(in[y]);
   cout << endl; 
 }
}

template <class T> void print(vector <T> in) 
{
 for (int y = 0; y < in.size(); y++) { cout << in[y] << "\t"; }
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

////////////////////////////////////////////////////////////////////////////////
// overloaded print routines
////////////////////////////////////////////////////////////////////////////////
struct n2 { string name; double val;};
inline bool operator<(const n2 & x, const n2 & y) {return x.val > y.val;} //hack
typedef vector <n2>     workL;

void print(workL in) 
{
 for (int y = 0; y < in.size(); y++)
 { cout << in[y].name << endl; }
}

////////////////////////////////////////////////////////////////////////////////
// converters
////////////////////////////////////////////////////////////////////////////////
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
{ for (int y = 0; y < in.size(); y++) out[y] = in[y]; }

////////////////////////////////////////////////////////////////////////////////
// data management -- copy and initialization (zeroing)
////////////////////////////////////////////////////////////////////////////////

template <class T> zero_array1 (T a[ARRAYSIZE], int size)
{  for (int i = 0; i < size; i++) a[i] = T(0); }

template <class T> zero_array2 (T a[ARRAYSIZE][ARRAYSIZE], int size)
{for (int i = 0; i < size; i++) for (int j = 0; j < size; j++) a[i][j] = T(0); }

template <class T>
void array1_cp_array1 (T in[ARRAYSIZE], T out[ARRAYSIZE], int n)
{ for (int y = 0; y < n; y++) out[y] = in[y];}

////////////////////////////////////////////////////////////////////////////////
// mathematical operations
////////////////////////////////////////////////////////////////////////////////


// normalize using the sum of squares method
//  (sum of the squares should equal 1.0 afterwards)
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
