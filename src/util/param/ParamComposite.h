#ifndef UTIL_PARAM_COMPOSITE_H
#define UTIL_PARAM_COMPOSITE_H

/*
* Simpatico - Simulation Package for Polymeric and Molecular Liquids
*
* Copyright 2010 - 2012, David Morse (morse012@umn.edu)
* Distributed under the terms of the GNU General Public License.
*/

#include <util/param/ParamComponent.h>     // base class
#include <util/param/ScalarParam.h>        // template class
#include <util/param/CArrayParam.h>        // template class
#include <util/param/DArrayParam.h>        // template class
#include <util/param/FArrayParam.h>        // template class
#include <util/param/CArray2DParam.h>      // template class
#include <util/param/DMatrixParam.h>       // template class
#include <util/archives/Serializable_includes.h>
#include <util/global.h>

#include <vector>

namespace Util
{

   class Begin;
   class End;
   class Blank;
   template <class Data> class Factory;

   /**
   * An object that can read multiple parameters from file.
   *
   * A ParamComposite has a private array of pointers to ParamComponent
   * objects. These are stored in the order in which they are read from
   * file by the ParamComposite::readParam() method. We will refer to this
   * array as the format array. Each element of the format array may point
   * to a Parameter object (which represents a single parameter), a Begin
   * object (which represents a line containing the class name and an
   * opening bracket) and End object (which represents a line containing
   * only a closing bracket), a Blank object (i.e., a blank line), or
   * another ParamComposite object.
   *
   * Any class that reads a block of parameters from the input parameter file
   * must be derived from ParamComposite. The readParam() method must read
   * the associated parameter file block. The virtual readParameters() method,
   * if re-implemented, should read only the body of the parameter file
   * block, without opening and closing lines. The default implementation of
   * ParamComposite::readParam() reads the opening line of the block, calls
   * readParameters() to read the body of the  parameter file block, and then
   * reads the closing line. Almost all subclasses of ParamComposite should
   * re-implement the readParameters() method, and rely on the default
   * implementation of ParamComposite::readParam() to add the Begin and
   * End lines.
   *
   * The setClassName() and className() functions may be used to set and get
   * a std::string containing the class name. The setClassName() method
   * should be called in the constructor of each subclass of ParamComposite.
   * The class name set in the constructor of a subclass will replace any
   * name set by a base class, because of the order in which constructors
   * are called. The class name string is used by the default implementation
   * of ParamComposite::readParam() in order to check that the opening line
   * of a parameter block contains the correct class name.
   *
   * The readParameters() method of each subclass should be implemented
   * using protected methods provided by ParamComposite to read individual
   * parameters and "child" ParamComposite objects.  The implementation of
   * readParameters() normally uses read< T >, which reads an individual
   * parameter, and readParamComposite, which reads a nested subblock.
   * There are also more specialized methods (e.g., readDArray<T>to read
   * different types of arrays and matrices of parameters.  Each of these
   * methods creates a new ParamComponent of a specific type, adds a
   * pointer to the new object to the format array, and invokes the
   * readParam method of the new object in order to read the associated
   * line or block of the parameter file.
   *
   * The ParamComposite::writeParam() method uses the format array to write
   * data to an output parameter file in the same format in which it was
   * read by a previous call to readParam().
   *
   * \ingroup Param_Module
   */
   class ParamComposite : public ParamComponent
   {

   public:

      /**
      * Constructor
      */
      ParamComposite();

      /**
      * Copy constructor
      */
      ParamComposite(const ParamComposite& other);

      /**
      * Constructor.
      *
      * Reserve space for capacity elements in the format array.
      *
      * \param capacity maximum length of parameter list
      */
      ParamComposite(int capacity);

      /**
      * Virtual destructor.
      */
      virtual ~ParamComposite();

      /// \name Initialization methods
      //@{

      /**
      * Read a parameter file block.
      *
      * Inherited from ParamComponent. This method reads the entire parameter
      * block, including the opening line "ClassName{" and the closing bracket
      * "}". The default implementation calls the virtual readParameters
      * method to read the body of the block, and adds Begin and End objects.
      *
      * \param in input stream for reading
      */
      virtual void readParam(std::istream &in);

