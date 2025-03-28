#pragma once

#include "k.h"

extern "C" {
  K kl_send_definition(K cx, K in);
  K kl_make_kernel_connection(K linkname);
}
