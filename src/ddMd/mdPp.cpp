/*
* Simpatico - Simulation Package for Polymeric and Molecular Liquids
*
* Copyright 2010 - 2012, David Morse (morse012@umn.edu)
* Distributed under the terms of the GNU General Public License.
*/

#include <ddMd/sp/processor/Processor.h>

// std headers
#include <fstream>
#include <unistd.h>
#include <stdlib.h>

using namespace Util;

/**
* @file mdPp.cpp
 */

/**
* Program for postprocessing ddSim MD trajectories.
*
* USAGE:
*
*    mdProcess [-t] [-i inputConfigIo]  param config first last
*
* Required arguments:
*
*     param  - name of parameter file
*     config - base name of configuration file(s)
*     first  - index of first configuration or frame
*     last   - index of last configuration or frame
*
* Options and option arguments:
*
*     -t
*        Sets to read a trajectory file. If not set, reads a sequence
*        of configuration files.
*
*     -i inputConfigIo
*        Specify input configuration/trajectory format. The required
*        argument inputConfigIo is the name of a SpConfigIo subclass.
*
* \ingroup DdMd_Module
*/
int main(int argc, char** argv)
{

   DdMd::Processor processor;

   // Read command-line arguments
   bool tFlag = false;
   bool iFlag = false;
   std::string configIoName = "SpDdMdConfigIo";
   int c;
   opterr = 0;
   while ((c = getopt(argc, argv, "ti:")) != -1) {
      switch (c) {
      case 't':
         tFlag = true;
         break;
      case 'i':
         iFlag = true;
         configIoName = optarg;
         break;
      case '?':
         Log::file() << "Unknown option -" << optopt << std::endl;
      }
   }
   if (argc - optind != 4) {
      std::cout << "optind = " << optind << std::endl;
      std::cout << "argc   = " << argc << std::endl;
      UTIL_THROW("Wrong number of arguments");
   }
   const char* paramFileName = argv[optind];
   const char* configFileName = argv[optind+1];
   int first = atoi(argv[optind+2]);
   int last = atoi(argv[optind+3]);
   std::cout << "paramFile  = " << paramFileName << std::endl;
   std::cout << "configFile = " << configFileName << std::endl;
   std::cout << "first      = " << first << std::endl;
   std::cout << "last       = " << last << std::endl;

   // Read parameter file
   std::ifstream paramFile;
   paramFile.open(paramFileName);
   processor.readParam(paramFile);
   paramFile.close();

   // Set input file format format
   if (iFlag) {
      std::cout << "Setting ConfigIo " << configIoName << std::endl;
      processor.setConfigIo(configIoName);
   }

   // Process dumps
   processor.analyzeDumps(first, last, configFileName);

   return 0;
}

