#include "pibaker.h"
#include "gpio.h"
#include <unistd.h>

class GPIOExample : public pi_baker::PiBaker {
public:
    GPIOExample() : PiBaker(true) {}

    void main() override {
        auto pin = gpio->openPin(7, pi_baker::PIN_OUT, pi_baker::PULL_UP_OFF);

        for (int i = 0; i < 10; i++) {
            pin.set(true);
            sleep(1);
            pin.set(false);
            sleep(1);
        }
    }
};

int main() {
    GPIOExample example;
    if (example.prepare() < 0) {
        return -1;
    }
    example.bake();
    return 0;
}
