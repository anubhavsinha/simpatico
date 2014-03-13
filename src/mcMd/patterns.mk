# ---------------------------------------------------------------------
# File: src/mcMd/patterns.mk
#
# This makefile contains the pattern rule used to compile all sources
# files in the directory tree rooted at directory src/mcMd, which
# contains the source code for the McMd namespace. It is included by
# all makefile files in this directory tree. 
#
# This file should be included in other makefiles after inclusion of
# the files src/config.mk, src/util/config.mk, src/inter/config.mk, 
# and src/mcMd/config.mk, because this file uses makefile variables
# defined in those files. 
#-----------------------------------------------------------------------

# All libraries needed in this namespace
LIBS=$(mcMd_LIB) $(inter_LIB) $(util_LIB)

# C preprocessor macro definitions need in this namespace
DEFINES=$(UTIL_DEFS) $(INTER_DEFS) $(MCMD_DEFS)

# Dependencies of source files in src/mcMd on makefile fragments
MAKE_DEPS= -A$(OBJ_DIR)/config.mk
MAKE_DEPS+= -A$(OBJ_DIR)/util/config.mk
MAKE_DEPS+= -A$(OBJ_DIR)/inter/config.mk
MAKE_DEPS+= -A$(OBJ_DIR)/mcMd/config.mk

# Pattern rule to compile all class source (*.cpp) files in src/mcMd
$(OBJ_DIR)/%.o:$(SRC_DIR)/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INCLUDES) $(DEFINES) -c -o $@ $<
ifdef MAKEDEP
	$(MAKEDEP) $(INCLUDES) $(DEFINES) $(MAKE_DEPS) -S$(SRC_DIR) -B$(OBJ_DIR) $<
endif

# Pattern rule to compile all test source (*.cc) files in src/mcMd/tests
$(OBJ_DIR)/% $(OBJ_DIR)/%.o:$(SRC_DIR)/%.cc $(LIBS)
	$(CXX) $(CPPFLAGS) $(TESTFLAGS) $(INCLUDES) $(DEFINES) -c -o $@ $<
	$(CXX) $(LDFLAGS) $(INCLUDES) $(DEFINES) -o $(@:.o=) $@ $(LIBS)
ifdef MAKEDEP
	$(MAKEDEP) $(INCLUDES) $(DEFINES) $(MAKE_DEPS) -S$(SRC_DIR) -B$(OBJ_DIR) $<
endif

