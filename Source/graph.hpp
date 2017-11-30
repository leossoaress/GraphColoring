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
  int CountColorsGrasp( std::vector<Vertex> &vertices );

  Vertex* vertices;

  bool SearchColorAdjacent( const int,const int );
  bool check_vertices_color(const int,const int,int*);

  void change_vertices_color(const int,const int,int*);
  void PrintGraphColoring();
  void PrintGraphColoring( std::vector<Vertex>& vector );
  
  bool VertexLeft();
  void OrderVertices( char );
  void SumAdjacentSaturation( const int );
  bool SearchColorAdjacentDSATUR( const int, const int );
  void OrderVerticesGrasp( char op, std::vector<Vertex> &vector );
  void SumAdjacentSaturationGrasp( const int id, std::vector<Vertex> &vector );
  bool SearchColorAdjacentGrasp( const int id, const int color, std::vector<Vertex> &vector );

public:

  Vertex* HeuristicConstructor();
  void HeuristicConstructor( Vertex* vector );
  void VND();
  void DSATUR();
  void Grasp();

  Graph( std::string );

};

#endif
