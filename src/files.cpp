#include "files.hpp"

namespace files_utils {
    ExceptionDirOrFilesWork::ExceptionDirOrFilesWork(const std::string & msgErr,
                                                     const int codeErr)
                                                     : msgErr(msgErr),
                                                       codeErr(codeErr)
    {}
    std::string ExceptionDirOrFilesWork::getMsgErr() noexcept
    {
        return this->msgErr;
    }
    int ExceptionDirOrFilesWork::getCodeErr() noexcept
    {
        return this->codeErr;
    }

    FilesPrinter::FilesPrinter() noexcept
    {}
    
    void FilesPrinter::printOwnerAndFilename(const std::string & owner,
                                             const std::string & filename)
    {
        std::cout << owner << "|" << filename << std::endl;
    }   
    
    std::string FilesPrinter::getOwnerFile(const std::string & fullPathFile)
    {
        std::string owner;
        struct stat st;
        struct passwd * pw;
        if (stat(fullPathFile.c_str(), &st) == -1) {
            // файл удален между получением записи о файле и выводом на экран
            throw ExceptionDirOrFilesWork(MSG_ERR_FOR_DONT_OPEN_FILE + strerror(errno),
                                          errno);
        }
        if (!S_ISREG(st.st_mode)) {
            // не файл
            return std::string();
        }
        pw = getpwuid(st.st_uid);
        if (pw != NULL) {
            owner = std::string(pw->pw_name);
        }
        else {
            owner = std::to_string(st.st_uid);
        }
        return owner;
    }

    void FilesPrinter::printFilesInDirByUsers(const std::string & targetDir)
    {
        std::string owner;
        std::string filename;
        DIR * dir = opendir(targetDir.data());
        if (dir == NULL) {
            throw ExceptionDirOrFilesWork(MSG_ERR_FOR_DONT_OPEN_DIR + strerror(errno),
                                          errno);
        }
        struct dirent *entry;
        while (1) {
            errno = 0;
            entry = readdir(dir);

            if (entry == NULL) {
                if (errno != 0) {
                    throw ExceptionDirOrFilesWork(MSG_ERR_FOR_DONT_OPEN_DIR + strerror(errno),
                                                  errno);
                }
                else {
                    std::cout << "Read of dir is pass GOOD" << std::endl;
                }
                closedir(dir);
                break;
            }
            
            filename = std::string(entry->d_name);
            owner = FilesPrinter::getOwnerFile(std::string(targetDir + "/" + entry->d_name));
            if (owner.empty()) {
                continue;
            }
            FilesPrinter::printOwnerAndFilename(owner,
                                                filename);
        }
    }
}