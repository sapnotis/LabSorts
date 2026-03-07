#include "animation.hpp"

#include "csv-parser.hpp"
#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>
#include <iostream>

void DisplaySortAnimation(FileParser& parser) {
    
    // get data prom parser

    std::vector<std::vector<int>> data = parser.getData();
    const unsigned int Nrows = data.size();
    const unsigned int Ncolumns = data[0].size();
    std::vector<int> FinalRow = data[Nrows-1];

    sf::Texture texture;
    if ( !texture.loadFromFile("images/" + parser.getImageName()) ) {
        std::cout << "Couldn't load texture" << std::endl;
        exit(0);
    }

    // unsigned int FrameTimeAsMs = 10 * 1000 / (Nrows - 1);

    // idiot check

    if ( Nrows == 0 || Ncolumns == 0 )
        exit(0);

    for ( unsigned int i=0; i < Nrows; i++ ) {
        if ( data[i].size() < Ncolumns )
            for ( unsigned int j = data[i].size(); j < Ncolumns; j++ )
                data[i].push_back(-1);
        else if ( data[i].size() > Ncolumns )
            for ( unsigned int j = Ncolumns; j < data[i].size(); j++ )
                data[i].pop_back();
    }

    // final position check

    std::vector<unsigned int> iteration_when_ready(Ncolumns, Nrows-1);

    for (unsigned int i=Nrows-1; i > 0; i--)
        for (unsigned int j=0; j < Ncolumns; j++)
            if ( data[i-1][j] == data[i][j] && iteration_when_ready[j] == i )
                iteration_when_ready[j] -= 1;

    // window

    // ==================== TIME CONFIG HERE ====================

    const unsigned int prepare_delay_ms = 0;
    const unsigned int sorting_time_ms = 9'000;
    const unsigned int admire_delay_ms = 3'000;

    // ==================== TIME CONFIG HERE ====================

    std::cout << "Disks:\tSteps:\tTime ms:" << std::endl;
    std::cout << Ncolumns << "\t" << Nrows << "\t" << prepare_delay_ms << " + " << sorting_time_ms << " + " << admire_delay_ms << std::endl;
    std::cout << "Running window" << std::endl;
    
    sf::RenderWindow window(sf::VideoMode(1294, 800), "Sorting: " + parser.getFileName());
    // window.setFramerateLimit(50);

    // disks setup

    unsigned int imgx = texture.getSize().x;
    unsigned int imgy = texture.getSize().y;

    std::vector<int> DesiredPositions = data[0];
    std::vector<sf::CircleShape> shades;
    std::vector<sf::CircleShape> disks;
    float minradius = ( ( 0.5 * window.getSize().y ) - 20 ) / Ncolumns;

    float k = 1.f / (Ncolumns);

    for ( int i = Ncolumns; i >= 1; i-- ) {

        sf::CircleShape disk( minradius*i );
        disk.setOrigin( minradius*i, minradius*i );
        disk.setTexture(&texture);

        float rel = (Ncolumns-i)*k;
        disk.setTextureRect(sf::IntRect(
            0.5*rel*imgx, 0.5*rel*imgy,
            imgx-rel*imgx, imgy-rel*imgy
        ));

        disk.setPosition( 0.5 * window.getSize().x, 0.5 * window.getSize().y );
        disks.push_back(disk);

        sf::CircleShape shade( minradius*i + 1 );
        shade.setOrigin( minradius*i + 1, minradius*i + 1 );
        shade.setFillColor(sf::Color(0, 0, 0, 100));
        shade.setPosition( 0.5 * window.getSize().x, 0.5 * window.getSize().y );
        shades.push_back(shade);
    }

    disks[0].setOutlineThickness(10);
    disks[0].setOutlineColor(sf::Color::Black);

    // sortings iteration calculation & display

    unsigned int iteration = 0;
    // bool IsMoving = false;

    sf::Clock clock;
    clock.restart();
    
    std::this_thread::sleep_for(std::chrono::milliseconds( prepare_delay_ms ));
    std::chrono::time_point time_of_start = std::chrono::high_resolution_clock::now();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::Resized)
            {
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }
        }

        std::chrono::time_point now = std::chrono::high_resolution_clock::now();
        auto time_of_running = std::chrono::duration_cast<std::chrono::milliseconds>(now - time_of_start);
        unsigned int time_of_running_ms = time_of_running.count();

        unsigned int desired_iteration = (unsigned int)( time_of_running_ms * Nrows / sorting_time_ms );

        if ( clock.getElapsedTime().asMilliseconds() > 200 ) {

            clock.restart();
            if ( desired_iteration > iteration )
                iteration = desired_iteration;
        }
        
        if ( iteration < Nrows )
            for ( unsigned int i = 0; i < Ncolumns; i++ )
                DesiredPositions[i] = 360.f / Ncolumns * ( data[iteration][i] - FinalRow[i] );
        else {
            for ( unsigned int i = 0; i < Ncolumns; i++ )
                DesiredPositions[i] = 0;
            if ( time_of_running_ms > sorting_time_ms + admire_delay_ms )
                window.close();
        }

        // display

        window.clear(sf::Color(120, 120, 120));

        if ( time_of_running_ms > sorting_time_ms ) {

            float rel = 1.5f * (time_of_running_ms - sorting_time_ms) / admire_delay_ms;
            if ( rel > 1.f )
                rel = 1.f;

            sf::RectangleShape admire_progress( { (float)window.getSize().x * rel, (float)window.getSize().y } );
            admire_progress.setFillColor( sf::Color(100, 100, 100, 255*(1-rel) ) );

            admire_progress.setPosition( 0, 0 );
            window.draw(admire_progress);
        }

        for ( unsigned int i = 0; i < disks.size(); i++ ) {

            disks[i].setRotation( (unsigned int)disks[i].getRotation() % 360 );

            int deltaRotation = (float)( DesiredPositions[i] ) - disks[i].getRotation();
            deltaRotation %= 360;

            if ( deltaRotation > 180 )
                deltaRotation -= 360;

            // std::cout << deltaRotation << " ";

            disks[i].rotate( 0.2 * (float)deltaRotation );

            window.draw(disks[i]);

            if ( iteration < iteration_when_ready[i] )
                window.draw(shades[i]);
        }
        // std::cout << std::endl;

        window.display();
    }
    
    std::cout << "Exit (success)" << std::endl;
}