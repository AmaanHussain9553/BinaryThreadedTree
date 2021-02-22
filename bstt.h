/*bstt.h*/

//
// Threaded binary search tree
//

#pragma once

#include <iostream>
#include <stack>

using namespace std;

template<typename KeyT, typename ValueT>
class avlt
{
private:
  struct NODE
  {
    KeyT   Key;
    ValueT Value;
    NODE*  Left;
    NODE*  Right;
    bool   isThreaded;
    int    Height;
  };

  NODE* Root;  // pointer to root node of tree (nullptr if empty)
  int   Size = 0;  // # of nodes in the tree (0 if empty)
  NODE* Next;//Node to store current node during inorder iteration
                    //for begin and next function

  //
  // helper functions to return the Left and Right pointers; in
  // particular the ptr returned for the Right is controlled by 
  // the "isThreaded" field:  if "isThreaded" is true then nullptr
  // pointer is returned, otherwise the actual underyling ptr
  // is returned.
  //
  NODE* _getActualLeft(NODE* cur) const
  {
    return cur->Left;
  }

  NODE* _getActualRight(NODE* cur) const
  {
    if (cur->isThreaded)  // then actual Right ptr is null:
      return nullptr;
    else  // actual Right is contents of Right ptr:
      return cur->Right;
  }

  //
  // _free
  // 
  // Frees the memory associated with the tree.
  //
  void _free(NODE* cur)
  {
    if (cur == nullptr)
      return;
    else
    {
      // 
      // use post-order traversal since need to free
      // sub-trees first, then we can free the root:
      // 
      _free(_getActualLeft(cur));
      _free(_getActualRight(cur));
      delete cur;
    }
  }

  //
  // _copy
  //
  // Makes a copy of "othercur" into "this" tree.
  //
  void _copy(NODE* othercur)
  {
    if (othercur == nullptr)
      return;
    else
    {
      //
      // preorder: copy root, then we can copy left and right:
      //
      insert(othercur->Key, othercur->Value);

      _copy(_getActualLeft(othercur));
      _copy(_getActualRight(othercur));
    }
  }

  // 
  // _dump
  //
  void _dump(ostream& output, NODE* cur) const
  {
    if (cur == nullptr)
      return;
    else
    {
      //
      // inorder traversal, with one output per line: either 
      // (key,value) or (key,value,THREAD)
      //
      // (key,value) if the node is not threaded OR thread==nullptr
      // (key,value,THREAD) if the node is threaded and THREAD denotes the next inorder key
      //
      _dump(output, _getActualLeft(cur));

      if(cur->isThreaded == true && cur->Right != nullptr){
              output <<"(" << cur->Key << "," << cur->Value << "," << cur->Right->Key << ")" << endl;
              cur->Right = NULL;
          }else{
              output <<"(" << cur->Key << "," << cur->Value << ")" << endl;
          }
          

      _dump(output, _getActualRight(cur));
    }
  }


public:
  //
  // default constructor:
  //
  // Creates an empty tree.
  //
  avlt()
  {
    Root = nullptr;
    Size = 0;
    Next = nullptr;
  }

  //
  // copy constructor
  //
  avlt(const avlt& other)
  {
    //
    // construct an empty tree, then let's make a copy of other:
    //
    Root = nullptr;
    Size = 0;
    Next = nullptr;

    _copy(other.Root);

    this->Next = other.Next;  // to be safe, copy this state as well:
  }

	//
  // destructor:
  //
  // Called automatically by system when tree is about to be destroyed;
  // this is our last chance to free any resources / memory used by
  // this tree.
  //
  virtual ~avlt()
  {
    _free(Root);
  }

  //
  // operator=
  //
  // Clears "this" tree and then makes a copy of the "other" tree.
  //
  avlt& operator=(const avlt& other)
  {
    //
    // we have an existing tree, so clear it:
    //
    this->clear();

    //
    // now copy the other one:
    //
    _copy(other.Root);

    this->Next = other.Next;  // to be safe, copy this state as well:

    return *this;
  }

