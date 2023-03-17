#include "utils.h"
#include "3rdparty/pstream.h"
#include "logger.h"
#include <sstream>

namespace Utils
{

// based on 3rd party lib (http://pstreams.sourceforge.net/)
int executeCommand(const std::string &cmd, const std::vector<std::string> &args,
                   std::string *pOutputStr, bool appendFromStdErr)
{
    std::string cmdLine = cmd;

    for (auto it = args.begin(); it != args.end(); ++it)
    {
        cmdLine += " \"";
        cmdLine += *it;
        cmdLine += "\"";
    }

    if (pOutputStr)
    {
        pOutputStr->clear();
    }

    redi::ipstream proc(cmdLine, redi::pstreams::pstdout | redi::pstreams::pstderr);
    std::string line;
    // read child's stdout
    while (std::getline(proc.out(), line))
    {
        if (pOutputStr)
        {
            *pOutputStr += line + "\n";
        }
    }
    // if reading stdout stopped at EOF then reset the state:
    if (proc.eof() && proc.fail())
    {
        proc.clear();
    }

    if (appendFromStdErr) {
        // read child's stderr
        while (std::getline(proc.err(), line))
        {
            if (pOutputStr)
            {
                *pOutputStr += line + "\n";
            }
        }
    }

    proc.close();
    if (proc.rdbuf()->exited())
    {
         return proc.rdbuf()->status();
    }
    return 0;
}


size_t findCaseInsensitive(std::string data, std::string toSearch, size_t pos)
{
    std::transform(data.begin(), data.end(), data.begin(), ::tolower);
    std::transform(toSearch.begin(), toSearch.end(), toSearch.begin(), ::tolower);
    return data.find(toSearch, pos);
}

std::string getFullCommand(const std::string &exePath, const std::string &executable, const std::string &arguments)
{
    char *canonicalPath = realpath(exePath.c_str(), NULL);
    if (!canonicalPath) {
        LOG("Executable not in valid path, ignoring.");
        return "";
    }

#if defined(USE_SIGNATURE_CHECK)
    if (std::string(canonicalPath).rfind("/Applications/Windscribe.app", 0) != 0) {
        // Don't execute arbitrary commands, only executables that are in our application directory
        LOG("Executable not in correct path, ignoring.");
        free(canonicalPath);
        return "";
    }
#endif
    
    std::string fullCmd = std::string(canonicalPath) + "/" + executable + " " + arguments;
    LOG("Resolved command: %s", fullCmd.c_str());
    free(canonicalPath);

    if (fullCmd.find(";") != std::string::npos || fullCmd.find("|") != std::string::npos || fullCmd.find("&") != std::string::npos) {
        // Don't execute commands with dangerous pipes or delimiters
        LOG("Executable command contains invalid characters, ignoring.");
        return "";
    }

    return fullCmd;
}

std::vector<std::string> getOpenVpnExeNames()
{
    std::vector<std::string> ret;
    std::string list;
    std::string item;
    int rc = 0;

    rc = Utils::executeCommand("ls", {"/Applications/Windscribe.app/Contents/Helpers"}, &list);
    if (rc != 0) {
        return ret;
    }

    std::istringstream stream(list);
    while (std::getline(stream, item)) {
        if (item.find("openvpn") != std::string::npos) {
            ret.push_back(item);
        }
    }
    return ret;
}

} // namespace Utils