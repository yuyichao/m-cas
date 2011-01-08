########################################################
#
#Created By YYC Makefile Creator 0.8.2
#
########################################################
CXX = g++ -c -fPIC
CC = gcc -c -fPIC
OPT_FLAGS = -O2
LINK = g++
MAKER = make
outpath = bin
INCLUDE_FLAGS =  -I./number.so -I./number.so/include -I./include
ORG_LIB_FLAGS = 
LIB_FLAGS =  -lnumber $(ORG_LIB_FLAGS)
LIB_PATH = -L/usr/lib -Lbin/./number.so
olist = $(outpath)/./src/trantri.o $(outpath)/./src/replace.o $(outpath)/./src/readexp.o $(outpath)/./src/tranmul.o $(outpath)/./src/tranpow.o $(outpath)/./src/primary.o $(outpath)/./src/tranlog.o $(outpath)/./src/expression.o $(outpath)/./src/main.o $(outpath)/./src/tranadd.o $(outpath)/./src/exptostr.o $(outpath)/./src/compprimary.o $(outpath)/./src/realprimary.o $(outpath)/./src/exptrans.o       
sofilelistc =  bin/./number.so/libnumber.so
FLAGS = -W -Wall
SO_FLAGS = -shared -fPIC
DEBUG_FLAGS = -g -D_DEBUG_
debug_o_list =  $(outpath)/./src/trantri_debug.o $(outpath)/./src/replace_debug.o $(outpath)/./src/readexp_debug.o $(outpath)/./src/tranmul_debug.o $(outpath)/./src/tranpow_debug.o $(outpath)/./src/primary_debug.o $(outpath)/./src/tranlog_debug.o $(outpath)/./src/expression_debug.o $(outpath)/./src/main_debug.o $(outpath)/./src/tranadd_debug.o $(outpath)/./src/exptostr_debug.o $(outpath)/./src/compprimary_debug.o $(outpath)/./src/realprimary_debug.o $(outpath)/./src/exptrans_debug.o
debug_so_list =  bin/./number.so/libnumber_debug.so
target_debug = m-cas_debug
DEBUGER = gdb
DEBUGER_FLAGS = 
hfilelist = ./number.so/libnumber.h ./number.so/include/number.so.h ./number.so/include/basic.h ./number.so/include/numbertype.h ./include/expression.h ./include/primary.h ./include/main.h ./include/all.h 
NEW_DIR =  $(outpath)/./number.so/src $(outpath)/./src
InstallDir = /usr/bin
target = m-cas
existo = `find -type f -name "*.o"`
TAR = tar --bzip2 -c
TAR_NAME = $(target).tar.bz2
UNTAR = tar --bzip2 -x
BROWSER = nautilus

.PHONY : all
all : $(NEW_DIR) $(outpath)/$(target) $(olist) $(moclist) $(sofilelistc)
$(outpath)/$(target) : $(NEW_DIR) $(olist) $(moclist) $(sofilelistc)
	$(LINK) $(FLAGS) $(OPT_FLAGS) $(LIB_PATH) -o $(outpath)/$(target) $(olist) $(LIB_FLAGS)
.PHONY : debug
debug : $(NEW_DIR) $(outpath)/$(target_debug) $(debug_o_list) $(moclist) $(debug_so_list)
$(outpath)/$(target_debug) : $(NEW_DIR) $(debug_o_list) $(moclist) $(debug_so_list)
	$(LINK) $(FLAGS) $(DEBUG_FLAGS) $(LIB_PATH) -o $(outpath)/$(target_debug) $(debug_o_list) $(debug_so_list) $(ORG_LIB_FLAGS)
$(outpath)/./number.so/src :
	mkdir -p $(outpath)/./number.so/src
$(outpath)/./src :
	mkdir -p $(outpath)/./src
$(outpath)/./src/trantri.o:  src/trantri.cpp include/all.h number.so/include/numbertype.h include/expression.h include/primary.h include/main.h
	$(CXX) $(OPT_FLAGS) $(FLAGS) $(INCLUDE_FLAGS) -o $(outpath)/./src/trantri.o ./src/trantri.cpp
$(outpath)/./src/trantri_debug.o:  src/trantri.cpp include/all.h number.so/include/numbertype.h include/expression.h include/primary.h include/main.h
	$(CXX) $(FLAGS) $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -o $(outpath)/./src/trantri_debug.o ./src/trantri.cpp
$(outpath)/./src/replace.o:  src/replace.cpp include/all.h number.so/include/numbertype.h include/expression.h include/primary.h include/main.h
	$(CXX) $(OPT_FLAGS) $(FLAGS) $(INCLUDE_FLAGS) -o $(outpath)/./src/replace.o ./src/replace.cpp
$(outpath)/./src/replace_debug.o:  src/replace.cpp include/all.h number.so/include/numbertype.h include/expression.h include/primary.h include/main.h
	$(CXX) $(FLAGS) $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -o $(outpath)/./src/replace_debug.o ./src/replace.cpp
