#include "Exception.h"

// C++ includes
#include <algorithm>
#include <sstream>

namespace ReactionsGenerator {
namespace internal {
/// Creates the location string from the file name and line number.
/// The result of this function on the file `/home/user/gitThermoFun/ThermoFun/src/Substance.cpp`
/// will be `ThermoFun/src/Substance.cpp`.
/// @param file The full path to the file
/// @param line The line number
std::string location(const std::string& file, int line)
{
    std::string str = "ThermoFun/";
    auto pos = std::find_end(file.begin(), file.end(), str.begin(), str.end()) - file.begin();
    std::stringstream ss;
    ss << file.substr(pos, file.size() - pos) << ":" << line;
    return ss.str();
}

std::string message(const Exception& exception, const std::string& file, int line)
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
    message << "*** Location: This error was encountered in " << loc << "." << std::endl;
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

} // namespace ThermoFun
