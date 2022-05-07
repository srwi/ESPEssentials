#pragma once

#include <Arduino.h>

#if defined(SUSPEND_TIMER1_ON_SPI_ACCESS)

    class SuspendTimerGuard
    {
    public:
        SuspendTimerGuard()
        {
            if (timer1_enabled())
            {
                m_suspended = true;
                ETS_FRC1_INTR_DISABLE();
            }
        };

        ~SuspendTimerGuard()
        {
            resume();
        }

        void resume()
        {
            if (m_suspended)
            {
                ETS_FRC1_INTR_ENABLE();
                m_suspended = false;
            }
        }
    private:
        bool m_suspended{false};
    };

    #define SUSPEND_TIMER1() SuspendTimerGuard stg
    #define RESUME_TIMER1() stg.resume()

#else

    #define SUSPEND_TIMER1()
    #define RESUME_TIMER1()

#endif