$(outpath)/./src/readexp.o:  src/readexp.cpp include/all.h number.so/include/numbertype.h include/expression.h include/primary.h include/main.h
	$(CXX) $(OPT_FLAGS) $(FLAGS) $(INCLUDE_FLAGS) -o $(outpath)/./src/readexp.o ./src/readexp.cpp
$(outpath)/./src/readexp_debug.o:  src/readexp.cpp include/all.h number.so/include/numbertype.h include/expression.h include/primary.h include/main.h
	$(CXX) $(FLAGS) $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -o $(outpath)/./src/readexp_debug.o ./src/readexp.cpp
$(outpath)/./src/tranmul.o:  src/tranmul.cpp include/all.h number.so/include/numbertype.h include/expression.h include/primary.h include/main.h
	$(CXX) $(OPT_FLAGS) $(FLAGS) $(INCLUDE_FLAGS) -o $(outpath)/./src/tranmul.o ./src/tranmul.cpp
$(outpath)/./src/tranmul_debug.o:  src/tranmul.cpp include/all.h number.so/include/numbertype.h include/expression.h include/primary.h include/main.h
	$(CXX) $(FLAGS) $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -o $(outpath)/./src/tranmul_debug.o ./src/tranmul.cpp
$(outpath)/./src/tranpow.o:  src/tranpow.cpp include/all.h number.so/include/numbertype.h include/expression.h include/primary.h include/main.h
	$(CXX) $(OPT_FLAGS) $(FLAGS) $(INCLUDE_FLAGS) -o $(outpath)/./src/tranpow.o ./src/tranpow.cpp
$(outpath)/./src/tranpow_debug.o:  src/tranpow.cpp include/all.h number.so/include/numbertype.h include/expression.h include/primary.h include/main.h
	$(CXX) $(FLAGS) $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -o $(outpath)/./src/tranpow_debug.o ./src/tranpow.cpp
$(outpath)/./src/primary.o:  src/primary.cpp include/all.h number.so/include/numbertype.h include/expression.h include/primary.h include/main.h
	$(CXX) $(OPT_FLAGS) $(FLAGS) $(INCLUDE_FLAGS) -o $(outpath)/./src/primary.o ./src/primary.cpp
$(outpath)/./src/primary_debug.o:  src/primary.cpp include/all.h number.so/include/numbertype.h include/expression.h include/primary.h include/main.h
	$(CXX) $(FLAGS) $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -o $(outpath)/./src/primary_debug.o ./src/primary.cpp
$(outpath)/./src/tranlog.o:  src/tranlog.cpp include/all.h number.so/include/numbertype.h include/expression.h include/primary.h include/main.h
	$(CXX) $(OPT_FLAGS) $(FLAGS) $(INCLUDE_FLAGS) -o $(outpath)/./src/tranlog.o ./src/tranlog.cpp
$(outpath)/./src/tranlog_debug.o:  src/tranlog.cpp include/all.h number.so/include/numbertype.h include/expression.h include/primary.h include/main.h
	$(CXX) $(FLAGS) $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -o $(outpath)/./src/tranlog_debug.o ./src/tranlog.cpp
$(outpath)/./src/expression.o:  src/expression.cpp include/all.h number.so/include/numbertype.h include/expression.h include/primary.h include/main.h
	$(CXX) $(OPT_FLAGS) $(FLAGS) $(INCLUDE_FLAGS) -o $(outpath)/./src/expression.o ./src/expression.cpp
$(outpath)/./src/expression_debug.o:  src/expression.cpp include/all.h number.so/include/numbertype.h include/expression.h include/primary.h include/main.h
	$(CXX) $(FLAGS) $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -o $(outpath)/./src/expression_debug.o ./src/expression.cpp
$(outpath)/./src/main.o:  src/main.cpp include/all.h number.so/include/numbertype.h include/expression.h include/primary.h include/main.h
	$(CXX) $(OPT_FLAGS) $(FLAGS) $(INCLUDE_FLAGS) -o $(outpath)/./src/main.o ./src/main.cpp
$(outpath)/./src/main_debug.o:  src/main.cpp include/all.h number.so/include/numbertype.h include/expression.h include/primary.h include/main.h
	$(CXX) $(FLAGS) $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -o $(outpath)/./src/main_debug.o ./src/main.cpp
$(outpath)/./src/tranadd.o:  src/tranadd.cpp include/all.h number.so/include/numbertype.h include/expression.h include/primary.h include/main.h
	$(CXX) $(OPT_FLAGS) $(FLAGS) $(INCLUDE_FLAGS) -o $(outpath)/./src/tranadd.o ./src/tranadd.cpp
$(outpath)/./src/tranadd_debug.o:  src/tranadd.cpp include/all.h number.so/include/numbertype.h include/expression.h include/primary.h include/main.h
	$(CXX) $(FLAGS) $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -o $(outpath)/./src/tranadd_debug.o ./src/tranadd.cpp
$(outpath)/./src/exptostr.o:  src/exptostr.cpp include/all.h number.so/include/numbertype.h include/expression.h include/primary.h include/main.h
	$(CXX) $(OPT_FLAGS) $(FLAGS) $(INCLUDE_FLAGS) -o $(outpath)/./src/exptostr.o ./src/exptostr.cpp
