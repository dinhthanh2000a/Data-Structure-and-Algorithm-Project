#include <iostream>
#include <queue>
#include <vector>
using namespace std;

template <class K, class V>
class BKUTree {
public:
    class AVLTree;
    class SplayTree;
    class Entry {
    public:
        K key;
        V value;
        Entry(K key, V value) : key(key), value(value) {}
    };

public:
    AVLTree* avl ;
    SplayTree* splay ;
    queue<K> keys;
    int unsigned maxNumOfKeys;

public:
    BKUTree(int maxNumOfKeys = 5) {
        avl = new AVLTree();
        splay = new SplayTree();
        this->maxNumOfKeys = maxNumOfKeys;
    }
    ~BKUTree() { this->clear(); }
    void add(K key, V value);
    void remove(K key);
    bool searchNode(K key, vector<K>& traversedList, V& returnvalue);
    V search(K key, vector<K>& traversedList);
    void traverseNLROnAVL(void (*func)(K key, V value));
    void traverseNLROnSplay(void (*func)(K key, V value));
    void clear();
    class SplayTree {
    public:
        class Node {
        public:
            Entry* entry;
            Node* left;
            Node* right;
            Node* parent;
            typename AVLTree::Node* corr;
            Node() {
                this->entry = NULL;               
                this->left = NULL;
                this->right = NULL;
                this->parent = NULL;
                this->corr = NULL;
            }
        };

