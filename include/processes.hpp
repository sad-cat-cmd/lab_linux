/**
 * @file processes.hpp
 * @brief Утилиты для порождения дочерних процессов и записи вывода ls в файл.
 *
 * Заголовочный файл объявляет namespace processes, содержащий:
 *  - класс исключения ExceptionProcess;
 *  - класс Process, инкапсулирующий создание дочернего процесса,
 *    запуск утилиты ls с перенаправлением вывода в файл.
 *
 * Используются системные вызовы:
 * fork(2), open(2), dup2(2), close(2), exec(2) (execlp), getpid(2),
 * waitpid(2).
 */

#ifndef PROCESSES_HPP
#define PROCESSES_HPP

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <exception>
#include <iostream>
#include <cerrno>
#include <string.h>
#include <fcntl.h>

/**
 * @namespace processes
 * @brief Пространство имён для работы с процессами.
 */
namespace processes {

    /**
     * @class ExceptionProcess
     * @brief Исключение для ошибок работы с процессами.
     *
     * Наследуется от std::exception. Хранит текстовое сообщение об ошибке
     * и код ошибки (значение errno на момент возникновения исключения).
     */
    class ExceptionProcess : public std::exception
    {
        private:
        /** @brief Текстовое описание ошибки. */
        std::string msgErr;

        /** @brief Код ошибки (errno). */
        int codeErr;

        public:
        /**
         * @brief Конструктор исключения.
         * @param msgErr Текстовое описание ошибки.
         * @param codeErr Код ошибки (обычно errno).
         */
        ExceptionProcess(const std::string & msgErr,
                         const int codeErr);

        /**
         * @brief Получить текстовое описание ошибки.
         * @return Строка с описанием ошибки.
         * @note Метод помечен noexcept — не бросает исключений.
         */
        std::string getMsgErr() noexcept;

        /**
         * @brief Получить код ошибки.
         * @return Код ошибки (значение errno).
         * @note Метод помечен noexcept — не бросает исключений.
         */
        int getCodeErr() noexcept;
    };

    /**
     * @class Process
     * @brief Класс, инкапсулирующий создание дочернего процесса.
     *
     * Хранит PID дочернего процесса (поле pid). Метод init() выполняет
     * fork(2); в дочернем процессе вызывает writeLsIntoFile(), который
     * открывает файл с именем "<PID>.txt", перенаправляет stdout в него
     * через dup2(2) и запускает ls через execlp(2).
     */
    class Process {
        private:
        /** @brief PID дочернего процесса (результат fork). */
        pid_t pid;

        /**
         * @brief Записать вывод ls в файл "<PID>.txt" (выполняется в потомке).
         *
         * Открывает файл с именем, равным PID текущего процесса (getpid(2)),
         * с флагами O_WRONLY | O_CREAT | O_TRUNC и правами 0644. Перенаправляет
         * стандартный вывод (STDOUT_FILENO) в файл через dup2(2), закрывает
         * исходный дескриптор и запускает ls через execlp(2). При ошибках
         * open/dup2/exec выводит сообщение в stderr и завершает процесс
         * через _exit(1).
         *
         * @note При успешном execlp управление не возвращается — процесс
         *       заменяется на ls.
         */
        void writeLsIntoFile();

        public:
        /**
         * @brief Конструктор по умолчанию.
         * @note Класс не хранит состояния, кроме pid; конструктор noexcept.
         */
        Process() noexcept;

        /**
         * @brief Деструктор.
         * @note Не выполняет waitpid — сбор потомков должен делать
         *       вызывающий код (или main) через waitpid(2).
         */
        ~Process() noexcept;

        /**
         * @brief Создать дочерний процесс через fork(2).
         *
         * В родителе сохраняет PID потомка в поле pid и возвращает управление.
         * В потомке (pid == 0) вызывает writeLsIntoFile(), который заменяет
         * процесс на ls через execlp(2). При ошибке fork(2) бросает
         * ExceptionProcess с сообщением и кодом ошибки.
         *
         * @throw ExceptionProcess Если fork(2) вернул -1 (например, EAGAIN —
         *        достигнут лимит процессов, ENOMEM — нехватка памяти).
         *
         * @note После вызова в родителе поле pid содержит PID потомка;
         *       в потомке поле pid равно 0 (результат fork), поэтому для
         *       имени файла внутри writeLsIntoFile используется getpid(2),
         *       а не this->pid.
         */
        void init();

        /**
         * @brief Получить PID дочернего процесса.
         * @return PID потомка (значение поля pid).
         * @note Метод помечен noexcept — не бросает исключений.
         *       В потомке возвращает 0 (результат fork), в родителе — PID
         *       созданного потомка.
         */
        pid_t getPid() noexcept;
    };
}

#endif // PROCESSES_HPP