/*
* Simpatico - Simulation Package for Polymeric and Molecular Liquids
*
* Copyright 2010 - 2014, The Regents of the University of Minnesota
* Distributed under the terms of the GNU General Public License.
*/

#include "TrajectoryReader.h"

namespace Tools
{

   using namespace Util;

   /*
   * Constructor.
   */
   TrajectoryReader::TrajectoryReader(Configuration& configuration, bool isBinary)
    : configurationPtr_(&configuration),
      isBinary_(isBinary)
   {  setClassName("TrajectoryReader"); }

   /*
   * Destructor.
   */
   TrajectoryReader::~TrajectoryReader()
   {}

}
