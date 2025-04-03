#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void insertFix(AVLNode<Key,Value>* p, AVLNode<Key, Value>* n); //logic slide 33
    void rotateRight(AVLNode<Key, Value>* g);
    void rotateLeft(AVLNode<Key,Value>* g);
    void removeFix(AVLNode<Key,Value>* n, int diff); //logic 42 



};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value> &new_item)
{
    // TODO
    //check if its empty first so you can just insert 
    if (this->root_ == nullptr){
      //nothing in the tree 
      this->root_ = new AVLNode<Key,Value>(new_item.first, new_item.second, nullptr);
      return; 
    }

    //walk tree to find insertion point 
    //keep track of current and parent 
    AVLNode<Key, Value>* current = static_cast<AVLNode<Key,Value>*>(this->root_); //start from root
    AVLNode<Key,Value>* parent = nullptr; //for now, nullptr, but will attatch new node here 

    while(current!= nullptr){
      //now yoy have to start traversing the entire tree 
      //so start moving 
      parent = current; // follow parent 
      if (new_item.first < current->getKey()) { //its smaller so u start going left to find ur spot to insert
        current = current->getLeft(); 
      }
      else if (new_item.first > current->getKey()){ //its larger so you have to go right
        current = current->getRight(); 
      }
      else{
        //when we dont insert new node and we have the key 
        current->setValue(new_item.second); //update value associated with key
        return; 
      }
    }
    //create new node if you need it now 
    AVLNode<Key, Value>* nodeToInsert = new AVLNode<Key, Value>(new_item.first, new_item.second, parent); //sets ptr 
    //now you can attatch 
    if (new_item.first < parent->getKey()){
      parent->setLeft(nodeToInsert); //attatch left 
    }
    else{
      parent->setRight(nodeToInsert); //attatch right 
    }
    //check and update all  balances so we did it right and its still an AVLtree 
    if (parent->getBalance() == -1 || parent->getBalance() == 1){
      parent->setBalance(0); //imablanced so balance it now
      return; 
    }
    else if (parent->getBalance() == 0){
      //update balance 
      if (nodeToInsert == parent->getLeft()){
        parent->setBalance(-1); //legt heavy 
      }
      else {
        parent->setBalance(1); //right heavy 
      }
      insertFix(parent,nodeToInsert); //call helper recursive and balance to properly fix 
    }


}

//helper
template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* g){
  //get children 
  AVLNode<Key, Value>* leftChild = g->getLeft(); //left child 
  AVLNode<Key, Value>* leftChildRight = leftChild -> getRight(); 

//rotate
  leftChild->setRight(g); //right child of left child
  g->setLeft(leftChildRight); 

//update pointers 
  if (leftChildRight != nullptr){
    leftChildRight->setParent(g); 
  }
  //get original parent of node rotating 
  AVLNode<Key,Value>* ogParent = g->getParent(); 
  leftChild->setParent(ogParent); //rotated leftchild already so new root of subtree 
  g->setParent(leftChild); //g has become a child of left child 

  //now rotated subtree has to be reattatched 
  if (ogParent == nullptr){
    //g was the root rto begin with so we can change rooto 
    this->root_ = leftChild; 
  }
  else{
    //now check left or right 
    if (ogParent->getLeft() == g){ //was left child? 
      ogParent->setLeft(leftChild); //left child as new left child of og parent 
    }
    else{
      ogParent->setRight(leftChild); //set left child as new right child of og parent
    }
  }

}

//helper
template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* g){
  //get children 
  AVLNode<Key, Value>* rightChild = g->getRight(); //right child 
  if (rightChild == nullptr){
    return; 
  }
  AVLNode<Key, Value>* rightChildLeft = rightChild->getLeft(); 

//rotate
  rightChild->setLeft(g); //left child of right child 
  g->setRight(rightChildLeft); 

//update pointers 
  if (rightChildLeft != nullptr){
    rightChildLeft->setParent(g); 
  }
  //get original parent of node rotating 
  AVLNode<Key,Value>* ogParent = g->getParent(); 
  rightChild->setParent(ogParent); //rotated leftchild already so new root of subtree 
  g->setParent(rightChild); //g has become a child of left child 

  //now rotated subtree has to be reattatched 
  if (ogParent == nullptr){
    //g was the root rto begin with so we can change rooto 
    this->root_ = rightChild; 
  }
  else{
    //now check left or right 
    if (ogParent->getLeft() == g){ //check which child of parent
      ogParent->setLeft(rightChild); //left child as new left child of og parent 
    }
    else{
      ogParent->setRight(rightChild); //set left child as new right child of og parent
    }
  }

}

