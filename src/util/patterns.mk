# ---------------------------------------------------------------------
# File: src/util/patterns.mk
#
# This makefile contains the pattern rules used to compile all sources
# files in the directory tree rooted at the src/util directory, which
# contains all source code for the Util namespace. It is included by
# all "makefile" files in this directory tree. 
#
# These patterns use the string $(UTIL_DEFS) of preprocessor macro
# definitions that is constructed in src/util/defines.mk. It also
# uses various variables defined in src/compiler.mk. It must thus 
# be included in other makefiles after these files. 
#-----------------------------------------------------------------------
# Compilation pattern rules

# Path(s) to search for header files. 
INCLUDES= -I$(SRC_DIR)

# Extra dependencies for all source files
UTIL_ALLDEPS= -A$(SRC_DIR)/compiler.mk
UTIL_ALLDEPS+= -A$(SRC_DIR)/util/defines.mk

# Rule to compile *.cpp class source (*.cpp) files.
%.o:%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INCLUDES) $(UTIL_DEFS) -c -o $@ $<
ifdef MAKEDEP
	$(MAKEDEP) $(INCLUDES) $(UTIL_DEFS) $(UTIL_ALLDEPS) $<
endif

#-----------------------------------------------------------------------
