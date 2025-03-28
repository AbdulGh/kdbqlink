#pragma once

#include "k.h"
#include "wstp.h"

#include <string>
#include <format>
#include <expected>

#define PANIC(str) throw std::runtime_error{std::format("__FILE__:__LINE__ - {}", str)}

namespace kl {

struct kernel_connection {
  const WSENV env {nullptr};
  const WSLINK lp {nullptr};

  static WSLINK link(WSENV my_env, std::string_view linkname) {
    int err = WSEOK;
    std::string cmdline = std::format("-linkname {} -linkmode connect", linkname);
    WSLINK attempt =  WSOpenString(my_env, cmdline.c_str(), &err);

    if (err != WSEOK) {
      const char* errmsg = WSErrorMessage(attempt);
      std::string copy {errmsg}; //...
      WSReleaseErrorMessage(attempt, errmsg);
      PANIC(std::move(copy));
    }

    return attempt;
  }

  kernel_connection(std::string_view linkname):
    env{ WSInitialize(0) }, lp { kernel_connection::link(env, linkname) }
  {}
};

char* q_intern(const char* c);
K make_kernel_connection(K linkname);
K eval(const std::string& str);
kernel_connection* get_handle(K);

}
