#ifndef MCMD_LINK_FACTORY_H
#define MCMD_LINK_FACTORY_H

/*
* Simpatico - Simulation Package for Polymeric and Molecular Liquids
*
* Copyright 2010 - 2014, The Regents of the University of Minnesota
* Distributed under the terms of the GNU General Public License.
*/

#include <util/param/Factory.h>
#include <iostream>

namespace McMd
{

   using namespace Util;

   class System;
   class BondPotential;

   /**
   * Factory for subclasses of MdBondPotential or McBondPotential.
   * 
   * \ingroup McMd_Link_Module
   */
   class LinkFactory : public Factory<BondPotential>
   {
   
   public:
   
      /**
      * Default constructor.
      */
      LinkFactory(System& system);

      /**
      * Return a pointer to a new McBondInteration, if possible.
      */
      BondPotential* factory(const std::string& subclass) const;

   private:

      // Pointer to parent System. 
      System* systemPtr_;

   };
  
}
#endif
