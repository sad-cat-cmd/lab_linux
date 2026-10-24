/**
 * @file files.hpp
 * @brief Утилиты для обхода каталога и вывода имён владельцев файлов.
 *
 * Заголовочный файл объявляет namespace files_utils, содержащий:
 *  - класс исключения ExceptionDirOrFilesWork;
 *  - класс FilesPrinter с методами обхода каталога и определения
 *    имени пользователя-владельца файла.
 *
 * Используются системные вызовы и функции из методички:
 * opendir(3), readdir(3), closedir(3), stat(2), getpwuid(3).
 */

#ifndef FILES_HPP
#define FILES_HPP

#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <cstring>
#include <cerrno>
#include <string>
#include <exception>

/**
 * @namespace files_utils
 * @brief Пространство имён для утилит работы с файлами и каталогами.
 */
namespace files_utils {

    /** @brief Сообщение об ошибке открытия каталога. */
    const std::string MSG_ERR_FOR_DONT_OPEN_DIR = std::string("Error with open dir: ");

    /** @brief Сообщение об ошибке получения информации о файле. */
    const std::string MSG_ERR_FOR_DONT_OPEN_FILE = std::string("Error with open file: ");

    /**
     * @class ExceptionDirOrFilesWork
     * @brief Исключение для ошибок работы с каталогами и файлами.
     *
     * Наследуется от std::exception. Хранит текстовое сообщение об ошибке
     * и код ошибки (значение errno на момент возникновения исключения).
     */
    class ExceptionDirOrFilesWork : public std::exception
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
        ExceptionDirOrFilesWork(const std::string & msgErr,
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
     * @class FilesPrinter
     * @brief Класс для обхода каталога и вывода имён владельцев файлов.
     *
     * Содержит статические методы: обход каталога, определение владельца
     * файла и форматированный вывод. Объекты класса создавать не обязательно —
     * все методы статические.
     */
    class FilesPrinter {
        private:
        /**
         * @brief Вывести имя владельца и имя файла в формате "owner|filename".
         * @param owner Имя пользователя-владельца (или числовой UID).
         * @param filename Имя файла (без пути).
         * @note Приватный статический метод, используется внутри класса.
         */
        inline static void printOwnerAndFilename(const std::string & owner,
                                                 const std::string & filename);

        /**
         * @brief Получить имя пользователя-владельца файла по полному пути.
         *
         * Выполняет stat(2) для файла, проверяет, что это обычный файл
         * (S_ISREG), затем по st_uid через getpwuid(3) получает имя
         * пользователя. Если UID отсутствует в базе, возвращает числовой UID
         * в виде строки.
         *
         * @param fullPathFile Полный путь к файлу (каталог + "/" + имя).
         * @return Имя пользователя-владельца или числовой UID в виде строки.
         *         Если файл не является обычным файлом (каталог, ссылка,
         *         устройство, сокет), возвращает пустую строку.
         * @throw ExceptionDirOrFilesWork Если stat(2) завершился ошибкой
         *        (например, файл удалён между readdir и stat — ENOENT).
         */
        static std::string getOwnerFile(const std::string & fullPathFile);

        public:
        /**
         * @brief Конструктор по умолчанию.
         * @note Класс не хранит состояния; конструктор помечен noexcept.
         */
        FilesPrinter() noexcept;

        /**
         * @brief Обойти каталог и вывести владельцев и имена всех обычных файлов.
         *
         * Открывает каталог через opendir(3), читает записи через readdir(3),
         * для каждой записи формирует полный путь и вызывает getOwnerFile.
         * Записи, не являющиеся обычными файлами (каталоги, ссылки, устройства),
         * пропускаются. По завершении обхода каталог закрывается через
         * closedir(3) и выводится сообщение "Read of dir is pass GOOD".
         *
         * @param targetDir Путь к каталогу для обхода.
         * @throw ExceptionDirOrFilesWork Если opendir(3) вернул NULL
         *        (каталог не существует или нет прав — EACCES, ENOENT),
         *        либо если readdir(3) завершился ошибкой (errno != 0).
         *
         * @note Вывод выполняется в std::cout в формате "owner|filename"
         *       (по одному файлу на строку).
         */
        static void printFilesInDirByUsers(const std::string & targetDir);
    };
}

#endif // FILES_HPP