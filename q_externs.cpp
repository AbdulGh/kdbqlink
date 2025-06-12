#include "WL.h"
#include "extern_headers/k.h"
#include "q_utils.h"

#include <iostream>

extern "C" {
  K kl_eval(K in_str) {
    if (in_str->t != KC) {
      return kl::err("Expected string to eval");
    }

    kl::KernelConnection& kc = kl::KernelConnection::singleton();

    std::string_view sv {
      reinterpret_cast<const char*>(in_str->G0),
      static_cast<size_t>(in_str->n)
    };

    kc.write(sv);
    std::optional<std::size_t> res = kc.read();

    if (!res) {
      return kl::err("Failed to get res");
    }

    return kj(*res);
  }
}

