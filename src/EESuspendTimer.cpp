#include "EESuspendTimer.h"

#if defined(SUSPEND_TIMER1_ON_SPI_ACCESS)

    namespace ESPEssentials
    {

        bool timer1Suspended;

        void EESuspendTimer1()
        {
            if (timer1_enabled())
            {
                ETS_FRC1_INTR_DISABLE();
                timer1Suspended = true;
            }
        }

        void EEResumeTimer1()
        {
            if (timer1Suspended)
            {
                ETS_FRC1_INTR_ENABLE();
                timer1Suspended = false;
            }
        }

    } // namespace ESPEssentials

#endif
