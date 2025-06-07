#include "q_utils.h"

namespace kl{
  K err(const char* s) {
    return krr(intern_string(s));
  }

  S intern_string(const char* s) {
    return ss(const_cast<char*>(s));
  }

  S intern_string(const std::string& s) {
    return intern_string(s.c_str());
  }
}
