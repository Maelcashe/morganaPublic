/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
This file is part of Morgana.
Author: Andrea Villa, andrea.villa81@fastwebnet.it

Morgana is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Morgana is distributed in the hope that it will be useful,but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Morgana. If not, see <http://www.gnu.org/licenses/>.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#include <cmath>
#include <iostream>

#include <boost/mpi.hpp>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>

#include "pMapItem.h"
#include "pMapItemShare.h"

#include "geoShapes.h"
#include "meshInit3d.hpp"

#include "dualMesh3d.hpp"
#include "printMesh.hpp"

using namespace std;
using namespace boost::mpi;
using namespace Teuchos;


int main(int argc, char *argv[])
{
  environment  env(argc,argv);
  communicator world;
  
  typedef linearHexa     GEOSHAPE3D;
  typedef linearQuad     GEOSHAPE2D;
  typedef pMapItemShare  ELMAP;
  typedef pMapItemShare  NODEMAP;
  typedef mesh2d<GEOSHAPE2D,ELMAP,NODEMAP> MESH2D;
  typedef mesh3d<GEOSHAPE3D,ELMAP,NODEMAP> MESH3D;
  
  //Mesh Loading
  string meshFile  = "./tests/morganaMeshes/mignonHexaB.unv";
  string colorFile = "./tests/morganaMeshes/mignonHexaB_color.unv";
  
  meshInit3d<GEOSHAPE3D,ELMAP,NODEMAP> init(world);
  init.femap_to_stdA(meshFile,colorFile);
  
  
  //Dual mesh
  dualMesh3d<GEOSHAPE3D,ELMAP,NODEMAP> dualGrid(world);
  dualGrid.setMesh3d(init.getGrid3d());
  dualGrid.setConnect3d(init.getConnectGrid3d());
  
  dualGrid.buildDualMesh();
  dualGrid.buildFiniteVolumeData();
  
  //Extract the dual meshes
  MESH3D dualGrid3d = dualGrid.getSubCellMesh();
  MESH2D dualGrid2d = dualGrid.getInterfaceMesh();
  
  
  //Local printing 3d
  string local3d = "localMesh3d_" + num2str(world.rank()) + "pid.inp";
  UInt pid       = world.rank();
  
  printMesh<GEOSHAPE3D,ELMAP,NODEMAP> localPrinter3d;
  localPrinter3d.paraviewLocal(local3d, pid, dualGrid3d);
  
  
  //Local printing 2d
  string local2d = "localMesh2d_" + num2str(world.rank()) + "pid.inp";
  
  printMesh<GEOSHAPE2D,ELMAP,NODEMAP> localPrinter2d;
  localPrinter2d.paraviewLocal(local2d, pid, dualGrid2d);
}
