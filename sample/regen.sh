test -f Makefile && make clean

rm -f *.h *.cc *.cpp Makefile *.conf

../codegen/phxrpc_pb2server -I ../ -I /usr/include -f search.proto -d . -u


