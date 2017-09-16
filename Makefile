
version = 0.8.0

all:
	@( cd phxrpc; make )
	@( cd codegen; make )
	@( cd sample; test -f Makefile || ./regen.sh; make )

boost:
	@( cd plugin_boost; make )

clean:
	@( rm -rf lib/*; )
	@( cd phxrpc; make clean )
	@( cd codegen; make clean )
	@( cd sample; test -f Makefile && make clean )
	@( cd plugin_boost; make clean )
	@( cd plugin_darwin; make clean )

