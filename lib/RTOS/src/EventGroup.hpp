/**
 * @file EventGroup.hpp
 * @author Taivison Martins Leite (taivison@grupocriar)
 * @brief Define a classe para manipular os EventGroups do FreeRTOS
 * @version 0.1
 * @date 2019-10-29
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef _EVENT_GROUP_H_
#define _EVENT_GROUP_H_

#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>

namespace RTOS
{
class EventGroup
{
private:
    EventGroupHandle_t handle;

public:
    /**
     * @brief Construct a new Event Group object
     * 
     */
    EventGroup();
    /**
     * @brief 
     * 
     * @param bitsToWaitFor Bits que devem ser aguardados
     * @param clearOnExit pdTRUE caso limpa os bits ao sair caso contrário pdFALSE. 
     * @param waitForAll pdTRUE para aguarda todos os bits, caso contrário pdFALSE.
     * @param ticks Periodo a ser aguardado pelo evento, use a Macro pdMS_TO_TICKS() para
     * converter de milissegundos para o número de ticks do FreeRTOS ou portMAX_DELAY para aguardar indefinidamente. 
     * @return EventBits_t 
     */
    EventBits_t WaitBits(EventBits_t bitsToWaitFor, BaseType_t clearOnExit, BaseType_t waitForAll,
                         TickType_t ticks);
    /**
     * @brief Seta os Bits de Evento
     * 
     * @param bitsToSet Os bits a serem ativados
     * @return EventBits_t 
     */
    EventBits_t SetBits(EventBits_t bitsToSet);
    /**
     * @brief Seta os Bits de Eventos, função segura para ser usada em Interrupções.
     * 
     * @param bitsToSet Os bits a serem ativados
     * @return BaseType_t 
     */
    inline BaseType_t SetBitsFromISR(EventBits_t bitsToSet);
    /**
     * @brief Obtém o estado atual dos bits de Evento
     * 
     * @return EventBits_t 
     */
    EventBits_t GetBits();
    /**
     * @brief Obtém o estado atual dos bits de Evento, função segura para ser usada em Interrupções.
     * 
     * @return EventBits_t 
     */
    inline EventBits_t GetBitsFromISR();
    /**
     * @brief Limpa os bits selecionados.
     * 
     * @param bitsToClear Os bits a serem limpados.
     * @return EventBits_t 
     */
    EventBits_t ClearBits(EventBits_t bitsToClear);
    /**
     * @brief Limpa os bits selecionados, função segura para ser usada em Interrupções.
     * 
     * @param bitsToClear Os bits a serem limpados.
     * @return BaseType_t 
     */
    inline BaseType_t ClearBitsFromISR(EventBits_t bitsToClear);
    /**
     * @brief Deleta o objeto 
     * 
     */
    void Delete();
};

inline EventBits_t EventGroup::GetBitsFromISR()
{
    return xEventGroupGetBitsFromISR(handle);
}

inline BaseType_t EventGroup::ClearBitsFromISR(EventBits_t bitsToClear)
{
    return xEventGroupClearBitsFromISR(handle, bitsToClear);
}

inline BaseType_t EventGroup::SetBitsFromISR(EventBits_t bitsToSet)
{
    BaseType_t woken;
    BaseType_t r = xEventGroupSetBitsFromISR(handle, bitsToSet, &woken);
    if (woken)
        portYIELD_FROM_ISR();
    return r;
}

} // namespace RTOS
#endif // _EVENT_GROUP_H_
