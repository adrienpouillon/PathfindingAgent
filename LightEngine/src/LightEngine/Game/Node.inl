#include "Node.h"

template<typename T>
inline Node<T>* Node<T>::GetNodeInTab(int i, int j, int lenght_i, std::vector<Node<T>*> allNodes)
{
    return allNodes[i * lenght_i + j];
}