    public:
        Node* root;
        SplayTree() {
            root = NULL;
        }
        ~SplayTree() {
            this->clear();
        };      
        void add(K key, V value) {
            if (searchNode(root, key) != NULL) throw ("Duplicate key"); //   Duplicate key in tree
            else {
                Node* pNode = newNode(key, value);
                insert(pNode); //add Node
            }
        }
        void support(Node* inNode) {   //add tree in BKUTree
            insert(inNode);
            
        }
        void add(Entry* entry) {  //the entry being passed
            add(entry->key, entry->value);
        }
        void remove(K key) {
            if (root==NULL) throw ("Not found");
            Node* test = searchKey(key); //search node
            if (test == NULL) throw ("Not found"); //node has key that does not exist
            else removeNode(test);
        }
        V search(K key) {
            if (root == NULL) throw ("Not found");
            Node* test = searchKey(key);
            if (test == NULL) throw ("Not found"); //node has key that does not exist
            else return (test->entry->value); //return value of node
            
        }
        void traverseNLR(void (*func)(K key, V value)) {           
            preOrder(root,func);
        }
        void deleteTree(Node* inNode) {
            if (inNode == NULL) return;
            /* delete both subtrees */

            deleteTree(inNode->left);
            deleteTree(inNode->right);

            /* delete the node */
            delete inNode->entry;
            inNode->entry = NULL;
            delete inNode;
            inNode = NULL;
        }
        void clear() { //destroy tree
            deleteTree(root);
            root = NULL;
            
        }
        Node* newNode(K key, V value) { //create new node
            Node* pNode = new Node();
            pNode->entry = new Entry(key, value);
            pNode->left = pNode->right = NULL;
            pNode->parent = NULL;
            return (pNode);
        }
        Node* maximum(Node* inNode) {  // node has key maximum in the left child of x
            while (inNode->right != NULL) {
                inNode = inNode->right;
            }
            return inNode;
        }
        void rightRotate(Node* inNode) {
            Node* pNode = inNode->left;
            inNode->left = pNode->right;
            if (pNode->right != NULL) {
                pNode->right->parent = inNode;
            }
            pNode->parent = inNode->parent;
            if (inNode->parent == NULL) { //node is root
                root = pNode;
            }
            else if (inNode == inNode->parent->right) { //node is right child
                inNode->parent->right = pNode;
            }
            else { //node is left child
                inNode->parent->left = pNode;
            }
            pNode->right = inNode;
            inNode->parent = pNode;
        }
        void leftRotate(Node* inNode)
        {
            Node* pNode = inNode->right;
            inNode->right = pNode->left;
            if (pNode->left != NULL) {
                pNode->left->parent = inNode;
            }
            pNode->parent = inNode->parent;
            if (inNode->parent == NULL) { //x is root
                root = pNode;
            }
            else if (inNode == inNode->parent->left) { //x is left child
                inNode->parent->left = pNode;
            }
            else { //x is right child
                inNode->parent->right = pNode;
            }
            pNode->left = inNode;
            inNode->parent = pNode;
        }
        void splayRotate_1(Node* inNode) {
            if (inNode->parent != NULL) { // node is not root
                if (inNode->parent == root) { // Zig (Right Rotation) / Zag (Left Rotation)  
                    if (inNode == inNode->parent->left) { //zig
                        rightRotate(inNode->parent);
                    }
                    else {  //zag
                        leftRotate(inNode->parent);
                    }
                }
                else {
                    Node* parentNode = inNode->parent; //parent of node
                    Node* grandparent = parentNode->parent; //grandparent of node 

                    if (inNode->parent->left == inNode && parentNode->parent->left == parentNode) { // (Zig-Zig)
                        rightRotate(grandparent);
                        rightRotate(parentNode);
                    }
                    else if (inNode->parent->right == inNode && parentNode->parent->right == parentNode) { // (Zag-Zag  )
                        leftRotate(grandparent);
                        leftRotate(parentNode);
                    }
                    else if (inNode->parent->right == inNode && parentNode->parent->left == parentNode) {  // (Zag-Zig)
                        leftRotate(parentNode);
                        rightRotate(grandparent);
                    }
                    else if (inNode->parent->left == inNode && parentNode->parent->right == parentNode) { // (Zig-Zag)
                        rightRotate(parentNode);
                        leftRotate(grandparent);
                    }
                }
            }
        }
        void splayRotate(Node* inNode) {   // move node to root of tree
            while (inNode->parent != NULL) { // node is not root
                if (inNode->parent == root) { // Zig (Right Rotation) / Zag (Left Rotation)  
                    if (inNode == inNode->parent->left) { //zig
                        rightRotate(inNode->parent);
                    }
                    else {  //zag
                        leftRotate(inNode->parent);
                    }
                }
                else {
                    Node* parentNode = inNode->parent; //parent of node
                    Node* grandparent = parentNode->parent; //grandparent of node 

                    if (inNode->parent->left == inNode && parentNode->parent->left == parentNode) { // (Zig-Zig)
                        rightRotate(grandparent);
                        rightRotate(parentNode);
                    }
                    else if (inNode->parent->right == inNode && parentNode->parent->right == parentNode) { // (Zag-Zag  )
                        leftRotate(grandparent);
                        leftRotate(parentNode);
                    }
                    else if (inNode->parent->right == inNode && parentNode->parent->left == parentNode) {  // (Zag-Zig)
                        leftRotate(parentNode);
                        rightRotate(grandparent);
                    }
                    else if (inNode->parent->left == inNode && parentNode->parent->right == parentNode) { // (Zig-Zag)
                        rightRotate(parentNode);
                        leftRotate(grandparent);
                    }
                }
            }
        }
        void insert(Node* inNode) { //insert node
            Node* pNode = NULL;
            Node* tempNode = root;
            while (tempNode != NULL) {
                pNode = tempNode;
                if (inNode->entry->key < tempNode->entry->key) tempNode = tempNode->left;
                else tempNode = tempNode->right;
            }
            inNode->parent = pNode;
            if (pNode == NULL) {
                root = inNode;
            }
            else if (inNode->entry->key < pNode->entry->key) pNode->left = inNode;
            else  pNode->right = inNode;
            splayRotate(inNode);
        }
        void changeNode(Node* node_1, Node* node_2) {     //swap node 
            if (node_1->parent == NULL) { //node_1 is root
                root = node_2;
            }
            else if (node_1 == node_1->parent->left) { //node_1 is left child
                node_1->parent->left = node_2;
            }
            else { //node_1 is right child
                node_1->parent->right = node_2;
            }
            if (node_2 != NULL)  node_2->parent = node_1->parent;
        }
        void removeNode(Node* inNode) {
            if (inNode == root && root->left==NULL && root->right==NULL) {
                delete inNode;
                root = NULL;
            }
            else {
                if (inNode->left == NULL) {
                    changeNode(inNode, inNode->right);
                    delete inNode;
                    inNode = NULL;
                }
                else if (inNode->right == NULL) {
                    changeNode(inNode, inNode->left);
                    delete inNode;
                    inNode = NULL;
                }
                else {
                    Node* maxNode = maximum(inNode->left); //maximum element in left subtree
                    if (maxNode->parent != inNode) {
                        Node* rootInnode = root;

                        Node* rootLeft = root->left;
                        rootLeft->parent = NULL;
                        Node* rootRight = root->right;
                        rootRight->parent = NULL;


                        root = rootLeft;
                        splayRotate(maxNode);
                        maxNode->right = rootInnode->right;
                        rootInnode->right->parent = maxNode;
                        //root = maxNode;
                        delete inNode;
                        inNode = NULL;
                    }
                    else {
                        changeNode(inNode, maxNode);
                        maxNode->right = inNode->right;
                        maxNode->right->parent = maxNode;
                        delete inNode;
                        inNode = NULL;
                    }
                }
            }
        }
        Node* searchNode(Node* inNode, K key) {
            
            if (inNode == NULL) return NULL;
            if (inNode->entry->key == key) {
                return inNode;
            }
            else if (inNode->entry->key < key) {
                return searchNode(inNode->right, key);
            }
            else if (inNode->entry->key > key) {
                return searchNode(inNode->left, key);
            }
            else return NULL;
        }
        Node* searchKey(K key) { //return Node has key in the tree
            
            Node* test = searchNode(root, key);
            if (test != NULL) splayRotate(test);
            return test;
        }
        Node* searchTraver(Node* node, K key, vector<K>& traversedList) {   // BKUTree search
            
            if (node == NULL) return NULL;
            if (node->entry->key == key) {
                return node;
            }
            else if (node->entry->key < key) {
                traversedList.push_back(node->entry->key);
                return searchTraver(node->right, key, traversedList);
            }
            else if (node->entry->key > key) {
                traversedList.push_back(node->entry->key);
                return searchTraver(node->left, key, traversedList);
            }

            else return NULL;
        }
        void preOrder(Node* inNode, void (*func)(K key, V value))
        {
            if (inNode != NULL)
            {
                (*func)(inNode->entry->key, inNode->entry->value);
                preOrder(inNode->left,func);
                preOrder(inNode->right,func);
            }
        }
    };

