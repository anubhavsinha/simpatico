#ifndef DDMD_GROUP_ITERATOR_H
#define DDMD_GROUP_ITERATOR_H

/*
* Simpatico - Simulation Package for Polymeric and Molecular Liquids
*
* Copyright 2010 - 2014, The Regents of the University of Minnesota
* Distributed under the terms of the GNU General Public License.
*/

#include <util/containers/PArrayIterator.h>

namespace DdMd
{

   using namespace Util;
   template <int N> class Group; 

   /**
   * Iterator for all Group < N > objects owned by a GroupStorage< N >.
   *
   * \ingroup DdMd_Storage_Group_Module
   */
   template <int N>
   class GroupIterator : public PArrayIterator< Group<N> >
   {};

}
#endif
