/*
* Simpatico - Simulation Package for Polymeric and Molecular Liquids
*
* Copyright 2010 - 2014, The Regents of the University of Minnesota
* Distributed under the terms of the GNU General Public License.
*/

#include "LogProgress.h"
#include <util/misc/Log.h>

namespace McMd
{

   using namespace Util;

   /*
   * Constructor.
   */
   LogProgress::LogProgress() 
   {  setClassName("LogProgress"); }

   /*
   * Read interval.
   */
   void LogProgress::readParameters(std::istream& in) 
   {  readInterval(in); }

   /*
   * Load interval and empty outputFileName.
   */
   void LogProgress::loadParameters(Serializable::IArchive& ar) 
   {  
      loadInterval(ar);     // Load value and add to param file format.
      ar & outputFileName_; // Load empty string, don't add to format.
   }

   /*
   * Dump configuration to file
   */
   void LogProgress::sample(long iStep) 
   {
      if (isAtInterval(iStep))  {
         Log::file() << "iStep = " << iStep << std::endl;
      }
   }
  
}
