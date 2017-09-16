
include ../../phxrpc.mk


all: test

clean:
	@( cd http ; make clean )
	@( cd rpc ; make clean )
	@( cd network ; make clean)

test:
	@( cd http ; make )
	@( cd rpc ; make )
	@( cd network ; make )
