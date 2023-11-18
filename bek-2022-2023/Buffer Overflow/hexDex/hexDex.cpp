#include <iostream>
#include <string>
#include <sstream>

inline std::string converter(std::string & input, size_t chunk_size) noexcept {
    std::string to_return = "";

    for (size_t i=0; i<input.length(); i += chunk_size) {
        std::string chunk = input.substr(i, chunk_size);
        unsigned decimal_value = 0;
        std::stringstream ss;
        ss << std::hex << chunk;
        ss >> decimal_value;
        to_return += std::to_string(decimal_value) + ' ';
    }
    return to_return;
}


int main(void) {
    std::string input = "";
    size_t chunk_size = 0;
    while (1) {
        std::cout << "\nEnter your hex value: ";
        std::cin >> input;
        std::cout << "Enter chunk size of conversion: ";
        std::cin >> chunk_size;
        std::cout << "\nThe converted is: " << converter(input, chunk_size) << std::endl;
    }
    return EXIT_SUCCESS;
}