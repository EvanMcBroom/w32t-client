#include <iostream>
#include <rpc.hpp>
#include <w32t.hpp>

namespace W32t {
    const RPC_WSTR endpoint{ reinterpret_cast<RPC_WSTR>(L"\\PIPE\\W32TIME_ALT") };
    const RPC_WSTR protoSeq{ reinterpret_cast<RPC_WSTR>(L"ncacn_np") };

    Rpc::Client Client(const std::wstring& server) {
        return Rpc::Client{ std::wstring{ server }, protoSeq, endpoint, W32tUuid() };
    }

    RPC_WSTR W32tUuid() {
        return Rpc::String(static_cast<RPC_CLIENT_INTERFACE*>(W32Time_v4_1_c_ifspec)->InterfaceId.SyntaxGUID);
    }
    
    void ShowTimeConfig(const Rpc::Client& client) {
        if (client.isConnected()) {
            wchar_t* source{ nullptr };
            auto result = client.call(W32TimeQuerySource, &source);
            if (result == ERROR_SUCCESS) {
                std::wcout << "Time source: " << source << std::endl;
            
                PW32TIME_CONFIGURATION_INFO configInfo{ nullptr };
                if (client.call(W32TimeQueryConfiguration, &configInfo) == ERROR_SUCCESS) {
                    for (int index{ 0 }; index < configInfo->cProviderConfig; index++) {
                        auto& providerName{ configInfo->pProviderConfig[index]->wszProviderName };
                        std::wcout << "Provider " << index + 1 << ": " << providerName << std::endl;
            
                        PW32TIME_CONFIGURATION_PROVIDER providerConfig{ nullptr };
                        if (client.call(W32TimeQueryProviderConfiguration, 0, providerName, &providerConfig) == ERROR_SUCCESS && providerConfig) {
                            std::wcerr << L"\tEnabled: " << ((providerConfig->ulEnabled) ? L"Yes" : L"No") << std::endl;
                            std::wcerr << L"\tDLL Name: " << providerConfig->wszDllName << std::endl;
                            midl_user_free(providerConfig);
                        }
            
                        PW32TIME_PROVIDER_INFO providerInfo{ nullptr };
                        if (client.call(W32TimeQueryProviderStatus, 0, providerName, &providerInfo) == ERROR_SUCCESS && providerInfo) {
                            switch (providerInfo->ulProviderType) {
                            case 0: {
                                auto ntpData{ reinterpret_cast<W32TIME_NTP_PROVIDER_DATA*>(providerInfo) };
                                std::wcerr << L"\tNTP provider peer count: " << ntpData->cPeerInfo << std::endl;
                                break;
                            }
                            case 1: {
                                auto hardwareData{ reinterpret_cast<W32TIME_HARDWARE_PROVIDER_DATA*>(providerInfo) };
                                std::wcerr << L"\tHardware provider id: " << hardwareData->wszReferenceIdentifier << std::endl;
                                break;
                            }
                            default:
                                std::wcerr << L"\tUnsupported provider status type" << std::endl;
                            }
                            midl_user_free(providerInfo);
                        }
                    }
                    midl_user_free(configInfo);
                }
                midl_user_free(source);
            }
        }
    }

    void ShowTimeConfig(const std::wstring& server) {
        auto client{ W32t::Client(server) };
        if (client.connect()) {
            ShowTimeConfig(client);
        }
    }
}