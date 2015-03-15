#ifdef MCMD_PERTURB
/*
* Simpatico - Simulation Package for Polymeric and Molecular Liquids
*
* Copyright 2010 - 2014, The Regents of the University of Minnesota
* Distributed under the terms of the GNU General Public License.
*/

#include "PerturbDerivative.h"           // class header
#include <mcMd/perturb/Perturbation.h>  
#include <util/misc/FileMaster.h>  

#include <cstdio> 

namespace McMd
{

   using namespace Util;

   /* 
   * Constructor.
   */
   PerturbDerivative::PerturbDerivative(System& system)
    : SystemAnalyzer<System>(system),
      isInitialized_(false)
   { setClassName("PerturbDerivative"); }

   /*
   * Read parameters and initialize.
   */
   void PerturbDerivative::readParameters(std::istream& in)
   {
      readInterval(in);
      readOutputFileName(in);
      read<int>(in,"nSamplePerBlock", nSamplePerBlock_);

      accumulator_.setNSamplePerBlock(nSamplePerBlock_);

      read<int>(in,"parameterIndex", parameterIndex_);

      // If nSamplePerBlock != 0, open an output file for block averages.
      if (accumulator_.nSamplePerBlock()) {
         fileMaster().openOutputFile(outputFileName(".dat"), outputFile_);
      }

      isInitialized_ = true;
   }

   /*
   * Load internal state from archive.
   */
   void PerturbDerivative::loadParameters(Serializable::IArchive &ar)
   {
      Analyzer::loadParameters(ar);
      loadParameter<int>(ar,"nSamplePerBlock", nSamplePerBlock_);
      loadParameter<int>(ar,"parameterIndex", parameterIndex_);
      ar & accumulator_;

      if (accumulator_.nSamplePerBlock()) {
         fileMaster().openOutputFile(outputFileName(".dat"), outputFile_);
      }
      isInitialized_ = true;
   }

   /*
   * Save internal state to archive.
   */
   void PerturbDerivative::save(Serializable::OArchive &ar)
   { ar & *this; }
   
   /*
   * Clear accumulator.
   */
   void PerturbDerivative::setup()
   {
      if (!isInitialized_) {
         UTIL_THROW("Object is not initialized");
      }
      accumulator_.clear();
   }


   /* 
   * Evaluate perturbation derivative, and add to accumulator.
   */
   void PerturbDerivative::sample(long iStep) 
   {
      if (isAtInterval(iStep))  {
         accumulator_.sample(system().perturbation().derivative(parameterIndex_), outputFile_);
      }
   }

   /*
   * Output results to file after simulation is completed.
   */
   void PerturbDerivative::output() 
   { 
      // If outputFile_ was used to write block averages, close it.
      if (accumulator_.nSamplePerBlock()) {
         outputFile_.close();
      }

      fileMaster().openOutputFile(outputFileName(".prm"), outputFile_);
      writeParam(outputFile_); 
      outputFile_.close();

      fileMaster().openOutputFile(outputFileName(".ave"), outputFile_);
      accumulator_.output(outputFile_); 
      outputFile_.close();

   }
   
}
#endif // ifdef  MCMD_PERTURB
