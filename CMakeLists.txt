cmake_minimum_required(VERSION 3.7)
project(phxrpc)

set(CMAKE_CXX_STANDARD 11)
set(PRJ_ROOT /work/c-lib/phxrpc)

include_directories(${PRJ_ROOT} ${PRJ_ROOT}/)

add_definitions(-D_REENTRANT -D_GNU_SOURCE -D_XOPEN_SOURCE)
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -fPIC")

set(LIB_HTTP_OBJS
        phxrpc/http/http_client.cpp
        phxrpc/http/http_client.cpp
        phxrpc/http/http_msg.cpp
        phxrpc/http/http_proto.cpp
        )
set(LIB_FILE_OBJS
        phxrpc/file/log_utils.cpp
        phxrpc/file/file_utils.cpp
        phxrpc/file/opt_map.cpp
        phxrpc/file/config.cpp)

set(LIB_RPC_OBJS
        phxrpc/rpc/phxrpc.pb.cc
        phxrpc/rpc/http_caller.cpp
        phxrpc/rpc/server_config.cpp
        phxrpc/rpc/client_config.cpp
        phxrpc/rpc/socket_stream_phxrpc.cpp
        phxrpc/rpc/uthread_caller.cpp
        phxrpc/rpc/client_monitor.cpp
        phxrpc/rpc/server_monitor.cpp
        phxrpc/rpc/monitor_factory.cpp
        phxrpc/rpc/hsha_server.cpp
        phxrpc/rpc/server_base.cpp
        )
set(LIB_NETWORK_OBJS
        phxrpc/network/socket_stream_base.cpp
        phxrpc/network/uthread_runtime.cpp
        phxrpc/network/uthread_epoll.cpp
        phxrpc/network/socket_stream_block.cpp
        phxrpc/network/socket_stream_uthread.cpp
        phxrpc/network/uthread_context_util.cpp
        phxrpc/network/uthread_context_base.cpp
        phxrpc/network/uthread_context_system.cpp
        phxrpc/network/timer.cpp
        )
add_custom_command(
        OUTPUT ${PRJ_ROOT}/phxrpc/rpc/phxrpc.pb.cc
        COMMAND protoc -I/usr/include -I. --cpp_out=${PRJ_ROOT} phxrpc/rpc/phxrpc.proto
        MAIN_DEPENDENCY phxrpc/rpc/phxrpc.proto
        WORKING_DIRECTORY ${PRJ_ROOT}
)
ADD_LIBRARY(phxrpc STATIC ${LIB_RPC_OBJS} ${LIB_FILE_OBJS} ${LIB_HTTP_OBJS} ${LIB_NETWORK_OBJS})