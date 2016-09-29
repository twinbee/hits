#include <fstream>
#include <string>

#include <vector>

#include <iostream>

using namespace std;

const bool VERBOSE = true;

int main()
{
 ifstream namesin;
 ifstream linksin; 
 
 ofstream namesout;
 ofstream linksout;
 
 namesin.open("./inzhang/url.txt");
 linksin.open("./inzhang/zhang_test"); 

 namesout.open("./in/usmNames");
 linksout.open("./in/usm"); 

 int source, dest;

 int size = 6;

 bit_vector buffer(size*size);
 
 while (!linksin.eof())
 {
  linksin >> source;
  linksin >> source;
  linksin >> dest;  
  
  buffer[source*size + dest] = true;
 }

 for (int i = 0; i < size; i++)
 {
  for (int j = 0; j < size; j++)
  {
   cout << buffer[i*size + j];
  }
  cout << endl;
 }

 cout << "Size in memory: " << sizeof(buffer) << " bytes";

 system("PAUSE");

 namesin.close();
 linksin.close(); 

 namesout.close();
 linksout.close(); 
  
   
}
