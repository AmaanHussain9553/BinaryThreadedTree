/*test01.cpp*/

//
// Unit tests for threaded binary search tree
//

#include <iostream>
#include <vector>

#include "bstt.h"

#include "catch.hpp"

using namespace std;


TEST_CASE("(1) empty tree") 
{
  bstt<int, int>  tree;
  REQUIRE(tree.size() == 0);
    
  tree.insert(2,4);
  tree.insert(4,8);
  tree.insert(8,16);
  REQUIRE(tree.size() == 3);
    
  tree.dump(cout);
    
  bstt<int, int>  tree2(tree);
  REQUIRE(tree2.size() == 3);
  
  tree2.insert(16,32);
  REQUIRE(tree2.size() == 4);
  
  bstt<int, int>  tree3 = tree;
  REQUIRE(tree3.size() == 3);
    
  tree3.insert(32,64);
  REQUIRE(tree3.size() == 4);
  
  int nigger;
  nigger = tree3(4);
  REQUIRE(nigger == 8);

    
  tree.clear();
  REQUIRE(tree.size() == 0);
    
  tree2.clear();
  REQUIRE(tree.size() == 0);
    
  tree3.clear();
  REQUIRE(tree.size() == 0);
    
  
   

  
}

/*if(cur->isThreaded){
              cur = cur->Right; //transverse the thread to go to they key connected with the thread...
              return cur->Key; //return the key at the threaded link
          }else if (cur->isThreaded == false){
              return cur->Key; //if there is no thread then just return the key to the right of the required key 
          }else if(cur == nullptr){
              break; //no right key exists, then break and return the default value
          }
        }
      if (key < cur->Key)  // search left:
      {
          //prev = cur;
          cur = cur->Left;
      }*/
