//a simple Cmd application demo by Olivier Potgieter.

#include <iostream>
#include <string>
#include <cstdlib>
#include <regex>
#include <filesystem>
#include <map>
#include <fstream>
#include <sstream>


// function declarations for compiler
bool isValidIpAddress(const std::string& ipAddress);
bool isValidUrl(const std::string& url);
std::vector<std::string> findTxtFilesInDirectory(const std::string& path);
void displayTxtFiles(const std::vector<std::string>& txtFiles);
void interactWithSelectedFile(const std::string& selectedFile);

int main()
{
    bool running = true;

    while (running)
    {
        std::cout << "Welcome to a simple demo application to showcase cmd functions.\n"
            "This application was created by Olivier Potgieter.\n"
            "Please select an option for what you would to do. \n"
            "1) Option 1 Ping command\n"
            "2) Option 2 Directory Summary\n"
            "3) Option 3 Text file explorer\n"
            "4) Option 4 Regex Demo\n"
            "5) Option 5 (Exit)\n"
            "Enter 'h' for help" << std::endl;
        std::string userInput;
        std::getline(std::cin, userInput);

        if (userInput == "h")
        {
            bool option1Running = true;
            while (option1Running)
            {
                std::cout << "In order to use this application please select an input that is listed before the option, for example if you want 1) Option 1, please enter 1.\n" << std::endl;
                std::getline(std::cin, userInput);
                if (userInput == "back")
                {
                    option1Running = false;
                }
            }
        }
        int selection;
        try {
            selection = std::stoi(userInput);
        }
        catch (std::invalid_argument&) {
            std::cout << "Invalid input: " << userInput << std::endl;
            continue;
        }

        switch (selection)
        {
            //Ping Demo
        case 1:
        {
            bool option1Running = true;
            while (option1Running)
            {
                std::cout << "You are in option 1. This is a Ping function, please enter an IP address or URL Enter 'back' to go back to the main menu." << std::endl;
                std::getline(std::cin, userInput);
                if (userInput == "back")
                {
                    option1Running = false;
                }
                else if (isValidIpAddress(userInput))
                {
                    std::cout << "You have entered a valid IP address: " << userInput << std::endl;
                    std::string command = "ping " + userInput;
                    system(command.c_str());
                    std::cout << "\nPing operation completed for IP address: " << userInput << "\n" << std::endl;
                }
                else if (isValidUrl(userInput))
                {
                    std::cout << "You have entered a valid URL: " << userInput << std::endl;
                    std::string command = "ping " + userInput;
                    system(command.c_str());
                    std::cout << "\nPing operation completed for URL: " << userInput << "\n" << std::endl;
                }
                else
                {
                    std::cout << "Invalid input: " << userInput << " Please try again \n" << std::endl;
                }
            }
            break;
        }
        //Directory Demo
        //IMPORT NOTE: due to using filesystem, this will only work on C++17 or higher
        case 2:
        {
            bool option2Running = true;
            while (option2Running)
            {
                std::cout << "You selected option 2. Please enter a directory path or type 'back' to return to the main menu:" << std::endl;
                std::getline(std::cin, userInput);
                if (userInput == "back")
                {
                    option2Running = false;
                }
                else
                {
                    std::filesystem::path dirPath(userInput);
                    if (std::filesystem::exists(dirPath) && std::filesystem::is_directory(dirPath))
                    {
                        unsigned int fileCount = 0;
                        unsigned int dirCount = 0;
                        uintmax_t totalSize = 0;
                        std::map<std::string, int> extensionCount;

                        for (const auto& entry : std::filesystem::recursive_directory_iterator(dirPath))
                        {
                            if (std::filesystem::is_regular_file(entry.status()))
                            {
                                fileCount++;
                                totalSize += std::filesystem::file_size(entry);
                                extensionCount[entry.path().extension().string()]++;
                            }
                            else if (std::filesystem::is_directory(entry.status()))
                            {
                                dirCount++;
                            }
                        }

                        std::cout << "\nTotal number of files: " << fileCount << std::endl;
                        std::cout << "\nTotal number of directories: " << dirCount << std::endl;
                        std::cout << "\nTotal size of all files: " << totalSize << " bytes" << std::endl;

                        std::filesystem::path largestFile;
                        uintmax_t largestFileSize = 0;

                        std::filesystem::path smallestFile;
                        uintmax_t smallestFileSize = std::numeric_limits<uintmax_t>::max();

                        std::filesystem::path mostRecentlyModifiedFile;
                        std::filesystem::file_time_type mostRecentModificationTime;

                        for (const auto& entry : std::filesystem::recursive_directory_iterator(dirPath))
                        {
                            if (std::filesystem::is_regular_file(entry.status()))
                            {
                                uintmax_t fileSize = std::filesystem::file_size(entry);

                                if (fileSize > largestFileSize)
                                {
                                    largestFileSize = fileSize;
                                    largestFile = entry.path();
                                }

                                if (fileSize < smallestFileSize)
                                {
                                    smallestFileSize = fileSize;
                                    smallestFile = entry.path();
                                }

                                auto modificationTime = std::filesystem::last_write_time(entry);
                                if (modificationTime > mostRecentModificationTime)
                                {
                                    mostRecentModificationTime = modificationTime;
                                    mostRecentlyModifiedFile = entry.path();
                                }
                            }
                        }

                        std::pair<std::string, int> mostCommonExtension = *std::max_element(
                            extensionCount.begin(), extensionCount.end(),
                            [](const auto& a, const auto& b) { return a.second < b.second; });

                        std::pair<std::string, int> leastCommonExtension = *std::min_element(
                            extensionCount.begin(), extensionCount.end(),
                            [](const auto& a, const auto& b) { return a.second < b.second; });

                        //Just cleaning up the text to display correctly
                        std::string pathStr = largestFile.string();
                        std::replace(pathStr.begin(), pathStr.end(), '\\', '/');
                        std::cout << "\nLargest file: " << pathStr << " (" << largestFileSize << " bytes)\n" << std::endl;


                        std::string pathStr2 = smallestFile.string();
                        std::replace(pathStr2.begin(), pathStr2.end(), '\\', '/');
                        std::cout << "Smallest file: " << pathStr2 << " (" << smallestFileSize << " bytes)\n" << std::endl;


                        std::string pathStr3 = mostRecentlyModifiedFile.string();
                        std::replace(pathStr3.begin(), pathStr3.end(), '\\', '/');
                        std::cout << "Most recently modified file: " << pathStr3 << "\n" << std::endl;

                        std::cout << "Most common file extension: " << mostCommonExtension.first << " (" << mostCommonExtension.second << " occurrences)\n" << std::endl;
                        std::cout << "Least common file extension: " << leastCommonExtension.first << " (" << leastCommonExtension.second << " occurrences)\n" << std::endl;
                    }
                    else
                    {
                        std::cout << "\nInvalid directory path: " << userInput << "\nPlease try again." << std::endl;
                    }
                }
            }
            break;
        }
        //TODO: Add text file explorer
        case 3:
        {
            bool option3Running = true;
            while (option3Running)
            {
                std::cout << "You are in option 3. Enter 'back' to go back to the main menu." << std::endl;
                std::string path = "./"; // path to your project directory
                std::vector<std::string> txtFiles = findTxtFilesInDirectory(path);

                if (txtFiles.empty()) {
                    std::cout << "No text files were found in the project directory: " << std::filesystem::absolute(path) << std::endl;
                }
                else {
                    displayTxtFiles(txtFiles);
                }

                std::string userInput;
                std::getline(std::cin, userInput);
                if (userInput == "back")
                {
                    option3Running = false;
                }
                else
                {
                    int fileNumber = std::stoi(userInput);
                    if (fileNumber > 0 && fileNumber <= txtFiles.size()) {
                        std::string selectedFile = txtFiles[fileNumber - 1];
                        std::cout << "You selected: " << selectedFile  << "\n" << std::endl;

                        interactWithSelectedFile(selectedFile);
                    }
                    else {
                        std::cout << "Invalid selection. Please enter a number from the list." << std::endl;
                    }
                }
            }
            break;
        }
        //TODO: add one more feature or remove?
        case 4:
        {
            bool option1Running = true;
            while (option1Running)
            {
                std::cout << "You are in option 4. Enter 'back' to go back to the main menu. --Function not complete yet" << std::endl;
                std::getline(std::cin, userInput);
                if (userInput == "back")
                {
                    option1Running = false;
                }
            }
            break;
        }
        case 5:
            std::cout << "Exiting the application..." << std::endl;
            running = false;
            break;
        default:
            std::cout << "Invalid input: " << userInput << std::endl;
            break;
        }
    }

    std::cout << "The application has concluded. Thank you for viewing my work." << std::endl;
    std::cin.get();
    return 0;
}


