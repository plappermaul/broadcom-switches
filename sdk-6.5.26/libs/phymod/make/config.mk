#
# This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
# 
# Copyright 2007-2022 Broadcom Inc. All rights reserved.
#
# PHYMOD make rules and definitions
#

#
# Provide reasonable defaults for configuration variables
#

# Default build directory
ifndef PHYMOD_BLDDIR
PHYMOD_BLDDIR = $(PHYMOD)/build
endif

# Location to build objects in
ifndef PHYMOD_OBJDIR
PHYMOD_OBJDIR = $(PHYMOD_BLDDIR)/obj
endif
override BLDDIR := $(PHYMOD_OBJDIR)

# Location to place libraries
ifndef PHYMOD_LIBDIR
PHYMOD_LIBDIR = $(PHYMOD_BLDDIR)
endif
LIBDIR := $(PHYMOD_LIBDIR)

# Option to retrieve compiler version
ifndef PHYMOD_CC_VERFLAGS
PHYMOD_CC_VERFLAGS := -v
endif
CC_VERFLAGS = $(PHYMOD_CC_VERFLAGS); 

# Default suffix for object files
ifndef PHYMOD_OBJSUFFIX
PHYMOD_OBJSUFFIX = o
endif
OBJSUFFIX = $(PHYMOD_OBJSUFFIX)

# Default suffix for library files
ifndef PHYMOD_LIBSUFFIX
PHYMOD_LIBSUFFIX = a
endif
LIBSUFFIX = $(PHYMOD_LIBSUFFIX)

#
# Set up compiler options, etc.
#

# Default include path
PHYMOD_INCLUDE_PATH = -I$(PHYMOD)/include
PHYMOD_INCLUDE_PATH += -I$(PHYMOD)/chip/

# Import preprocessor flags avoiding include duplicates
TMP_PHYMOD_CPPFLAGS := $(filter-out $(PHYMOD_INCLUDE_PATH),$(PHYMOD_CPPFLAGS))

# Convenience Makefile flags for building specific chips
ifdef PHYMOD_CHIPS
PHYMOD_DSYM_CPPFLAGS := -DPHYMOD_CONFIG_INCLUDE_CHIP_DEFAULT=0 
PHYMOD_DSYM_CPPFLAGS += $(foreach chip,$(PHYMOD_CHIPS),-DPHYMOD_CONFIG_INCLUDE_${chip}=1) 
endif
ifdef PHYMOD_NCHIPS
PHYMOD_DSYM_CPPFLAGS += $(foreach chip,$(PHYMOD_NCHIPS),-DPHYMOD_CONFIG_INCLUDE_${chip}=0)
endif
TMP_PHYMOD_CPPFLAGS += $(PHYMOD_DSYM_CPPFLAGS)

override CPPFLAGS = $(TMP_PHYMOD_CPPFLAGS) $(PHYMOD_INCLUDE_PATH)

# Import compiler flags
override CFLAGS = $(PHYMOD_CFLAGS)


#
# Define standard targets, etc.
#

ifdef LOCALDIR
override BLDDIR := $(BLDDIR)/$(LOCALDIR)
endif

ifndef LSRCS
LSRCS = $(wildcard *.c)
endif
ifndef LOBJS
LOBJS = $(addsuffix .$(OBJSUFFIX), $(basename $(LSRCS)))
endif
ifndef BOBJS
BOBJS = $(addprefix $(BLDDIR)/,$(LOBJS))
endif

# Use PHYMOD_QUIET=1 to control printing of compilation lines.
ifdef PHYMOD_QUIET
Q = @
endif
