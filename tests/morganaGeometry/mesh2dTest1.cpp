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

#include "typesInterface.hpp"

#include "pMapItem.h"
#include "pMapItemShare.h"

#include "pGraph.hpp"

#include "geoShapes.h"
#include "mesh2d.hpp"

// mpirun -np 2 ./bin/morgana

using namespace std;
using namespace boost::mpi;

int main(int argc, char *argv[])
{
  typedef geoElement<linearTriangle> GEOELEMENT;
  typedef pMapItemShare ELMAP;
  typedef pMapItem      NODEMAP;
  
  environment  env(argc,argv);
  communicator world;
  
  assert(world.size() == 2);
  mesh2d<linearTriangle,ELMAP,NODEMAP> grid2d;
  
  
  if(world.rank() == 0)
  {
    //Nodes
    pVect<point3d,NODEMAP> nodes;
    
    nodes.reserve(3);
    nodes.push_back(point3d(0.0, 0.0, 0.0),pMapItem(1,1));
    nodes.push_back(point3d(1.0, 0.0, 0.0),pMapItem(2,2));
    nodes.push_back(point3d(1.0, 1.0, 0.0),pMapItem(3,3));
    
    //Elements
    GEOELEMENT tri(true);
    pGraph<GEOELEMENT,ELMAP,NODEMAP> elList;
    
    elList.reserve(1);
    tri.setGeoId(1); tri(1) = 1; tri(2) = 2; tri(3) = 3;
    elList.push_back(tri, pMapItemShare(1,1,false,true));
    
    //The grid2d
    grid2d.setNodes(nodes);
    grid2d.setElements(elList);
  }
  else
  {
    //Nodes
    pVect<point3d,NODEMAP> nodes;
    
    nodes.reserve(3);
    nodes.push_back(point3d(0.0, 0.0, 0.0),pMapItem(1,1));
    nodes.push_back(point3d(1.0, 1.0, 0.0),pMapItem(2,3));
    nodes.push_back(point3d(0.0, 1.0, 0.0),pMapItem(3,4));
    
    //Elements
    GEOELEMENT tri(true);
    pGraph<GEOELEMENT,ELMAP,NODEMAP> elList;
    
    elList.reserve(1);
    tri.setGeoId(1); tri(1) = 1; tri(2) = 2; tri(3) = 3;
    elList.push_back(tri, pMapItemShare(1,1,false,true));
    
    //The grid2d
    grid2d.setNodes(nodes);
    grid2d.setElements(elList);
  }
  
  
  world.barrier();
  if(world.rank() == 0)
  {
    cout << "I am: " << world.rank() << endl;
    cout << grid2d << endl;
  }
  sleep(1);
  
  world.barrier();
  if(world.rank() == 1)
  {
    cout << "I am: " << world.rank() << endl;
    cout << grid2d << endl;
  }
  sleep(1);
}

/*
I am: 0
Mesh 2d
Nodes    : 3
Elements : 1
Edges    : 0

I am: 1
Mesh 2d
Nodes    : 3
Elements : 1
Edges    : 0
 */

