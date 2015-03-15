#ifndef MCMD_BOND_H
#define MCMD_BOND_H

/*
* Simpatico - Simulation Package for Polymeric and Molecular Liquids
*
* Copyright 2010 - 2014, The Regents of the University of Minnesota
* Distributed under the terms of the GNU General Public License.
*/

#include "Group.h"

namespace McMd
{

   using namespace Util;

   /**
   * A Group of 2 covalently bonded Atoms.
   *
   * \ingroup McMd_Chemistry_Module
   */
   typedef Group<2> Bond;

}

#endif
