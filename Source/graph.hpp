#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include<fstream>
#include <vector>
#include <algorithm>
#include <string>

struct Vertex{
  int id;
  int saturation;
  int degree;
  int color;
  bool colored;
};

class Graph{

  int vertex_number;
  int* vertex_colors;
  int** adjacent_matrix;
  int CountColors( Vertex* );

  Vertex* vertices;

  bool SearchColorAdjacent( const int,const int );
  bool check_vertices_color(const int,const int,int*);

  void change_vertices_color(const int,const int,int*);
  void PrintGraphColoring();
  
  bool VertexLeft();
  void OrderVertices( char );
  void SumAdjacentSaturation( const int );
  bool SearchColorAdjacentDSATUR( const int, const int );

public:

  Vertex* HeuristicConstructor();
  void VND();
  void DSATUR();

  Graph( std::string );

};

#endif
