#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp> // for SFML
#include "BFS.h"
#include "DFS.h"

using namespace sf;
using namespace std;

int main(){
    string fileName;
    cout << "Please input the name of the file: ";
    cin >> fileName;

    ifstream Maze(fileName);
    if (!Maze){
        cerr << "Can't open input file!";
        return -1;
    }

    int R, C;
    Maze >> R >> C;

    char** contentsArray = new char* [R];
    for (int i = 0; i < R; i++){
        contentsArray[i] = new char[C + 1];  // +1 for null-termination if needed
        Maze >> ws;  // Eat up any leading whitespace
        Maze.read(contentsArray[i], C);
    }

    Maze.close();

    RenderWindow window(VideoMode(15 * C, 15 * R), "Maze Drawer");

    vector<RectangleShape> rectangles(R * C);
    int startingPos[2] = {-1, -1};  // Initialization moved here to ensure it is always set

    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            int index = i * C + j;
            rectangles[index].setSize(Vector2f(15, 15));
            rectangles[index].setPosition(j * 15, i * 15);
            if(contentsArray[i][j] == '#') {
                rectangles[index].setFillColor(Color::Black);
            } else if(contentsArray[i][j] == '.') {
                rectangles[index].setFillColor(Color::White);
            } else if(contentsArray[i][j] == 'S') {
                rectangles[index].setFillColor(Color::Green);
                startingPos[0] = i;
                startingPos[1] = j;
            } else if(contentsArray[i][j] == 'E') {
                rectangles[index].setFillColor(Color::Red);
            }
        }
    }

    string searchingMethod;
    cout << "Please input the desired searching method (DFS/BFS): ";
    cin >> searchingMethod;
    while (searchingMethod != "DFS" && searchingMethod != "BFS") {
        cout << "Please input a valid option (DFS/BFS): ";
        cin >> searchingMethod;
    }

    // Clear window and draw initial maze state
    window.clear();
    for (auto &rect : rectangles) {
        window.draw(rect);
    }
    window.display();

    if (searchingMethod == "BFS") {
        BFS(window, startingPos, R, C, contentsArray);
    } else {
        DFS(window, startingPos, R, C, contentsArray);
    }

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }
    }

    // Deallocate each sub-array
    for (int i = 0; i < R; i++) {
        delete[] contentsArray[i];
    }
    delete[] contentsArray;

    return 0;
}
