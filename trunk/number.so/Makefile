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
outpath = ../bin/./number.so
INCLUDE_FLAGS =  -I.././number.so -I.././number.so/include -I.././include -I. -I./include
ORG_LIB_FLAGS = 
LIB_FLAGS =  $(ORG_LIB_FLAGS)
LIB_PATH = -L/usr/lib -L/usr/lib -L../bin/./number.so
olist = $(outpath)/./src/numbertype.o $(outpath)/./src/complexha.o $(outpath)/./src/dataarray.o $(outpath)/./src/basic.o $(outpath)/./src/highaccuracynumber.o       
sofilelistc = 
FLAGS = -W -Wall -DLINUX_
SO_FLAGS = -shared -fPIC
DEBUG_FLAGS = -g -D_DEBUG_
debug_o_list =  $(outpath)/./src/numbertype_debug.o $(outpath)/./src/complexha_debug.o $(outpath)/./src/dataarray_debug.o $(outpath)/./src/basic_debug.o $(outpath)/./src/highaccuracynumber_debug.o
debug_so_list = 
target_debug = libnumber_debug.so
DEBUGER = gdb
DEBUGER_FLAGS = 
hfilelist = ./libnumber.h ./include/number.so.h ./include/basic.h ./include/numbertype.h 
NEW_DIR =  $(outpath)/./src
InstallDir = /usr/bin
target = libnumber.so
existo = `find -type f -name "*.o"`
LIB_DIR = /usr/lib

.PHONY : all
all : $(NEW_DIR) $(outpath)/$(target) $(olist) $(moclist) $(sofilelistc)
$(outpath)/$(target) : $(NEW_DIR) $(olist) $(moclist) $(sofilelistc)
	$(LINK) $(FLAGS) $(OPT_FLAGS) $(LIB_PATH) $(SO_FLAGS) -o $(outpath)/$(target) $(olist) $(LIB_FLAGS)
.PHONY : debug
debug : $(NEW_DIR) $(outpath)/$(target_debug) $(debug_o_list) $(moclist) $(debug_so_list)
$(outpath)/$(target_debug) : $(NEW_DIR) $(debug_o_list) $(moclist) $(debug_so_list)
	$(LINK) $(FLAGS) $(DEBUG_FLAGS) $(LIB_PATH) $(SO_FLAGS) -o $(outpath)/$(target_debug) $(debug_o_list) $(debug_so_list) $(ORG_LIB_FLAGS)
$(outpath)/./src :
	mkdir -p $(outpath)/./src
$(outpath)/./src/numbertype.o:  src/numbertype.cpp .././number.so/include/number.so.h .././number.so/include/basic.h .././number.so/include/numbertype.h
	$(CXX) $(OPT_FLAGS) $(FLAGS) $(INCLUDE_FLAGS) -o $(outpath)/./src/numbertype.o ./src/numbertype.cpp
$(outpath)/./src/numbertype_debug.o:  src/numbertype.cpp .././number.so/include/number.so.h .././number.so/include/basic.h .././number.so/include/numbertype.h
	$(CXX) $(FLAGS) $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -o $(outpath)/./src/numbertype_debug.o ./src/numbertype.cpp
$(outpath)/./src/complexha.o:  src/complexha.cpp .././number.so/include/number.so.h .././number.so/include/basic.h .././number.so/include/numbertype.h
	$(CXX) $(OPT_FLAGS) $(FLAGS) $(INCLUDE_FLAGS) -o $(outpath)/./src/complexha.o ./src/complexha.cpp
$(outpath)/./src/complexha_debug.o:  src/complexha.cpp .././number.so/include/number.so.h .././number.so/include/basic.h .././number.so/include/numbertype.h
	$(CXX) $(FLAGS) $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -o $(outpath)/./src/complexha_debug.o ./src/complexha.cpp
$(outpath)/./src/dataarray.o:  src/dataarray.cpp .././number.so/include/number.so.h .././number.so/include/basic.h .././number.so/include/numbertype.h
	$(CXX) $(OPT_FLAGS) $(FLAGS) $(INCLUDE_FLAGS) -o $(outpath)/./src/dataarray.o ./src/dataarray.cpp
$(outpath)/./src/dataarray_debug.o:  src/dataarray.cpp .././number.so/include/number.so.h .././number.so/include/basic.h .././number.so/include/numbertype.h
	$(CXX) $(FLAGS) $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -o $(outpath)/./src/dataarray_debug.o ./src/dataarray.cpp
$(outpath)/./src/basic.o:  src/basic.cpp .././number.so/include/number.so.h .././number.so/include/basic.h .././number.so/include/numbertype.h
	$(CXX) $(OPT_FLAGS) $(FLAGS) $(INCLUDE_FLAGS) -o $(outpath)/./src/basic.o ./src/basic.cpp
$(outpath)/./src/basic_debug.o:  src/basic.cpp .././number.so/include/number.so.h .././number.so/include/basic.h .././number.so/include/numbertype.h
	$(CXX) $(FLAGS) $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -o $(outpath)/./src/basic_debug.o ./src/basic.cpp
$(outpath)/./src/highaccuracynumber.o:  src/highaccuracynumber.cpp .././number.so/include/number.so.h .././number.so/include/basic.h .././number.so/include/numbertype.h
	$(CXX) $(OPT_FLAGS) $(FLAGS) $(INCLUDE_FLAGS) -o $(outpath)/./src/highaccuracynumber.o ./src/highaccuracynumber.cpp
$(outpath)/./src/highaccuracynumber_debug.o:  src/highaccuracynumber.cpp .././number.so/include/number.so.h .././number.so/include/basic.h .././number.so/include/numbertype.h
	$(CXX) $(FLAGS) $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -o $(outpath)/./src/highaccuracynumber_debug.o ./src/highaccuracynumber.cpp
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
distclean : clean
	rm Makefile
.PHONY : install uninstall
install : all
	mkdir -p $(InstallDir)
	cp $(outpath)/$(target) $(LIB_DIR)/$(target)
uninstall :
	rm $(LIB_DIR)/$(target) &> /dev/null || true
	if [ "`ls -A1 $(InstallDir) | wc -l `" = "0" ] ; then rmdir $(InstallDir) ; fi
