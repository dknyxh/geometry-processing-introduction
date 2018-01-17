#include "edges.h"
#include "euler_characteristic.h"
#include <igl/read_triangle_mesh.h>
#include <igl/viewer/Viewer.h>
#include <igl/viewer/Viewer.h>
#include <igl/edge_topology.h>
#include <igl/jet.h>


#include <igl/per_vertex_normals.h>
#include <igl/per_face_normals.h>
#include <igl/per_corner_normals.h>


#include <igl/decimate.h>
#include <igl/euler_characteristic.h>
#include <cmath>

  Eigen::MatrixXd N_vertices;
Eigen::MatrixXd N_faces;
Eigen::MatrixXd N_corners;

  Eigen::MatrixXd U;
  Eigen::MatrixXi G;
    Eigen::MatrixXd V;
  Eigen::MatrixXi F;
Eigen::VectorXi J;

int faces_num = 10000;
// This function is called every time a keyboard button is pressed
bool key_down(igl::viewer::Viewer& viewer, unsigned char key, int modifier)
{
  switch(key)
  {
    case '1':
      viewer.data.set_normals(N_faces);
      return true;
    case '2':
      viewer.data.set_normals(N_vertices);
      return true;
    case '3':
      viewer.data.set_normals(N_corners);
      return true;

    case '4':
      igl::decimate(V,F,faces_num,U,G,J);
      faces_num = floor(faces_num * 0.9);
      viewer.data.clear();
      viewer.data.set_mesh(U,G);
      std::cout<<"Euler Characteristic: "<<euler_characteristic(G)<<std::endl;
    default: break;
  }
  return false;
}



int main(int argc, char *argv[])
{



  // Load in a mesh
  igl::read_triangle_mesh(argc>1 ? argv[1] : "../data/space_shuttle.off", V, F);

  Eigen::MatrixXi E = edges(F);
  int Chi = euler_characteristic(F);
  std::cout<<"Edge list E is "<<E.rows()<<"x"<<E.cols()<<std::endl;
  std::cout<<"Euler Characteristic: "<<euler_characteristic(F)<<std::endl;
  //std::cout<<"V"<<V <<std::endl;
  //std::cout<<"edge_flaps"<<igl::edge_flaps();
  //std::cout<<"F"<<F <<std::endl;
  //Eigen::MatrixXi o1,o2,o3;
  //igl::edge_topology(V, F, o1, o2, o3);
  //std::cout<<"edge_topology o1"<<o1 << std::endl;
  //std::cout<<"edge_topology o2"<<o2 << std::endl;
  //std::cout<<"edge_topology o3"<<o3 << std::endl;

  // Create a libigl Viewer object 
  igl::viewer::Viewer viewer;
  // Set the vertices and faces for the viewer
  viewer.data.set_mesh(V, F);
  
Eigen::MatrixXd C;
  Eigen::VectorXd Z = V.col(2);
  igl::jet(Z,true,C);

  //viewer.data.set_colors(C);


  // Compute per-face normals
  igl::per_face_normals(V,F,N_faces);

  // Compute per-vertex normals
  igl::per_vertex_normals(V,F,N_vertices);

  // Compute per-corner normals, |dihedral angle| > 20 degrees --> crease
  igl::per_corner_normals(V,F,20,N_corners);

  // Plot the mesh
  viewer.callback_key_down = &key_down;
  viewer.core.show_lines = false;
  viewer.data.set_mesh(V, F);
  viewer.data.set_normals(N_faces);
  std::cout<<
    "Press '1' for per-face normals."<<std::endl<<
    "Press '2' for per-vertex normals."<<std::endl<<
    "Press '3' for per-corner normals."<<std::endl;



  // Launch a viewer instance



  viewer.launch();
  return 0;
}

