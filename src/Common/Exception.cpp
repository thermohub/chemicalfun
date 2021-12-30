// ChemicalFun is a C++ and Python library 
// for Chemical Formula Parser and Reactions Generator.
//
// Copyright (C) 2018-2022 G.D.Miron, D.Kulik, S.Dmytriieva
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include "Exception.h"

// C++ includes
#include <algorithm>
#include <sstream>

namespace ChemicalFun {
namespace internal {
/// Creates the location string from the file name and line number.
/// The result of this function on the file `/home/user/gitThermoFun/ThermoFun/src/Substance.cpp`
/// will be `ThermoFun/src/Substance.cpp`.
/// @param file The full path to the file
/// @param line The line number
std::string location(const std::string& file, int line)
{
    std::string str = "ChemicalFun/";
    auto pos = std::find_end(file.begin(), file.end(), str.begin(), str.end()) - file.begin();
    std::stringstream ss;
    ss << file.substr(pos, file.size() - pos) << ":" << line;
    return ss.str();
}

std::string message(const Exception& exception, const std::string& file, int /*line*/)
{
    std::string error = exception.error.str();
    std::string reason = exception.reason.str();
    int line_ = exception.line;
    std::string loc = location(file, line_);
    unsigned length = std::max(error.size(), std::max(reason.size(), loc.size())) + 16;
    std::string bar(length, '*');
    std::stringstream message;
    message << std::endl;
    message << bar << std::endl;
    message << "*** Error: " << error << std::endl;
    message << "*** Reason: " << reason << std::endl;
    //message << "*** Location: This error was encountered in " << loc << "." << std::endl;
    message << bar << std::endl;
    message << std::endl;
    return message.str();
}
}

auto errorSubstanceNotFound(std::string function, std::string name, int line) -> void
{
    Exception exception;
    exception.error << "Substance not found.";
    exception.reason << "In function " << function << " substance " << name << " was not found.";
    exception.line = line;
    RaiseError(exception);
}


//auto errorModelParameters(std::string type, std::string name, int line) -> void
//{
//    Exception exception;
//    exception.error << "The parameters of the model are not correctly defiend";
//    exception.reason << "The " << type <<  " parameters for " << name << " were not correctly defined. ";
//    exception.line = line;
//    RaiseError(exception);
//}

auto funError(const std::string& error, const std::string& reason, const int& line, const std::string& file) -> void
{
    Exception exception;
    exception.error << error;
    exception.reason << reason;
    exception.line = line;
    RaiseError(exception);
}

auto funErrorIf(bool condition, const std::string& error, const std::string& reason, const int& line, const std::string& file) -> void
{
    if (condition)
        funError(error, reason, line, file);
}


} // namespace ThermoFun