//Simple function to just ensure that the IP address is within the correct format and lenght to be a valid IP address.
bool isValidIpAddress(const std::string& ipAddress)
{
    int num = 0;
    int dots = 0;
    int len = ipAddress.length();
    if (len < 7 || len > 15)
        return false;
    for (int i = 0; i < len; i++) {
        char c = ipAddress[i];
        if (c == '.') {
            if (++dots > 3)
                return false;
            if (num > 255)
                return false;
            num = 0;
        }
        else if (c >= '0' && c <= '9') {
            num = num * 10 + (c - '0');
            if (num > 255)
                return false;
        }
        else
            return false;
    }
    if (dots < 3)
        return false;
    return true;
}

//Simple function to just ensure that the URL is within the correct format and lenght to be a valid URL. 
// Note this approach is lazy and is simply using regex, better solutin would be to add a list of known domains for the the final part of the check instead of just ensuring the url ends with .something
bool isValidUrl(const std::string& url)
{
    std::regex urlPattern("^www\\..+\\..+$");
    return std::regex_match(url, urlPattern);
}


// Simple function to Text Explorer
std::vector<std::string> findTxtFilesInDirectory(const std::string& path) {
    std::vector<std::string> txtFiles;
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (entry.path().extension() == ".txt") {
            txtFiles.push_back(entry.path().filename().string());
        }
    }
    return txtFiles;
}

