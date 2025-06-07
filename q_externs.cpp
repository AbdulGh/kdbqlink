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

    std::cout << "Hello" << std::endl;
    std::cout << in_str->n << std::endl;
    std::cout << std::string_view{in_str->s, (unsigned long)in_str->n} << std::endl;

    kc.write(in_str->s);
    std::optional<std::size_t> res = kc.read();

    if (!res) {
      return kl::err("Failed to get res");
    }

    return kj(*res);
  }
}

