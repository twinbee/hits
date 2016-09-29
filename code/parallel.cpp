/////////////////////////////////////////////////////////////////////////////
// Author: Matthew Bennett
// Date: 10-30-05
// HITS ALGORITHM  // MESSAGE PASSING IMPLEMENTATION
//
// HITS is a page ranking algorithm originally proposed by Kleinberg [1998]
//  It operates similarly to Google PageRank
// 
//  Each page has two float values associated with it: Authority and Hub
//
//  The hub value of page P is a sum of the authority values P links to
//  The authority value of page P is a sum of the hub values pages linked to P
//
// Over the First iteration, Hub values are initialized to the number of pages
//  linked to. Adjacency values are initialized to the number referencing pages.
// At each succeeding step, hubs and authoroties are normalized w second moments
//
// The input file is an adjacency matrix for the directed graph formed when
//  the pages are represented as vertices and the links are represented as edges
//
// An example graph of six pages is given in the file "simple"
//  Additionally, a file may be used to name the vertices of the graph
//  for easy readability.
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>

#include <vector> //bit_vector class
#include <string>

#include <algorithm>

#include "utility.cpp"

#include "mpi.h"

using namespace std;

extern const bool VERBOSE;
extern const bool INTERACTIVE;
extern const int  ARRAYSIZE;

typedef vector <bool> adjMatrixL;
typedef vector <adjMatrixL> adjMatrix;

typedef vector <string> namesL;

void hits(double g[ARRAYSIZE*ARRAYSIZE], int numVertices,
                 int first, int last,
                 double a[ARRAYSIZE], double H[ARRAYSIZE]);

///////////////////////////////////////////////////////////////////////////////
// Main procedure
///////////////////////////////////////////////////////////////////////////////

