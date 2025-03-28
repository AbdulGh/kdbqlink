#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "wstp.h"
#include "statics.h"

int main(int argc, char* argv[]) {
    int err;

    kl::env = WSInitialize(0);
    if (!kl::env) {
        fprintf(stderr, "Error initializing WSTP.\n");
        return 1;
    }

    kl::lp = WSOpenString(kl::env, "-linkname 'myWSTPLink' -linkmode connect", &err);
    if (!kl::lp || err != WSEOK) {
        std::cout << err << std::endl;
        const char* errmsg = WSErrorMessage(kl::lp);
        if (errmsg) {
            std::cerr << "Error opening WSTP link: " << errmsg << std::endl;
            WSReleaseErrorMessage(kl::lp, errmsg);
        }
        WSDeinitialize(kl::env);
        return 1;
    }

    WSPutFunction(kl::lp, "EvaluatePacket", 1);
        WSPutFunction(kl::lp, "Set", 2);
            WSPutSymbol(kl::lp, "x");
            WSPutReal64(kl::lp, 3.14);
    WSEndPacket(kl::lp);
    WSFlush(kl::lp);

    int pkt;
    double result;
    while ((pkt = WSNextPacket(kl::lp)) && pkt != RETURNPKT) {
        WSNewPacket(kl::lp);
    }

    WSClose(kl::lp);
    WSDeinitialize(kl::env);

    return 0;
}
