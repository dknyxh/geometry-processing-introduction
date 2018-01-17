#include "edges.h"
#include "euler_characteristic.h"
#include <igl/read_triangle_mesh.h>
#include <igl/viewer/Viewer.h>
#include <igl/viewer/Viewer.h>
#include <igl/edge_topology.h>
#include <igl/jet.h>
int main(int argc, char *argv[])
{
  Eigen::MatrixXd V;
  Eigen::MatrixXi F;
  // Load in a mesh
  igl::read_triangle_mesh(argc>1 ? argv[1] : "../shared/data/bunny.off", V, F);

  Eigen::MatrixXi E = edges(F);
  int Chi = euler_characteristic(F);
  std::cout<<"Edge list E is "<<E.rows()<<"x"<<E.cols()<<std::endl;
  std::cout<<"Euler Characteristic: "<<Chi<<std::endl;
  std::cout<<"V"<<V <<std::endl;
  //std::cout<<"edge_flaps"<<igl::edge_flaps();
  std::cout<<"F"<<F <<std::endl;
  Eigen::MatrixXi o1,o2,o3;
  igl::edge_topology(V, F, o1, o2, o3);
  std::cout<<"edge_topology o1"<<o1 << std::endl;
  std::cout<<"edge_topology o2"<<o2 << std::endl;
  std::cout<<"edge_topology o3"<<o3 << std::endl;
  // Create a libigl Viewer object 
  igl::viewer::Viewer viewer;
  // Set the vertices and faces for the viewer
  viewer.data.set_mesh(V, F);
  
Eigen::MatrixXd C;
  Eigen::VectorXd Z = V.col(2);
  igl::jet(Z,true,C);

  viewer.data.set_colors(C);





  // Launch a viewer instance



  viewer.launch();
  return 0;
}

