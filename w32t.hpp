#pragma once
#include <ms-w32t.h>
#include <rpc.hpp>
#include <string>

// https://docs.microsoft.com/en-us/openspecs/windows_protocols/ms-w32t/
// RPC interfaces registered as uuid(8fb6d884-2388-11d0-8c35-00c04fda2795)
namespace W32t {
    extern const RPC_WSTR endpoint;
    extern const RPC_WSTR protoSeq;
    extern const RPC_WSTR uuid;

    Rpc::Client Client(const std::wstring& server);

    // Shows the W32Time config for a Windows server
    // Written to show an example of using the Rpc::Client class
    void ShowTimeConfig(const Rpc::Client& client);
    void ShowTimeConfig(const std::wstring& server);

    RPC_WSTR W32tUuid();
}