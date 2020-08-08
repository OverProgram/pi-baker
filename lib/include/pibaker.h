//
// Created by tomer on 08/08/2020.
//

#ifndef PIBAKER_PIBAKER_H
#define PIBAKER_PIBAKER_H

#include "gpio.h"
#include <memory>

namespace pi_baker {
    class PiBaker {
    public:
        explicit PiBaker(bool gpioEnable = false);
        ~PiBaker();

        int prepare();

        void bake();

        virtual void main() = 0;

    protected:
        bool gpioEnable;

        std::unique_ptr<GPIO> gpio;
    };
}

#endif //PIBAKER_PIBAKER_H
