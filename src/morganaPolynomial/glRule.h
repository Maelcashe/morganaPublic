/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
This file is part of Morgana.
Author: Andrea Villa, andrea.villa81@fastwebnet.it

Morgana is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Morgana is distributed in the hope that it will be useful,but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Morgana. If not, see <http://www.gnu.org/licenses/>.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/


#ifndef GLRULE_H
#define GLRULE_H

#include "typesInterface.hpp"
#include "simpleFormats.hpp"


/*! The generator of the Gauss-Lobatto nodes in one dimension. The user provides the number of the nodes \c NN and the
algorithm generates both the nodes coordinates \c xgl and the associated weights \c wgl. Since these data are obtained
numerically it is necessary to provide a convergence tolerance \c toll */
class glRule
{
  public:
    UInt N;
    
  public:
    glRule(const UInt & NN);
    void compute(sVect<Real> & xgl, sVect<Real> & wgl, const Real & toll) const;
};


#endif
