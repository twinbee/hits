/////////////////////////////////////////////////////////////////////////////
// Author: Matthew Bennett
// Date: 10-30-05
// HITS ALGORITHM  // SERIAL IMPLEMENTATION
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

#include <vector>
#include <string>

#include <algorithm>

#include "utility.cpp"

#include "mpi.h"

using namespace std;

void hits(const adjMatrix & g, adjMatrixL & a, adjMatrixL & h);

int main(int argc, char *argv [])
{
 MPI::Init(argc, argv);

 int size = MPI::COMM_WORLD.Get_size();
 //size corresponds to the total number of processes
 int rank = MPI::COMM_WORLD.Get_rank();
 //rank is the process number in [0..size-1] for this process
 
 if (size > 1) cout << "\nSERIAL VERSION should be run only with 1 processor\n";
 cout << "HITS ALGORITHM: SERIAL\n" << flush;
 
 double time, start;

 string filename("huge");

 adjMatrix g;  //the directed link graph
 
 adjMatrixL a; //our Authorities
 adjMatrixL h; //our hubs

 namesL n;     //names for the vertexes (page names or URLs)
 
 n2    tt;    //temporary string , value tuple
 workL wt;    //temporary worlist for sorting results
 
 int iterations = 10;
 int cur_iter   = 0;

/*
 char choice = 'n';
 while(choice != 'y')
 { 
  cout << "\n\nEnter the filename containing an adjacency matrix [simple] ";
  cin >> filename;
 g = load(filename, n);
  print(g);
  cout << "Is this the file you want? (y/n)";
  cin.get(choice); cin.get(choice);
 }

  cout << "How many iterations? ";
  cin >> iterations;
*/

 g = load(filename, n);

 start = MPI::Wtime(); //check and store the clock for timing purposes 

 while(cur_iter++ < iterations)
 {
  hits(g, a, h);
 } 

/*
 //print final results
 {
  wt.clear();
  for (int x = 0; x < a.size(); x++)
  {
   tt.name = n[x];
   tt.val = a[x];
   wt.push_back(tt);
  }
  sort(wt.begin(), wt.end() );

  cout << "\n<----Iteration " << cur_iter << endl;
  print(wt);
 }
*/

 time = MPI::Wtime() - start; //calculuate running time
 cout << "WallTime to execute: " << time << "ms.\n"; 
 
 MPI::Finalize();
 return 0;
}

//////////////////////////////////////////////////////////////////////////////
// serial hits algorithm
//////////////////////////////////////////////////////////////////////////////

void hits(const adjMatrix & in, adjMatrixL & a, adjMatrixL & h)
{
 double summator  = 0;
 double sigma_squares = 0;

 adjMatrixL hbak (h); //copy old hub values
 adjMatrixL abak (a); //copy old authorty values
 
 a.clear(); //clear vector of authority values
 h.clear(); //clear vector of hub values

 //if this is the first run, initialize the hubs and authorities
 static bool init = false;
 if (!init)
 {

  for (int y = 0; y < in.size(); y++)
  { 
   summator = 0;
   for (int x = 0; x < in[y].size(); x++)
   {
    summator      += in[y][x];
   }
   h.push_back(summator);
  }

  for (int y = 0; y < in.size(); y++)
  { 
   summator = 0;
   for (int x = 0; x < in[y].size(); x++)
   { 
    summator += in[x][y];
   }
   a.push_back(summator);
  }

  init = true;
  return;
 }

  //calculate all hubs
  for (int y = 0; y < in.size(); y++)
  { 
   summator = 0.0;

   for (int x = 0; x < in[y].size(); x++)
   { 
    summator += in[y][x] * abak[x];
   }
   h.push_back(summator);
  }

  //normalize the hub values as in Kleinber[98]
  sigma_squares = 0;
  for (int x = 0; x < h.size(); x++)
  {
   sigma_squares += (h[x])*(h[x]);
  }
  for (int x = 0; x < h.size(); x++)
  {
   h[x] = h[x]*h[x] / sigma_squares;
  }


  //Calculate all authorities
  for (int y = 0; y < in.size(); y++)
  { 
   summator = 0.0;
   
   for (int x = 0; x < in[y].size(); x++)
   {
    summator += in[x][y] * hbak[x];
   }
   a.push_back(summator);
  }

  //normalize the authority values as in Kleinberg[98]
  sigma_squares = 0;
  for (int x = 0; x < a.size(); x++)
  {
   sigma_squares += (a[x]) * (a[x]);
  }
  for (int xx = 0; xx < a.size(); xx++)
  {
   a[xx] = a[xx]*a[xx] / sigma_squares;
  }
}


