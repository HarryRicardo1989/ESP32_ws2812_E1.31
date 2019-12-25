#include "Task.hpp"

namespace RTOS
{

Task::Task()
{
    task_handle = nullptr;
}

void Task::runTask(void *arg)
{
    Task* pTask = (Task*)arg;
    pTask->run(pTask->data);
    pTask->Delete();
}

void Task::Start(void *data, string taskName, uint32_t stackSize, UBaseType_t priority, BaseType_t coreID)
{
    if (task_handle == nullptr)
    {
        this->data = data;
        xTaskCreatePinnedToCore(&runTask, taskName.c_str(), stackSize, this, priority, &task_handle, coreID);
    }
}

void Task::Delete(void)
{
    vTaskDelete(task_handle);
}

void Task::Pause(void)
{
    vTaskSuspend(task_handle);
}

void Task::Resume(void)
{
    vTaskResume(task_handle);
}

BaseType_t Task::Notify(void)
{
    return xTaskNotifyGive(task_handle);
}

void Task::NotifyFromISR(void)
{
    BaseType_t woken;
    vTaskNotifyGiveFromISR(task_handle, &woken);
    if (woken)
        portYIELD_FROM_ISR();
}

void Task::Delay(uint32_t tempo)
{
    vTaskDelay(tempo / portTICK_PERIOD_MS);
}

void Task::DelayUntil(TickType_t *tempoAnterior, uint32_t tempo)
{
    vTaskDelayUntil(tempoAnterior,pdMS_TO_TICKS(tempo));
}

uint32_t Task::NotifyTake(TickType_t ticks)
{
    return ulTaskNotifyTake(pdTRUE,ticks);
}

} // namespace RTOS