#include "EventGroup.hpp"

namespace RTOS
{

EventGroup::EventGroup()
{
    handle = xEventGroupCreate();
}

EventBits_t EventGroup::WaitBits(EventBits_t bitsToWaitFor, BaseType_t clearOnExit,
                                 BaseType_t waitForAll, uint32_t timeToWait)
{
    return xEventGroupWaitBits(handle, bitsToWaitFor, clearOnExit, waitForAll,
                               (timeToWait / portTICK_PERIOD_MS));
}

EventBits_t EventGroup::SetBits(EventBits_t bitsToSet)
{
    return xEventGroupSetBits(handle, bitsToSet);
}

EventBits_t EventGroup::GetBits()
{
    return xEventGroupGetBits(handle);
}

EventBits_t EventGroup::ClearBits(EventBits_t bitsToClear)
{
    return xEventGroupClearBits(handle, bitsToClear);
}

void EventGroup::Delete()
{
    vEventGroupDelete(handle);
}
} // namespace RTOS