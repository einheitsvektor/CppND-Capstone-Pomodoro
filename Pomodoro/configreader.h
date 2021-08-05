#ifndef CONFIGREADER_H
#define CONFIGREADER_H

#include <ctime>
#include <unistd.h>
#include <string>
#include <sys/types.h>
#include <pwd.h>
#include <fstream>
#include <sstream>

class Configreader {
public:
    Configreader();
    // Getter
    std::string getToday() const;
    std::string getConfigPath() const;
    int initRoundCount() const;
    int initFocusCount() const;

    // Setter for destructor
    void setConfig(int currentRoundCount, int currentFocusCount);

private:
    const std::string today;
    const std::string configPath;
    int roundCount;
    int focusCount;

    std::string setToday();
    std::string setConfigPath();
};

#endif // CONFIGREADER_H
