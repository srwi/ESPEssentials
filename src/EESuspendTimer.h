#pragma once

#include <Arduino.h>

#if defined(SUSPEND_TIMER1_ON_SPI_ACCESS)

    namespace ESPEssentials
    {

        void EESuspendTimer1();
        void EEResumeTimer1();

    } // namespace ESPEssentials

    #define SUSPEND_TIMER1 ESPEssentials::EESuspendTimer1()
    #define RESUME_TIMER1 ESPEssentials::EEResumeTimer1()

#else

    #define SUSPEND_TIMER1
    #define RESUME_TIMER1

#endif
