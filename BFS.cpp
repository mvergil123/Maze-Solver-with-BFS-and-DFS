#include "BFS.h"
#include <queue>
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp> // for SFML

using namespace sf;
using namespace std;

void BFS(sf::RenderWindow& window, int(&StartingPOS)[2], int row, int column, char** contentsArray) {
    // Direction vectors for moving in the 4 cardinal directions (up, left, down, right)
    int dirX[4] = {0, -1, 0, 1};
    int dirY[4] = {-1, 0, 1, 0};

    // Setup the queue for BFS with the initial position and mark it as visited
    queue<pair<int, int>> q;
    vector<vector<bool>> visited(row, vector<bool>(column, false));
    q.push({StartingPOS[0], StartingPOS[1]});
    visited[StartingPOS[0]][StartingPOS[1]] = true;

    while (!q.empty()) {
        // Handle SFML window events to keep the application responsive
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        // Dequeue the front node from the queue
        pair<int, int> current = q.front();
        q.pop();

        // Render the current node as magenta to indicate active exploration
        RectangleShape rect(Vector2f(15, 15));
        rect.setPosition(current.second * 15, current.first * 15);
        rect.setFillColor(Color::Magenta);

        // Clear the window before each new frame to update the visual state
        window.clear();

        // Redraw the entire maze grid based on the current state of exploration
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < column; j++) {
                RectangleShape tile(Vector2f(15, 15));
                tile.setPosition(j * 15, i * 15);

                // Assign colors based on the state: wall, start, end, visited, or unvisited
                if (contentsArray[i][j] == '#') {
                    tile.setFillColor(Color::Black); // Wall
                } else if (i == StartingPOS[0] && j == StartingPOS[1]) {
                    tile.setFillColor(Color::Green); // Start position
                } else if (visited[i][j]) {
                    tile.setFillColor(Color::Magenta); // Visited nodes
                } else if (contentsArray[i][j] == 'E') {
                    tile.setFillColor(Color::Red); // End position
                } else {
                    tile.setFillColor(Color::White); // Unvisited nodes
                }
                window.draw(tile);
            }
        }

        // Display the updated maze frame
        window.display();
        sleep(milliseconds(50)); // Delay to slow down the animation for visibility

        // Explore all possible neighboring positions (up, left, down, right)
        for (int i = 0; i < 4; i++) {
            int nx = current.first + dirX[i];
            int ny = current.second + dirY[i];

            // Check if the neighbor is within bounds and not a wall or already visited
            if (nx >= 0 && nx < row && ny >= 0 && ny < column && contentsArray[nx][ny] != '#' && !visited[nx][ny]) {
                visited[nx][ny] = true;
                q.push({nx, ny});

                // If the neighbor is the end position, end the search
                if (contentsArray[nx][ny] == 'E') {
                    std::cout << "Maze completed successfully!" << std::endl;
                    return;  // Successfully reached the end
                }
            }
        }
    }

    // If the queue empties and no path to the end was found, print failure message
    std::cout << "Failed to complete the maze." << std::endl;
}
