/*
* Simpatico - Simulation Package for Polymeric and Molecular Liquids
*
* Copyright 2010 - 2014, The Regents of the University of Minnesota
* Distributed under the terms of the GNU General Public License.
*/

#include "Molecule.h"

namespace McMd
{

   using namespace Util;

   class Species;
   class System;

   // Inline member functions

   /// Constructor.
   Molecule::Molecule()
    : speciesPtr_(0),
      systemPtr_(0),
      firstAtomPtr_(0),
      #ifdef INTER_BOND
      firstBondPtr_(0),
      #endif
      #ifdef INTER_ANGLE
      firstAnglePtr_(0),
      #endif
      #ifdef INTER_DIHEDRAL
      firstDihedralPtr_(0),
      #endif
      nAtom_(0),
      #ifdef INTER_BOND
      nBond_(0),
      #endif
      #ifdef INTER_ANGLE
      nAngle_(0),
      #endif
      #ifdef INTER_DIHEDRAL
      nDihedral_(0),
      #endif
      id_(NullIndex)
   {}

} 
