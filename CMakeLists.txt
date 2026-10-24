cmake_minimum_required(VERSION 3.16)

# Имя проекта
project(lab1 LANGUAGES CXX)

# 1. Устанавливаем стандарт C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # Отключаем GNU-расширения для чистого C++17

# 3. Собираем список исходников
# Вместо GLOB (который не рекомендуется для больших проектов [citation:4]) лучше перечислить файлы явно.
# Добавьте сюда все свои .cpp файлы из папки src
set(SOURCES
    src/main.cpp
    src/files.cpp
)

# 4. Создаем исполняемый файл
add_executable(${PROJECT_NAME} ${SOURCES})

# 5. Подключаем папку с заголовками
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 6. Включаем предупреждения компилятора, чтобы лучше видеть ошибки
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)

# 7. Для отладки можно установить тип сборки по умолчанию
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()