#include <iostream>
#include <w32t.hpp>

void wmain(int argc, wchar_t** argv) {
    if (argc == 2) {
        W32t::ShowTimeConfig(argv[1]);
    }
    else if (argc == 5) {
        auto client{ W32t::Client(argv[1]) };
        client.setAuthData(argv[2], argv[3], argv[4]);
        if (client.connect()) {
            W32t::ShowTimeConfig(client);
        }
    }
    else {
        std::wcout << argv[0] << L" <server> [<domain> <username> <password>]\n";
    }
}