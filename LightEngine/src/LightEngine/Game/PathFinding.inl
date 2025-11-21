#include "PathFinding.h"
#include "../Utils.h"
#include "../Debug.h"
#include "Grid.h"
#include "Agent.h"


template<typename T>
inline void PathFinding<T>::Find(Node<T>* startNode, Node<T>* endNode, Grid* grid, Agent* pOwner)
{
    SetReadFinish(false);
  
    FindPath(startNode, endNode, grid, GetPath(), pOwner);
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
        mPath.clear();

        return &mPath;
    }
}

template<typename T>
PathFinding<T>::~PathFinding()
{
    mReadFinish = true;
    mPath.clear();
}
