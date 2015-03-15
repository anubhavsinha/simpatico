#ifndef DDMD_ANGLE_H
#define DDMD_ANGLE_H

/*
* Simpatico - Simulation Package for Polymeric and Molecular Liquids
*
* Copyright 2010 - 2014, The Regents of the University of Minnesota
* Distributed under the terms of the GNU General Public License.
*/

#include "Group.h"

namespace DdMd
{

   typedef Group<3> Angle;

   #if 0
   /**
   * A of 3 atoms interacting via an angle interaction.
   *
   * \ingroup DdMd_Chemistry_Module
   */
   class Angle : public Group<2>
   {};
   #endif

}
#endif
