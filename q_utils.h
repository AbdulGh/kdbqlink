#pragma once

#include <string>

#include "extern_headers/k.h"

namespace kl {
  extern K err(const char* err_msg);
  extern S intern_string(const std::string& s);
  extern S intern_string(const char* s);
}
