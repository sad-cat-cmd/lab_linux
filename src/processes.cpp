#include "processes.hpp"

namespace processes {
    ExceptionProcess::ExceptionProcess(const std::string & msgErr,
                                       const int codeErr)
                                                     : msgErr(msgErr),
                                                       codeErr(codeErr)
    {}
    std::string ExceptionProcess::getMsgErr() noexcept
    {
        return this->msgErr;
    }
    int ExceptionProcess::getCodeErr() noexcept
    {
        return this->codeErr;
    }

    Process::Process() noexcept
    {}
    Process::~Process() noexcept
    {}

    void Process::writeLsIntoFile()
    {
        std::string fileName = std::to_string(this->pid) + ".txt";
        // только для записи + создание если нет + затереть, если уже есть
        int fd = open(fileName.c_str(),
                      O_WRONLY | O_CREAT | O_TRUNC,
                      0644);
        if (fd == -1) {
            std::cerr << "open(" << fileName << "): "
                          << strerror(errno) << "\n";
            _exit(1);
        }
        if (dup2(fd, STDOUT_FILENO) == -1) {
            std::cerr << "dup2: " << strerror(errno) << "\n";
            close(fd);
            _exit(1);
        }
        close(fd);
        execlp("ls", "ls", "-l", (char*)nullptr);

        std::cerr << "exec ls: " << strerror(errno) << "\n";
        _exit(1);
    }
    void Process::init(){
        pid_t result = fork();
        if (result == -1) {
            throw ExceptionProcess(std::string("error create (fork):") + strerror(errno),
                                   -1);      
        }
        // функциональность дочернего процесса
        if (result == 0) 
        {
            this->pid = getpid();
            this->writeLsIntoFile();
        }
    }
    pid_t Process::getPid() noexcept {
        return this->pid;
    }
}