//helper for insert 
template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key,Value>* p, AVLNode<Key, Value>* n){
  //first if null return 
  if (p == nullptr){
    return; 
  }
  //let g = parent(p)
  AVLNode<Key,Value>* g = p->getParent(); 
  if (g == nullptr){
    return; 
  }

  //assume p is left child of g
  if (p == g->getLeft()){
    //Update g's balance to new accurate value for now
    g->updateBalance(-1); // is this gonna mess up the others ??? +=?? 

    if (g->getBalance() == 0){
      return; //balanced and done 
    }
    else if (g->getBalance() == -1){
      insertFix(g, p); //says to recurse 
    }
    else if (g->getBalance() == -2){
      //case 3 
      //If zig-zig then rotateRight(g); b(p) = b(g) = 0
      if (p->getLeft() == n){
        rotateRight(g); 
        p->setBalance(0); 
        g->setBalance(0); 
      }
      else {
        //• If zig-zag then rotateLeft(p); rotateRight(g); 
        rotateLeft(p); 
        rotateRight(g); 
        //– Case 3a: b(n) == -1 then b(p) = 0; b(g) = +1; b(n) = 0;
        if (n->getBalance() == -1){
          p->setBalance(0);
          g->setBalance(1); 
        }
        else if (n->getBalance() == 0){
          //– Case 3b: b(n) == 0 then b(p) = 0; b(g) = 0; b(n) = 0;
          p->setBalance(0);
          g->setBalance(0); 
        }
        else if (n->getBalance() == 1){
          p->setBalance(-1); 
          g->setBalance(0); 
        }
        n->setBalance(0); 

      }
    }
  }
  else if (g->getRight() == p){ //assume p is a right child 
    g->updateBalance(1); 

    if (g->getBalance() == 0){
      return; //BALANCED !!
    }
    else if (g->getBalance() == 1){
      insertFix(g,p); //recurse 
    }
    else if (g->getBalance() == 2){
      //do zig - zig and zig-zag rotations
      //zig zig (right right)
      //should icheck null here
      if (p->getRight() == n){
        rotateLeft(g);
        p->setBalance(0);
        g->setBalance(0);
      }
      else { //right left 
        rotateRight(p); //right around p
        rotateLeft(g); //left around g

        if (n->getBalance() == 1){
          //right heavy so balance 
          p->setBalance(0);
          g->setBalance(-1); 
        }
        else if (n->getBalance() == 0){
          p->setBalance(0); 
          g->setBalance(0); 
        }
        else if (n->getBalance() == -1){
          //left heavy so balance 
          p->setBalance(1); 
          g->setBalance(0); 
        }
        n->setBalance(0); 
      }

    }
  }

}
template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key,Value>* n, int diff){
  //if n is null return 
  if (n == nullptr){
    return; 
  }
  //Let p = parent(n) and if p is not NULL let ndiff (nextdiff) = +1 if n is a left child and -1 otherwise
  AVLNode<Key,Value>* p = n->getParent(); 
  int ndiff = 0; 
  if (p != nullptr){
    if (n == p->getLeft()){
      ndiff = 1; 
    }
    else {
      ndiff = -1; 
    }
  }
  //Case 1: b(n) + diff == -2
  //[Perform the check for the mirror case where b(n) + diff == +2, flipping left/right and -1/+1]
  //Let c = left(n), the taller of the children
  if (diff == -1){
  if (n->getBalance() + diff == -2){
    AVLNode<Key, Value>* tallerChild = n->getLeft(); 
    if (tallerChild == nullptr){
      n->setBalance(n->getBalance() +diff); 
      return; 
    }
    int childBalance = tallerChild->getBalance(); 

    if (childBalance == -1){
      //Case 1a: b(c) == -1 // zig-zig case
      //rotateRight(n), b(n) = b(c) = 0, removeFix(p, ndiff)
      rotateRight(n); 
      n->setBalance(0); 
      tallerChild->setBalance(0); 
      removeFix(p, ndiff); 
    }
    else if (childBalance == 0){
      //– Case 1b: b(c) == 0 // zig-zig case
     //rotateRight(n), b(n) = -1, b(c) = +1 // Done!
     rotateRight(n);
     n->setBalance(-1); 
     tallerChild->setBalance(1); 
     return; //done
    }
    else if (childBalance == 1){
      //Case 1c: b(c) == +1 // zig-zag case
//Let g = right(c)
// rotateLeft(c) then rotateRight(n)
//If b(g) was +1 then b(n) = 0, b(c) = -1, b(g) = 0
//If b(g) was 0 then b(n) = 0, b(c) = 0, b(g) = 0
// If b(g) was -1 then b(n) = +1, b(c) = 0, b(g) = 0
//removeFix(p, ndiff);
      AVLNode<Key,Value>* g = tallerChild->getRight();  
      // if (g == nullptr){ //sanity checks????seg fault help
      //   n->setBalance(n->getBalance() +diff); 
      //   return; 
      // } 
      rotateLeft(tallerChild); 
      rotateRight(n); 

      if (g->getBalance() == 1){
        n->setBalance(0); 
        tallerChild->setBalance(-1); 
        g->setBalance(0); 
      }
      else if (g->getBalance() == 0){
        n->setBalance(0); 
        tallerChild->setBalance(0);
        g->setBalance(0);  
      }
      else if (g->getBalance() == -1){
        n->setBalance(1); 
        tallerChild->setBalance(0); 
        g->setBalance(0); 
      }
      removeFix(p, ndiff); 
    }
  }
  else if (n->getBalance() + diff == -1){
    n->setBalance(-1); 
    return; // done
  }
  else if(n->getBalance() + diff == 0){
    n->setBalance(0); 
    removeFix(p, ndiff); 
  }
  }
  else if (diff == 1){
  //have to do case one the opposite way +2 instead of -2
  if (n->getBalance() + diff == 2){
    //right heavy 
    AVLNode<Key, Value>* tallerChild = n->getRight(); 
    // if (tallerChild == nullptr){
    //   //add sanity check again 
    //   n->setBalance(n->getBalance() + diff);
    //   return; //seg ault preveention
    // }
    int childBalance = tallerChild->getBalance(); 

    if (childBalance == 1){
      // zig-zig case
      //rotateRight(n), b(n) = b(c) = 0, removeFix(p, ndiff)
      rotateLeft(n); 
      n->setBalance(0); 
      tallerChild->setBalance(0); 
      removeFix(p, ndiff); 
    }
    else if (childBalance == 0){
      //– Case 1b: b(c) == 0 // zig-zig case
     //rotateRight(n), b(n) = -1, b(c) = +1 // Done!
     rotateLeft(n);
     n->setBalance(1); 
     tallerChild->setBalance(-1); 
     return; //done
    }
    else if (childBalance == -1){
      //Case 1c: b(c) == +1 // zig-zag case
//Let g = right(c)
// rotateLeft(c) then rotateRight(n)
//If b(g) was +1 then b(n) = 0, b(c) = -1, b(g) = 0
//If b(g) was 0 then b(n) = 0, b(c) = 0, b(g) = 0
// If b(g) was -1 then b(n) = +1, b(c) = 0, b(g) = 0
//removeFix(p, ndiff);
      AVLNode<Key,Value>* g = tallerChild->getLeft(); 
      // if (g == nullptr){
      //   n->setBalance(n->getBalance() + diff); 
      //   return; //seg fault sanity check
      // }
      rotateRight(tallerChild); 
      rotateLeft(n); 

      if (g->getBalance() == -1){
        n->setBalance(0); 
        tallerChild->setBalance(1); 
        g->setBalance(0); 
      }
      else if (g->getBalance() == 0){
        n->setBalance(0); 
        tallerChild->setBalance(0);  
      }
      else if (g->getBalance() == 1){
        n->setBalance(-1); 
        tallerChild->setBalance(0); 
        g->setBalance(0); 
      }
      removeFix(p, ndiff); 
      //return; //FIXED 1
    }
  }
  else if (n->getBalance() + diff == 1){
    n->setBalance(1); 
    return; // done
  }
  else if(n->getBalance() + diff == 0){
    n->setBalance(0);
    removeFix(p,ndiff); 
  }
  } //END OF CASE 1 

}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    AVLNode<Key,Value>* removeNode = static_cast<AVLNode<Key,Value>*>(this->internalFind(key)); 
    if (removeNode == nullptr){
      return; //there's nothing to remove, theres nothing there 
    }

    //check if node has 2 children, bc then u hhave to swap w pred to work 
    if (removeNode->getLeft() != nullptr && removeNode->getRight() != nullptr){
      //2 children exist
        AVLNode<Key,Value>* predecessor = static_cast<AVLNode<Key,Value>*>(this->predecessor(removeNode)); 
        this->nodeSwap(removeNode, predecessor); 
    }
    //now it should only have one child bc its been swapped 
    //get parent so we know where to reconnect once remove actually happens 
    AVLNode<Key,Value>* reconnect = removeNode->getParent(); 
    //find which child should now replace the rempoved node
    AVLNode<Key,Value>* toReplace = nullptr; 
    if (removeNode->getLeft() != nullptr){
      //left child 
      toReplace = removeNode->getLeft();//promotes left child 
    }
    else if((removeNode->getRight()) != nullptr){
      //right
      toReplace = removeNode->getRight(); 
    }
    //start checking for balance 
    int balDiff = 0; 

    if (reconnect == nullptr){
      //its saying the node that were deleting is the root  
      this->root_ = toReplace; //make child new root
      if (toReplace != nullptr){
        toReplace->setParent(nullptr); //parent has to be nullptr bc it doesnt have a aprent its the new top of the tree
      }
    }
    else {
      //its not the root it was a child
       if (reconnect->getLeft() == removeNode){
        reconnect->setLeft(toReplace);  //replace left child 
        balDiff = 1; //removed left child so balance is skewed, leans right
       }
       else{
        reconnect->setRight(toReplace); 
        balDiff = -1; //right was removed so oopposite way, leans left 
       }
       if (toReplace != nullptr){
        toReplace->setParent(reconnect); //reconnects it by updating parent 
       }
    }
    delete removeNode; // finally removes it
    removeFix(reconnect,balDiff); //rebalance tree starting where u reconnect 

}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
