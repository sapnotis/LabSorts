#include <SFML/Graphics.hpp>

#include "csv-parser.hpp"
#include "animation.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    FileParser parser(argc, argv);
    DisplaySortAnimation(parser);

    return 0;
}