#pragma once
#include <chrono>
#include <string>
#include <iostream>

std::string inline getTimestampStr()
{
    return to_string(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
}