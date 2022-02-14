#include <iostream>
#include <rpc.hpp>
#include <vector>

#pragma comment(lib, "Rpcrt4.lib")

// https://docs.microsoft.com/en-us/windows/win32/rpc/the-midl-user-allocate-function
// https://docs.microsoft.com/en-us/windows/win32/rpc/the-midl-user-free-function
extern "C" {
    void __RPC_FAR* __RPC_USER midl_user_allocate(size_t cBytes) { return malloc(cBytes); }
    void __RPC_USER midl_user_free(void* pBuffer) { free(pBuffer); }
}

namespace Rpc {
    Client::Client(const std::wstring& server, RPC_WSTR protoSeq, RPC_WSTR endpoint, RPC_WSTR uuid)
        : protoSeq(protoSeq), server(server), endpoint(endpoint), uuid(uuid) {
    }

    Client::~Client() {
        RpcStringFreeW(&this->stringBinding);
        if (connected) {
            RpcBindingFree(&this->binding);
        }
    }

    bool Client::connect() {
        RpcTryExcept
        if (RpcStringBindingComposeW(this->uuid, this->protoSeq, reinterpret_cast<RPC_WSTR>(server.data()), endpoint, nullptr, &this->stringBinding) == RPC_S_OK) {
            if (RpcBindingFromStringBindingW(this->stringBinding, &this->binding) == RPC_S_OK) {
                if (username.length() && password.length()) {
                    this->identity.Domain = reinterpret_cast<unsigned short*>(domain.data());
                    this->identity.DomainLength = domain.length();
                    this->identity.User = reinterpret_cast<unsigned short*>(this->username.data());
                    this->identity.UserLength = username.length();
                    this->identity.Password = reinterpret_cast<unsigned short*>(this->password.data());
                    this->identity.PasswordLength = password.length();
                    this->identity.Flags = SEC_WINNT_AUTH_IDENTITY_UNICODE;
                    if (RpcBindingSetAuthInfoW(this->binding, nullptr, RPC_C_AUTHN_LEVEL_PKT_PRIVACY, RPC_C_AUTHN_WINNT, &this->identity, 0) == RPC_S_OK) {
                        connected = true;
                    }
                }
                else {
                    connected = true;
                }
            }
        }
        else {
            std::wcerr << L"Error composing string for RPC binding: " << server << std::endl;
        }
        RpcExcept(EXCEPTION_EXECUTE_HANDLER)
            std::wcerr << L"Could not connect to: " << reinterpret_cast<LPWSTR>(this->stringBinding);
        RpcEndExcept
            return connected;
    }

    void Client::setAuthData(const std::wstring& domain, const std::wstring& username, const std::wstring& password) {
        this->domain = domain;
        this->username = username;
        this->password = password;
    }

    RPC_WSTR String(const UUID& uuid) {
        RPC_WSTR rpcString;
        return (UuidToStringW(&uuid, &rpcString) == RPC_S_OK) ? rpcString : nullptr;
    }
}