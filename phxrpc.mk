#--------------------------------------------------------------------

where-am-i = $(abspath $(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST)))

#$(warning $(dir $(call where-am-i)))

PHXRPC_ROOT=$(dir $(call where-am-i))

OS := $(shell uname)

PROTOBUF_ROOT=/usr
BOOST_ROOT=/usr/include/boost

PLUGIN_BOOST_LDFLAGS = -Wl,--whole-archive -L$(PHXRPC_ROOT)/lib/ -lphxrpc_plugin_boost \
		-Wl,--no-whole-archive -L$(BOOST_ROOT)/lib/ -lboost_context

#--------------------------------------------------------------------

ifeq ($(debug),y)
# (1) Debug
	OPT = -g2
else
# (2) Production
	OPT = -O2
endif

CXX = g++
AR = ar cru

SOFLAGS = -shared -Wl,-h,$@
LINKER = $(CC)
LINT = lint -c
RM = /bin/rm -f

CXXFLAGS = -std=c++11 -Wall -D_REENTRANT -D_GNU_SOURCE -D_XOPEN_SOURCE -fPIC -m64 $(OPT) \
		 -I$(PROTOBUF_ROOT)/include \
		 -I$(PHXRPC_ROOT)

LDFLAGS = -L$(PROTOBUF_ROOT)/lib/ $(PROTOBUF_ROOT)/lib//x86_64-linux-gnu/libprotobuf.a \
		  -lstdc++ -lpthread -lm

PBFLAGS = -I $(PROTOBUF_ROOT)/include -I $(PHXRPC_ROOT)

#--------------------------------------------------------------------

# make rule
%.o : %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
