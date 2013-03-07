CC = g++
CFLAGS = -Wall -c -g

ROOFIT_INCLUDE := $(shell cd $(CMSSW_BASE); scram tool info roofitcore | grep INCLUDE= | sed 's|INCLUDE=||')
ROOFIT_LIBDIR := $(shell cd $(CMSSW_BASE); scram tool info roofitcore | grep LIBDIR= | sed 's|LIBDIR=||')

INCLUDES = -I. -I$(ROOTSYS)/include  -I$(ROOFIT_INCLUDE)/ -I$(CMSSW_BASE)/src/ -I$(CMSSW_BASE)/src/Francesco/ -I$(CMSSW_BASE)/src/HiggsAnalysis/CombinedLimit/interface

ROOTSYS  ?= ERROR_RootSysIsNotDefined

ROOTFLAG = `${ROOTSYS}/bin/root-config --cflags --libs` 


EXTRALIBS  :=  -L$(ROOTSYS)/lib -L$(ROOFIT_LIBDIR)/ -L$(CMSSW_BASE)/lib/slc5_amd64_gcc462/ -lHtml -lMathCore -lGenVector -lMinuit -lEG -lRooFitCore -lRooFit



fitSidebands: fitSidebands.cpp SidebandFitter.o 
	$(CC) -Wall $(INCLUDES) -o fitSidebands fitSidebands.cpp SidebandFitter.o  $(ROOTFLAG) $(EXTRALIBS) -lHiggsAnalysisCombinedLimit


#RooLevelledExp.o
fitBackground: fitBackground.cpp 
	$(CC) -Wall $(INCLUDES) $(ROOFITINCS)  fitBackground.cpp  $(ROOTFLAG) $(EXTRALIBS) -lHiggsAnalysisCombinedLimit    -o fitBackground 


plotBackgroundFit: plotBackgroundFit.cpp
	$(CC) -Wall $(INCLUDES) $(ROOFITINCS) plotBackgroundFit.cpp   $(ROOTFLAG) $(EXTRALIBS) -lHiggsAnalysisCombinedLimit    -o plotBackgroundFit

SidebandFitter.o: SidebandFitter.cc 
	$(CC) $(CFLAGS) $(INCLUDES) SidebandFitter.cc $(ROOTFLAG) $(EXTRALIBS) -lHiggsAnalysisCombinedLimit


create_datacards: create_datacards.cpp DataCardUtils.o
	$(CC) -Wall $(INCLUDES) -o create_datacards create_datacards.cpp DataCardUtils.o  $(ROOTFLAG) $(EXTRALIBS) -lHiggsAnalysisCombinedLimit


DataCardUtils.o: DataCardUtils.cc 
	$(CC) $(CFLAGS) $(INCLUDES) DataCardUtils.cc  $(ROOTFLAG) $(EXTRALIBS)


clean:
	rm *.o
