#include "Semaphore.hpp"

namespace RTOS
{
    Semaphore::Semaphore()
    {
        this->semaphore_handle = xSemaphoreCreateBinary();
    }

    BaseType_t Semaphore::Take(TickType_t ticks)
    {
        return xSemaphoreTake(semaphore_handle,ticks);
    }
    
} // namespace RTOS
