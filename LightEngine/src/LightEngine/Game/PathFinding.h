#pragma once
#include <queue>
#include "Cursor.h"
#include "Node.h"
#include "Compare.h"

template<typename T>
class PathFinding
{
protected:
	Cursor* mCursor;
	bool mFinish;
	std::priority_queue<Node<T>*, std::vector<Node<T>*>, CompareASTAR<T>> mQueue;
	std::vector<Node<T>*> mPath;
	T* mStartNode;
	T* mEndNode;
public:
	PathFinding() { Start(); }

	void Start() { SetCursor(nullptr); SetPathFinish(false); SetStartNode(nullptr); SetEndNode(nullptr); }

	void Update();

	void InitQueue();

	void PathBegin();
	void FindPath();
	void CreatePath();


	void SetCursor(Cursor* cursor) { mCursor = cursor; }
	Cursor* GetCursor() { return mCursor; }

	void SetPathFinish(bool finish);
	bool GetPathFinish();

	void SetPath(std::vector<Node<T>*> path) { mPath = path; }
	std::vector<Node<T>*>* GetPath() { return &mPath; }

	void SetStartNode(bool startNode) { mStartNode = startNode; }
	bool GetStartNode() { return mStartNode; }

	void SetEndNode(bool endNode) { mEndNode = endNode; }
	bool GetEndNode() { return mEndNode; }

	~PathFinding();
};

#include "PathFinding.inl"