#ifndef MDCF_PROCESSOR_TEST_H
#define MDCF_PROCESSOR_TEST_H

#include <mdCf/processor/Processor.h>
#include <mdCf/chemistry/Atom.h>
#include <mdCf/chemistry/Group.h>

#include <test/ParamFileTest.h>
#include <test/UnitTestRunner.h>

using namespace Util;
using namespace MdCf;

class ProcessorTest : public ParamFileTest
{

private:

   Processor processor_;

public:

   ProcessorTest() 
    : processor_()
   {}

   virtual void setUp()
   { 
      processor_.readParam("in/Processor"); 
   }

   void testReadParam();

   void testAddAtoms();

};

inline void ProcessorTest::testReadParam()
{  
   printMethod(TEST_FUNC); 
   if (verbose() > 0) {
      processor_.writeParam(std::cout);
   }
   TEST_ASSERT(processor_.nSpecies() == 0);
}

inline void ProcessorTest::testAddAtoms()
{
   printMethod(TEST_FUNC);

   Atom* atomPtr22;
   atomPtr22 = processor_.atoms().newPtr();
   atomPtr22->id = 22;
   processor_.atoms().add();
   TEST_ASSERT(atomPtr22 == processor_.atoms().ptr(22));
   TEST_ASSERT(0 == processor_.atoms().ptr(15));
   TEST_ASSERT(0 == processor_.atoms().ptr(13));
   TEST_ASSERT(processor_.atoms().size() == 1);

   Atom* atomPtr13;
   atomPtr13 = processor_.atoms().newPtr();
   atomPtr13->id = 13;
   processor_.atoms().add();
   TEST_ASSERT(atomPtr13 == processor_.atoms().ptr(13));
   TEST_ASSERT(atomPtr22 == processor_.atoms().ptr(22));
   TEST_ASSERT(0 == processor_.atoms().ptr(15));
   TEST_ASSERT(processor_.atoms().size() == 2);

   Atom* atomPtr15;
   atomPtr15 = processor_.atoms().newPtr();
   atomPtr15->id = 15;
   processor_.atoms().add();
   TEST_ASSERT(atomPtr15 == processor_.atoms().ptr(15));
   TEST_ASSERT(atomPtr13 == processor_.atoms().ptr(13));
   TEST_ASSERT(atomPtr22 == processor_.atoms().ptr(22));
   TEST_ASSERT(processor_.atoms().size() == 3);
}

TEST_BEGIN(ProcessorTest)
TEST_ADD(ProcessorTest, testReadParam)
TEST_ADD(ProcessorTest, testAddAtoms)
TEST_END(ProcessorTest)

#endif
