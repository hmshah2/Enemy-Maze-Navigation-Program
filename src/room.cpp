#include "room.h"
#include "disjointsets.h"
#include <queue>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <limits.h>
using namespace std;
using namespace cs225;

#include "../lib/cs225/HSLAPixel.h"
#include "../lib/cs225/PNG.h"
    //default constructor, creates empty room
    Room::Room(){
        width = 0;
        height = 0;
    }
    //helpers for tests
    int Room::getWidth(){
        return width;
    }
    int Room::getHeight(){
        return height;
    }
    std::vector<pair<int,int>> Room::getpathDirection(){
        return pathDirection;
    }
    std::vector<std::vector<std::vector<int>>> Room::getEdges() {
        return edges;
    }
    std::vector<std::vector<int>> Room::getSolved(){
        return solved;
    }
    Room* Room::roomHelper(int w,int h, bool ob){
        width = w;
        height = h;
        if(ob){
            v = std::vector<std::vector<char>>(width, std::vector<char>(height, 'o'));
            edges = std::vector<std::vector<std::vector<int>>>(width, std::vector<std::vector<int>>(height, std::vector<int>(4, -1)));
        }
        else{
            v = std::vector<std::vector<char>>(width, std::vector<char>(height, 'n'));
            edges = std::vector<std::vector<std::vector<int>>>(width, std::vector<std::vector<int>>(height, std::vector<int>(4, 0)));
        }
        return this;
    }

    std::vector<int> Room::getEdgesHelper(int x, int y){
        std::vector<int> edge =std::vector<int>(4,-1);
        if(x-1 >= 0){
          edge[0] = edges[x-1][y][0];  
        }
        if(y-1 >= 0){
            edge[1] = edges[x][y-1][1];
        }
        if(x+1 < width){
           edge[2] = edges[x+1][y][2]; 
        }
        if(y+1 < height){
            edge[3] = edges[x][y+1][3];
        } 
        return edge;
    }
    char Room::getObstacle(int x, int y){
        return v[x][y];
    }
    //input, width and height
    //creates a maze room of size 2*width-1 by 2*height-1
    //stored in v vector with edges in edges vector
    void Room::makeRoom(int new_width, int new_height){
        width = new_width * 2 - 1;
        height = new_height *2 - 1;
        solved = vector<vector<int>>(width,vector<int>(height,INT_MAX));
        edges = std::vector<std::vector<std::vector<int>>>(width, std::vector<std::vector<int>>(height, std::vector<int>(4, -1)));
        if(!v.empty()){ v.clear();}
        v = std::vector<std::vector<char>>(width, std::vector<char>(height, 'o'));
        for(int i = 0; i<width; i=i+2){
            for(int j =0; j<width;j=j+2){
                v[i][j] = 'n';
            }
            
        }
        DisjointSets set;
        set.addelements(new_width * new_height);
        int x;
        int y;
        int dir;
        while(set.size(0) != (new_width * new_height)){
            x = rand()%new_width;
            y = rand()%new_height;
            dir = rand()%4;
            if(dir == 0 && 2*(x+1)<(width + 1)/2 && v[2*x +1][2*y] == 'o' && set.find(y*new_width + x) != set.find(y*new_width + (x +1))){
                set.setunion(y*new_width + x, y*new_width + x +1);
                setObstacle(2*x+1, 2*y, false);
 
            }
            if(dir == 1 && 2*(y+1)<height && v[2*x][2*y+1] == 'o' && set.find(y*new_width + x) != set.find((y+1)*new_width + x)){
                set.setunion(y*new_width + x, (y+1)*new_width + x);
                setObstacle(2*x, 2*y+1, false);
            }
            if(dir == 2 && x>0 && v[2*x-1][2*y] == 'o' && set.find(y*new_width + x) != set.find(y*new_width + x -1)){
                set.setunion(y*new_width + x, y*new_width + x -1);
                setObstacle(2*x-1, 2*y, false);
 
            }
            if(dir == 3 && y>0 && v[2*x][2*y-1] == 'o' && set.find(y*new_width + x) != set.find((y-1)*new_width + x)){
                set.setunion(y*new_width + x, (y-1)*new_width + x);
                setObstacle(2*x, 2*y-1, false);
 
            }
        }
    }
    //input: x coordinate, y coordinate, direction
    //output: bool value, true if you can travel in the given direction from (x,y)
    bool Room::canTravel(int x, int y, int dir) const{
        if(x< 0 || y< 0 || x >= width || y >= height || v[x][y] == 'o' ){
            return false;
        }
        switch(dir) {
            case 0: 
                if(x < width - 1 && v[x+1][y] != 'o'){
                    return true;
                }
                break;
            case 1: 
                if(y < height - 1 && v[x][y+1] != 'o'){
                    return true;
                }
                break;
            case 2: 
                if(x > 0 && v[x-1][y] != 'o'){
                    return true;
                }
                break;
            case 3: 
                if(y > 0 && v[x][y-1] != 'o'){
                    return true;
                }
                break;
        }
        return false;
    }
    //sets obstacle with edges -1 or removes obstacle
    //inputs: x coordinate, y coordinate, bool value to add or remove
    void Room::setObstacle(int x, int y, bool exists){
        if(x >= 0 && x<width && y >= 0 && y<height){
            if(exists){
                v[x][y] = 'o';
                if(x-1 >= 0){
                    edges[x-1][y][0] = -1;
                }
                if(y-1 >=0){
                    edges[x][y-1][1] = -1;  
                }
                if(x+1 < width){
                    edges[x+1][y][2] = -1;  
                }
                if(y+1 < height){
                    edges[x][y+1][3] = -1;
                }
                edges[x][y][0] = -1;
                edges[x][y][1] = -1;
                edges[x][y][2] = -1;
                edges[x][y][3] = -1;
            }
            else{
                v[x][y] = 'n';
                if(x-1 >= 0){
                    edges[x-1][y][0] = 0;
                }
                if(y-1 >=0){
                    edges[x][y-1][1] = 0;  
                }
                if(x+1 < width){
                    edges[x+1][y][2] = 0;  
                }
                if(y+1 < height){
                    edges[x][y+1][3] = 0;
                } 
                edges[x][y][0] = 0;
                edges[x][y][1] = 0;
                edges[x][y][2] = 0;
                edges[x][y][3] = 0;
            }
        }
    }

    /**
    * Iterates through the width and height of the room and determines the next position to be visited
    * based on the lowest walking distance required to reach that node
    *
    * @param dist 2D vector tracking the minimum distance to travel to each node in the room
    * @param visited marks if a node has been visited or not
    * @returns A pair<int,int> representing the x,y coordinates of the next visited node with the least distance 
    */

    pair<int,int> Room::minDistance(vector<vector<int>> dist, vector<vector<bool>> visited) const {
        int min = INT_MAX;
        pair<int,int> min_index;
        for(int x = 0; x < width; x++) {
            for(int y = 0; y < height; y++) {
                if(v[x][y] != 'o' && !visited[x][y] && dist[x][y] <= min) {
                    min = dist[x][y];
                    min_index = {x,y};
                }
            }
        }
        return min_index;
    }

    /**
    * Implements Dijkstra’s Algorithm to calculate the minimum distance between nodes using edge weights
    * If an obstacle is present in a certain position, the distance to travel to that node will be considered invalid (INT_MAX)
    * for each visited node in the room, if a shorter path to that node is found the walking distance assigned will be updated.
    * 
    * @returns a 2D vector of the nodes in a room and the direction from which they are visited (0, 1, 2, or 3)
    */

    vector<vector<int>> Room::solveRoom(){
        vector<vector<int>> dist(width,vector<int>(height,INT_MAX));
        vector<vector<bool>> visited(width,vector<bool>(height,false));
        vector<vector<int>> parents(width,vector<int>(height,-1));
        dist[0][0] = 0;
        int count = 0;
        for (int i = 0; i < width; i++) {
            for(int j = 0; j < height; j++){
                if(v[i][j] == 'o') {
                    count++;
                }
            }
        }
        parents[0][0] = -1;
        for(int c = 0; c < width*height-1-count; c++) {
            pair<int,int> p = minDistance(dist,visited);
            int x = p.first;
            int y = p.second;
            visited[x][y] = true;
            vector<pair<int,int>> v1;
            if (canTravel(x, y, 0) &&  !visited[x+1][y] && dist[x][y] != INT_MAX && dist[x][y] + edges[x][y][0] < dist[x+1][y]) { // right
                dist[x+1][y] = dist[x][y] + edges[x][y][0];
                parents[x+1][y]=0;
            }
            if (canTravel(x, y, 1) &&  !visited[x][y+1] && dist[x][y] != INT_MAX && dist[x][y] + edges[x][y][1] < dist[x][y+1]) { // down
                dist[x][y+1] = dist[x][y] + edges[x][y][1];
                parents[x][y+1]=1;
            }
            if (canTravel(x, y, 2) &&  !visited[x-1][y] && dist[x][y] != INT_MAX && dist[x][y] + edges[x][y][2] < dist[x-1][y]) { // left
                dist[x-1][y] = dist[x][y] + edges[x][y][2];
                parents[x-1][y]=2;
            }
            if (canTravel(x, y, 3) &&  !visited[x][y-1] && dist[x][y] != INT_MAX && dist[x][y] + edges[x][y][3] < dist[x][y-1]) { // up
                dist[x][y-1] = dist[x][y] + edges[x][y][3];
                parents[x][y-1]=3;
            }
        }
        solved = dist;
        return parents;
}