void displayTxtFiles(const std::vector<std::string>& txtFiles) {
    std::cout << "Found the following text files:" << std::endl;
    for (int i = 0; i < txtFiles.size(); i++) {
        std::cout << i + 1 << ") " << txtFiles[i] << std::endl;
    }
    std::cout << "Please confirm with which file you wish to interact by entering its number:" << std::endl;
}

void interactWithSelectedFile(const std::string& selectedFile) {
    std::string userInput;
    do {
        std::ifstream file(selectedFile);
        std::string word;
        int wordCount = 0;
        int lineCount = 0;
        int charCount = 0;
        std::string line;

        while (std::getline(file, line))
        {
            lineCount++;
            charCount += line.length(); // count characters in each line
            std::istringstream iss(line);
            while (iss >> word)
            {
                wordCount++;
            }
        }

        std::cout << "Word count: " << wordCount << std::endl;
        std::cout << "Line count: " << lineCount << std::endl;
        std::cout << "Character count: " << charCount << "\n" << std::endl;

        // TODO: Implement features to interact with the selected file
        std::cout << "Please select an option for what you want to do with this file : \n" << std::endl;
        std::cout << "1) Search for a word within the text file" << std::endl;
        std::cout << "2) Replace all word within the text file" << std::endl;
        std::cout << "3) Read File in reverse" << std::endl;
        std::cout << "4) Give Freqency of each word" << std::endl;
        std::cout << "5) List longest and shortest word" << std::endl;
        std::cout << "6) List Average word lenght\n" << std::endl;
        std::cout << "7) Enter 'back' to go back to the file selection." << std::endl;
        std::getline(std::cin, userInput);

        int selection;
        try {
            selection = std::stoi(userInput);
        }
        catch (std::invalid_argument&) {
            std::cout << "Invalid input: " << userInput << std::endl;
            continue;
        }

        switch (selection)
        {
        case 1:
        {
            std::cout << "\nEnter the term you want to search for: ";
            std::string searchTerm;
            std::getline(std::cin, searchTerm);

            std::ifstream file(selectedFile);
            std::string line;
            int position = 0;
            int count = 0;

            while (std::getline(file, line))
            {
                size_t foundPos = line.find(searchTerm);
                while (foundPos != std::string::npos)
                {
                    std::cout << "Term found at character position: " << position + foundPos << std::endl;
                    std::cout << "Term ends at character position: " << position + foundPos + searchTerm.length() - 1 << std::endl;
                    count++;
                    foundPos = line.find(searchTerm, foundPos + 1); // search for next occurrence
                }
                position += line.length() + 1; // +1 for the newline character that std::getline skips
            }

            if (count == 0) {
                std::cout << "\nThe term was not found.\n" << std::endl;
            }
            else {
                std::cout << "\nThe term was found " << count << " times.\n" << std::endl;
            }
            break;
        }
        case 2:
            // Handle "Replace all word within the text file"
            break;
        case 3:
            // Handle "Read File in reverse"
            break;
        case 4:
            // Handle "Give Frequency of each word"
            break;
        case 5:
            // Handle "List longest and shortest word"
            break;
        case 6:
            // Handle "List Average word length"
            break;
        default:
            std::cout << "Invalid selection. Please enter a number from the list." << std::endl;
            break;
        }
    } while (userInput != "back");
}

