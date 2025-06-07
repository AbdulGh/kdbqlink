#pragma once

#include "extern_headers/wstp.h"

#include <string>
#include <format>
#include <expected>

#define PANIC(str) throw std::runtime_error{std::format("__FILE__:__LINE__ - {}", str)}

namespace kl {

class KernelConnection {
  private:
  const WSENV env {nullptr};
  const WSLINK lp {nullptr};

  static WSLINK link(WSENV my_env, std::string_view linkname) {
    int err = WSEOK;
    std::string cmdline = std::format("-linkmode launch -linkname 'wolfram -wstp'");
    WSLINK attempt = WSOpenString(my_env, cmdline.c_str(), &err);

    if (err != WSEOK) {
      const char* errmsg = WSErrorMessage(attempt);
      if (errmsg) {
        std::string copy {errmsg}; //...
        WSReleaseErrorMessage(attempt, errmsg);
        PANIC(std::move(copy));
      }
      else {
        throw std::runtime_error{std::to_string(err)};
      }
    }
    else if (attempt == nullptr) {
      PANIC("Something when wrong calling WSOpenString");
    }

    return attempt;
  }

  KernelConnection(std::string_view linkname):
    env{ WSInitialize(0) }, lp { KernelConnection::link(env, linkname) }
  {}

  public:

  KernelConnection(KernelConnection&) = delete;
  KernelConnection(KernelConnection&&) = delete;
  KernelConnection& operator=(KernelConnection&) = delete;
  KernelConnection& operator=(KernelConnection&&) = delete;

  static KernelConnection& singleton() {
    static KernelConnection kc {"TODO_LINKNAME"};
    return kc;
  }

  void write(const std::string& to_evaluate) {
    WSPutFunction(lp, "EvaluatePacket", 1);
    WSPutFunction(lp, "ToExpression", 1);
    WSPutString(lp, to_evaluate.c_str());
    WSEndPacket(lp);
  }

  std::optional<std::size_t> read() {
    int pkt;
    while ((pkt = WSNextPacket(lp)) && pkt != RETURNPKT)
      WSNewPacket(lp);

    int result;
    if (WSGetInteger(lp, &result)) {
      return result;
    } else {
      return std::nullopt;
    }
  }

  ~KernelConnection() {
    if (lp) {
      WSPutFunction(lp, "EvaluatePacket", 1);
      WSPutFunction(lp, "Exit", 0);
      WSEndPacket(lp);

      while (WSNextPacket(lp) != 0) {
        WSNewPacket(lp);
      }
      WSClose(lp);
    }

    if (env) {
      WSDeinitialize(env);
    }
  }
};



}
