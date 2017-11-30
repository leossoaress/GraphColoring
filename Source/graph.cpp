#include "graph.hpp"
#include "readmatrix.hpp"

Graph::Graph( std::string filename )
{
  adjacent_matrix = ReadFile( filename, &vertex_number);
  vertex_colors = new int[vertex_number];
  vertices = new Vertex[vertex_number];

  for (int i = 0; i < vertex_number; ++i )
  {
    vertex_colors[i] = -1;
    vertices[i].color = -1;
    vertices[i].saturation = 0;
    vertices[i].id = i;
    vertices[i].degree = 0;
    vertices[i].colored = false;
  }
  
  for( int i = 0; i < vertex_number; ++i )
    for( int j = 0; j < vertex_number; ++j )
      if( adjacent_matrix[i][j] != 0 )
        vertices[i].degree++;
}

bool Graph::VertexLeft()
{
  for( int i = 0; i < vertex_number; ++i )
    if( vertices[i].colored == false )
      return true;
  
  return false;
}

void Graph::OrderVertices( char op )
{
  switch(op)
  {
    case 's':
    {
      int max;

      for( int i = 0; i < vertex_number; ++i )
      {
        max = i;
        for( int j = i + 1; j < vertex_number; ++j )
        {
          if( vertices[j].saturation > vertices[max].saturation )
            max = j;
          else if( vertices[j].saturation == vertices[max].saturation && vertices[j].degree > vertices[max].degree )
            max = j;
        }
        std::swap( vertices[i], vertices[max] );
      }
      break;
    }

    case 'd':
    {
      int max;

      for( int i = 0; i < vertex_number; ++i )
      {
        max = i;

        for( int j = 0; j < vertex_number; ++j )
          if( vertices[j].degree > vertices[max].degree )
            max = j;  
      
        std::swap( vertices[i], vertices[max] );
      }
      break;
    }
  }
}

void Graph::SumAdjacentSaturation( const int id )
{
  for( int i = 0; i < vertex_number; ++i )
    if( adjacent_matrix[id][i] != 0 )
      for( int j = 0; j < vertex_number; ++j )
        if( !vertices[j].colored && vertices[j].id == i)
          vertices[j].saturation++;
}

void Graph::DSATUR()
{
  OrderVertices( 'd' );
  vertices[0].colored = true;
  vertices[0].saturation = INT32_MIN;

  SumAdjacentSaturation( vertices[0].id );

  vertices[0].color = 0;

  OrderVertices( 's' );

  while( VertexLeft() )
  {
    for( int i = 0; i < vertex_number; ++i )
    {
      if( !vertices[i].colored )
      {
        vertices[i].colored = true;
        vertices[i].saturation = INT32_MIN;
        SumAdjacentSaturation( vertices[i].id );
        int proposed_color = 0;
        while( SearchColorAdjacentDSATUR( vertices[i].id, proposed_color ) )
          proposed_color++;
        vertices[i].color = proposed_color;
      }
      break;
    }
    OrderVertices( 's' );
  }

  PrintGraphColoring();
  std::cout << CountColors( vertices ) << std::endl;
}

int Graph::CountColors( Vertex* vertices )
{
  std::vector<int> aux;
  int colors = 0;

  for( int i = 0; i < vertex_number; ++i )
  {
    if( !( std::find(aux.begin(), aux.end(), vertices[i].color ) != aux.end() ) )
    {
      aux.push_back( vertices[i].color );
      colors++;
    }
  }
  return colors;
}

int Graph::CountColorsGrasp( std::vector<Vertex> &vertices )
{
  std::vector<int> aux;
  int colors = 0;

  for( int i = 0; i < vertex_number; ++i )
  {
    if( !( std::find(aux.begin(), aux.end(), vertices[i].color ) != aux.end() ) )
    {
      aux.push_back( vertices[i].color );
      colors++;
    }
  }
  return colors;
}

void Graph::PrintGraphColoring()
{
  for( int i = 0; i < vertex_number; ++i )
    std::cout << "V" << vertices[i].id+1 << " C" << vertices[i].color << std::endl;
}

void Graph::PrintGraphColoring( std::vector<Vertex>& vector )
{
  for( int i = 0; i < vertex_number; ++i )
    std::cout << "V" << vector[i].id+1 << " C" << vector[i].color << std::endl;
}

bool Graph::SearchColorAdjacent(const int id,const int color)
{
  for( int j = 0; j < vertex_number; ++j )
  {
    if( adjacent_matrix[id][j] != 0 )
    {
      if( vertices[j].color == color )
        return true;
    }
  }
  return false;
}

Vertex* Graph::HeuristicConstructor()
{
  for( int i = 0; i < vertex_number; ++i )
  {
    int proposed_color = 0;
    
    while( SearchColorAdjacent( i, proposed_color ) )
      proposed_color++;
    
    vertices[i].color = proposed_color;
  }

  //std::cout << CountColors( vertices ) << std::endl;
  //PrintGraphColoring();
  return vertices;
}

void Graph::HeuristicConstructor( Vertex* vector )
{
  for( int i = 0; i < vertex_number; ++i )
  {
    int proposed_color = 0;
    
    while( SearchColorAdjacent( i, proposed_color ) )
      proposed_color++;
    
    vector[i].color = proposed_color;
  }
  
}

