cmake_minimum_required(VERSION 3.22.1)
# The RpcTryExcept and RpcTryFinally macros used by the RPC class uses SEH
# If you would like to use these you should force the use of Win32 before the project is defined
# Otherwise, you may omit this line and generate solution files for Win32 and x64 builds
set(CMAKE_GENERATOR_PLATFORM Win32 CACHE INTERNAL "Force 32-bit compilation for SEH")
project(w32t-client
    VERSION "0.1"
    DESCRIPTION "An MS-W32T client for an example of working with RPC interfaces"
    HOMEPAGE_URL "https://github.com/EvanMcBroom/w32t-client"
    # DO NOT remove the C language
    # If you do the generated RPC stubs will not participate in the build
    LANGUAGES C CXX 
)

# Add the midl.cmake file to include the target_idl_sources function
include(cmake/midl.cmake)

set(client ${PROJECT_NAME})
add_executable(${client})
set_target_properties(${client} PROPERTIES
    # The Rpc::Client class uses some features of the newer C++ standards
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE
)
target_sources(${client} PRIVATE
    client.cpp
    rpc.hpp
    rpc.cpp
    w32t.hpp
    w32t.cpp
)
target_idl_sources(TARGET ${client} SOURCES ms-w32t.idl)
target_compile_definitions(${client} PUBLIC WIN32_LEAN_AND_MEAN)
target_precompile_headers(${client} PUBLIC <Windows.h>)