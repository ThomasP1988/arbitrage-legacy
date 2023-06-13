#pragma once
#include <sstream>
#include <string>
#include <iomanip>

std::string inline getStringFromDouble(double number)
{
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(8) << number;
    return stream.str();
}