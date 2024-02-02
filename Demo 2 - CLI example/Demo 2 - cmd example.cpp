//a simple Cmd application demo by Olivier Potgieter.

#include <iostream>
#include <string>
#include <cstdlib>
#include <regex>
#include <filesystem>
#include <map>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <cmath>
#include <unordered_set>



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
            "4) Option 5 (Exit)\n"
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
        {   system("cls");
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
        {   system("cls");
            bool option2Running = true;
            while (option2Running)
            {
                std::cout << "You selected option 2. Please enter a directory path\nor type 'back' to return to the main menu:" << std::endl;
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
        case 3:
        {   system("cls");
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
        case 4:
            system("cls");
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


// Simple function to Check that there are valid Text files within the directory.
std::vector<std::string> findTxtFilesInDirectory(const std::string& path) {
    std::vector<std::string> txtFiles;
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (entry.path().extension() == ".txt") {
            txtFiles.push_back(entry.path().filename().string());
        }
    }
    return txtFiles;
}
// Simple function to display the text files found in the directory.
void displayTxtFiles(const std::vector<std::string>& txtFiles) {
    std::cout << "Found the following text files:" << std::endl;
    for (int i = 0; i < txtFiles.size(); i++) {
        std::cout << i + 1 << ") " << txtFiles[i] << std::endl;
    }
    std::cout << "Please confirm with which file you wish to interact by entering its number:" << std::endl;
}
//Function that allows the code to interact with the selected file. most of the features are within this function.
void interactWithSelectedFile(const std::string& selectedFile) {
    std::string userInput;
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

    do {
        // TODO: Implement features to interact with the selected file
        std::cout << "Please select an option for what you want to do with this file : \n" << std::endl;
        std::cout << "1) Search for a word within the text file" << std::endl;
        std::cout << "2) Replace all word within the text file" << std::endl;
        std::cout << "3) Read File in reverse" << std::endl;
        std::cout << "4) Give Freqency of each word" << std::endl;
        std::cout << "5) List longest and shortest word" << std::endl;
        std::cout << "6) List Average word lenght" << std::endl;
        std::cout << "7) List total Number of unique words" << std::endl;
        std::cout << "8) List most and least frequent starting letter" << std::endl;
        std::cout << "9) List most and least frequent ending letter\n" << std::endl;
        std::cout << " Enter 'back' to go back to the file selection." << std::endl;
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
            system("cls");

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
        {
            system("cls");
            std::cout << "\nPlease note that you have selected the Replace function, due to this changing data\n";
            std::cout << "In order to adhere to best data practises a new file will be created with the _modified suffix\n";
            std::cout << "Please keep in mind that you will need to select the modified text file if you wish to continue working within it.\n" << std::endl;
            std::cout << "\nEnter the term you want to replace: ";
            std::string termToReplace;
            std::getline(std::cin, termToReplace);

            std::cout << "Enter the term you want to replace it with: ";
            std::string replacementTerm;
            std::getline(std::cin, replacementTerm);

            std::ifstream file(selectedFile);
            std::string line;
            int count = 0;

            // Create a new file with _modified appended to the name
            std::string modifiedFileName = selectedFile.substr(0, selectedFile.find_last_of(".")) + "_modified.txt";
            // Check if the file already exists
            if (std::filesystem::exists(modifiedFileName)) {
                std::cout << "\nA modified file with this name already exists. That file will now be overwritten.\n" << std::endl;
            }
            std::ofstream modifiedFile(modifiedFileName);

            while (std::getline(file, line))
            {
                size_t foundPos = line.find(termToReplace);
                while (foundPos != std::string::npos)
                {
                    line.replace(foundPos, termToReplace.length(), replacementTerm);
                    foundPos = line.find(termToReplace, foundPos + replacementTerm.length()); // search for next occurrence
                    count++;
                }
                modifiedFile << line << "\n";
            }

            file.close();
            modifiedFile.close();

            if (count == 0) {
                std::cout << "\nThe term was not found. No modifications were made.\n" << std::endl;
                // Delete the unmodified file
                std::filesystem::remove(modifiedFileName);
            }
            else {
                std::cout << "\nThe term was found " << count << " times and replaced.\n" << std::endl;
                std::cout << "The modified file has been saved as: " << modifiedFileName << std::endl;
                std::cout << "\n" << modifiedFileName << " will now be opened. \n" << std::endl;
                // Open the file
                std::string command = "start " + modifiedFileName;
                system(command.c_str());
                break;
            }
            break;
        }
        case 3:
        {   
            system("cls");
            std::ifstream file(selectedFile);
            std::vector<std::string> lines;
            std::string line;

            while (std::getline(file, line))
            {
                lines.push_back(line);
            }

            std::string reversedFileName = selectedFile.substr(0, selectedFile.find_last_of(".")) + "_reversed.txt";
            // Check if the file already exists
            if (std::filesystem::exists(reversedFileName)) {
                std::cout << "\nA reversed file with this name already exists. That file will now be overwritten.\n" << std::endl;
            }
            std::ofstream reversedFile(reversedFileName);

            for (auto it = lines.rbegin(); it != lines.rend(); ++it)
            {
                reversedFile << *it << "\n";
            }

            file.close();
            reversedFile.close();

            std::cout << "\nThe reversed file has been saved as: " << reversedFileName << std::endl;
            std::cout << "\n"<< reversedFileName <<" will now be opened. \n" << std::endl;
            // Open the file
            std::string command = "start " + reversedFileName;
            system(command.c_str());
            break;
        }
        case 4:
        {
            system("cls");
            std::ifstream file(selectedFile);
            std::unordered_map<std::string, int> wordFrequency;
            std::string word;

            while (file >> word)
            {
                // Remove punctuation marks from the word due to a counting error I encoutered I added the below 
                word.erase(std::remove_if(word.begin(), word.end(), ::ispunct), word.end());
                wordFrequency[word]++;
            }

            std::pair<std::string, int> mostFrequentWord = *std::max_element(wordFrequency.begin(), wordFrequency.end(),
                [](const auto& a, const auto& b) {
                return a.second < b.second;
            });

            std::cout << "Most occurring word: " << mostFrequentWord.first << " ,appeared " << mostFrequentWord.second << " times.\n";

            break;
        }
        case 5:
        {
            system("cls");
            std::ifstream file(selectedFile);
            std::string word;
            std::string longestWord;
            std::string shortestWord;

            if (file >> word) {
                // Remove punctuation from the first word
                word.erase(std::remove_if(word.begin(), word.end(), ::ispunct), word.end());
                longestWord = word;
                shortestWord = word;
            }

            while (file >> word)
            {
                std::string cleanWord = word;
                // Remove punctuation from the word
                cleanWord.erase(std::remove_if(cleanWord.begin(), cleanWord.end(), ::ispunct), cleanWord.end());
                
                if (cleanWord.length() > longestWord.length()) {
                    longestWord = cleanWord;
                }
                if (cleanWord.length() < shortestWord.length()) {
                    shortestWord = cleanWord;
                }
            }

            std::cout << "Longest word: " << longestWord << " ,length: " << longestWord.length() << "\n";
            std::cout << "Shortest word: " << shortestWord << " ,length: " << shortestWord.length() << "\n";

            break;
        }
        case 6:
        {
            system("cls");
            std::ifstream file(selectedFile);
            std::string word;
            int totalLength = 0;
            int wordCount = 0;

            while (file >> word)
            {
                totalLength += word.length();
                wordCount++;
            }

            double averageLength = static_cast<double>(totalLength) / wordCount;
            int roundedDown = std::floor(averageLength);
            int roundedUp = std::ceil(averageLength);

            std::cout << "Average word length: " << averageLength << " Characters long\n";
            std::cout << "Rounded down: " << roundedDown << " Characters long\n";
            std::cout << "Rounded up: " << roundedUp << " Characters long\n";

            break;
        }
        case 7:
        {
            std::ifstream file(selectedFile);
            std::unordered_set<std::string> uniqueWords;
            std::string word;

            while (file >> word)
            {
                // Remove punctuation from the word
                word.erase(std::remove_if(word.begin(), word.end(), ::ispunct), word.end());

                uniqueWords.insert(word);
            }

            std::cout << "Number of unique words: " << uniqueWords.size() << "\n";

            break;
        }
        case 8:
        {
            std::ifstream file(selectedFile);
            std::unordered_map<char, int> letterFrequency;
            std::string word;

            while (file >> word)
            {
                // Remove punctuation from the word
                word.erase(std::remove_if(word.begin(), word.end(), ::ispunct), word.end());

                if (!word.empty()) {
                    char firstLetter = std::tolower(word[0]);
                    letterFrequency[firstLetter]++;
                }
            }

            auto mostFrequentLetter = *std::max_element(letterFrequency.begin(), letterFrequency.end(),
                [](const auto& a, const auto& b) {
                return a.second < b.second;
            });

            auto leastFrequentLetter = *std::min_element(letterFrequency.begin(), letterFrequency.end(),
                [](const auto& a, const auto& b) {
                return a.second < b.second;
            });

            std::cout << "Most frequent starting letter: " << mostFrequentLetter.first << " ,appeared " << mostFrequentLetter.second << " times.\n";
            std::cout << "Least frequent starting letter: " << leastFrequentLetter.first << " ,appeared " << leastFrequentLetter.second << " times.\n";

            break;
        }
        case 9:
        {
            std::ifstream file(selectedFile);
            std::unordered_map<char, int> letterFrequency;
            std::string word;

            while (file >> word)
            {
                // Remove punctuation from the word
                word.erase(std::remove_if(word.begin(), word.end(), ::ispunct), word.end());

                if (!word.empty()) {
                    char lastLetter = std::tolower(word.back());
                    letterFrequency[lastLetter]++;
                }
            }

            auto mostFrequentLetter = *std::max_element(letterFrequency.begin(), letterFrequency.end(),
                [](const auto& a, const auto& b) {
                return a.second < b.second;
            });

            auto leastFrequentLetter = *std::min_element(letterFrequency.begin(), letterFrequency.end(),
                [](const auto& a, const auto& b) {
                return a.second < b.second;
            });

            std::cout << "Most frequent ending letter: " << mostFrequentLetter.first << ", appeared " << mostFrequentLetter.second << " times.\n";
            std::cout << "Least frequent ending letter: " << leastFrequentLetter.first << ", appeared " << leastFrequentLetter.second << " times.\n";

            break;
        }



        default:
            std::cout << "Invalid selection. Please enter a number from the list." << std::endl;
            break;
        }
    } while (userInput != "back");
}

