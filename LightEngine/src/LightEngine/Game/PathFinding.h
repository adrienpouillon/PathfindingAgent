#pragma once
#include <queue>
#include "Node.h"
#include "Compare.h"

class Cursor;

template<typename T>
class PathFinding
{
public://protected:
	Cursor* mCursor;
	bool mFinish;
	std::priority_queue<Node<T>*, std::vector<Node<T>*>, CompareASTAR<T>> mQueue;
	std::vector<Node<T>*> mPath;
	Node<T>* mStartNode;
	Node<T>* mEndNode;
public:
	PathFinding() { Start(); }

	void Start() { SetCursor(nullptr); SetPathFinish(true); SetStartNode(nullptr); SetEndNode(nullptr); }

	void Update();

	void InitQueue();
	void ResetQueue();

	void PathBegin(Node<T>* startNode, Node<T>* endNode);
	void FindPath();
	void FindPathWithDebug();
	void CreatePath();


	void SetCursor(Cursor* cursor) { mCursor = cursor; }
	Cursor* GetCursor() { return mCursor; }

	void SetPathFinish(bool finish);
	bool GetPathFinish();

	void SetPath(std::vector<Node<T>*> path) { mPath = path; }
	std::vector<Node<T>*>* GetPath() { return &mPath; }

	void SetStartNode(Node<T>* startNode) { mStartNode = startNode; }
	Node<T>* GetStartNode() { return mStartNode; }

	void SetEndNode(Node<T>* endNode) { mEndNode = endNode; }
	Node<T>* GetEndNode() { return mEndNode; }

	~PathFinding();
};

#include "PathFinding.inl"