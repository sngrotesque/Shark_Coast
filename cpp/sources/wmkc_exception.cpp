#include <wmkc_exception.hpp>

void wmkcErr_exception(wmkcErr_obj err)
{
    std::string message = std::string(err.func) + "[" + std::to_string(err.code) + "]: " + err.message;
    throw std::runtime_error(message);
}

void wmkcErr_exception(wmkcSSize errCode, std::string funcName, std::string errMessage)
{
    throw std::string(funcName + "[" + std::to_string(errCode) + "]: " + errMessage);
}