  //
  // clear:
  //
  // Clears the contents of the tree, resetting the tree to empty.
  //
  void clear()
  {
    _free(Root);

    // now reset to empty:
    Root = nullptr;
    Size = 0;
    Next = nullptr;
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
  // height:
  //
  // Returns the height of the tree, -1 if empty.
  //
  // Time complexity:  O(1) 
  //
  int height() const
  {
    if (Root == nullptr)
      return -1;
    else
      return Root->Height;
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
    NODE* cur = Root;

    while (cur != nullptr)
    {
      if (key == cur->Key)  // already in tree
      {
        value = cur->Value;
        return true;
      }

      if (key < cur->Key )  // search left:
      {
        cur = _getActualLeft(cur);
      }
      else
      {
        cur = _getActualRight(cur);
      }
    }//while  

    // if get here, not found
    return false;
  }

  //
  // range_search
  //
  // Searches the tree for all keys in the range [lower..upper], inclusive.
  // It is assumed that lower &lt;= upper.  The keys are returned in a vector;
  // if no keys are found, then the returned vector is empty.
  //
  // Time complexity: O(lgN + M), where M is the # of keys in the range
  // [lower..upper], inclusive.
  //
  // NOTE: do not simply traverse the entire tree and select the keys
  // that fall within the range.  That would be O(N), and thus invalid.
  // Be smarter, you have the technology.
  //
  /*vector&lt;KeyT&gt; range_search(KeyT lower, KeyT upper)
  {
    vector&lt;KeyT&gt;  keys;

    //
    // TODO
    //

    return keys;
  }*/
  
  
  //
  // _RightRotate
  //
  // Rotates the tree around the node N, where Parent is N's parent.  Note that
  // Parent could be null, which means N is the root of the entire tree.  If 
  // Parent denotes a node, note that N could be to the left of Parent, or to
  // the right.  You'll need to take all this into account when linking in the
  // new root after the rotation.  Don't forget to update the heights as well.
  //
  void _RightRotate(NODE* Parent, NODE* N)
  {
     NODE *L = N->Left;//setting up all the nodes to take into account
     NODE* A = L->Left;
     NODE* B = L->Right;
     NODE* C = N->Right;
     
     L->Right = N;//rotating L and N
     N->Left = B;
     
     if(Parent == NULL){ //Setting the parent to L and testing the 3 cases
      Root = L;
     }
     else if(L->Key < Parent->Key){
        Parent->Left = L;
     }
     else if(L->Key > Parent->Key){
        Parent->Right = L;
     }
     
     //4. Update the height of L and N
     int HA, HB, HC;
     if(B == nullptr){
        HB = -1;
     }else{
        HB = B->Height;
     }
     
     if(C == nullptr){
        HC = -1;
     }else{
        HC = C->Height;
     }
     
     N->Height = 1+ max(HB,HC);
     
      if(A == nullptr){
        HA = -1;
     }else{
        HA = A->Height;
     }
     
     L->Height = 1 + max(HA, N->Height);
     
  }
  
  
  //
  // _LeftRotate
  //
  // Rotates the tree around the node N, where Parent is N's parent.  Note that
  // Parent could be null, which means N is the root of the entire tree.  If 
  // Parent denotes a node, note that N could be to the left of Parent, or to
  // the right.  You'll need to take all this into account when linking in the
  // new root after the rotation.  Don't forget to update the heights as well.
  //
  void _LeftRotate(NODE* Parent, NODE* N)
  {
     NODE* R = N->Right;
     NODE* C = R->Right;
     NODE* B = R->Left;
     NODE* A = N->Left;
     
     R->Left = N;
     N->Right = B;
     
     if(Parent == NULL){ //Setting the parent to L and testing the 3 cases
          Root = R;
     }
     else if(R->Key < Parent->Key){
        Parent->Left = R;
     }
     else if(R->Key > Parent->Key){
        Parent->Right = R;
     }
     
     //4. Update the height of L and N
     int HA, HB, HC;
     if(A == nullptr){
        HA = -1;
     }else{
        HA = A->Height;
     }
     
     if(B == nullptr){
        HB = -1;
     }else{
        HB = B->Height;
     }
     
     N->Height = 1+ max(HA,HB);
     
      if(C == nullptr){
        HC = -1;
     }else{
        HC = C->Height;
     }
     
     R->Height = 1 + max(HC, N->Height);
  }

  //
  // HeightCheck
  // 
  // look at current node and checks if it is a trouble node
  // to do the 4 different cases of Right rotate, left rotate,
  // left right rotate or right left rotate
  //
  int heightCheck(NODE* check){
      if(check == NULL){
          return 0;
      }else{
          return height((check->Left) - height(check->Right));
      }
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
    NODE* parent = nullptr;
    NODE* cur = Root;
    stack <NODE> nodes;

    //
    // 1. Search to see if tree already contains key:
    //
    while (cur != nullptr)
    {
      if (key == cur->Key){  // already in tree
        return;
      }
        
      //push value into stack
      nodes.push(cur);  

      if (key < cur->Key)  // search left:
      {
        parent = cur;
        cur = _getActualLeft(cur);
      }
      else
      {
        parent = cur;
        cur = _getActualRight(cur);
      }
    }//while

    //
    // 2. if we get here, key is not in tree, so allocate
    // a new node to insert:
    // 
    NODE* n = new NODE();
    n->Key = key;
    n->Value = value;
    n->Left = nullptr;
    n->Right = nullptr;
    n->isThreaded = false;
    n->height = parent->Height - 1;

    
    //
    // 3. link in the new node:
    //
    // NOTE: cur is null, and parent denotes node where
    // we fell out of the tree.  if parent is null, then
    // the tree is empty and the Root pointer needs 
    // to be updated.
    //
    if (parent == nullptr)
    {
      Root = n;
      //
      // we are the only node in the tree, and thus 
      // the last node inorder: so threaded is true
      // and ptr is null:
      //
      n->isThreaded = true;
      n->Right = nullptr;
      //n->Height = 0;
    }
    else if (key < parent->Key)
    {
      parent->Left = n;
      
      //When inserting to the left of a node checks if a right is 
      //already present, if not present then increses height of parent
      NODE* temp = _getActualRight(parent->Right);
  
      //
      // we are to the left of our parent, which means
      // our parent is next inorder --- so threaded 
      // points to our parent:
      //
      n->isThreaded = true;
      n->Right = parent;
    }
    else
    {
      //
      // we are to the right of our parent, which means
      // our successor is our parent's successor --- so
      // copy that threaded pointer before we change it:
      //
      n->isThreaded = true;
      n->Right = parent->Right;
      
      //When inserting to the right of a node checks if a left is 
      //already present, if not present then increses height of parent
      NODE* temp = _getActualRight(parent->Left);

      // parent is no longer threaded since they link to us:
      parent->isThreaded = false;
      parent->Right = n;
    }

    // 
    // 4. Update size and we're done
    //
    Size++;   
    
    
    /* 5. Get the heightConfigure factor of this ancestor  
        node to check whether this node became  
        unheightConfigured */
    int heightConfigure = heightCheck(parent, n);
    
    // Left Left Case  
    if (heightConfigure > 1 && key < parent->Left->key)  
        return _RightRotate(parent, n);  
  
    // Right Right Case  
    if (heightConfigure < -1 && key > parent->Right->key)  
        return _LeftRotate(parent, n);  
  
    // Left Right Case  
    if (heightConfigure > 1 && key > parent->Left->key)  
    {  
        parent->Left = _LeftRotate(parent->Left);  
        return _RightRotate(parent, n);  
    }  
  
    // Right Left Case  
    if (heightConfigure < -1 && key < parent->Right->key)  
    {  
        parent->Right = _RightRotate(parent->Right);  
        return _LeftRotate(parent, n);  
    }
    
    //
    // 6. Walk back up tree using stack and update heights.
    //
    while (!nodes.empty())
    {
      cur = nodes.top();
      nodes.pop();

      int hL = (cur->Left == nullptr) ? -1 : cur->Left->Height;
      int hR = (cur->Right == nullptr) ? -1 : cur->Right->Height;
      int hCur = 1 + std::max(hL, hR);

      if (cur->Height == hCur)  // didn't change, so no need to go further:
        break;
      else  // height changed, update and keep going:
        cur->Height = hCur;
    }//while
    
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
    ValueT  value = ValueT{};

    //
    // perform search, and either we find it --- and value is 
    // updated --- or we don't and we'll return default value:
    //
    search(key, value);

    return value;
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
    NODE* cur = Root;

    //
    // we have to do a traditional search, and then work from
    // there to follow right / thread:
    //
    while (cur != nullptr)
    {
      if (key == cur->Key)  // found it:
      {
        // 
        // we want the key to the right, either immediately right
        // or the threaded inorder key.  Just need to make sure
        // we have a pointer first:
        if (cur->Right == nullptr)  // threaded but null:
          return KeyT{ };
        else
          return cur->Right->Key;
      }

      if (key < cur->Key)  // search left:
      {
        cur = _getActualLeft(cur);
      }
      else
      {
        cur = _getActualRight(cur);
      }
    }//while  

    //
    // if get here, not found, so return default key:
    //
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
  //      cout &lt;&lt; key &lt;&lt; endl;
  //
  void begin()
  {
    Next = Root;

    if (Next != nullptr)  // advance as left as possible:
    {
      while (Next->Left != nullptr)
        Next = Next->Left;
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
  //      cout &lt;&lt; key &lt;&lt; endl;
  //
  bool next(KeyT& key)
  {
    if (Next == nullptr)
    {
      //
      // no more keys:
      //
      return false;
    }

    //
    // we have at least one more, so grab it now,
    // advance, and return true:
    //
    key = Next->Key;

    if (_getActualRight(Next) == nullptr)
    {
      // follow the thread:
      Next = Next->Right;
    }
    else
    {
      Next = Next->Right;  // go right, and then left as far as we can:
      
      while (Next->Left != nullptr)
        Next = Next->Left;
    }

    return true;
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
    output << "********************* AVLT ***********************" << endl;

    output << "** size: " << this->size() << endl;
    
    printInOrder(Root,output);

    output << "**************************************************" << endl;
  }
	
};