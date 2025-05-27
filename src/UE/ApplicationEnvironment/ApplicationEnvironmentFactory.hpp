#pragma once

#include "IApplicationEnvironment.hpp"
#include <memory>

namespace ue
{

std::unique_ptr<IApplicationEnvironment> createApplicationEnvironment(int &argc, char *argv[]);

}
