# ---------------------------------------------------------------------
# File: src/util/patterns.mk
#
# This makefile contains the pattern rule used to compile all sources
# files in the directory tree rooted at the src/util directory, which
# contains all source code for the Util namespace. It is included by
# all "makefile" files in this directory tree. 
#
# This file should be included in other makefiles after inclusion of
# the files src/config.mk and src/util/config.mk because this file
# uses makefile variables defined in those files.
#-----------------------------------------------------------------------

# All libraries needed in this namespace
LIBS=$(util_LIB)

# Preprocessor macro definitions
DEFINES=$(UTIL_DEFS)

# Dependencies of source files in src/util on makefile fragments
MAKE_DEPS= -A$(OBJ_DIR)/config.mk
MAKE_DEPS+= -A$(OBJ_DIR)/util/config.mk

# Pattern rule to compile *.cpp class source files in src/util
$(OBJ_DIR)/%.o:$(SRC_DIR)/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INCLUDES) $(DEFINES) -c -o $@ $<
ifdef MAKEDEP
	$(MAKEDEP) $(INCLUDES) $(DEFINES) $(MAKE_DEPS) -S$(SRC_DIR) -B$(OBJ_DIR) $<
endif

# Pattern rule to compile *.cc test source files in src/util/tests
$(OBJ_DIR)/% $(OBJ_DIR)/%.o:$(SRC_DIR)/%.cc $(LIBS)
	$(CXX) $(CPPFLAGS) $(TESTFLAGS) $(INCLUDES) $(DEFINES) -c -o $@ $<
	$(CXX) $(LDFLAGS) $(INCLUDES) $(DEFINES) -o $(@:.o=) $@ $(LIBS)
ifdef MAKEDEP
	$(MAKEDEP) $(INCLUDES) $(DEFINES) $(MAKE_DEPS) -S$(SRC_DIR) -B$(OBJ_DIR) $<
endif