      /**
      * Read optional parameter file block.
      *
      * Read an optional ParamComposite. This meethod attempts to reads the
      * beginning "ClassName{" line, and then continues to read the parameter
      * block and closing bracket line only if the beginning line matches.
      * The default implementation calls the readParametersst::istream&()
      * member function to read the enclosed parameter block.
      *
      * \param in input stream for reading
      */
      virtual void readParamOptional(std::istream &in);

      /**
      * Read body of parameter block, without opening and closing lines.
      *
      * Most subclasses of ParamComposite should overload readParameters.
      * The default implementation is empty. All subclasses must overload
      * either readParameters or readParam, but not both.
      *
      * \param in input stream for reading
      */
      virtual void readParameters(std::istream &in)
      {};

      /**
      * Write all parameters to an output stream.
      *
      * The default implementation iterates through the format array, and
      * calls the readParam member function of each ReadComponent in the
      * array.  This is sufficient for most subclasses.
      *
      * \param out output stream for reading
      */
      virtual void writeParam(std::ostream &out);

      /**
      * Load all parameters from an archive.
      *
      * This function is inherited from Serializable. The default
      * implementation for a ParamComposite calls loadParameters, and
      * adds Begin and End lines. All subclasses of ParamComposite
      * should overload loadParameters.
      *
      * \param ar input/loading archive.
      */
      virtual void load(Serializable::IArchive &ar);

      /**
      * Load an optional ParamComposite.
      *
      * Loads isActive, and calls load(ar) if active.
      *
      * \param ar input/loading archive.
      */
      virtual void loadOptional(Serializable::IArchive &ar);

      /**
      * Load state from archive, without adding Begin and End lines.
      *
      * This default implementation is empty, and should be re-implemented
      * by all subclasses that have an internal state which should be saved
      * in a restart file. Subclass implementations should load the entire
      * internal state from the archive, including persistent private data
      * that does not appear in the parameter file format.
      *
      * \param ar input/loading archive.
      */
      virtual void loadParameters(Serializable::IArchive &ar)
      {};

      /**
      * Saves all parameters to an archive.
      *
      * This simple default implementation calls the save method for all items
      * in the parameter file format array. This is not sufficient for classes
      * that contain any persistent private data that does not appear in the
      * parameter file format.
      *
      * If a class also defines a serialize method template, which allows
      * instances to be serialized to any type of archive, then the save
      * method can be implemented as follows:
      * \code
      *    void save(Serializable::OArchive& ar)
      *    { ar & *this; }
      * \endcode
      *
      * \param ar output/saving archive.
      */
      virtual void save(Serializable::OArchive &ar);

      /**
      * Saves isActive flag, then calls save() iff isActive is true.
      *
      * \param ar output/saving archive.
      */
      void saveOptional(Serializable::OArchive &ar);

      /**
      * Resets ParamComposite to its empty state.
      *
      * This method deletes Parameter, Begin, End, and Blank objects in the
      * format array (i.e., all "leaf" objects in the format tree), invokes
      * the resetParam() for any child ParamComposite in the format array
      * (the nodes in the true), and clears the array of pointers.
      */
      void resetParam();

      //@}
      /// \name read* methods
      /// \brief Each of these methods invokes an associated add* method to
      /// create a new ParamComponent object, and then invoke the readParam()
      /// method of the new object to read the associated line or block of a
      /// parameter file.
      //@{

      /**
      * Add and read a required child ParamComposite.
      *
      * \param in    input stream for reading
      * \param child child ParamComposite object
      * \param next  true if the indent level is one higher than parent.
      */
      void
      readParamComposite(std::istream &in, ParamComposite &child,
                         bool next = true);

      /**
      * Add and attempt to read an optional child ParamComposite.
      *
      * \param in    input stream for reading
      * \param child child ParamComposite object
      * \param next  true if the indent level is one higher than parent.
      */
      void
      readParamCompositeOptional(std::istream &in,
                                 ParamComposite &child, bool next = true);

