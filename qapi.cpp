#include "q_utils.h"
#include "wstp.h"

#include <string>

namespace kl {
  K send_definition(K cx, K in) {
    kernel_connection* handle = get_handle(cx);

    if (in->t != -KS) {
      PANIC(std::format("Expected symbol (found {})", in->t));
    }
    K to_send = eval(std::string{"get "} + in->s);

    WSPutFunction(handle->lp, "EvaluatePacket", 1);
    WSPutFunction(handle->lp, "Set", 2);
    WSPutSymbol(handle->lp, in->s);

    switch (to_send->t) {
      case -KF:
        WSPutReal(handle->lp, to_send->f);
        break;
      case -KH:
        WSPutInteger16(handle->lp, to_send->h);
        break;
      case -KI:
        WSPutInteger32(handle->lp, to_send->i);
        break;
      case -KJ:
        WSPutInteger64(handle->lp, to_send->j);
        break;
      case KF:
        WSPutRealList(handle->lp, kF(to_send), to_send->n);
        break;
      case KH:
        WSPutInteger16List(handle->lp, kH(to_send), to_send->n);
        break;
      case KI:
        WSPutInteger32List(handle->lp, kI(to_send), to_send->n);
        break;
      case KJ:
        WSPutInteger64List(handle->lp, (long*)kJ(to_send), to_send->n);
        break;
      default:
        return krr(q_intern("nyi"));
    }

    WSEndPacket(handle->lp);
    WSFlush(handle->lp);

    long int dims; //dont care
    WSCheckFunction(handle->lp, "ReturnPacket", &dims);
    return kb(true);
  }
}

extern "C" {
  K kl_send_definition(K cx, K in) {
    try {
      return kl::send_definition(cx, in);
    }
    catch (const std::exception& e) {
      return krr(kl::q_intern(e.what()));
    }
  }

  K kl_make_kernel_connection(K linkname) {
    try {
      return kl::make_kernel_connection(linkname);
    }
    catch (const std::exception& e) {
      return krr(kl::q_intern(e.what()));
    }
  }
}
