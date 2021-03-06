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

#include "pGraphItemOriented.h"
#include "traitsMpiOptimization.hpp"

// mpirun -np 2 ./bin/morgana

using namespace std;
using namespace boost::mpi;


/*! Run with two processors */
int main(int argc, char *argv[])
{
  environment  env(argc,argv);
  communicator world;
  
  pGraphItemOriented recP;
  
  assert(world.size() == 2);
  
  if(world.rank() == 0)
  {
    pGraphItemOriented P(3);
    P.setData(1,3,true);
    P.setData(2,4,false);
    P.setData(3,5,true);
    
    world.send(1,0,P);
    world.recv(1,1,recP);
  }
  else
  {
    pGraphItemOriented P(2);
    P.setData(1,6,true);
    P.setData(2,7,true);
    
    world.recv(0,0,recP);
    world.send(0,1,P);
  }
  
  
  world.barrier();
  if(world.rank() == 0)
  {
    cout << "Process: " << world.rank() << " Size: " << world.size() << endl;
    cout << recP << endl;
  }
  
  world.barrier();
  if(world.rank() == 1)
  {
    cout << "Process: " << world.rank() << " Size: " << world.size() << endl;
    cout << recP << endl;
  }
}

/* Pid 0
Process: 0 Size: 2
Num Points : 2
Nodes Id's : 
Id: 1 6; Orient: 1
Id: 2 7; Orient: 1
*/

/* Pid 1
Process: 1 Size: 2
Num Points : 3
Nodes Id's : 
Id: 1 3; Orient: 1
Id: 2 4; Orient: 0
Id: 3 5; Orient: 1
*/

