/**
 * @file Task.hpp
 * @author Taivison Martins Leite (taivison@grupocriar)
 * @brief Define a classe para Manipulação de Tarefas no FreeRTOS
 * @version 0.1
 * @date 2019-10-29
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef _TASK_H_RTOS_
#define _TASK_H_RTOS_

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <string>

using namespace std;

namespace RTOS
{

class Task
{
protected:
    TaskHandle_t task_handle;
    void *data;
    /**
     * @brief Coloca a Tarefa no estado de Bloqueio até que receba uma Notificação
     * 
     * @param ticks Periodo a ser aguardado pela notificação, use a Macro pdMS_TO_TICKS() para
     * converter de milissegundos para o número de ticks do FreeRTOS ou portMAX_DELAY para aguardar indefinidamente. 
     * @return uint32_t Retorna o valor anterior do contador de Notificação
     */
    uint32_t NotifyTake(TickType_t ticks);
    /**
     * @brief Função que roda a Tarefa, Deve ser implementada pela classe que herda esta classe.
     * 
     * @param args Argumento a ser passado para a função
     */
    virtual void run(void *args) = 0;
    /**
     * @brief Função que é passada para a criação da Tarefa, é responsável por invocar a função da tarefa
     * 
     * @param data Argumento a ser passado para função
     */
    static void runTask(void *data);

public:
    /**
    * @brief Construct a new Task object
    * 
    */
    Task();
    /**
     * @brief Inicia a Tarefa e coloca ela no Scheduler do FreeRTOS.
     * 
     * @param data Dado a ser passado para função executada na tarefa.
     * @param taskName Nome da Tarefa.
     * @param stackSize Tamanho da Stack da Tarefa.
     * @param priority Prioridade da Tarefa
     * @param coreID Número do Nuclue que executará a tarefa pode ser 0/1.
     */
    void Start(void *data, string taskName, uint32_t stackSize, UBaseType_t priority,
               BaseType_t coreID);
    /**
     * @brief Deleta o objeto da tarefa e remove-o do Scheduler do FreeRTOS.
     * 
     */
    void Delete(void);
    /**
     * @brief Pausa a Tarefa
     * 
     */
    void Pause(void);
    /**
     * @brief Retorna a Tarefa para fila de tarefas que podem ser executadas.
     * 
     */
    void Resume(void);
    /**
     * @brief Envia uma Notificação para esta tarefa.
     * 
     * @return BaseType_t Retorna sempre pdPASS.
     */
    BaseType_t Notify(void);
    /**
     * @brief Envia uma Notificação para esta tarefa, esta função deve ser usada em uma Interrupção
     * 
     */
    void NotifyFromISR(void);

    /**
     * @brief Get the Task Name
     * 
     * @return char* 
     */
    inline char *GetTaskName(void)
    {
        return pcTaskGetTaskName(task_handle);
    }

    /**
     * @brief Coloca a Tarefa em um estado de bloqueio até que se passe o tempo.
     * 
     * @param tempo Tempo em Milissegundos.
     */
    static void Delay(uint32_t tempo);

    /**
     * @brief Coloca a Tarefa em um estado de bloqueio até que se passe o tempo exato a partir
     * da ultima vez que ela começou a ser executada.
     * 
     * @param tempoAnterior Ponteiro para a última vez que a tarefa começou a ser executada
     * @param tempo Tempo em Milissegundos
     */
    static void DelayUntil(TickType_t *tempoAnterior, uint32_t tempo);
};
} // namespace RTOS
#endif