//takes in a bst of enemies
//randomly adds enemies throughout the room in spots where there isn't an enemy and aren't the entrance
void Room::addEnemies(BST* enemies) {
    int num_Enemies = (width*height)/10;
    for (int i = 0; i < num_Enemies;) {
        int x = rand()%width;
        int y = rand()%height;
        if (v[x][y] != 'e' && x!=0 && y!=0 && x < width && y < height) {
            int num = rand()%(enemies->getAllNodes(0).size());
            std::vector<int> nodes = enemies->getAllNodes(num % 3);
            int diff = nodes[num]/1000000;
            std::vector<int> temp{x, y};
            enemy_difficulties.insert({temp, diff});
            setEnemy(x, y, true, diff);
            i++;
        }
    }
}

//gets the number of enemies that have been added into the room
unsigned Room::getNumEnemies() {
    return enemy_difficulties.size();
}

//sets enemy with edges of difficulty or removes enemy
//inputs: x coordinate, y coordinate, bool value to add or remove, enemy difficulty
void Room::setEnemy(int x, int y, bool exists, int difficulty){
        if(x >= 0 && x<width && y >= 0 && y<height){
            if(exists){
                v[x][y] = 'e';
                if(x-1 >= 0){
                    edges[x-1][y][0] += difficulty;
                }
                if(y-1 >=0){
                    edges[x][y-1][1] += difficulty;  
                }
                if(x+1 < width){
                    edges[x+1][y][2] += difficulty;  
                }
                if(y+1 < height){
                    edges[x][y+1][3] += difficulty;
                }
            }
            else{
                v[x][y] = 'n';
                if(x-1 >= 0){
                    edges[x-1][y][0] = 0;
                }
                if(y-1 >=0){
                    edges[x][y-1][1] = 0;  
                }
                if(x+1 < width){
                    edges[x+1][y][2] = 0;  
                }
                if(y+1 < height){
                    edges[x][y+1][3] = 0;
                } 
            }
        }
    }
