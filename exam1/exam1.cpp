#define CATCH_CONFIG_MAIN

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cmath>
using namespace std;

void swap(std::string input, int idex1, int idex2);
bool binary_search(int arr[], int val, int start, int end);
// Uncomment this part if doing catch test
// #include "catch.hpp"
// #include "exam1.hpp"
// TEST_CASE("Your Exam 1 test cases", "[exam1]")
// {
//     // just a placeholder
//     int course = 301;
//     int course_copy = 201 + 100;
//     std::cout << "Good luck with ECE " << course << std::endl;
//     REQUIRE(course == course_copy);
// }

// Uncomment this part if doing regular coding
int main(){
    // cout << "Good luck!" << endl;

//     int year = 2025, result = 0, divisor;
// for (divisor = 1; divisor < 10; divisor++)
// {
//     result += (year % divisor == 0) ? 1 : 0;
// }
// std::cout << result << std::endl;

// int codeBug = 404, eceFix = 103;
// char extension = 'f';
// // Line 3
// codeBug = (eceFix = 1) * 301;
// cout << codeBug << endl;
// char grade = 'A';

// if (grade >= 65 && grade <= 70) {

// std::cout << "Your final grade for ECE0301 is " << grade << std::endl;
// }
// }
// std::string input = "Square of 45";
//     int of_pos = input.find("of");
//     if (of_pos == std::string::npos)
//     {
//         std::cout << "Error: cannot find 'of' in input string" << std::endl;
//         return EXIT_FAILURE;
//     }
//     double number = std::stod(input.substr(of_pos + 3));
//     double result = number * number;

//     std::cout << "Where do you want to store the result? (c for console, f for file): ";
//     char answer;
//     std::cin >> answer;
//     if (answer == 'c')
//     {
//         std::cout << std::fixed << std::setprecision(2);
//         std::cout << result << std::endl;
//     }
//     else
//     {void swap(std::string input, int idex1, int idex2)
//         std::ofstream outfile;
//         outfile.open("result.txt");
//         outfile << result << std::endl;
//         outfile.close();
//     }
//     return EXIT_SUCCESS;
// int usr_number;
// std::cin >> usr_number;
// if (std::cin.good())
// {
//     std::cout << "Your input: ";
//     std::cout << std::fixed << std::setprecision(3);
//     std::cout << usr_number << std::endl;
// }
// else
// {
//     std::cout << "Invalid input" << std::endl;
// }
// int main(int argc, char *argv[])
// {
    // if (argc > 4)
    // {
    //     std::cout << "Usage: only need" << " argv[1]" << " argv[2]" << " argv[3]";
    //     return EXIT_FAILURE;
    // }
    // else if (argc == 3)
    // {
    //     std::cout << argv[1] << " " << argv[2] << " " << argc << std::endl;
    //     return EXIT_FAILURE;
    // }
    // else
    // {
    //     std::cout << "Result is " << std::stoi(argv[1]) + argc  << std::endl;
    //     return EXIT_SUCCESS;
    // }
// P = 5, I = 6;
// P = 5, R = 0.1;
// ifstream file_read;
// ofstream file_write;
// string line;
// std::cout << "Begin file processing" << std::endl;

// while (getline(file_read, line))
// {
//     file_write << line;
// }
// file_write << std::endl;
// file_write << "End of file" << std::endl;
// }
// int result;
// string str =  "PITT.800.797";
// try
// {
//     result = std::stoi(str);
//     std::cout << "result" << std::endl;
// }
// catch (std::invalid_argument)
// {
//     std::cout << "Invalid input" << std::endl;
// }
// string starting_str = "I love ECE301!";
// int index = starting_str.find("l");

// if (index != std::string::npos)
// {
//     string str = starting_str.substr(index,6);
// }
// else
// {
//     string str = "Not found";
// }
// }
//int main() {
//     std::string input = "antelope";
//     int index1 = 1, index2 = 4;
//     swap(input, index1, index2);
//     cout << input << endl;
//     return 0;
// }

// void swap(std::string input, int idx1, int idx2) {
//     char temp = input[idx1];
//     input[idx1] = input[idx2];
//     input[idx2] = temp;
// }
// std::string input = "elephant";
//     int index1 = 2, index2 = 5;
//     swap(input, index1, index2);
//     return 0;
const int size = 9;
int arr[size] = {0, 2, 4, 6, 8, 10, 12, 20, 25};
int searchval = 9;
bool found = binary_search(arr, searchval, 0, size);
}
// // Assume the binary_search function implementation below:

bool binary_search(int arr[], int val, int start, int end) {
    int mid = start+(end-start)/2;
    if (val == arr[mid]) {
        return true;
    }
    if (start == end-1) {
        return false;
    }
    if (val > arr[mid]) {
        return binary_search(arr, val, mid, end);
    } else {
        return binary_search(arr, val, start, mid);
    }
}
// Start = 0, End = 9

// void swap(std::string input, int idex1, int idex2) {
//     char temp = input[idex1];
//     input[idex1] = input[idex2];
//     input[idex2] = temp;
//     cout << input << endl;
// }
// double exam_equation(int a, double x, int n) {
//    if ( n < 0 || a < 0 ) {
//      return x ; 
//     }
//    double res = 0;
//    for (int i=1; i < n ; i++) {
//         res = x + pow(a/i, 2) ;
//     }
//     return res;
//}
//}
