#ifndef DDMD_GROUP_EXCHANGER_H
#define DDMD_GROUP_EXCHANGER_H

/*
* Simpatico - Simulation Package for Polymeric and Molecular Liquids
*
* Copyright 2010 - 2012, David Morse (morse012@umn.edu)
* Distributed under the terms of the GNU General Public License.
*/

#include <util/boundary/Boundary.h> 
#include <util/space/Dimension.h> 
#include <util/global.h>             

namespace Util {
   class IntVector;
   template <typename T, int M, int N> class FMatrix;
   template <typename T> class GPArray; 
}

namespace DdMd
{

   class Atom;
   class AtomStorage;
   class AtomMap;
   class Buffer;
   using namespace Util;

   /**
   * Interface for a GroupStorage<N> for use in Exchanger.
   *
   * \ingroup DdMd_Storage_Module
   */
   class GroupExchanger
   {

   public:

      /**
      * Default constructor.
      */
      GroupExchanger();

      /**
      * Destructor.
      */
      virtual ~GroupExchanger();

      /**
      * Find and mark groups that span boundaries.
      *
      * \param bound  boundaries of domain for this processor
      * \param inner  inner slab boundaries (extended domain of neighbors)
      * \param outer  outer slab boundaries (extended domain of this processor)
      * \param gridFlags  element i is 0 iff gridDimension[i] == 1, 1 otherwise
      * \param map AtomMap object
      */
      virtual
      void beginAtomExchange(const FMatrix<double, Dimension, 2>& bound, 
                             const FMatrix<double, Dimension, 2>& inner, 
                             const FMatrix<double, Dimension, 2>& outer, 
                             const Boundary& boundary,
                             const IntVector& gridFlags, 
                             const AtomMap& map) = 0;
   
      #ifdef UTIL_MPI
      /**
      * Pack groups for exchange.
      *
      * Usage: This is called after atom exchange plans are set, 
      * but before atoms marked for exchange in direction i, j
      * have been removed from the atomStorage.
      *
      * \param i  index of Cartesian communication direction
      * \param j  index for sign of direction
      * \param buffer  Buffer object into which groups are packed
      */
      virtual void pack(int i, int j, Buffer& buffer) = 0;
   
      /**
      * Unpack groups from buffer and find available associated atoms.
      *
      * This method should unpack groups, add new ones to a GroupStorage,
      * set pointers to all Group atoms that are in the AtomStorage, and
      * nullify pointers to atoms that are not present.
      *
      * \param buffer  Buffer object from which groups are unpacked
      * \param atomStorage  AtomStorage used to find atoms pointers
      */
      virtual void unpack(Buffer& buffer, AtomStorage& atomStorage) = 0;
      #endif // ifdef UTIL_MPI
   
      /**
      * Setup communication plans for exchanging ghosts required by groups.
      *
      * This function is called after exchanging all atoms and groups but
      * before exchanging any ghosts. It sets flags in ghost communication 
      * plans for all atoms in groups that are incomplete at this point.
      *
      * \param atomStorage  AtomStorage object
      * \param sendArray  Matrix of arrays of pointers to ghosts to send
      * \param gridFlags  element i is 0 iff gridDimension[i] == 1, 1 otherwise
      */
      virtual
      void beginGhostExchange(AtomStorage& atomStorage, 
                              FMatrix< GPArray<Atom>, Dimension, 2 >&  sendArray,
                              IntVector& gridFlags) = 0;
   
      /**
      * Finalize assignment of pointers to atoms in all groups.
      *
      * This function must be called after all ghosts have been exchanged. It
      * sets pointers to atoms for all groups, and checks that the group are
      * compact. 
      *
      * If the processor grid contains only one processor in some directions, 
      * one processor may contain one or more atom images with the same atom
      * index. In this case, the function chooses among these "images" so
      * as to create spatially compact groups, in which the separation of 
      * each pair of sequentially numbered atoms within a group satisfies 
      * the minimum image convention. To do this, it may create additional
      * images of some groups ("ghost" groups) and divide the local atoms 
      * of each group among two or more images, so that every local atom
      * belongs to one image of the group. These additional ghost groups 
      * are destroyed at beginning of the next atom exchange by the 
      * beginAtomExchange method. Ghost groups are never created if the 
      * processor grid contains at least two processors in every direction.
      *
      * \param map AtomMap object used to find atom pointers
      * \param boundary Boundary object used to check minimum image convention
      */
      virtual void 
      finishGhostExchange(const AtomMap& map, const Boundary& boundary) = 0;
   
      /**
      * Return true if the container is valid, or throw an Exception.
      *
      * This function may be called after completion of atom and group exchange,
      * but before ghost exchange.
      *
      * \param atomStorage  associated AtomStorage object
      * \param communicator domain communicator 
      * \param hasGhosts    true if the atomStorage has ghosts, false otherwise
      */
      virtual bool
      #ifdef UTIL_MPI
      isValid(const AtomStorage& atomStorage, 
              MPI::Intracomm& communicator) = 0;
      #else
      isValid(const AtomStorage& atomStorage) = 0;
      #endif

      /**
      * Return true if the container is valid, or throw an Exception.
      *
      * This function should only be called after completion of ghost exchange,
      * when all groups should be complete.
      *
      * \param atomStorage  associated AtomStorage object
      * \param boundary  Boundary object, used to check spatial compactness
      * \param communicator domain communicator 
      */
      virtual bool
      #ifdef UTIL_MPI
      isValid(const AtomStorage& atomStorage, const Boundary& boundary, 
                   MPI::Intracomm& communicator) = 0;
      #else
      isValid(const AtomStorage& atomStorage, const Boundary& boundary) = 0;
      #endif

   }; // class GroupExchanger

} // namespace DdMd
#endif