//sets walking weight for edges by adding walk if a node is not an obstacle
//input: walking weight
void Room::setWalkingDistance(int walk){
    if(walk <=0){
        return;
    }
    for(int x=0; x < (int)edges.size(); x++){
       for(int y=0; y < (int)edges[0].size(); y++){
            for(int d=0; d<4; d++){
                if(edges[x][y][d] != -1 && v[x][y] != 'e'){
                    edges[x][y][d] += walk;
                }
            }
        } 
    }
}
//creates a png file based on the room data with enemies, enemy edge weights, and obstacles drawn
//output:cs225::PNG that can be drawn to a file
    cs225::PNG* Room::drawRoom() const{
        cs225::PNG* pic = new cs225::PNG(width * 10 + 1, height * 10 + 1);
        for(unsigned i = 0; i < pic->width(); i++) {
            pic->getPixel(i, 0) = cs225::HSLAPixel(0, 0, 0);
            pic->getPixel(i, pic->height()-1) = cs225::HSLAPixel(0, 0, 0);
        }
        for(unsigned i = 0; i < pic->height(); i++) {
            pic->getPixel(0, i) = cs225::HSLAPixel(0, 0, 0);
            pic->getPixel(pic->width()-1, i) = cs225::HSLAPixel(0, 0, 0);
        }
        for(int x = 0; x < width; x++) {
            for(int y = 0; y < height; y++) {
                if(v[x][y]=='o') {
                    for(int k=0; k < 6; k++){
                       pic->getPixel(x * 10, y * 10 + k + 2) = cs225::HSLAPixel(0, 0, 0); 
                       pic->getPixel(x * 10 + 9, y * 10 + k + 2) = cs225::HSLAPixel(0, 0, 0);
                       pic->getPixel(x * 10 + k + 2, y * 10) = cs225::HSLAPixel(0, 0, 0);
                       pic->getPixel(x * 10 + k + 2, y * 10 + 9) = cs225::HSLAPixel(0, 0, 0);
                    }
                    pic->getPixel(x * 10 + 1, y * 10 + 1) = cs225::HSLAPixel(0, 0, 0); 
                    pic->getPixel(x * 10 + 8, y * 10 + 1) = cs225::HSLAPixel(0, 0, 0);
                    pic->getPixel(x * 10 + 8, y * 10 + 8) = cs225::HSLAPixel(0, 0, 0);
                    pic->getPixel(x * 10 + 1, y * 10 + 8) = cs225::HSLAPixel(0, 0, 0);
                }
                else if(v[x][y] == 'e'){
                    int difficulty = 0;
                    for (auto & instance : enemy_difficulties) {
                        std::vector<int> temp{x, y};
                        if (instance.first == temp) {
                         difficulty = instance.second;
                        }
                    }
                    double d = getColor(difficulty);
                    pic->getPixel(x * 10 + 3, y * 10 + 2) = cs225::HSLAPixel(d, 1, 0.5); 
                    pic->getPixel(x * 10 + 4, y * 10 + 2) = cs225::HSLAPixel(d, 1, 0.5);
                    pic->getPixel(x * 10 + 5, y * 10 + 2) = cs225::HSLAPixel(d, 1, 0.5);
                    pic->getPixel(x * 10 + 6, y * 10 + 2) = cs225::HSLAPixel(d, 1, 0.5);
                    pic->getPixel(x * 10 + 2, y * 10 + 3) = cs225::HSLAPixel(d, 1, 0.5); 
                    pic->getPixel(x * 10 + 4, y * 10 + 3) = cs225::HSLAPixel(d, 1, 0.5);
                    pic->getPixel(x * 10 + 5, y * 10 + 3) = cs225::HSLAPixel(d, 1, 0.5);
                    pic->getPixel(x * 10 + 7, y * 10 + 3) = cs225::HSLAPixel(d, 1, 0.5);
                    pic->getPixel(x * 10 + 2, y * 10 + 4) = cs225::HSLAPixel(d, 1, 0.5); 
                    pic->getPixel(x * 10 + 4, y * 10 + 4) = cs225::HSLAPixel(d, 1, 0.5);
                    pic->getPixel(x * 10 + 5, y * 10 + 4) = cs225::HSLAPixel(d, 1, 0.5);
                    pic->getPixel(x * 10 + 7, y * 10 + 4) = cs225::HSLAPixel(d, 1, 0.5);
                    pic->getPixel(x * 10 + 2, y * 10 + 5) = cs225::HSLAPixel(d, 1, 0.5);
                    pic->getPixel(x * 10 + 3, y * 10 + 5) = cs225::HSLAPixel(d, 1, 0.5);
                    pic->getPixel(x * 10 + 4, y * 10 + 5) = cs225::HSLAPixel(d, 1, 0.5); 
                    pic->getPixel(x * 10 + 5, y * 10 + 5) = cs225::HSLAPixel(d, 1, 0.5);
                    pic->getPixel(x * 10 + 6, y * 10 + 5) = cs225::HSLAPixel(d, 1, 0.5);
                    pic->getPixel(x * 10 + 7, y * 10 + 5) = cs225::HSLAPixel(d, 1, 0.5);
                    pic->getPixel(x * 10 + 2, y * 10 + 6) = cs225::HSLAPixel(d, 1, 0.5);
                    pic->getPixel(x * 10 + 3, y * 10 + 6) = cs225::HSLAPixel(d, 1, 0.5);
                    pic->getPixel(x * 10 + 4, y * 10 + 6) = cs225::HSLAPixel(d, 1, 0.5); 
                    pic->getPixel(x * 10 + 5, y * 10 + 6) = cs225::HSLAPixel(d, 1, 0.5);
                    pic->getPixel(x * 10 + 6, y * 10 + 6) = cs225::HSLAPixel(d, 1, 0.5);
                    pic->getPixel(x * 10 + 7, y * 10 + 6) = cs225::HSLAPixel(d, 1, 0.5);
                    pic->getPixel(x * 10 + 2, y * 10 + 7) = cs225::HSLAPixel(d, 1, 0.5); 
                    pic->getPixel(x * 10 + 4, y * 10 + 7) = cs225::HSLAPixel(d, 1, 0.5);
                    pic->getPixel(x * 10 + 5, y * 10 + 7) = cs225::HSLAPixel(d, 1, 0.5);
                    pic->getPixel(x * 10 + 7, y * 10 + 7) = cs225::HSLAPixel(d, 1, 0.5);
                    for(int k = 0; k < 10; k++){
                        pic->getPixel(x * 10, y * 10 + k) = cs225::HSLAPixel(d, 1, 0.5);
                        pic->getPixel(x * 10 + 9, y * 10 + k) = cs225::HSLAPixel(d, 1, 0.5);
                        pic->getPixel(x * 10 + k, y * 10) = cs225::HSLAPixel(d, 1, 0.5);
                        pic->getPixel(x * 10 + k, y * 10 + 9) = cs225::HSLAPixel(d, 1, 0.5);
                    }
                }
                
            }
        }
        return pic;
    }
