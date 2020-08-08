//
// Created by tomer on 07/08/2020.
//

#ifndef RPITEST_PI_H
#define RPITEST_PI_H

#include <vector>

namespace pi_baker {
    class GPIO;

    enum PinDirection {
        PIN_IN,
        PIN_OUT
    };

    enum PinPullUp {
        PULL_UP = 1,
        PULL_DOWN = 2,
        PULL_UP_OFF = 0
    };

    class Pin {
    public:
        Pin(GPIO* ref, unsigned int pin, PinDirection dir, PinPullUp pullUp);
        ~Pin();

        bool get();
        void set(bool val);

        void setDir(PinDirection dir);
        void setPullUpDown(PinPullUp state);

    private:
        GPIO* ref;
        unsigned int pin;
    };

    class GPIO {
    public:
        GPIO();
        ~GPIO();

        Pin openPin(unsigned int pin, PinDirection dir, PinPullUp pullUp);
        void inpGPIO(unsigned int pin);
        void outGPIO(unsigned int pin);

        volatile unsigned int& gpioSet();
        volatile unsigned int& gpioClear();
        volatile unsigned int& gpioPull();
        volatile unsigned int& gpioPullClk();

        bool getGPIO(unsigned int pin);

        std::vector<unsigned int> openPins;

    private:

        volatile unsigned int *gpio;

        void checkPin(unsigned int pin);

        static constexpr int PAGE_SIZE = (4*1024);
        static constexpr int BLOCK_SIZE = (4*1024);
        static constexpr int BCM2708_PERI_BASE = 0x3F000000;
        static constexpr int GPIO_BASE = BCM2708_PERI_BASE + 0x200000;
    };
}

#endif //RPITEST_PI_H
