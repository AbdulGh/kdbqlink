#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "extern_headers/wstp.h"

#include "WL.h"

int main(int argc, char* argv[]) {
  kl::KernelConnection& kc = kl::KernelConnection::singleton();
  kc.write("1 + 1");
  std::optional<std::size_t> res = kc.read();

  if (!res) {
    std::cout << "no res" << std::endl;
  }
  else {
    std::cout << "its " << *res << std::endl;
  }
}