//helper for difficulty colors
    double Room::getColor(int difficulty) const{
        if(difficulty <=10){
            return 125;
        }
        if(difficulty <=20){
            return 175;
        }
        if(difficulty <=30){
            return 200;
        }
        if(difficulty <=40){
            return 55;
        }
        if(difficulty <=50){
            return 35;
        }
        if(difficulty <=60){
            return 20;
        }
        if(difficulty <=70){
            return 330;
        }
        if(difficulty <=80){
            return 295;
        }
        if(difficulty <=90){
            return 275;
        }
        return 0;
    }

    /**
    * Travels backward through the solution path accessing previously visited nodes.
    * Based on the direction traveled to the previous node (up, down, left, or right)
    * a horizontal or vertical line of pixels will be drawn to the PNG file
    * The solution path is colored the provided difficulty level
    * The path ends when there are other nodes left to backtrack to, this position is the room's starting point (0,0)
    *
    * @param difficulty Enemy difficulty level 
    * @returns A PNG of the room with the solution path through the graph.
    */

    PNG* Room::drawRoomSolution(int difficulty){
        double d = getColor(difficulty);
        PNG* pic = drawRoom();
        vector<vector<int>> solution = solveRoom();
        int prev = -1;
        int next = 0;
        pair<int,int> current = {0,0};
        for(int i = height-1 ; i>=0; i--){
            if(solution[width -1][i] != -1){
                current = {width-1, i};
                break;
            }
        }
        if(current == pair<int,int>{0,0}){
            cout<< "no valid solution"<<endl;
            return pic;
        }
        prev = solution[current.first][current.second];
        while(prev != -1){
            for(int i = 0; i <= 5; i++) {
                switch(next) {
                    case(0):
                        pic->getPixel(current.first*10+4 + i, current.second*10+4) = cs225::HSLAPixel(d, 1, 0.5);
                        pic->getPixel(current.first*10+4 + i, current.second*10+5) = cs225::HSLAPixel(d, 1, 0.5);
                        break;
                    case(1):
                        pic->getPixel(current.first*10+4, current.second*10+4 + i) = cs225::HSLAPixel(d, 1, 0.5);
                        pic->getPixel(current.first*10+5, current.second*10+4 + i) = cs225::HSLAPixel(d, 1, 0.5);
                        break;
                    case(2):
                        pic->getPixel(current.first*10+5 - i, current.second*10+4) = cs225::HSLAPixel(d, 1, 0.5);
                        pic->getPixel(current.first*10+5 - i, current.second*10+5) = cs225::HSLAPixel(d, 1, 0.5);
                        break;
                    case(3):
                        pic->getPixel(current.first*10+4, current.second*10+5 - i) = cs225::HSLAPixel(d, 1, 0.5);
                        pic->getPixel(current.first*10+5, current.second*10+5 - i) = cs225::HSLAPixel(d, 1, 0.5);
                        break;
                    default:
                        break;
                }
                switch(prev) {
                    case(2):
                        pic->getPixel(current.first*10+4 + i, current.second*10+4) = cs225::HSLAPixel(d, 1, 0.5);
                        pic->getPixel(current.first*10+4 + i, current.second*10+5) = cs225::HSLAPixel(d, 1, 0.5);
                        break;
                    case(3):
                        pic->getPixel(current.first*10+4, current.second*10+4 + i) = cs225::HSLAPixel(d, 1, 0.5);
                        pic->getPixel(current.first*10+5, current.second*10+4 + i) = cs225::HSLAPixel(d, 1, 0.5);
                        break;
                    case(0):
                        pic->getPixel(current.first*10+5 - i, current.second*10+4) = cs225::HSLAPixel(d, 1, 0.5);
                        pic->getPixel(current.first*10+5 - i, current.second*10+5) = cs225::HSLAPixel(d, 1, 0.5);
                        break;
                    case(1):
                        pic->getPixel(current.first*10+4, current.second*10+5 - i) = cs225::HSLAPixel(d, 1, 0.5);
                        pic->getPixel(current.first*10+5, current.second*10+5 - i) = cs225::HSLAPixel(d, 1, 0.5);
                        break;
                    default:
                        break;
                }
            }
            pathDirection.push_back(current);
            next = prev;
            switch(prev) {
                case(0):
                    current.first -= 1;
                    break;
                case(1):
                    current.second -= 1;
                    break;
                case(2):
                    current.first += 1;
                    break;
                case(3):
                    current.second += 1;
                    break;
                default:
                    break;
            }
            prev = solution[current.first][current.second];
        }
        if(next == 0) {
            for(int i = 0; i < 10; i++) {
                pic->getPixel(current.first*10 + i, current.second*10 + 4) = cs225::HSLAPixel(d, 1, 0.5);
                pic->getPixel(current.first*10 + i, current.second*10 + 5) = cs225::HSLAPixel(d, 1, 0.5);
            }
        }
        else {
            for(int i = 0; i < 10; i++) {
                pic->getPixel(current.first*10 + 4, current.second*10 + i) = cs225::HSLAPixel(d, 1, 0.5);
                pic->getPixel(current.first*10 + 5, current.second*10 + i) = cs225::HSLAPixel(d, 1, 0.5);
            }
        }
        return pic;
    }
