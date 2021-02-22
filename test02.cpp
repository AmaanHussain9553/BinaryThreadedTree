/*test01.cpp*/

//
// Unit tests for threaded binary search tree
//

#include <iostream>
#include <vector>

#include "bstt.h"

#include "catch.hpp"

using namespace std;


TEST_CASE("(2) Begin and next functions test") 
{
  bstt<int, int>  tree;
  REQUIRE(tree.size() == 0);
  int key;
  int value;
  
  tree.insert(30,value);
  tree.insert(15,value);
  tree.insert(8,value);  
  tree.insert(25,value);
  tree.insert(20,value);
  tree.insert(28,value);
  tree.insert(50,value);
  tree.insert(70,value);  
  tree.insert(60,value);  
  REQUIRE(tree.size() == 9);
    
  //tree.dump(cout);
    
  tree.begin();
  
  tree.next(key);
  REQUIRE(key == 8);
    
  tree.next(key);
  REQUIRE(key == 15);
     
  tree.next(key);
  REQUIRE(key == 20);
   
  tree.next(key);
  REQUIRE(key == 25);
     
  tree.next(key);
  REQUIRE(key == 28);
    
  tree.next(key);
  REQUIRE(key == 30);
    
  tree.next(key);
  REQUIRE(key == 50);
       
  tree.next(key);
  REQUIRE(key == 60);
    
  tree.next(key);
  REQUIRE(key == 70);
      
}