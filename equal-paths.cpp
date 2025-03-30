#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <iostream> 

#endif

#include "equal-paths.h"
using namespace std;

//leaf to root path must be the same to be true
// You may add any prototypes of helper functions here
int getHeight(Node* root); //finds height
bool check(int height, int currHeight, Node* root); //checks if roots to leaf paths are equal 


bool equalPaths(Node * root)
{
    // Add your code below
    //return true if equal paths, false else
    //no loops allowed, recursion 
    if (root == nullptr){
      return true;  //no tree, theyre auto the same
    }

    //leaf nodes are all true bc theres 1 path 
    //check for leaf node 
    if (root->left == nullptr && root->right == nullptr){
      return true;  //no leaf nodes so all the same
    }

    //get first height from root to leaf
    int height = getHeight(root); 
    //check to verify same height 
    if (check(height,1,root)){
      return true;
    }
    else {
      return false; 
    }

}

//make helper function to find height of first leaf
int getHeight(Node* root){

  if (root == nullptr){
    return 0; //nothikng no height, no node 
  }

  //if its a leaf node, height is 1 
  if (root->left == nullptr && root->right == nullptr){
    return 1; // has no children can return now
  }

  //try left subtree & right subtree 
  //right subtree 
  int rightH = getHeight(root->right); //recursively get height
  if (rightH != 0) { //it exists theres a node
    return rightH +1; //return that h up
  }

  //left 
  int leftH = getHeight(root->left);
  if (leftH != 0){
    return leftH + 1; 
  }
    //RETURN some number here so it doesn't warning in compiler
  return 0; //works with 1 or 0 (should realistically nevcer hit this)

}

bool check(int height, int currHeight, Node* root){
  if (root == nullptr){
    return true; //nothing there, so all equal 
  }

  if (root->left == nullptr && root->right == nullptr) { //lead node
    if (currHeight == height){ //check if current and expected are the same
      return true;
    }
    if (currHeight != height){ //wrong height
      return false; 
    }
  }
  if (check(height, currHeight+1, root->left) == false){ //recursively check left treee 
    return false;  //if it fails false and exit
  }
  if (check(height, currHeight+1, root->right) == false){ //check right 
    return false; 
  }
  return true; //if it hasn't failed and exited by now, its good 

}


