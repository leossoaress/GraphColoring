#include "graph.hpp"

#include <iostream>

int main( int argc, char *argv[] ){

  std::string filename;

  if( argc == 1 )                        
    return 0;
  else
    filename = argv[1];

  Graph graph( filename );

  //graph.HeuristicConstructor();
  
  //graph.VND();
  
  //graph.DSATUR();

  graph.Grasp();
  
  return 0;
}
