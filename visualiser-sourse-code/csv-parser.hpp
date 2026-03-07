#ifndef _CSV_PARSER_
#define _CSV_PARSER_

#include <string>
#include <vector>
using std::string;

class FileParser {
private:
    string data_file;
    string image_file;
    std::vector<std::vector<int>> data;
public:
    FileParser(int argc, char *argv[]);
    ~FileParser();
    string getFileName();
    string getImageName();
    std::vector<std::vector<int>> getData();
};

#endif