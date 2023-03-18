#include <iostream>
#include <concurrent_vector.h>

int main() {
    tbb::concurrent_vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);

    for (auto& i : vec) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    return 0;
}
