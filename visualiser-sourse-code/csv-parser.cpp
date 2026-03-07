#include "csv-parser.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
using std::filesystem::directory_iterator;

unsigned int str2uns(std::string str) {
    unsigned int uns = 0;
    while ( !str.empty() ) {
        if ( 48 <= (unsigned int)str[0] && (unsigned int)str[0] <= 57 ) {
            uns *= 10;
            uns += (unsigned int)(str[0] - '0');
        }
        str.erase(0, 1);
    }
    return uns;
}

FileParser::FileParser(int argc, char *argv[])
{
    if (argc <= 1) {
        for (const auto & entry : directory_iterator("sorts"))
            std::cout << entry.path() << std::endl;
        for (const auto & entry : directory_iterator("images"))
            std::cout << entry.path() << std::endl;
        exit(0);
    }

    if (argc == 2) {
        std::cout << "Use args: datasheet, image" << std::endl;
        std::cout << "e.g.: \"datasheet.csv image.png\"" << std::endl;
        exit(0);
    }

    if (argc >= 4) {
        std::cout << "Too many arguments" << std::endl;
        exit(0);
    }

    data_file = argv[1];
    image_file = argv[2];

    std::ifstream file("sorts/" + data_file);
    std::ifstream image("images/" + image_file);

    if ( !file.is_open() || !image.is_open() ) {
        if ( !file.is_open() )
            std::cout << "Visualizer couldn't find \"" + data_file + "\" in sorts/. Run with no arguments to see all files" << std::endl;
        if ( !image.is_open() )
            std::cout << "Visualizer couldn't find \"" + image_file + "\" in images/. Run with no arguments to see all files" << std::endl;
        exit(0);
    }
    
    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        std::vector<int> row;
        unsigned int number;

        while (std::getline(ss, cell, ',')) {
            number = str2uns(cell);
            row.push_back(number);
        }
        data.push_back(row);
    }

    file.close();
    image.close();
}

FileParser::~FileParser() { }

string FileParser::getFileName()
{
    return this->data_file;
}

string FileParser::getImageName()
{
    return this->image_file;
}

std::vector<std::vector<int>> FileParser::getData()
{
    return this->data;
}