$(outpath)/./src/exptostr_debug.o:  src/exptostr.cpp include/all.h number.so/include/numbertype.h include/expression.h include/primary.h include/main.h
	$(CXX) $(FLAGS) $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -o $(outpath)/./src/exptostr_debug.o ./src/exptostr.cpp
$(outpath)/./src/compprimary.o:  src/compprimary.cpp include/all.h number.so/include/numbertype.h include/expression.h include/primary.h include/main.h
	$(CXX) $(OPT_FLAGS) $(FLAGS) $(INCLUDE_FLAGS) -o $(outpath)/./src/compprimary.o ./src/compprimary.cpp
$(outpath)/./src/compprimary_debug.o:  src/compprimary.cpp include/all.h number.so/include/numbertype.h include/expression.h include/primary.h include/main.h
	$(CXX) $(FLAGS) $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -o $(outpath)/./src/compprimary_debug.o ./src/compprimary.cpp
$(outpath)/./src/realprimary.o:  src/realprimary.cpp include/all.h number.so/include/numbertype.h include/expression.h include/primary.h include/main.h
	$(CXX) $(OPT_FLAGS) $(FLAGS) $(INCLUDE_FLAGS) -o $(outpath)/./src/realprimary.o ./src/realprimary.cpp
$(outpath)/./src/realprimary_debug.o:  src/realprimary.cpp include/all.h number.so/include/numbertype.h include/expression.h include/primary.h include/main.h
	$(CXX) $(FLAGS) $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -o $(outpath)/./src/realprimary_debug.o ./src/realprimary.cpp
$(outpath)/./src/exptrans.o:  src/exptrans.cpp include/all.h number.so/include/numbertype.h include/expression.h include/primary.h include/main.h
	$(CXX) $(OPT_FLAGS) $(FLAGS) $(INCLUDE_FLAGS) -o $(outpath)/./src/exptrans.o ./src/exptrans.cpp
$(outpath)/./src/exptrans_debug.o:  src/exptrans.cpp include/all.h number.so/include/numbertype.h include/expression.h include/primary.h include/main.h
	$(CXX) $(FLAGS) $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -o $(outpath)/./src/exptrans_debug.o ./src/exptrans.cpp
bin/./number.so/libnumber.so : ./number.so/Makefile  ./number.so/libnumber.h ./number.so/include/number.so.h ./number.so/include/basic.h ./number.so/include/numbertype.h  ./number.so/src/numbertype.cpp ./number.so/src/complexha.cpp ./number.so/src/dataarray.cpp ./number.so/src/basic.cpp ./number.so/src/highaccuracynumber.cpp 
	cd ./number.so && $(MAKER)
bin/./number.so/libnumber_debug.so : ./number.so/Makefile  ./number.so/libnumber.h ./number.so/include/number.so.h ./number.so/include/basic.h ./number.so/include/numbertype.h  ./number.so/src/numbertype.cpp ./number.so/src/complexha.cpp ./number.so/src/dataarray.cpp ./number.so/src/basic.cpp ./number.so/src/highaccuracynumber.cpp 
	cd ./number.so && $(MAKER) debug
.PHONY : clean cleano cleanmoc cleanso cleantarget distclean cleanold
cleanold :
	rm `find -name "*~"` 2> /dev/null || true
clean : cleanold cleano cleanmoc cleanso cleantarget cleandebug
	for pathname in `find . -depth \( ! -regex '.*/\..*' \) -type d` ; do if [ "`ls -A1 $${pathname} | wc -l`" = "0" ] ; then rmdir $${pathname} ; fi ; done
cleanmoc :
	rm $(moclist) 2> /dev/null || true
cleano :
	rm $(existo) 2> /dev/null || true
cleantarget :
	rm $(outpath)/$(target) 2> /dev/null || true
.PHONY : cleandebug
cleandebug :
	rm $(outpath)/$(target_debug) 2> /dev/null || true
cleanso :
	cd ./number.so && $(MAKER) clean || true
distclean : clean
	cd ./number.so && $(MAKER) distclean || true
	rm Makefile
.PHONY : run rundebug
run : debug
	$(outpath)/$(target_debug)
rundebug : debug
	$(DEBUGER) $(DEBUGER_FLAGS) $(outpath)/$(target_debug)
.PHONY : install uninstall
install : all
	mkdir -p $(InstallDir)
	cp $(outpath)/$(target) $(InstallDir)/$(target)
	cd ./number.so && $(MAKER) install || true
uninstall :
	rm $(InstallDir)/$(target) &> /dev/null || true
	if [ "`ls -A1 $(InstallDir) | wc -l `" = "0" ] ; then rmdir $(InstallDir) ; fi
	cd ./number.so && $(MAKER) uninstall || true
.PHONY : tar ctar untar
tar : ctar
	$(TAR) `for name in \`find -type f \( ! -regex '.*/\..*' \)\` ; do echo $${name#./}; done ` > $(TAR_NAME)
untar :
	cat $(TAR_NAME) | $(UNTAR)
ctar :
	rm $(TAR_NAME) 2> /dev/null || true
.PHONY : bro
bro :
	$(BROWSER) .
