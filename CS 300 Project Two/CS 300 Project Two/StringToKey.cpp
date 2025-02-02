#include "StringToKey.hpp"
#include <cctype>
#include <string>
#include <iostream>
using namespace std;

int stringToKey(const std::string& course) {
    int letterPart = 0;
    int numPart = 0;

    // Encode letters into a base-26 number
    for (char ch : course) {
        if (std::isalpha(ch)) {
            letterPart = letterPart * 26 + (ch - 'A' + 1);
        }
    }

    // Extract the numeric part
    for (char ch : course) {
        if (std::isdigit(ch)) {
            numPart = numPart * 10 + (ch - '0');
        }
    }

    // Merge letter and number parts uniquely
    return letterPart * 1000 + numPart; // Use a multiplier to avoid overlap
}