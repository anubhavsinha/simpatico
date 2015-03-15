/*
* Simpatico - Simulation Package for Polymeric and Molecular Liquids
*
* Copyright 2010 - 2014, The Regents of the University of Minnesota
* Distributed under the terms of the GNU General Public License.
*/

#include "BondStorage.h"
#include "GroupStorage.tpp"
#include <ddMd/chemistry/Bond.h>

namespace DdMd
{

   /*
   * Read parameters and allocate memory.
   */
   BondStorage::BondStorage()
   {  setClassName("BondStorage"); }

   /*
   * Read parameters and allocate memory.
   */
   void BondStorage::readParameters(std::istream& in)
   {  GroupStorage<2>::readParameters(in); }

}
