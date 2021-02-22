# BinaryThreadedTree
Header File for Binary Search Tree that is threaded so that when the tree is printed using inorder traversal, the elements are printed from ascending to descending order

This header file is create an instance of the binary threaded tree with the following implementations
  
  -> Using a struct to create each node in the tree with a left and right pointer
  -> A default constructor 
  -> A destructor that uses two helper functions
  -> [] and () operator functions to retrieve values from the tree
  -> insert function to find a node with a certain key to find its value
  -> search function to find a node with a certain key
  -> A recursive function that prints the tree in-order
  -> The in-order printing utilizes the begin() and next() that have been 
      implemented to help the inorder traversal
  -> A dump function that prints out all the contents of the tree
  -> This project also uses templates to be able to create the tree data
     structure with any data type while being able to create the threaded links
 
 This commit also includes test cases which tests the implementation and wedge cases 
 of the binary threaded tree's implementation.
 
 Coding Platform:
 C++ IDE, no graphics required, makefile included
