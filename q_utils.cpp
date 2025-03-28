#include "q_utils.h"
#include <iostream>

namespace kl {

char* q_intern(const char* c) {
  return ss(const_cast<char*>(c));
}

K eval(const std::string& str) {
  return k(0, const_cast<char*>(str.c_str()), 0);
}

static void destroy_kernel_connection(kernel_connection* bye) {
  WSClose(bye->lp);
  WSDeinitialize(bye->env);
  delete bye;
}

kernel_connection* get_handle(K cx) {
  std::cout << "here 1" << std::endl;
  if(cx->t != 112 || cx->n != 2 || kK(cx)[1] != (K)&destroy_kernel_connection) {
    PANIC("Expected handle to mma kernel");
  }
  std::cout << "here 2" << std::endl;
  return reinterpret_cast<kernel_connection*>(kK(cx)[0]);
}

K make_kernel_connection(K linkname) {
  if (linkname->t != -KS) {
    return krr(q_intern("Expected symbol as linkname"));
  }
  K details = reinterpret_cast<K>(new kernel_connection{linkname->s});
  K dtor = reinterpret_cast<K>(&destroy_kernel_connection);
  K result = knk(2, details, dtor);
  result->t = 112;
  return result;
}

}
