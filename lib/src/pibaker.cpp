//
// Created by tomer on 08/08/2020.
//
#include "pibaker.h"

namespace pi_baker {
    PiBaker::PiBaker(bool gpioEnable)
    : gpioEnable(gpioEnable)
    , gpio(nullptr)
    {}

    int PiBaker::prepare() {
        if (gpioEnable) {
            gpio = std::make_unique<GPIO>();
        }

        return 0;
    }

    void PiBaker::bake() {
        main();
    }

    PiBaker::~PiBaker() = default;
}
