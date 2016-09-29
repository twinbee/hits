#include <iostream>
#include <fstream>

#include <vector>
#include <string>

#include <cstdlib>
#include <ctime>

#include "utility.cpp"

using namespace std;

int main(int argc, char ** argv)
{
 const int size = 80;
 
 srand((unsigned)time(NULL));
 
 ofstream outfile;

 outfile.open("./in/size3k");
 int s = 1024+2048;
 
 float f;
 char c;  

 if (outfile.fail())
 {
  cout << "failure";
  system("PAUSE");
 } 
 
 for (int i = 0; i < s; i++)
 {
  for (int j = 0; j < s; j++)
  {
   f = rand()/float(RAND_MAX);
   if (i < 17 || j < 17) 
   {
    if (f < 0.2) c = '0';
    else c = '1';          
   }   
   else
   {
    if (f < 0.6) c = '0';
    else c = '1'; 
   } 
   if (i==j) c = '0';
   outfile.put(c);
  }
  outfile << "\n";  
 }    
 
 outfile.close();
 
 return 0;
}
