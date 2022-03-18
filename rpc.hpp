// Copyright (C) 2022 Evan McBroom

#pragma once
#include <rpc.h>
#include <string>

// We include rpcndr.h for the error_status_t typedef, but that will add annotations to the midl_user_* functions
// with Microsoft's source-code annotation language (SAL). We could modify the function signatures to match these
// annotations but there is no guarantee they will be consistent across SDK functions. So instead we will disable
// the two warnings regarding the mismatch in function annotations to prevent the VS projects with default settings
// from yelling at us.
#include <rpcndr.h>
#pragma warning(disable : 28251)
#pragma warning(disable : 28252)

// Function signatures:
// https://docs.microsoft.com/en-us/windows/win32/rpc/the-midl-user-allocate-function
// https://docs.microsoft.com/en-us/windows/win32/rpc/the-midl-user-free-function
extern "C" {
    void* __RPC_USER midl_user_allocate(_In_ size_t size);
    void __RPC_USER midl_user_free(void* pBuffer);
}


namespace Rpc {
    class Client {
    public:
        Client(const std::wstring& server, RPC_WSTR protoSeq, RPC_WSTR endpoint, RPC_WSTR uuid = nullptr);
        ~Client();

        template<class Func, class... Args>
        error_status_t call(Func function, Args... arguments) const {
            RpcTryExcept
                return function(this->binding, arguments...);
            RpcExcept(EXCEPTION_EXECUTE_HANDLER)
                std::wcerr << L"Exception during RPC function call for binding: " << reinterpret_cast<LPWSTR>(this->stringBinding) << std::endl;
                std::wcerr << GetExceptionCode() << std::endl;
                return GetExceptionCode();
            RpcEndExcept
        }
        bool connect();
        auto isConnected() const { return connected; }
        auto rpcString() const { return stringBinding; }
        void setAuthData(const std::wstring& domain, const std::wstring& username, const std::wstring& password);

    private:
        RPC_BINDING_HANDLE binding{ INVALID_HANDLE_VALUE };
        bool connected{ false };
        std::wstring domain;
        RPC_WSTR endpoint;
        SEC_WINNT_AUTH_IDENTITY_W identity = { 0 };
        std::wstring password;
        RPC_WSTR protoSeq;
        std::wstring server;
        RPC_WSTR stringBinding{ nullptr };
        std::wstring username;
        RPC_WSTR uuid;
    };

    RPC_WSTR String(const UUID& uuid);
}