      /**
      * Add a new ScalarParam < Type > object, and read its value.
      *
      * \param in     input stream for reading
      * \param label  Label string
      * \param value  reference to new ScalarParam< Type >
      * \param isRequired  Is this a required parameter?
      */
      template <typename Type>
      ScalarParam<Type>& read(std::istream &in, const char *label, Type &value,
                              bool isRequired);

      /**
      * Add and read a new required ScalarParam < Type > object.
      */
      template <typename Type>
      ScalarParam<Type>& read(std::istream &in, const char *label, Type &value)
      {  return read<Type>(in, label, value, true); }

      /**
      * Add and read a C array parameter.
      *
      * \param in     input stream for reading
      * \param label  Label string for new array
      * \param value  pointer to array
      * \param n      number of elements
      * \param isRequired  Is this a required parameter?
      * \return reference to the new CArrayParam<Type> object
      */
      template <typename Type>
      CArrayParam<Type>&
      readCArray(std::istream &in, const char *label, Type *value, int n,
                 bool isRequired);

      /**
      * Add and read a required C array parameter.
      */
      template <typename Type>
      CArrayParam<Type>&
      readCArray(std::istream &in, const char *label, Type *value, int n)
      {  return readCArray<Type>(in, label, value, n, true); }

      /**
      * Add and read a DArray < Type > parameter.
      *
      * \param in  input stream for reading
      * \param label  Label string for new array
      * \param array  DArray object
      * \param n  number of elements
      * \param isRequired  Is this a required parameter?
      * \return reference to the new DArrayParam<Type> object
      */
      template <typename Type>
      DArrayParam<Type>&
      readDArray(std::istream &in, const char *label,
                 DArray<Type>& array, int n, bool isRequired);

      /**
      * Add a required DArray < Type > parameter.
      */
      template <typename Type>
      DArrayParam<Type>&
      readDArray(std::istream &in, const char *label,
                 DArray<Type>& array, int n)
      {  return readDArray<Type>(in, label, array, n, true); }

      /**
      * Add and read an FArray < Type, N > fixed-size array parameter.
      *
      * \param in  input stream for reading
      * \param label  Label string for new array
      * \param array  FArray object
      * \param isRequired  Is this a required parameter?
      * \return reference to the new FArrayParam<Type, N> object
      */
      template <typename Type, int N>
      FArrayParam<Type, N>&
      readFArray(std::istream &in, const char *label, FArray<Type, N >& array,
                 bool isRequired);

      /**
      * Add and read a required FArray < Type, N > array parameter.
      */
      template <typename Type, int N>
      FArrayParam<Type, N>&
      readFArray(std::istream &in, const char *label, FArray<Type, N >& array)
      {  return readFArray<Type>(in, label, array, true); }

      /**
      * Add and read a CArray2DParam < Type > C 2D array parameter.
      *
      * This reads m rows of n elements into an array of type array[][np].
      *
      * \param in  input stream for reading
      * \param label  Label string for new array
      * \param value  pointer to array
      * \param m  number of rows (1st dimension)
      * \param n  logical number of columns (2nd dimension)
      * \param np  physical number of columns (elements allocated per row)
      * \param isRequired  Is this a required parameter?
      * \return reference to the CArray2DParam<Type> object
      */
      template <typename Type>
      CArray2DParam<Type>&
      readCArray2D(std::istream &in, const char *label,
                   Type *value, int m, int n, int np, bool isRequired);

      /**
      * Add and read a required CArray2DParam < Type > C 2D array parameter.
      */
      template <typename Type>
      CArray2DParam<Type>&
      readCArray2D(std::istream &in, const char *label,
                   Type *value, int m, int n, int np)
      {  return readCArray2D<Type>(in, label, value, m, n, np, true); }

      /**
      * Add and read a DMatrix < Type > C two-dimensional matrix parameter.
      *
      * \param in  input stream for reading
      * \param label  Label string for new array
      * \param matrix  DMatrix object
      * \param m  number of rows (1st dimension)
      * \param n  number of columns (2nd dimension)
      * \param  isRequired  Is this a required parameter?
      * \return reference to the DMatrixParam<Type> object
      */
      template <typename Type>
      DMatrixParam<Type>&
      readDMatrix(std::istream &in, const char *label, DMatrix<Type>& matrix,
                  int m, int n, bool isRequired);

