#include "processes.hpp"
#include <list>

inline void printErr(const std::string & msgErr,
                     int codeErr)
{
    std::cout << msgErr << "\n" << codeErr << std::endl;
}

int main() {
    std::list<processes::Process *> processes;
    for (int i = 0; i < 5; i++) {
        processes::Process * newProcess = new processes::Process();
        processes.push_back(newProcess);
        try {
            newProcess->init();
        }
        catch (processes::ExceptionProcess & exc) {
            printErr(exc.getMsgErr(),
                     exc.getCodeErr());
        }
    }
        for (int i = 0; i < 5; i++) {
        int status = 0;
        // ждать всех + статус завершения в status + родитель спит
        pid_t done = waitpid(-1, &status, 0);
        if (done == -1) {
            if (errno == ECHILD) {
                break; 
            }   // больше нет детей
            std::cerr << "waitpid: " << strerror(errno) << "\n";
        }
    }
    return 0;
}