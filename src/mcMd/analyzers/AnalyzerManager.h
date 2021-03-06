#ifndef MCMD_ANALYZER_MANAGER_H
#define MCMD_ANALYZER_MANAGER_H

/*
* Simpatico - Simulation Package for Polymeric and Molecular Liquids
*
* Copyright 2010 - 2014, The Regents of the University of Minnesota
* Distributed under the terms of the GNU General Public License.
*/

#include "Analyzer.h"                  // template parameter
#include <util/param/Manager.h>          // base class template

namespace McMd
{

   using namespace Util;

   /**
   * Manager for a list of Analyzer objects.
   *
   * \ingroup McMd_Manager_Module
   * \ingroup McMd_Analyzer_Module
   */
   class AnalyzerManager : public Manager<Analyzer>
   {

   public:

      /**
      * Constructor.
      */
      AnalyzerManager();

      /**
      * Destructor.
      */
      virtual ~AnalyzerManager();

      /**
      * Read parameter file. 
      *
      * \param in input parameter file stream.
      */
      virtual void readParameters(std::istream &in);

      /**
      * Load state from an archive.
      *
      * \param ar loading (input) archive.
      */
      virtual void loadParameters(Serializable::IArchive& ar);

      /**
      * Save state to an archive.
      *
      * \param ar saving (output) archive.
      */
      virtual void save(Serializable::OArchive& ar);

      /**
      * Call initialize method of each Analyzer.
      * 
      * This method should be called just before the main
      * simulation loop, after an initial configuration is
      * known.
      */
      void setup();
 
      /**
      * Call sample method of each Analyzer.
      */
      void sample(long iStep);
 
      /**
      * Call output method of each analyzer.
      */
      void output();

   };

}
#endif
