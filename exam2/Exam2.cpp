#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <vector>

#include "Exam2.hpp"

void SampleClass::displayMessage() const
{
  std::cout << "Object-oriented programming rocks!" << std::endl;
}
// int main()[string args[]]) {
// std::vector<int> series = {1, 2, 3, 5, 8, 13};
// series.insert(series.begin() + 4, 4);
// series.push_back(21);
// series.insert(series.begin() + 8, 8);
// series.erase(series.begin() + 1);
// series.pop_back();
// series.erase(series.begin() + 2);
// for (int num : series) {
//     std::cout << num << " ";
// }
// std::cout << std::endl;
// }

// struct Chip
// {
//     int id;
//     std::string name;
//     int transistors;
// };

// std::vector<Chip> chips(5);
// chips[0] = {7400, "nand2", 40},
// chips[1] = {7430, "nand8", 50},
// chips[2] = {7408, "and2", 40},
// chips[3] = {7414, "hex_inv", 60},
// chips.push_back({7473, "jk_ff", 90});
// chips[2] = {7410, "nand4", 60};
// chips.push_back({7486, "xor2", 70});
// chips.pop_back();