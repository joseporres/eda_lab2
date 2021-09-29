#ifndef AVLTree_H
#define AVLTree_H
#include "node.h"

using namespace std;

template <typename T>
class AVLTree
{
private:
    NodeBT<T> root;

    void calcHeight(NodeBT<T>&);
    void displayPreOrder(NodeBT<T> *node);
    bool find(NodeBT<T> *node, T value);
    int height(NodeBT<T> *node);
    bool isBalanced(NodeBT<T> *node);
    T minValue(NodeBT<T> *node);
    T maxValue(NodeBT<T> *node);
    int size(NodeBT<T> *node);
    void remove(NodeBT<T> *&node, T value);
    void displayPretty(NodeBT<T> node, int level);

    /add for avl*/
    int balancingFactor(NodeBT<T> *node);
    void insert(NodeBT<T> *&node, T value);
    void balance(NodeBT<T> *&node);
    void left_rota(NodeBT<T> *&node); 
    void right_rota(NodeBT<T> *&node); 
public:
    AVLTree() : root(nullptr) {}

    void insert(T value)
    {
        insert(this->root, value);
    }
    bool find(T value)
    {
        return find(this->root, value);
    }
    void displayPreOrder()
    {
        displayPreOrder(this->root);
    }
    int height()
    {
        return height(this->root);
    }
    T minValue()
    {
        return minValue(this->root);
    }
    T maxValue()
    {
        return maxValue(this->root);
    }
    bool isBalanced()
    {
        return isBalanced(this->root);
    }
    int size()
    {
        return size(this->root);
    }
    void remove(T value)
    {
        remove(this->root, value);
    }
    void displayPretty()
    {
        displayPretty(this->root, 1);
    }
    ~AVLTree(){
        if(this->root != nullptr){
            this->root->killSelf();
        }
    }

};
#endif