int main(int argc, char * argv[])
{
 int     fakeArg1;
 char ** fakeArg2;
 MPI::Init(fakeArg1, fakeArg2);

 int size = MPI::COMM_WORLD.Get_size();
 //size corresponds to the total number of processes
 int rank = MPI::COMM_WORLD.Get_rank();
 //rank is the process number in [0..size-1] for this process
 
 double time, start;

 string filename("huge");
 if (argc >= 1 ) filename = argv[1];

 adjMatrix g;   //the directed link graph

 //the directed link graph, array representation
 bool ga[ARRAYSIZE*ARRAYSIZE];

 int numVertices;      //number of vertices in our graph
 
 double aa[ARRAYSIZE]; //our Authorities
 double ha[ARRAYSIZE]; //our hubs

 namesL n;     //names for the vertexes (page names or URLs)
 
 n2    tt;    //temporary string , value tuple
 workL wt;    //temporary worlist for sorting results
 
 int iterations = 100;
 int cur_iter   = 0;

if (rank == 0)
{
 char choice = 'n';
 if (INTERACTIVE)
 while(choice != 'y')
 { 
  cout << "\n\nEnter the filename containing an adjacency matrix [simple] " << flush;
  cin >> filename;
 
  load(filename, n, g);

  print(g);
  cout << "Is this the file you want? (y/n)" << flush;
  cin >> choice;

  cout << "How many iterations? ";
  cin >> iterations;
 } 

  load(filename, n, g);

  vec2_2_array1( g, ga);  //convert our loaded vector into an array
  numVertices = g.size(); //and save its size

  g.clear(); //we don't need it anymore. clear stack space
 
 if (VERBOSE)
 {
  cout << "HITS ALGORITHM: PARALLEL "
   << size << " proccessors"
   << " on " << numVertices  << " vertices  " << flush;
 }
 else
 {
  cout << "{" << size << ", " << flush;
 }
 
 }

 if (size > 1) // then we are in the parallel version
 { //TODO: only send relevant rows and columns
  MPI::COMM_WORLD.Bcast(&numVertices, 1, MPI::INT, 0);
  MPI::COMM_WORLD.Bcast(&ga, numVertices*numVertices, MPI::DOUBLE, 0);
 }

  float rmdr = (numVertices)/float(size) - int((numVertices)/(size));//for round
  int step= int((numVertices)/(size));
  
  int first, last;

  int lastBigProcess;
  for (lastBigProcess = 0; lastBigProcess < size - 1; lastBigProcess++)
  {
   if ( (numVertices - (lastBigProcess-1)*step)/float(numVertices) <= rmdr )
    break;
  } 

  if (rank > lastBigProcess)
  {
   first = lastBigProcess*(step+1) + (rank-lastBigProcess)*step;
   last  = lastBigProcess*(step+1) + (rank-lastBigProcess+1)*step-1;   
  }
  else //rank <= lastBigProcess
  {
   first = (rank)  *(step+1);
   last  = (rank+1)*(step+1)-1;
  }  
  
  //cout << endl << rank << " gets " << first << " to " << last << endl;
  
  if (rank == size) last = numVertices-1;

///////////////////////////////////////////////////////////////////////////////
// BEGINNING OF THE ALGORITHM IMPLEMENTATION
///////////////////////////////////////////////////////////////////////////////

 start = MPI::Wtime(); //check and store the clock for timing purposes 

 while(cur_iter++ < iterations)
 {
  hits(ga, numVertices, first, last, aa, ha);

 if (size > 1) // then we are in the parallel version
 { 
  MPI::COMM_WORLD.Gather( &aa[first], step, MPI::DOUBLE, 
                          &aa[0],     step, MPI::DOUBLE, 0);

  MPI::COMM_WORLD.Gather( &ha[first], step, MPI::DOUBLE, 
                          &ha[0],     step, MPI::DOUBLE, 0);
 }

 if (rank == 0 && cur_iter > 1)
 {
   normalize(aa, numVertices);
   normalize(ha, numVertices);
 } //end output block

 if (size > 1)
 { 
  MPI::COMM_WORLD.Bcast(&aa, numVertices, MPI::DOUBLE, 0);
  MPI::COMM_WORLD.Bcast(&ha, numVertices, MPI::DOUBLE, 0);
 }
} 

 if (rank == 0)
 {

  wt.clear();
  for (int x = 0; x < numVertices; x++)
  {
   tt.name = n[x];
   tt.val = aa[x];
   wt.push_back(tt);
  }
  sort(wt.begin(), wt.end() );

 if (VERBOSE)
 {
  cout << "\n<----Iteration " << cur_iter << endl;
  print(wt);

  cout << "\nAuthorities: \t";    print(aa, numVertices);
  cout << "\nHubs: \t\t";         print(ha, numVertices);
  cout << endl;
 }

 time = MPI::Wtime() - start; //calculuate running time

 if (VERBOSE)
 {
  cout << "WallTime to execute: " << time << " s.\n"; 
 }
 else
 {
  cout << time << "}, ";        
 }
 
 }

 MPI::Finalize();
 return 0;
}

//////////////////////////////////////////////////////////////////////////////
// Modified implementation of the HITS algorithm for message passing
//////////////////////////////////////////////////////////////////////////////

void hits(bool in[ARRAYSIZE*ARRAYSIZE], int numVertices,
          int first, int last,
          double a[ARRAYSIZE], double h[ARRAYSIZE])
{
 double summator  = 0;

 double abak[ARRAYSIZE];
 double hbak[ARRAYSIZE];

 array1_cp_array1(h, hbak, numVertices);
 array1_cp_array1(a, abak, numVertices);
 
 //if this is the first run, initialize the hubs and authorities
 static bool init = false;
 if (!init)
 {
  for (int y = 0; y < numVertices; y++)
  { 
   summator = 0;
   for (int x = 0; x < numVertices; x++)
   {
    summator += in[y*numVertices + x];
   }
   h[y] = summator;
  }

  for (int y = 0; y < numVertices; y++)
  { 
   summator = 0;
   for (int x = 0; x < numVertices; x++)
   { 
    summator += in[x*numVertices + y];
   }
   a[y] = summator;
  }

  init = true;
  return;
 }

  // parallel-dependant set

  //Calculate all authorities
  for (int y = first; y <= last; y++)
  { 
   summator = 0.0;
   
   for (int x = 0; x < numVertices; x++)
   {
    summator += in[x*numVertices + y] * hbak[x];
   }
   a[y] = summator;
  }

  //calculate all hubs
  for (int y = first; y <= last; y++)
  { 
   summator = 0.0;

   for (int x = 0; x < numVertices; x++)
   {
    summator += in[y*numVertices + x] * abak[x];
   }
   h[y] = summator;
  }
}