bool Graph::SearchColorAdjacentDSATUR( const int id, const int color )
{
  for( int j = 0; j < vertex_number; ++j )
    if( adjacent_matrix[id][j] != 0 )
      for( int i = 0; i < vertex_number; ++i )
        if(vertices[i].id == j && vertices[i].color == color)
            return true;

  return false;
}

void Graph::VND()
{
  Vertex* basesolution = HeuristicConstructor();
  int color_count = CountColors( basesolution );

  std::cout << color_count << std::endl;

  std::vector<int> actualcolors;

  for( int i = 0; i < vertex_number; ++i )
    if( !( std::find( actualcolors.begin(), actualcolors.end(), basesolution[i].color ) != actualcolors.end() ) )
      actualcolors.push_back( basesolution[i].color );
    
  for( int i = 0; i < color_count; ++i )
  {
    for( int j = 0; j < vertex_number; ++j )
    {
      if( basesolution[j].color == i )
      {
        for( int k = 0; k < color_count; ++k )
        {
          if( k != i && ( ( std::find( actualcolors.begin(), actualcolors.end(), k ) != actualcolors.end() ) ) )
          {
            if( !SearchColorAdjacentDSATUR( basesolution[j].id, k ) )
            {
              basesolution[j].color = k;
              break;
            }
          }
        }
      }
    }

    for( int i = 0 ; i < vertex_number; ++i )
    {
      if( !( std::find( actualcolors.begin(), actualcolors.end(), basesolution[i].color ) != actualcolors.end() ) )
      {
        actualcolors.push_back( basesolution[i].color );
      }
    }
  }

  color_count = CountColors( basesolution );
  PrintGraphColoring();
  std::cout << color_count << std::endl;
  
}

void Graph::OrderVerticesGrasp( char op, std::vector<Vertex> &vector )
{
  switch(op)
  {
      case 's':
      {
        int a;
        Vertex temp;
        for(unsigned int i=0;i<vector.size();i++)
        {
          a=i;
          for(unsigned int j=i+1;j<vector.size();j++)
          {
            if(vector[j].saturation > vector[a].saturation)
              a=j;
            else if(vector[j].saturation == vector[a].saturation)
            {
              if(vector[j].degree > vector[a].degree)
                a=j;
            }
          }
          temp=vector[i];
          vector[i]=vector[a];
          vector[a]=temp;
        }
        break;
      }
      case 'd':
      {
        int a;
        Vertex temp;
        for(unsigned int i=0;i<vector.size();i++)
        {
          a=i;
          for(unsigned int j=i+1;j<vector.size();j++)
          {
            if(vector[j].degree > vector[a].degree)
              a=j;
          }
          temp=vector[i];
          vector[i]=vector[a];
          vector[a]=temp;
        }
        break;
      }
    }
}

void Graph::SumAdjacentSaturationGrasp( const int id, std::vector<Vertex> &vector )
{
  for( int i = 0; i < vertex_number; ++i )
    if( adjacent_matrix[id][i] != 0 )
      for( unsigned int j = 0; j < vector.size(); ++j )
        if( !vector[j].colored && vector[j].id == i)
          vector[j].saturation++;
}

bool Graph::SearchColorAdjacentGrasp( const int id, const int color, std::vector<Vertex> &vector )
{

  for( int j = 0; j < vertex_number; ++j )
  {
    if( adjacent_matrix[id][j] != 0 )
    {
      if(vector[j].color == color )
        return true;
    }
  }
  return false;

  /*
  for( int j = 0; j < vertex_number; ++j )
  {
    if( adjacent_matrix[id][j] != 0 )
    {
      for( unsigned int i = 0; vector.size(); ++i )
        if( vector[i].id == j && vector[i].color == color )
          return true;
    }
  }
  return false;*/
}

void Graph::Grasp()
{
  Vertex* basesolution = new Vertex[vertex_number];
  basesolution = vertices;  

  std::vector<Vertex> basesolution_;
  
  HeuristicConstructor( basesolution );

  for( int i = 0; i < vertex_number; ++i )
    basesolution_.push_back( basesolution[i] );

  int solution = CountColorsGrasp( basesolution_ );

  for( int p = 0; p < 50; p++ )
  {
    
    std::vector<int> actualcolors;

    for( int l = 0; l < vertex_number; ++l )
      if( !( std::find( actualcolors.begin(), actualcolors.end(), basesolution[l].color ) != actualcolors.end() ) )
        actualcolors.push_back( basesolution_[l].color );
      
    int i = rand()%(solution-0 + 1) + 0;

    if( !(std::find( actualcolors.begin(), actualcolors.end(), i ) != actualcolors.end()) )
      continue;

    for( int j = 0; j < vertex_number; ++j )
    {
      if( basesolution_[j].color == i )
      {
        for( int k = 0; k < solution; ++k )
        {
          if( k != i && ( std::find( actualcolors.begin(), actualcolors.end(), k ) != actualcolors.end() ) )
          {
            if( !SearchColorAdjacentGrasp( basesolution_[j].id, k, basesolution_ ) )
            {
              basesolution_[j].color = k;
              break;
            }
          }
        }
      }
    }

    for( int l = 0 ; l < vertex_number; ++l )
    {
      if( !( std::find( actualcolors.begin(), actualcolors.end(), basesolution_[l].color ) != actualcolors.end() ) )
      {
        actualcolors.push_back( basesolution_[l].color );
      }
    }
    

    int color_count = CountColorsGrasp( basesolution_ );

    if( color_count < solution )
      solution = color_count;

    std::cout << "Numero de cores: " << solution << std::endl;
  }
}