      /**
      * Add and read a required DMatrix < Type > C matrix parameter.
      */
      template <typename Type>
      DMatrixParam<Type>&
      readDMatrix(std::istream &in, const char *label,
                  DMatrix<Type>& matrix, int m, int n)
      {  return readDMatrix<Type>(in, label, matrix, m, n, true); }

      /**
      * Add and read a class label and opening bracket.
      *
      * \param in  input stream for reading
      * \param label  class name string, without trailing bracket
      * \param isRequired Is this the beginning of a required element?
      * \return reference to the new Begin object
      */
      Begin& readBegin(std::istream &in, const char* label,
                       bool isRequired = true);

      /**
      * Add and read the closing bracket.
      *
      * \param in  input stream for reading
      * \return reference to the new End object
      */
      End& readEnd(std::istream &in);

      /**
      * Add and read a new Blank object, representing a blank line.
      *
      * \param in input stream for reading
      * \return reference to the new Blank object
      */
      Blank& readBlank(std::istream &in);

      //@}
      /// \name load* methods
      /// \brief Each of these methods invokes an associated add* method to
      /// create a new ParamComponent object, and then invokes the load()
      /// method of the new object to load the associated parameter value
      /// from an input archive.
      //@{

      /**
      * Add and load a required child ParamComposite.
      *
      * \param ar  input archive for loading
      * \param child  child ParamComposite object
      * \param next  true if the indent level is one higher than parent.
      */
      void
      loadParamComposite(Serializable::IArchive &ar, ParamComposite &child,
                         bool next = true);

      /**
      * Add and load contents of optional child ParamComposite if isActive.
      *
      * \param ar  input archive for loading
      * \param child  child ParamComposite object
      * \param next  true if the indent level is one higher than parent.
      */
      void
      loadParamCompositeOptional(Serializable::IArchive &ar,
                                 ParamComposite &child, bool next = true);

      /**
      * Add a new Param < Type > object, and load its value.
      *
      * \param ar  archive for loading
      * \param label  Label string
      * \param value  reference to the Type variable
      * \param isRequired Is this a required parameter?
      * \return reference to the new CArrayParam<Type> object
      */
      template <typename Type>
      ScalarParam<Type>& loadParameter(Serializable::IArchive &ar,
                                       const char *label, Type &value,
                                       bool isRequired);

      /**
      * Add and load new required ScalarParam < Type >  object.
      */
      template <typename Type>
      ScalarParam<Type>& loadParameter(Serializable::IArchive &ar,
                                       const char *label, Type &value)
      {  return loadParameter<Type>(ar, label, value, true); }

      /**
      * Add a C array parameter, and load its elements.
      *
      * \param ar  archive for loading
      * \param label  Label string for new array
      * \param value  pointer to array
      * \param n  number of elements
      * \param isRequired Is this a required parameter?
      * \return reference to the new CArrayParam<Type> object
      */
      template <typename Type>
      CArrayParam<Type>&
      loadCArray(Serializable::IArchive &ar, const char *label,
                 Type *value, int n, bool isRequired);

      /**
      * Add and load a required CArrayParam< Type > array parameter.
      */
      template <typename Type>
      CArrayParam<Type>&
      loadCArray(Serializable::IArchive &ar, const char *label,
                 Type *value, int n)
      {  return loadCArray<Type>(ar, label, value, n, true); }

      /**
      * Add a DArray < Type > parameter, and load its elements.
      *
      * \param ar  archive for loading
      * \param label  Label string for new array
      * \param array  DArray object
      * \param n  number of elements (logical size)
      * \param isRequired  Is this a required parameter?
      * \return reference to the new DArrayParam<Type> object
      */
      template <typename Type>
      DArrayParam<Type>&
      loadDArray(Serializable::IArchive &ar, const char *label,
                 DArray<Type>& array, int n, bool isRequired);

