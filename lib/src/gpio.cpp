//
// Created by tomer on 07/08/2020.
//
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdexcept>
#include <cerrno>
#include <sstream>
#include <algorithm>

#include "gpio.h"

namespace pi_baker {
    GPIO::GPIO() {
        int memFd;

        if ((memFd = open("/dev/gpiomem", O_RDWR | O_SYNC)) < 0) {
            std::stringstream ss;
            ss << "Could not open /dev/gpiomem! Error code " << errno;
            throw std::runtime_error(ss.str());
        }

        void* gpioMap = mmap(nullptr, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memFd, GPIO_BASE);
        close(memFd);

        if (gpioMap == MAP_FAILED) {
            throw std::runtime_error("Could not map gpio!");
        }

        gpio = (volatile unsigned int*)gpioMap;
    }

    void GPIO::inpGPIO(unsigned int pin) {
        *(gpio + (pin/10)) &= ~(7 << ((pin % 10) * 3));
    }

    void GPIO::outGPIO(unsigned int pin) {
        *(gpio + (pin/10)) |= (1 << ((pin % 10) * 3));
    }

    volatile unsigned int &GPIO::gpioSet() {
        return *(gpio + 7);
    }

    volatile unsigned int &GPIO::gpioClear() {
        return *(gpio + 10);
    }

    volatile unsigned int &GPIO::gpioPull() {
        return *(gpio + 57);
    }

    volatile unsigned int &GPIO::gpioPullClk() {
        return *(gpio + 38);
    }

    bool GPIO::getGPIO(unsigned int pin) {
        return (*(gpio + 13) & (1 << pin));
    }

    Pin GPIO::openPin(unsigned int pin, PinDirection dir, PinPullUp pullUp) {
        checkPin(pin);
        return Pin(this, pin, dir, pullUp);
    }

    GPIO::~GPIO() {
        munmap((void *) gpio, BLOCK_SIZE);
    }

    void GPIO::checkPin(unsigned int pin) {
        if (std::find(openPins.begin(), openPins.end(), pin) != openPins.end()) {
            std::stringstream ss;
            ss << "Pin " << pin << " already in use!";
            throw std::runtime_error(ss.str());
        }
    }

    Pin::Pin(GPIO *ref, unsigned int pin, PinDirection dir, PinPullUp pullUp) : ref(ref), pin(pin) {
        ref->openPins.push_back(pin);

        setDir(dir);

        setPullUpDown(pullUp);
    }

    Pin::~Pin() {
        for (int i = 0; i < ref->openPins.size(); i++) {
            if (ref->openPins[i] == pin) {
                ref->openPins.erase(ref->openPins.begin() + i);
            }
        }
    }

    bool Pin::get() {
        return ref->getGPIO(pin);
    }

    void Pin::set(bool val) {
        if (val) {
            ref->gpioSet() = 1 << pin;
        } else {
            ref->gpioClear() = 1 << pin;
        }
    }

    void Pin::setDir(PinDirection dir) {
        ref->inpGPIO(pin);
        if (dir == PIN_OUT) {
            ref->outGPIO(pin);
        }
    }

    void Pin::setPullUpDown(PinPullUp state) {
        int shift = (pin & 0xf) << 1;
        uint32_t bits = ref->gpioPull();
        bits &= ~(3 << shift);
        bits |= state << shift;
        ref->gpioPull() = bits;
    }
}
