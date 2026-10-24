#include "files.hpp"

const std::string & DIR_FOR_SCANNING = std::string("/lib/");

inline void printInfoAboutErr(const std::string & errMsg,
                              int errCode) 
{
    std::cout << errMsg << std::endl;
    std::cout << errCode << std::endl;
}

int main() {
    std::cout << "Current dir: " + DIR_FOR_SCANNING << std::endl;
    try {
        files_utils::FilesPrinter::printFilesInDirByUsers(DIR_FOR_SCANNING);
    }
    catch (files_utils::ExceptionDirOrFilesWork & exc) {
        printInfoAboutErr(exc.getMsgErr(),
                          exc.getCodeErr());
    }
    return 1;
}