    class AVLTree {
    public:
        class Node {
        public:
            Entry* entry;
            Node* left;
            Node* right;
            Node* parent;
            int balance;
            int height;
            typename SplayTree::Node* corr;
            Node() {
                this->entry = NULL;
                this->left = NULL;
                this->right = NULL;
                this->height = 0;
                this->balance = 0;
                this->parent = NULL;
                this->corr = NULL;
            }           
        };

    public:
        Node* root;
        AVLTree() {
            root = NULL;
        }
        ~AVLTree() {
            this->clear();
        };      
        void add(K key, V value) {
            Node* test = searchNode(root, key);
            if (test != NULL) throw ("Duplicate key"); //   Duplicate key in tree
            else {
                Node* pNode = newNode(key, value);
                insert(pNode); //add Node
            }

        }
        void support(Node* inNode) {   //add tree in BKUTree
            insert(inNode);
        }
        void add(Entry* entry) {  //the entry being passed
            add(entry->key, entry->value);
        }
        void remove(K key) {
            if (root==NULL) throw ("Not found");
            Node* test = searchKey(key); //search node
            if (test == NULL) throw ("Not found"); //node has key that does not exist
            else removeNode(test);
        }
        V search(K key) {
            if (root==NULL) throw ("Not found");
            Node* test = searchKey(key);
            if (test == NULL) throw ("Not found"); //node has key that does not exist
            else return (test->entry->value); //return value of node
        }
        void traverseNLR(void (*func)(K key, V value)) {                    
            preOrder(root, func);
        }
        void deleteTree(Node* inNode) {
            if (inNode == NULL) return;
            /* delete both subtrees */
            deleteTree(inNode->left);
            deleteTree(inNode->right);
            /* delete the node */
            delete inNode->entry;
            inNode->entry = NULL;
            delete inNode;
            inNode = NULL;
        }
        void clear() {  //destroy tree
            deleteTree(root);
            root = NULL;
        }
        Node* newNode(K key, V value) { //create new node
            Node* pNode = new Node();
            pNode->entry = new Entry(key, value);
            pNode->left = NULL;
            pNode->right = NULL;
            pNode->balance = 0;
            pNode->height = 1;
            pNode->parent = NULL;
            return(pNode);
        }
        Node* maximum(Node* inNode) { // node has key maximum in the left child of x
            while (inNode->right != NULL) {
                inNode = inNode->right;
            }
            return inNode;
            
        }
        void leftRotate(Node* inNode) {           
                Node* pNode = inNode->right;
                inNode->right = pNode->left;
                if (pNode->left != NULL) {
                    pNode->left->parent = inNode;
                }
                pNode->parent = inNode->parent;
                if (inNode->parent == NULL) { //node is root
                    root = pNode;
                }
                else if (inNode == inNode->parent->left) { //node is left child
                    inNode->parent->left = pNode;
                }
                else { //node is right child
                    inNode->parent->right = pNode;
                }
                pNode->left = inNode;
                inNode->parent = pNode;
                inNode->height = 1 + max((inNode->left == NULL) ? 0 : inNode->left->height, (inNode->right == NULL) ? 0 : inNode->right->height);
                inNode->balance = (((inNode->left == NULL) ? 0 : inNode->left->height) - ((inNode->right == NULL) ? 0 : inNode->right->height));
                pNode->height = 1 + max((pNode->left == NULL) ? 0 : pNode->left->height, (pNode->right == NULL) ? 0 : pNode->right->height);
                pNode->balance = (((pNode->left == NULL) ? 0 : pNode->left->height) - ((pNode->right == NULL) ? 0 : pNode->right->height));
            
        }
        void rightRotate(Node* inNode) {
            Node* pNode = inNode->left;
            inNode->left = pNode->right;
            if (pNode->right != NULL) {
                pNode->right->parent = inNode;
            }
            pNode->parent = inNode->parent;
            if (inNode->parent == NULL) { //node is root
                root = pNode;
            }
            else if (inNode == inNode->parent->right) { //node is right child
                inNode->parent->right = pNode;
            }
            else { //node is left child
                inNode->parent->left = pNode;
            }
            pNode->right = inNode;
            inNode->parent = pNode;

            inNode->height = 1 + max((inNode->left == NULL) ? 0 : inNode->left->height, (inNode->right == NULL) ? 0 : inNode->right->height);
            inNode->balance = (((inNode->left == NULL) ? 0 : inNode->left->height) - ((inNode->right == NULL) ? 0 : inNode->right->height));
            pNode->height = 1 + max((pNode->left == NULL) ? 0 : pNode->left->height, (pNode->right == NULL) ? 0 : pNode->right->height);
            pNode->balance = (((pNode->left == NULL) ? 0 : pNode->left->height) - ((pNode->right == NULL) ? 0 : pNode->right->height));
        }
        void insert(Node* inNode) {  //insert node
            Node* pNode = NULL;
            Node* tempNode = root;
            while (tempNode != NULL) { //find parent of node 
                pNode = tempNode;
                if (inNode->entry->key < tempNode->entry->key) tempNode = tempNode->left;
                else  tempNode = tempNode->right;
            }

            inNode->parent = pNode;

            if (pNode == NULL) { // root = NULL
                root = inNode;
            }
            else if (inNode->entry->key < pNode->entry->key)  pNode->left = inNode; //add node 
            else   pNode->right = inNode;

            Node* xNode = inNode;

            while (pNode != NULL) { //balance from pNode to root
                pNode->height = 1 + max((pNode->left == NULL) ? 0 : pNode->left->height, (pNode->right == NULL) ? 0 : pNode->right->height);
                pNode->balance = (((pNode->left == NULL) ? 0 : pNode->left->height) - ((pNode->right == NULL) ? 0 : pNode->right->height));
                Node* yNode = pNode->parent;
                int test = (yNode == NULL) ? 0 : (((yNode->left == NULL) ? 0 : yNode->left->height) - ((yNode->right == NULL) ? 0 : yNode->right->height));
                if (test <= -2 || test >= 2) {//grandparent is unbalanced
                    if (pNode == yNode->left) {  //pNode is left child
                        if (xNode == yNode->left->left) // Insertion into left subtree of left child of x
                            rightRotate(yNode);

                        else if (xNode == yNode->left->right) {// Insertion into right subtree of left child of x
                            leftRotate(pNode);
                            rightRotate(yNode);
                        }
                    }
                    else if (pNode == yNode->right) {
                        if (xNode == yNode->right->right) // Insertion into right subtree of right child of x
                            leftRotate(yNode);

                        else if (xNode == yNode->right->left) {// Insertion into left subtree of right child of x
                            rightRotate(pNode);
                            leftRotate(yNode);
                        }
                    }
                    break;
                }
                pNode = pNode->parent;
                xNode = xNode->parent;
            }

        }
        void changeNode(Node* node_1, Node* node_2) {   //swap node 
            if (node_1->parent == NULL) { //node_1 is root
                root = node_2;
            }
            else if (node_1 == node_1->parent->left) { //node_1 is left child
                node_1->parent->left = node_2;
                node_1->parent->height = 1 + max((node_1->parent->left == NULL) ? 0 : node_1->parent->left->height, (node_1->parent->right == NULL) ? 0 : node_1->parent->right->height);
                node_1->parent->balance = (((node_1->parent->left == NULL) ? 0 : node_1->parent->left->height) - ((node_1->parent->right == NULL) ? 0 : node_1->parent->right->height));
            }

            else { //node_1 is right child
                node_1->parent->right = node_2;
                node_1->parent->height = 1 + max((node_1->parent->left == NULL) ? 0 : node_1->parent->left->height, (node_1->parent->right == NULL) ? 0 : node_1->parent->right->height);
                node_1->parent->balance = (((node_1->parent->left == NULL) ? 0 : node_1->parent->left->height) - ((node_1->parent->right == NULL) ? 0 : node_1->parent->right->height));

            }
            if (node_2 != NULL)
                node_2->parent = node_1->parent;
        }
        void balanceTree(Node* inNode) {  // balance tree after delete node
            Node* pNode = inNode;

            while (pNode != NULL) {
                pNode->height = 1 + max((pNode->left == NULL) ? 0 : pNode->left->height, (pNode->right == NULL) ? 0 : pNode->right->height);
                pNode->balance = (((pNode->left == NULL) ? 0 : pNode->left->height) - ((pNode->right == NULL) ? 0 : pNode->right->height));
                int check = pNode->balance;
                if (check <= -2 || check >= 2) { //grandparent is unbalanced
                    Node* a, * b, * c;
                    a = pNode;
                    if (a->left == NULL || a->right == NULL) {
                        if (a->left == NULL && a->right != NULL) {//left rotate
                            if (a->balance == -2 && a->right->right == NULL) {   //  2-5-4 
                                
                                    rightRotate(a->right);
                                    leftRotate(a);
                                
                            }
                            else leftRotate(a);
                        }
                        else if (a->right == NULL && a->left != NULL) { // right rotate
                            if (a->balance == 2 && a->left->left == NULL) {
                                
                                    leftRotate(a->left);
                                    rightRotate(a);
                                
                            }
                            else rightRotate(a);
                        }
                    }
                    else {
                        //taller child of a will be b
                        if (a->left->height > a->right->height) b = a->left;
                        else b = a->right;

                        //taller child of b will be c
                        if (b->left->height > b->right->height)  c = b->left;
                        else if (b->left->height < b->right->height) {
                            c = b->right;
                        }
                        else { //same height, go for single rotation
                            if (b == a->left)  c = b->left;
                            else  c = b->right;
                        }
                        if (b == a->left) {
                            if (c == a->left->left)    rightRotate(a);
                            else if (c == a->left->right) {
                                leftRotate(b);
                                rightRotate(a);
                            }
                        }
                        else if (b == a->right) {
                            if (c == a->right->right)     leftRotate(a);

                            else if (c == a->right->left) {
                                rightRotate(b);
                                leftRotate(a);
                            }
                        }
                    }
                }
                pNode = pNode->parent;
            }
        }
        void removeNode(Node* inNode) {
            if (inNode == root && root->left == NULL && root->right == NULL) {
                delete inNode;
                root = NULL;
            }
            else {
                if (inNode->left == NULL) {
                    changeNode(inNode, inNode->right);
                    balanceTree(inNode->parent);
                    delete inNode;
                    inNode = NULL;
                }
                else if (inNode->right == NULL) {
                    changeNode(inNode, inNode->left);
                    balanceTree(inNode->parent);
                    delete inNode;
                    inNode = NULL;
                }
                else {
                    Node* maxNode = maximum(inNode->left); //minimum element in right subtree
                    Node* parentmaxNode = maxNode->parent;
                    int testParent = 0; // if (maxNode->parent != inNode) test++
                    if (maxNode->parent != inNode) {
                        changeNode(maxNode, maxNode->left);
                        maxNode->left = inNode->left;
                        maxNode->left->parent = maxNode;
                        testParent++;
                    }

                    changeNode(inNode, maxNode);    // doi cho maxNode voi node can xoa
                    maxNode->right = inNode->right;
                    maxNode->right->parent = maxNode;
                    maxNode->height = 1 + max((maxNode->left == NULL) ? 0 : maxNode->left->height, (maxNode->right == NULL) ? 0 : maxNode->right->height);
                    maxNode->balance = (((maxNode->left == NULL) ? 0 : maxNode->left->height) - ((maxNode->right == NULL) ? 0 : maxNode->right->height));

                    if (parentmaxNode != NULL) {
                        if (testParent > 0) balanceTree(parentmaxNode);
                        else balanceTree(maxNode);
                    }
                    delete inNode;
                    inNode = NULL;
                }
            }
        }
        Node* searchNode(Node* inNode, K key){
            
            if (inNode == NULL) return NULL;
            if (inNode->entry->key == key) {

                return inNode;
            }
            else if (inNode->entry->key < key) {

                return searchNode(inNode->right, key);
            }
            else if (inNode->entry->key > key) {

                return searchNode(inNode->left, key);
            }

            else return NULL;
        }
        Node* searchKey(K key) {  //return Node has key in the tree
            
            return searchNode(root, key);
        }
        Node* searchTraver(Node* inNode, K key, vector<K>& traversedList) {  // BKUTree search
            
            if (inNode == NULL) return NULL;
            if (traversedList.size() > 0 && inNode->entry->key == traversedList.front()) return NULL;  // go through node r'
           // if (node==root) traversedList.push_back(node->entry->key);   //node = avl->root
            //if (node->corr->parent==NULL) traversedList.push_back(node->entry->key); // node = splay->root
            if (inNode->entry->key == key) {
                return inNode;
            }
            else if (inNode->entry->key < key) {
                traversedList.push_back(inNode->entry->key);
                return searchTraver(inNode->right, key, traversedList);
            }
            else if (inNode->entry->key > key) {
                traversedList.push_back(inNode->entry->key);
                return searchTraver(inNode->left, key, traversedList);
            }

            else return NULL;
        }
        void preOrder(Node* inNode, void (*func)(K key, V value))
        {

            if (inNode != NULL)
            {
                (*func)(inNode->entry->key,inNode->entry->value);
                preOrder(inNode->left,func);
                preOrder(inNode->right,func);
            }
        }       
    };
};