      /**
      * Add and load a required DArray< Type > array parameter.
      */
      template <typename Type>
      DArrayParam<Type>&
      loadDArray(Serializable::IArchive &ar, const char *label,
                 DArray<Type>& array, int n)
      {  return loadDArray<Type>(ar, label, array, n, true); }

      /**
      * Add and load an FArray < Type, N > fixed-size array parameter.
      *
      * \param ar  archive for loading
      * \param label  Label string for new array
      * \param array  FArray object
      * \param isRequired  Is this a required parameter?
      * \return reference to the new FArrayParam<Type, N> object
      */
      template <typename Type, int N>
      FArrayParam<Type, N>&
      loadFArray(Serializable::IArchive &ar, const char *label,
                 FArray<Type, N >& array, bool isRequired);

      /**
      * Add and load a required FArray < Type > array parameter.
      */
      template <typename Type, int N>
      FArrayParam<Type, N>&
      loadFArray(Serializable::IArchive &ar, const char *label,
                 FArray<Type, N >& array)
      {  return loadFArray<Type, N>(ar, label, array, true); }

      /**
      * Add and load a CArray2DParam < Type > C 2D array parameter.
      *
      * \param ar  archive for loading
      * \param label  Label string for new array
      * \param value  pointer to array
      * \param m  number of rows (1st dimension)
      * \param n  logical number of columns (2nd dimension)
      * \param np  physical number of columns (elements allocated per row)
      * \param isRequired  Is this a required parameter?
      * \return reference to the CArray2DParam<Type> object
      */
      template <typename Type>
      CArray2DParam<Type>&
      loadCArray2D(Serializable::IArchive &ar, const char *label,
                   Type *value, int m, int n, int np, bool isRequired);

      /**
      * Add and load a required < Type > matrix parameter.
      */
      template <typename Type>
      CArray2DParam<Type>&
      loadCArray2D(Serializable::IArchive &ar, const char *label,
                   Type *value, int m, int n, int np)
      {  return loadCArray2D<Type>(ar, label, value, m, n, np, true); }

      /**
      * Add and load a DMatrix < Type > matrix parameter.
      *
      * \param ar  archive for loading
      * \param label  Label string for new array
      * \param matrix  DMatrix object
      * \param m  number of rows (1st dimension)
      * \param n  number of columns (2nd dimension)
      * \param isRequired  Is this a required parameter?
      * \return reference to the DMatrixParam<Type> object
      */
      template <typename Type>
      DMatrixParam<Type>&
      loadDMatrix(Serializable::IArchive &ar, const char *label,
                  DMatrix<Type>& matrix, int m, int n, bool isRequired);

      /**
      * Add and load a required < Type > matrix parameter.
      */
      template <typename Type>
      DMatrixParam<Type>&
      loadDMatrix(Serializable::IArchive &ar, const char *label,
                  DMatrix<Type>& matrix, int m, int n)
      {  return loadDMatrix<Type>(ar, label, matrix, m, n, true); }

      //@}

      /// \name add* methods
      /// \brief These methods add a ParamComponent object to the format
      /// array, but do not read any data from an input stream.
      //@{

      /**
      * Add a child ParamComposite object to the format array.
      *
      * \param child child ParamComposite object
      * \param next  true if the indent level is one higher than parent.
      */
      void addParamComposite(ParamComposite& child, bool next = true);

      /**
      * Create and add a class label and opening bracket.
      *
      * \param label class name string, without trailing bracket
      * \return reference to the new begin object.
      */
      Begin& addBegin(const char* label);

      /**
      * Create and add a closing bracket.
      *
      * \return reference to the new End object.
      */
      End& addEnd();

      /**
      * Create and add a new Blank object, representing a blank line.
      *
      * \return reference to the new Blank object
      */
      Blank& addBlank();

      //@}

      /**
      * Get class name string.
      */
      std::string className() const;

      /**
      * Is this ParamComposite required in the input file?
      */
      bool isRequired() const;

      /**
      * Is this parameter active?
      */
      bool isActive() const;

   protected:

      /**
      * Set class name string.
      *
      * Should be set in subclass constructor.
      */
      void setClassName(const char* className);

