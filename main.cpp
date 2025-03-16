#include <DSP_lib.h>
#include <iostream>

int main() {
    DSP::libver version = DSP::lib_version();
    std::cout << "DSPE Library Version: " << version.major << "." << version.minor << std::endl;

    std::string version_info = DSP::lib_version_string();
    std::cout << version_info << std::endl;
    int a;
    std::cin >> a;

    return 0;
}