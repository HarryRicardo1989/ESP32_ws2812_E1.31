/**
 * @file Queue.hpp
 * @author Taivison Martins Leite (taivison@grupocriar)
 * @brief Define a classe para manipular as Filas do FreeRTOS
 * @version 0.1
 * @date 2019-10-29
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef _QUEUE_H_RTOS_
#define _QUEUE_H_RTOS_

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

namespace RTOS
{
/**
* @brief Template de Classe para gerenciar o uso de Filas do FreeRTOS.
* 
* @tparam T Define o tipo que a fila irá armazenar. 
*/
template <typename T>
class Queue
{
private:
    QueueHandle_t queue;

public:
    /**
    * @brief Constroi um Objeto de Fila na Memória
    * 
    * @param size Quantidade de Posições que a fila irá possuir.
    */
    Queue(UBaseType_t size);

    /**
     * @brief Constroi um Objeto de Fila na Memória
     * 
     * @param _handle Handle de uma Queue já existente
     */
    Queue(QueueHandle_t *_handle);
    /**
     * @brief Metodo para inserir um item ao final da Fila
     * 
     * @param item O item a ser inserido.
     * @param ticks Periodo a ser aguardado caso a fila esteja cheia, use a Macro pdMS_TO_TICKS() para
     * converter de milissegundos para o número de ticks do FreeRTOS ou portMAX_DELAY para aguardar indefinidamente.
     * @return BaseType_t Retorna pdTRUE caso o item tenha sido inserido com sucesso,
     * caso contrário errQUEUE_FULL.
     */
    BaseType_t Enqueue(T item, TickType_t ticks);
    /**
     * @brief Metodo para inserir um item ao final da Fila, pode ser utilizado em Interrupções.
     * 
     * @param item O item a ser inserido
     * @return BaseType_t Retorna pdTRUE caso o item tenha sido inserido com sucesso,
     * caso contrário errQUEUE_FULL.
     */
    BaseType_t EnqueueFromISR(T item);

    /**
     * @brief Somente para ser usada em Filas de 1 elemento
     * Ela irá sobreescrever o item atual
     * 
     * @param item Item a ser postado na fila
     */
    void OverWrite(T item);

    /**
     * @brief Remove o primeiro item da fila e retorna o seu valor.
     * 
     * @param item Ponteiro para o buffer no qual será armazenado o item
     * @param ticks Periodo a ser aguardado caso a fila esteja vazia, use a Macro pdMS_TO_TICKS() para
     * converter de milissegundos para o número de ticks do FreeRTOS ou portMAX_DELAY para aguardar indefinidamente.
     * @return true Caso o item tenha sido recebido
     * @return false Caso tenha falhado em receber o item
     */
    inline bool TryDequeue(T *item, TickType_t ticks);
    /**
     * @brief Retorna o Primeiro item da fila sem remove-lo.
     * 
     * @param item Ponteiro para o buffer no qual será armazenado o item
     * @param ticks Periodo a ser aguardado caso a fila esteja vazia, use a Macro pdMS_TO_TICKS() para
     * converter de milissegundos para o número de ticks do FreeRTOS ou portMAX_DELAY para aguardar indefinidamente.
     * @return true Caso o item tenha sido recebido
     * @return false Caso tenha falhado em receber o item
     */
    inline bool TryPeek(T* item,TickType_t ticks);
    /**
     * @brief Retorna o Número de Itens contidos na Fila.
     *  
     * @return UBaseType_t Quantidade de Itens na Fila.
     */
    UBaseType_t Count(void);
    /**
     * @brief Retorna o Número de Itens contidos na Fila, versão segura para ser usada em Interrupções.
     * 
     * @return UBaseType_t 
     */
    UBaseType_t CountFromISR(void);
};

template <typename T>
Queue<T>::Queue(UBaseType_t size)
{
    queue = xQueueCreate(size, sizeof(T));
}

template<typename T>
Queue<T>::Queue(QueueHandle_t *_handle)
{
    queue = *_handle;
}

template <typename T>
BaseType_t Queue<T>::Enqueue(T item, TickType_t ticks)
{
    return xQueueSendToBack(queue, (void *)&item, ticks);
}

template <typename T>
void Queue<T>::OverWrite(T item)
{
    xQueueOverwrite(queue,(void *)&item);
}

template <typename T>
inline bool Queue<T>::TryDequeue(T *item,TickType_t ticks)
{
    return xQueueReceive(queue, (void *)item, ticks) == pdTRUE;
}

template <typename T>
inline bool Queue<T>::TryPeek(T *item,TickType_t ticks)
{
    //Bloqueia a Tarefa até que exista um item na fila.
    return xQueuePeek(queue, (void *)item, ticks) == pdTRUE;
}

template <typename T>
UBaseType_t Queue<T>::Count(void)
{
    return uxQueueMessagesWaiting(queue);
}

template <typename T>
UBaseType_t Queue<T>::CountFromISR(void)
{
    return uxQueueMessagesWaitingFromISR(queue);
}

} // namespace RTOS

#endif