      /**
      * Set or unset the isActive flag.
      *
      * Required to re-implement readParam[Optional].
      *
      * \param isRequired flag to set true or false.
      */
      void setIsRequired(bool isRequired);

      /**
      * Set or unset the isActive flag.
      *
      * Required to re-implement readParam[Optional].
      *
      * \param isActive flag to set true or false.
      */
      void setIsActive(bool isActive);

   private:

      /// Array of pointers to ParamComponent objects.
      std::vector<ParamComponent*> list_;

      /// Array of booleans, elements false for ParamComposite, true otherwise.
      std::vector<bool> isLeaf_;

      /// Number of ParamComponent objects in format array
      int size_;

      /// Name of subclass.
      std::string className_;

      /// Is this parameter required ?
      bool isRequired_;

      /// Is this parameter active ?
      bool isActive_;

      /**
      * Set this to the parent of a child component.
      *
      * This function sets the indent and (ifdef UTIL_MPI)
      * the ioCommunicator of the child component.
      *
      * \param param child ParamComponent
      * \param next  if true, set indent level one higher than that of parent.
      */
      void setParent(ParamComponent& param, bool next = true);

      /**
      * Add a new ParamComponent object to the format array.
      *
      * \param param Parameter object
      * \param isLeaf Is this a leaf or a ParamComposite node?
      */
      void addComponent(ParamComponent& param, bool isLeaf = true);

   };

   // Method templates for scalar parameters

   /*
   * Add a ScalarParam to the format array, and read its contents from file.
   */
   template <typename Type>
   ScalarParam<Type>&
   ParamComposite::read(std::istream &in, const char *label, Type &value,
                        bool isRequired)
   {
      ScalarParam<Type>* ptr;
      ptr = new ScalarParam<Type>(label, value, isRequired);
      setParent(*ptr);
      ptr->readParam(in);
      addComponent(*ptr);
      return *ptr;
   }

   /*
   * Add a new ScalarParam< Type > object, and load its value.
   */
   template <typename Type>
   ScalarParam<Type>&
   ParamComposite::loadParameter(Serializable::IArchive &ar, const char *label,
                                 Type &value, bool isRequired)
   {
      ScalarParam<Type>* ptr;
      ptr = new ScalarParam<Type>(label, value, isRequired);
      setParent(*ptr);
      ptr->load(ar);
      addComponent(*ptr);
      return *ptr;
   }

   // Templates for 1D C Arrays

   /*
   * Add a a CArrayParam associated with a built-in array, and read it.
   */
   template <typename Type>
   CArrayParam<Type>&
   ParamComposite::readCArray(std::istream &in, const char *label,
                              Type *value, int n, bool isRequired)
   {
      CArrayParam<Type>* ptr;
      ptr = new CArrayParam<Type>(label, value, n, isRequired);
      setParent(*ptr);
      ptr->readParam(in);
      addComponent(*ptr);
      return *ptr;
   }

   /*
   * Add a C array parameter, and load its elements.
   */
   template <typename Type>
   CArrayParam<Type>&
   ParamComposite::loadCArray(Serializable::IArchive &ar, const char *label,
                              Type *value, int n, bool isRequired)
   {
      CArrayParam<Type>* ptr;
      ptr = new CArrayParam<Type>(label, value, n, isRequired);
      setParent(*ptr);
      ptr->load(ar);
      addComponent(*ptr);
      return *ptr;
   }

   // Templates for DArray parameters

   /*
   * Add a a DArrayParam associated with a DArray<Type> container, and read it.
   */
   template <typename Type>
   DArrayParam<Type>&
   ParamComposite::readDArray(std::istream &in, const char *label,
                              DArray<Type>& array, int n, bool isRequired)
   {
      DArrayParam<Type>* ptr;
      ptr = new DArrayParam<Type>(label, array, n, isRequired);
      setParent(*ptr);
      ptr->readParam(in);
      addComponent(*ptr);
      return *ptr;
   }

