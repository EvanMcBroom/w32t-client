# MS-W32T Client

An example RPC client for the [MS-W32T server interfaces](https://docs.microsoft.com/en-us/openspecs/windows_protocols/ms-w32t/82603ee2-d358-42b9-8e56-434633ce186d).
The main goal of this project is to show how to integrate the use of the Microsoft IDL compiler (e.g. `midl.exe`) into a software project managed by CMake.

The project provides a [CMake](https://cmake.org/) file that defines a new function:

```
target_idl_sources(<target> <CLIENT|SERVER> [items1...])
```

The `target_idl_sources` function may be used to generate source code with the client or server stub functions for a given set of IDL files.
The generated source code will be added to the list of source files for a target.

## Building

The MS-W32T client requires [CMake](https://cmake.org/) and a Windows SDK to generate and run the build system files for your platform.
All Windows 10 and 11 SDKs are supported.

```
git clone https://github.com/EvanMcBroom/w32t-client.git
cd w32t-client
mkdir builds
cd builds
cmake ..
cmake --build .
```

CMake will build the example `w32t-client` utility which may be used to display information about sources of time for servers you have the permissions to query.

```
w32t-client.exe
w32t-client.exe <server> [<domain> <username> <password>]

w32t-client.exe example-server.local
Time source: Free-running System Clock
Provider 1: NtpClient
        Enabled: Yes
        DLL Name: C:\Windows\system32\w32time.dll
        NTP provider peer count: 8
Provider 2: NtpServer
        Enabled: Yes
        DLL Name: C:\Windows\system32\w32time.dll
        NTP provider peer count: 8
Provider 3: VMICTimeProvider
        Enabled: Yes
        DLL Name: C:\Windows\System32\vmictimeprovider.dll
```

## Modifying

The MS-W32T client project is designed to be usable as a template for creating RPC clients for other interfaces.
To use the project as a template for other interfaces:

- Replace the `w32t.hpp` and `w32t.cpp` files with appropriate source files for your interfaces
- Replace the `ms-w32t.idl` file with appropriate IDL files for your interfaces
- Update `CMakeLists.txt` to reflect these file changes

Please reference these files as needed to see how to provide appropriate replacements.
