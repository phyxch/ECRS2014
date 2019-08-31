name := ECRS
G4TARGET := $(name)
G4EXLIB := true
TSYLIBDIR :=./lib

ifndef G4INSTALL
  G4INSTALL = ../../..
endif
CPPFLAGS +=  -L$(TSYLIBDIR)
EXTRALIBS := -lFortranMagField -lg2c 
                                                                                
include $(G4INSTALL)/config/architecture.gmk  

ifndef G4LISTS_BASE
  EXTRALIBS += -L$(G4LIB)/.lists_build/$(G4SYSTEM)
  G4LISTS_BASE = $(G4INSTALL)/hadronic_lists
else
  EXTRALIBS += -L$(G4LISTS_BASE)/lists/.lists_build/lib/$(G4SYSTEM)
endif

.PHONY: all
all: lib bin link

link: bin

include $(G4INSTALL)/config/binmake.gmk
