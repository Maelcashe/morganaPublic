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
#include "pMap.hpp"
#include "pMapGlobalManip.h"

using namespace std;
using namespace boost::mpi;

 
int main(int argc, char *argv[])
{
  //Comm dev---------------------------------------------------------
  boost::mpi::environment  env(argc, argv);
  boost::mpi::communicator newCommDev;
  
  bool commActive = (newCommDev.rank() >= 2);
  boost::mpi::communicator oldCommDev = newCommDev.split(commActive);
  
  assert(newCommDev.size() == 4);
  
  //Build map--------------------------------------------------------
  pMap<pMapItem> newMap;
  
  //Reduce map-------------------------------------------------------
  if(commActive)
  {    
    pMap<pMapItem> oldMap;
    
    if(oldCommDev.rank() == 0)
    {
      oldMap.push_back(pMapItem(1,1,oldCommDev.rank()));
      oldMap.push_back(pMapItem(2,2,oldCommDev.rank()));
    }
  
    if(oldCommDev.rank() == 1)
    {
      oldMap.push_back(pMapItem(1,3,oldCommDev.rank()));
      oldMap.push_back(pMapItem(2,4,oldCommDev.rank()));
    }
  
    pMapGlobalManip<pMapItem> pManip(oldCommDev);
    pManip.expandCommunicator(commActive,
                              oldCommDev,
                              oldMap,
                              newCommDev,
                              newMap);
  }
  
  //Printout---------------------------------------------------------
  for(UInt pid=0; pid < newCommDev.size(); pid++)
  {
    if(pid == newCommDev.rank())
    { cout << newMap << endl; }
    
    newCommDev.barrier();
  }
}
