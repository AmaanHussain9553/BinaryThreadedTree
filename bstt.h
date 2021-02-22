/*bstt.h*/

//
// Threaded binary search tree
//

#pragma once

#include <iostream>

using namespace std;

template<typename KeyT, typename ValueT>
class bstt
{
private:
  struct NODE
  {
    KeyT   Key;
    ValueT Value;
    NODE*  Left;
    NODE*  Right;
    bool   isThreaded;
  };

  NODE* Root;  // pointer to root node of tree (nullptr if empty)
  int   Size = 0;  // # of nodes in the tree (0 if empty)
  NODE* currentNode = nullptr;//Node to store current node during inorder iteration
                              //for begin and next function


public:
  //
  // default constructor:
  //
  // Creates an empty tree.
  //
  bstt()
  {
    Root = nullptr;
    Size = 0;
  }
  
  //
  // clearHelper
  //
  //THis function is used to take in a node as a parameter and clear
  // it and and is a helper function for the actual clear function
  // This function is also used in the destructor
  void clearHelper(NODE* cur)
  {
      if(cur == nullptr) 
          return;
      else{
          clearHelper(cur->Left);//recursion call for left subtree
          if(cur->isThreaded == false){
              clearHelper(cur->Right);//if the right is not threaded
                                      //then recursion call for right subtree
          }
          delete cur;
      }  
  }
  
  //
  // clear:
  //
  // Clears the contents of the tree, resetting the tree to empty.
  //
  void clear()
  { 
        clearHelper(Root);
        Root = nullptr;
        Size = 0;
  }
 
  //
  // insertPreorder
  //      
  //This function is a helper function that takes in a node as a parameter
  //and copies the content of the other tree whose root is sent in a as a 
  //parameter and copies the contents into the concurrent Root node that 
  //is the main tree
  //
  void insertPreorder(NODE* other) {
    if (other == NULL) 
        return; 
  
    /* insert the first value which is the node */
    insert(other->Key,other->Value);
    
  
    /* then recur on left subtree */
    insertPreorder(other->Left);  
  
    /* now recur on right subtree */
    if(!(other->isThreaded)){
        insertPreorder(other->Right); 
    }
}

  //
  // copy constructor
  //
  bstt(const bstt& other)
  {   
      Root = nullptr;
      Size = 0;
      NODE* temp = other.Root;//sets the node to temp so that it can be
                              //passed into the helper function to insertPreorder
      insertPreorder(temp);
      
  }


  //
  // destructor:
  //
  // Called automatically by system when tree is about to be destroyed;
  // this is our last chance to free any resources / memory used by
  // this tree.
  //
  virtual ~bstt()
  {
     clearHelper(Root);
     //destroys everything in the tree at the absolute end of 
     //execution
  }
  
 
  //
  // operator=
  //
  // Clears "this" tree and then makes a copy of the "other" tree.
  //
  bstt& operator=(const bstt& other)
  {
      //First clears this tree
      clearHelper(Root);
      Root = NULL;
      Size = 0;
      NODE* temp = other.Root;//sets the node to temp so that it can be
                              //passed into the helper function to insertPreorder
      insertPreorder(temp);
  }
  

  // 
  // size:
  //
  // Returns the # of nodes in the tree, 0 if empty.
  //
  // Time complexity:  O(1) 
  //
  int size() const
  {
    return Size;
  }


  // 
  // search:
  //
  // Searches the tree for the given key, returning true if found
  // and false if not.  If the key is found, the corresponding value
  // is returned via the reference parameter.
  //
  // Time complexity:  O(lgN) on average
  //
  bool search(KeyT key, ValueT& value) const
  {
    NODE* current = Root;
    while(current != nullptr){
        /* If the keys are the same */
        if(key == current->Key){
            value = current->Value;
            return true;
        }
        /*if the key is less than the cur->Key*/
        else if(key < current->Key){
            current = current->Left;
        }
        else{
            /*accounting for the threaded aspect*/
            if(current->isThreaded){
                current = nullptr;
            }
            else{
                current = current ->Right;
            }
        }
    }
		return false;
  }


  //
  // insert
  //
  // Inserts the given key into the tree; if the key has already been insert then
  // the function returns without changing the tree.
  //
  // Time complexity:  O(lgN) on average
  //
  void insert(KeyT key, ValueT value)
  {
    NODE* prev = nullptr;
    NODE* cur = Root;
    while (cur != nullptr)
    {
      if (key == cur->Key)  // already in tree
        return;

      if (key < cur->Key)  // search left:
      {
        prev = cur ;
        cur = cur->Left;
      }
       else                //searches right
      {
        prev = cur;
        if(cur->isThreaded) //if the right is threaded
            cur = nullptr; //stop the transversal, meaning the thread is not there
        else //not threaded, meaning there is a node to go to
            cur = cur->Right;
      }
    }
    
    //at this point the tree is not empty and the key is 
    //not present in the tree
    NODE* newNode = new NODE;
         newNode->Key = key;
         newNode->Value = value;
         newNode->Left = nullptr;
         newNode->Right = nullptr;
         newNode->isThreaded = true;
         
     //if tree is empty    
    if(prev == nullptr ){
       Root = newNode;
    }//end if()
    
    /*when the key is less than the key being checked*/
    else if(key < prev->Key){
        newNode->Right = prev;
        prev->Left = newNode;
    }//end of if(key < cur->key)
    
    else if(key > prev->Key){
        newNode->Right = prev->Right;
        prev->isThreaded = false;
        prev->Right = newNode;
    }//end of else if(key > prev->Key)	
    Size++;//incrementing size for newNode added
  }


