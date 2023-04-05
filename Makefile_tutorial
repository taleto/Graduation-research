#tutorial%: tutorial%.cc
#	   g++ $@.cc -o $@ -I../include -O2 -std=c++11 -pedantic -W -Wall -Wshadow -fPIC -pthread  -L../lib -Wl,-rpath,../lib -lpythia8 -ldl

tutorial4: tutorial4.cc
	   g++ $@.cc -o $@ -I../include -O2 -std=c++11 -L../lib -Wl,-rpath,../lib -lpythia8 -ldl $(ROOT_LIB) `root-config --cflags --glibs`

jetinfo: jetinfo.cc
	   g++ $@.cc -o $@ -I../include -O2 -std=c++11 -L../lib -Wl,-rpath,../lib -lpythia8 -ldl $(ROOT_LIB) `root-config --cflags --glibs`


dataset: dataset.cc
	   g++ $@.cc -o $@ -I../include -O2 -std=c++11 -L../lib -Wl,-rpath,../lib -lpythia8 -ldl $(ROOT_LIB) `root-config --cflags --glibs`
	   
Pbjetinfo: Pbjetinfo.cc
	   g++ $@.cc -o $@ -I../include -O2 -std=c++11 -L../lib -Wl,-rpath,../lib -lpythia8 -ldl $(ROOT_LIB) `root-config --cflags --glibs`
#fj_tutorial: fj_tutorial.cc
#		g++ $@.cc -o $@ -I../include -O2   -L../lib -Wl,-rpath, ../lib -lpythia8 -ldl $(ROOT_LIB) `root-config --cflags --glibs` -w $(CXX_COMMON) $(FASTJET3_INCLUDE) $(FASTJET3_LIB) `fastjet-install/bin/fastjet-config --cxxflags --libs --plugins`

# FASTJET3.
#fj_tutorial: fj_tutorial.cc
##ifeq ($(FASTJET3_USE),true)
 #   $(CXX) $@.cc -o $@ -w $(CXX_COMMON) $(FASTJET3_INCLUDE) $(FASTJET3_LIB)
#lse
 #   $(error Error: $@ requires FASTJET3)
#ndif


#tutorial4: tutorial4.cc
#	   g++ $@.cc -o $@ -I../include -O2 -std=c++11 -L../lib -Wl,-rpath,../lib -lpythia8 -ldl $(ROOT_LIB) `root-config --cflags --glibs` $(FASTJET3_INCLUDE) $(FASTJET3_LIB)