template <class K, class V>
void BKUTree<K, V>::add(K key, V value) {
    if (avl->searchKey(key) != NULL) throw ("Duplicate key"); //Duplicate key
    if (keys.size() < maxNumOfKeys) {
        keys.push(key);
    }
    else if (keys.size() == maxNumOfKeys) { // full queue
        keys.pop();
        keys.push(key);
    }
    typename AVLTree::Node* pNodeAVL = avl->newNode(key, value);
    typename SplayTree::Node* pNodeSplay = splay->newNode(key, value);
    pNodeAVL->corr = pNodeSplay; // the address of the corresponding node on the Splay Tree
    pNodeSplay->corr = pNodeAVL; //the address of the corresponding node on the AVL Tree
    avl->support(pNodeAVL);  //add node in the AVLTree 
    splay->support(pNodeSplay);  //add node in the SplayTree
}
template <class K, class V>
void BKUTree<K, V>::remove(K key) {
    if (splay->root == NULL && avl->root == NULL) throw "Not found";
    typename AVLTree::Node* pNodeAVL = avl->searchKey(key);

    if (pNodeAVL == NULL) throw ("Not found");
    typename SplayTree::Node* pNodeSplay = pNodeAVL->corr;
    queue<K> test = keys;
    int count = 0;
    while (!test.empty()) { //check key exist ?
        if (test.front() == key) count++;
        test.pop();

    }
    if (count > 0) { //key exist
        int size = keys.size();
        while (size > 0) { // delete key out of queue
            if (keys.front() != key) {
                test.push(keys.front());
                keys.pop();
                keys.push(test.front());
                test.pop();

            }
            else keys.pop();
            size--;
        }
    }
    avl->removeNode(pNodeAVL);

    splay->splayRotate(pNodeSplay);
    splay->removeNode(pNodeSplay);
    
    if (count > 0 && splay->root != NULL) {
        if (keys.size() < maxNumOfKeys) keys.push(splay->root->entry->key);  //add key of new node at node deleted
        else if (keys.size() == maxNumOfKeys) {
            keys.pop();
            keys.push(splay->root->entry->key);
        }
    }
}
template <class K, class V>
bool BKUTree<K, V>::searchNode(K key, vector<K>& traversedList, V& returnvalue) {
    if (key == splay->root->entry->key) { // the key is at the root of the Splay Tree
        returnvalue = splay->root->entry->value;

        return true;
    }
    queue<K> test = keys;
    int count = 0;
    while (!test.empty())
    {
        if (test.front() == key) count++;
        test.pop();

    }
    if (count > 0) {  // the key is in a queue
        typename SplayTree::Node* pNodeSplay = splay->searchTraver(splay->root, key, traversedList);
        returnvalue = pNodeSplay->entry->value;
        splay->splayRotate_1(pNodeSplay);
        return true;
    }
    else if (count == 0) { // the key is not in a queue
        if (splay->root == NULL) return false;
        else {
            typename AVLTree::Node* pNodeAVL = avl->searchTraver(splay->root->corr, key, traversedList); //Search on the AVL sub-tree 
            if (pNodeAVL == NULL) {
                typename AVLTree::Node* pNodeAVL1 = avl->searchTraver(avl->root, key, traversedList);   // If not found,  search from the root of the AVL tree total
                if (pNodeAVL1 == NULL) return false; //if not found
                else {
                    returnvalue = pNodeAVL1->entry->value;
                    splay->splayRotate_1(pNodeAVL1->corr);
                    return true;
                }
            }
            else {
                returnvalue = pNodeAVL->entry->value;
                splay->splayRotate_1(pNodeAVL->corr);
                return true;
            }
        }
    }
    return false;
}
template <class K, class V>
V BKUTree<K, V>::search(K key, vector<K>& traversedList) {
    if (splay->root == NULL && avl->root==NULL) throw ("Not found");
    V returnvalue;
    bool  test = searchNode(key, traversedList, returnvalue);
    if (test == true) {
        if (keys.size() < maxNumOfKeys) keys.push(key);  //add key of new node at node deleted
        else if (keys.size() == maxNumOfKeys) {
            keys.pop();
            keys.push(key);
        }
    }
    else  throw ("Not found");
    return returnvalue;
}
template <class K, class V>
void BKUTree<K, V>::traverseNLROnAVL(void (*func)(K key, V value)) {
        avl->traverseNLR(func);
}
template <class K, class V>
void BKUTree<K, V>::traverseNLROnSplay(void (*func)(K key, V value)) {
    splay->traverseNLR(func);
}
template <class K, class V>
void BKUTree<K, V>::clear() {
    avl->clear();
    splay->clear();
    avl = NULL;
    splay = NULL;
}

int main() {
    BKUTree<int, int>* tree = new BKUTree<int, int>();
    int keys[] = {1, 3, 5, 7, 9, 2, 4};
    for (int i = 0; i < 7; i++) tree->add(keys[i], keys[i]);
    tree->traverseNLROnAVL(printKey);
}