#include <fstream>
#include <sstream>

#include "configreader.h"

Configreader::Configreader()
    : today(setToday())
    , configPath(setConfigPath())
    , roundCount(0) // Initialize with default values
    , focusCount(0)
{
    std::string line, configDate = "0", rc, fc;
    std::ifstream filestream(this->getConfigPath());
    // If configFile exists
    if (filestream.is_open()) {
        std::getline(filestream, line);
        std::istringstream linestream(line);
        linestream >> configDate >> rc >> fc;

        // If there is already a session from today, read those values.
        if (configDate == this->getToday()) {
            try { this->roundCount = std::stoi(rc, nullptr, 10); }
            catch (...) { this->roundCount = 0; }
            try { this->focusCount = std::stoi(fc, nullptr, 10); }
            catch (...) { this->focusCount = 0; }
        }
    }
}

// Getter
std::string Configreader::getToday() const { return this->today; }
std::string Configreader::getConfigPath() const { return this->configPath; }

int Configreader::initRoundCount() const { return this->roundCount; }
int Configreader::initFocusCount() const { return this->focusCount; }

// Setter
// Set the current date as a unique identifier
std::string Configreader::setToday() {
    std::time_t t = std::time(nullptr);
    std::tm *today = std::localtime(&t);
    return std::to_string(today->tm_year+1900) + std::to_string(today->tm_yday);
}

// C-style way to get the users home directory
std::string Configreader::setConfigPath() {
    struct passwd *pw = getpwuid(getuid());
    return std::string{pw->pw_dir} + "/.config/PomodoroConfig";
}

// On exit: Write the current count stats into a file on the users computer.
// User can pick up where he/she left if it is on the same day.
void Configreader::setConfig(int currentRoundCount, int currentFocusCount) {
    std::ofstream configFile;
    configFile.open(this->getConfigPath());
    configFile << this->getToday()
               << ' ' << std::to_string(currentRoundCount)
               << ' ' << std::to_string(currentFocusCount);
    configFile.close();
}