   /*
   * Add a DArray < Type > parameter, and load its elements.
   */
   template <typename Type>
   DArrayParam<Type>&
   ParamComposite::loadDArray(Serializable::IArchive &ar, const char *label,
                              DArray<Type>& array, int n, bool isRequired)
   {
      DArrayParam<Type>* ptr;
      ptr = new DArrayParam<Type>(label, array, n, isRequired);
      setParent(*ptr);
      ptr->load(ar);
      addComponent(*ptr);
      return *ptr;
   }

   // Templates for fixed size FArray array objects.

   /*
   * Add and read an FArray<Type, N> fixed size array.
   */
   template <typename Type, int N>
   FArrayParam<Type, N>&
   ParamComposite::readFArray(std::istream &in, const char *label,
                              FArray<Type, N>& array, bool isRequired)
   {
      FArrayParam<Type, N>* ptr;
      ptr = new FArrayParam<Type, N>(label, array, isRequired);
      setParent(*ptr);
      ptr->readParam(in);
      addComponent(*ptr);
      return *ptr;
   }

   /*
   * Add and load an FArray < Type, N > fixed-size array parameter.
   */
   template <typename Type, int N>
   FArrayParam<Type, N>&
   ParamComposite::loadFArray(Serializable::IArchive &ar, const char *label,
                              FArray<Type, N >& array, bool isRequired)
   {
      FArrayParam<Type, N>* ptr;
      ptr = new FArrayParam<Type, N>(label, array, isRequired);
      setParent(*ptr);
      ptr->load(ar);
      addComponent(*ptr);
      return *ptr;
   }

   // Templates for built-in two-dimensional C Arrays

   /*
   * Add and read a CArray2DParam associated with a built-in 2D array.
   */
   template <typename Type>
   CArray2DParam<Type>&
   ParamComposite::readCArray2D(std::istream &in, const char *label,
                                Type *value, int m, int n, int np,
                                bool isRequired)
   {
      CArray2DParam<Type>* ptr;
      ptr = new CArray2DParam<Type>(label, value, m, n, np, isRequired);
      setParent(*ptr);
      ptr->readParam(in);
      addComponent(*ptr);
      return *ptr;
   }

   /*
   * Add and load a CArray2DParam < Type > C two-dimensional array parameter.
   */
   template <typename Type>
   CArray2DParam<Type>&
   ParamComposite::loadCArray2D(Serializable::IArchive &ar, const char *label,
                                Type *value, int m, int n, int np,
                                bool isRequired)
   {
      CArray2DParam<Type>* ptr;
      ptr = new CArray2DParam<Type>(label, value, m, n, np, isRequired);
      setParent(*ptr);
      ptr->load(ar);
      addComponent(*ptr);
      return *ptr;
   }

   // Templates for DMatrix containers

   /*
   * Add and read a DMatrixParam associated with a built-in array.
   */
   template <typename Type>
   DMatrixParam<Type>&
   ParamComposite::readDMatrix(std::istream &in, const char *label,
                               DMatrix<Type>& matrix, int m, int n,
                               bool isRequired)
   {
      DMatrixParam<Type>* ptr;
      ptr = new DMatrixParam<Type>(label, matrix, m, n, isRequired);
      setParent(*ptr);
      ptr->readParam(in);
      addComponent(*ptr);
      return *ptr;
   }

   /*
   * Add and load a DMatrix < Type > C two-dimensional matrix parameter.
   */
   template <typename Type>
   DMatrixParam<Type>&
   ParamComposite::loadDMatrix(Serializable::IArchive &ar, const char *label,
                               DMatrix<Type>& matrix, int m, int n,
                               bool isRequired)
   {
      DMatrixParam<Type>* ptr;
      ptr = new DMatrixParam<Type>(label, matrix, m, n, isRequired);
      setParent(*ptr);
      ptr->load(ar);
      addComponent(*ptr);
      return *ptr;
   }

   /*
   * Get class name string.
   */
   inline std::string ParamComposite::className() const
   {  return className_; }

   /*
   * Is this ParamComposite required in the input file?
   */
   inline bool ParamComposite::isRequired() const
   { return isRequired_; }

   /*
   * Is this parameter active?
   */
   inline bool ParamComposite::isActive() const
   { return isActive_; }

}
#endif
