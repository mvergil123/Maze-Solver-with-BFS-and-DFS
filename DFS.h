#ifndef DFS_H
#define DFS_H

#include <SFML/Graphics.hpp> // for SFML


void DFS(sf::RenderWindow& window, int(&StartingPOS)[2], int row, int column, char** contentsArray);

#endif