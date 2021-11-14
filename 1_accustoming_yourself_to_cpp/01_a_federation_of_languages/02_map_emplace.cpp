#include <iostream>
#include <map>
#include <string_view>

void print_map(std::string_view comment, const std::map<std::string, int>& m)
{
    std::cout << comment;
    for (const auto& [key, value] : m) {
        std::cout << key << " = " << value << "; ";
    }
    std::cout << "\n";
}

int main()
{
    // Create a map (that map to integers)
    std::map<std::string, int> m;

    print_map("Initial map: ", m);

    if (m["key0"]) {
        std::cout << "contains key0" << std::endl;
    }

    print_map("Updated map: ", m);

    /*
     * Initial map:
     * Updated map: key0 = 0;
     */
}