  //
  // []
  //
  // Returns the value for the given key; if the key is not found,
  // the default value ValueT{} is returned.
  //
  // Time complexity:  O(lgN) on average
  //
  ValueT operator[](KeyT key) const
  {
    NODE* prev = nullptr;
    NODE* cur = Root;
    while (cur != nullptr)
    {
      if (key == cur->Key)  // already in tree
        return cur->Value;

      if (key < cur->Key)  // search left:
      {
          prev = cur;
          cur = cur->Left;
      }
      else                 //search right
      {
          prev = cur;
          if(cur->isThreaded) //if the right is threaded
              cur = nullptr; //stop the transversal, meaning the thread is not there
          else //not threaded, meaning there is a node to go to
              cur = cur->Right;
      }
    }//while

    return ValueT{ };
  }


  //
  // ()
  //
  // Finds the key in the tree, and returns the key to the "right".
  // If the right is threaded, this will be the next inorder key.
  // if the right is not threaded, it will be the key of whatever
  // node is immediately to the right.
  //
  // If no such key exists, or there is no key to the "right", the
  // default key value KeyT{} is returned.
  //
  // Time complexity:  O(lgN) on average
  //
  KeyT operator()(KeyT key) const
  {
    //NODE* prev = nullptr;
    NODE* cur = Root;
    
    while (cur != nullptr)
    {
      if (key == cur->Key && cur->Right != nullptr){  // the key is in current/root, i.e the key has been found...
          cur = cur->Right; //transverse to the right
          return cur->Key;
      }  
      
      if(key < cur->Key)
          cur = cur->Left;
          
      else{
          //prev = cur;
          if(cur->isThreaded) //if the right is threaded
              break; //stop the transversal, meaning the thread is not there
          else //not threaded, meaning there is a node to go to
              cur = cur->Right;
      }
    }//while

    return KeyT{ };
  }


  //
  // begin
  //
  // Resets internal state for an inorder traversal.  After the 
  // call to begin(), the internal state denotes the first inorder
  // key; this ensure that first call to next() function returns
  // the first inorder key.
  //
  // Space complexity: O(1)
  // Time complexity:  O(lgN) on average
  //
  // Example usage:
  //    tree.begin();
  //    while (tree.next(key))
  //      cout << key << endl;
  //
  void begin()
  {
    NODE* cur = Root;
    if(cur!= NULL){
        while(cur->Left != NULL){
            cur = cur->Left;
        }
        currentNode = cur;//sets the first value in the inorder traversal
    }
  }


  //
  // next
  //
  // Uses the internal state to return the next inorder key, and 
  // then advances the internal state in anticipation of future
  // calls.  If a key is in fact returned (via the reference 
  // parameter), true is also returned.
  //
  // False is returned when the internal state has reached null,
  // meaning no more keys are available.  This is the end of the
  // inorder traversal.
  //
  // Space complexity: O(1)
  // Time complexity:  O(lgN) on average
  //
  // Example usage:
  //    tree.begin();
  //    while (tree.next(key))
  //      cout << key << endl;
  //
   bool next(KeyT& key)
  {
    NODE* cur = currentNode;
    
    //Tree is empty and no traversal to be done
    if(cur == nullptr){
        return false;
    }
    
    //Tree not empty and traversal starts here
    else{
    	key = cur->Key;
    
    //Accounts so that if threaded it moves to threaded value
    if(cur->Left == NULL && cur->isThreaded == true){
        cur = cur->Right;
    }
    
    //Keeps going further left towards the bottom so that is printed
    //first as is the format for inorder traversal
    else if(cur->Left == NULL && cur->isThreaded == false){
       cur = cur->Right;
        while(cur->Left != NULL){
            cur = cur->Left;
        }
    }
    
    //Keeps going further right towards the bottom so that is printed
    //first as is the format for inorder traversal 
    else if(cur->Left != NULL && cur->isThreaded == false){
       cur = cur->Right;
        while(cur->Left != NULL){
            cur = cur->Left;
        }
    }
    
    //Just goes into the value that it is threaded as it is already
    //at the bottom of the tree
    else if(cur->Left != NULL && cur->isThreaded == true){
        cur = cur->Right;
    }
    
    //sets the currentNode to the value that is to be next key
    //in the inorder traversal
    currentNode = cur;
    return true;
  }
    return false;
  }


  //
  // printInOrder
  //
  // This function is a helper function for the dump which uses recursion as 
  // the way inorder traversal is usually done to correspond and make sure that 
  // the begin and next functions that we have built are working as they should
  //
   void printInOrder(NODE* cur, ostream& output)const {
   
      //If tree is empty
      if(cur == nullptr){
          return;
      }
      
      //Tree is not empty and inorder traversal starts here
      else{
      
          //recurive call for left subtree
          printInOrder(cur->Left, output);
          
          //Accounts for threading in printing out the key, value and thread(only if thread exists)
          if(cur->isThreaded == true && cur->Right != nullptr){
              output <<"(" << cur->Key << "," << cur->Value << "," << cur->Right->Key << ")" << endl;
              cur->Right = NULL;
          }else{
              output <<"(" << cur->Key << "," << cur->Value << ")" << endl;
          }
          
          //Accounts for thread and if thread doesnt exist then it traverses to the right
          if(cur->isThreaded == false){
              printInOrder(cur->Right, output);
          }
      }
   }
    

  //
  // dump
  // 
  // Dumps the contents of the tree to the output stream, using a
  // recursive inorder traversal.
  //
  void dump(ostream& output) const
  {
    output << "**************************************************" << endl;
    output << "********************* BSTT ***********************" << endl;

    output << "** size: " << this->size() << endl;
    
    printInOrder(Root,output);

    output << "**************************************************" << endl;
  }
	
};
