#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <chrono>
#include "bst.h"

std::string BST::file_to_string(const std::string& filename){
  std::ifstream text(filename);

  std::stringstream strStream;
  if (text.is_open()) {
    strStream << text.rdbuf();
  }
  return strStream.str();
}

BST::BST() {

    std::string lines;
    std::ifstream file("/workspaces/cs225/CS225-Final-Project-4/src/large_twitch_features.csv");
    std::vector<int> gamers;
    if (file.is_open()) {
        while (std::getline(file, lines)) {
            std::stringstream ss(lines);
            std::string str;
            std::vector<std::string> row;
            while (std::getline(ss, str, ',')) {
                row.push_back(str);
                
            }
            if (std::stoi(row[0]) >= 0) {
                //std::cout << row[0] << std::endl;
                gamers.push_back(std::stoi(row[0]));
            }  
        }
    }
    
    file.close();
    sort(gamers.rbegin(), gamers.rend());
    gamers.erase(gamers.begin() + 1000, gamers.end());
    
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine e(seed);
    std::shuffle(gamers.begin(), gamers.end(), e);
    
    for (unsigned i = 0; i < 1000; i++) {
        insert(gamers[i]);
    }
}
struct BST::Node* BST::getRoot(){
    return root;
}
std::vector<int> BST::getAllNodes(int c){
    if (c == 0) {
        allNodes_one.clear();
        preOrder(root);
        return allNodes_one;
    } else if (c == 1) {
        allNodes_two.clear();
        inOrder(root);
        return allNodes_two;
    } else {
        allNodes_three.clear();
        postOrder(root);
    }
    return allNodes_three;
}

void BST::insert(const int & key) {
    Node* n = new Node(key);
    if (root == NULL) {
        root = n;
        return;
    }
    Node* back = NULL;
    Node* temp = root;
    while (temp != NULL) {
        if (temp->key > key) {
            back = temp;
            temp = temp->left;
        } else if (temp->key < key) {
            back = temp;
            temp = temp->right;
        }
    }
    if (back->key > key) {
        back->left = n;
    } else {
        back->right = n;
    }
}

void BST::preOrder(Node* n) {
    if (n == NULL) {
        return;
    }
    allNodes_one.push_back(n->key);
    preOrder(n->left);
    preOrder(n->right);
}

void BST::inOrder(Node* n) {
    if (n == NULL) {
        return;
    }
    inOrder(n->left);
    allNodes_two.push_back(n->key);
    inOrder(n->right);
}

void BST::postOrder(Node* n) {
    if (n == NULL) {
        return;
    }
    postOrder(n->left);
    postOrder(n->right);
    allNodes_three.push_back(n->key);
}
