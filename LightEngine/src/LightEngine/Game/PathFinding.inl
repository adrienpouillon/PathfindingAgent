#include "PathFinding.h"
#include "../Utils.h"
#include "../Debug.h"
#include "Grid.h"



template<typename T>
inline void PathFinding<T>::Find(Node<T>* startNode, Node<T>* endNode, Grid<T>* grid)
{
    SetReadFinish(false);
    /*if (GetPath() == nullptr)
    {
        SetReadFinish(false);
        SetPath(std::vector<Node<T>*>());
    }*/
    /*std::vector<Node<T>*>* path = */FindPath(startNode, endNode, grid, GetPath());

    //SetPath(*path);
}

template<typename T>
void PathFinding<T>::SetReadFinish(bool readFinish)
{
    mReadFinish = readFinish;
    if (mReadFinish == true)
    {
        SetPath(std::vector<Node<T>*>());
    }
}

template<typename T>
bool PathFinding<T>::GetReadFinish()
{
    return mReadFinish;
}

template<typename T>
inline void PathFinding<T>::SetPath(std::vector<Node<T>*> path)
{
    mPath = path;
}

template<typename T>
inline std::vector<Node<T>*>* PathFinding<T>::GetPath()
{
    if (mReadFinish == false && mPath.size() > 0)
    {
        return &mPath;
    }
    else
    {
        mPath = std::vector<Node<T>*>();
        return &mPath;
    }
}

template<typename T>
PathFinding<T>::~PathFinding()
{

}
