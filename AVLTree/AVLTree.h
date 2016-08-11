#pragma once
#include <iostream>
#include <math.h>
using namespace std;

template<class K,class V>
struct AVLTreeNode
{
	K _key;
	V _value;
	AVLTreeNode<K, V> *_left;
	AVLTreeNode<K, V> *_right;
	AVLTreeNode<K, V> *_parent;
	int _bf; //平衡因子
	AVLTreeNode(const K&key, const V&value)
		:_key(key)
		, _value(value)
		, _left(NULL)
		, _right(NULL)
		, _bf(0)
	{}
};
template<class K, class V>
class AVLTree
{
	typedef AVLTreeNode<K, V> Node;
public:
	AVLTree()
		:_root(NULL)
	{}
	bool Insert(const K&key, const V&value)
	{
		//1.先直接插入
		if (_root == NULL)
		{
			Node *tmp = new Node(key, value);
			_root = tmp;
			_root->_parent = NULL;
			return true;
		}
		else
		{
			Node *cur = _root;
			Node *parent = NULL;
			while (cur)
			{
				if (cur->_key < key)
				{
					parent = cur;
					cur = cur->_right;
				}
				else if (cur->_key>key)
				{
					parent = cur;
					cur = cur->_left;
				}
				else
				{
					return false;
				}
			}
			if (parent->_key > key)
			{
				Node *tmp = new Node(key, value);
				parent->_left = tmp;
				tmp->_parent = parent;
				cur = tmp;
			}
			else
			{
				Node *tmp = new Node(key, value);
				parent->_right = tmp;
				tmp->_parent = parent;
				cur = tmp;
			}
			//2.修改平衡因子
			while (parent!=NULL)
			{
				if (cur == parent->_left)
				{
					parent->_bf--;
				}
				if (cur==parent->_right)
				{
					parent->_bf++;
				}
				if (parent->_bf == 1 || parent->_bf == -1)//当parent的平衡因子等于1/-1时对上层祖先路径有影响
				{
					cur = parent;
					parent = cur->_parent;
				}
				else if (parent->_bf == 0)//当平衡因子等于0时，对祖先路径没有影响
				{
					break;
				}
				else//当平衡因子绝对值大于1，说明需要旋转
				{
					//1.右单旋
					if (parent->_bf == -2 && cur->_bf == -1)
					{
						RotateR(parent);
						break;
						//parent = parent->_parent;
					}
					else if (parent->_bf == 2 && cur->_bf == 1)//2.左单旋
					{
						RotateL(parent);
						break;
						//parent = parent->_parent;
					}
					else if (parent->_bf == -2 && cur->_bf == 1)//3.左右双旋
					{
						RotateLR(parent);
						break;
						//parent = parent->_parent;
					}
					else//4.右左双旋
					{
						RotateRL(parent);
						break;
						//parent = parent->_parent;
					}
				}

			}
		}
	}
	
	//bool Remove(const K&key)
	//{
	//	if (_root == NULL)
	//	{
	//		return false;
	//	}
	//	else
	//	{
	//		Node *tmp = NULL;
	//		Node *cur = _root;
	//		Node *parent = NULL;
	//		while (cur)
	//		{
	//			if (cur->_key > key)
	//			{
	//				parent = cur;
	//				cur = cur->_left;
	//			}
	//			else if (cur->_key < key)
	//			{
	//				parnet = cur;
	//				cur = cur->_right;
	//			}
	//			else
	//			{
	//				break;
	//			}
	//		}
	//		if (cur == NULL)
	//		{
	//			return false;
	//		}
	//		//找到节点删除
	//		//删除，1.左孩子或者右孩子为空
	//		//2.左右孩子都不为空

	//		//先删除；
	//		if (cur->_left == NULL) //左为空
	//		{
	//			if (parent == NULL)  //当删除根节点时，那么它的左子树为空，所以要更改根节点
	//			{
	//				_root = cur->_right;
	//			}
	//			else
	//			{
	//				if (parent->_left == cur)
	//				{
	//					parent->_left = cur->_right;
	//				}
	//				else
	//				{
	//					parent->_right = cur->_right;
	//				}
	//				tmp = cur->_right;
	//			}

	//			delete cur;
	//		}
	//		else if (cur->_right == NULL)//右为空
	//		{
	//			if (parent == NULL)  //当删除根节点时，那么它的右子树为空，所以要更改根节点
	//			{
	//				_root = cur->_left;
	//			}
	//			else
	//			{
	//				if (parent->_left == cur)
	//				{
	//					parent->_left = cur->_left;
	//				}
	//				else
	//				{
	//					parent->_right = cur->_left;
	//				}
	//				tmp = cur->_left;
	//			}
	//			delete cur;
	//		}
	//		else //左右都不为空
	//		{
	//			Node *parent = cur;
	//			Node *subLeft = cur->_right;
	//			while (subLeft->_left)  //在cur之后找中序遍历的下一个节点
	//			{
	//				parent = subLeft;
	//				subLeft = subLeft->_left;
	//			}
	//			cur->_key = subLeft->_key;
	//			cur->_value = subLeft->_value;

	//			if (parent->_right == subLeft) //subleft的位置不同，则修改的情况也不同
	//			{
	//				parent->_right = subLeft->_right;
	//			}
	//			else
	//			{
	//				parent->_left = subLeft->_right;
	//			}
	//			tmp = subLeft->_right;
	//			delete subLeft;
	//		}
	//		
	//	}
	//}
	void InOrder()
	{
		_InOrder(_root);
	}
	int Hight(Node *root)
	{
		if (root == NULL)
		{
			return 0;
		}
		int leftH = Hight(root->_left);
		int rightH = Hight(root->_right);
		return (leftH > rightH ? leftH : rightH) + 1;
	}
	Node* retRoot()
	{
		return _root;
	}
	bool IsBalance(Node *root)
	{
		if (root == NULL)
		{
			return true;
		}
		int leftH = Hight(root->_left);
		int rightH = Hight(root->_right);
		if ((rightH - leftH) !=root->_bf)
		{
			cout << root->_key << ":" << root->_bf << endl;
			return false;
		}
		return IsBalance(root->_left) && IsBalance(root->_right);
	}
	Node *Find(const K&key)
	{
		return _Find(_root, key);
	}
protected:
	Node * _Find(Node* root, const K&key)
	{
		if (root == NULL)
		{
			return NULL;
		}
		if (root->_key == key)
		{
			return root;
		}
		_Find(root->_left, key);
		_Find(root->_right, key);
	}
	void RotateR(Node *parent)
	{
		Node *subL = parent->_left;
		Node *subLR = subL->_right;
		parent->_left = subLR;
		if (subLR)
		{
			subLR->_parent = parent;
		}
		subL->_right = parent;
		Node *ppNode = parent->_parent;

		parent->_parent = subL;

		if (ppNode == NULL)
		{
			_root = subL;
			subL->_parent = NULL;
		}
		else
		{
			if (ppNode->_left == parent)
			{
				ppNode->_left = subL;
				subL->_parent = ppNode;
			}
			else
			{
				ppNode->_right = subL;
				subL->_parent = ppNode;
			}
		}
		parent->_bf = subL->_bf = 0;
	}
	void RotateL(Node *parent)
	{
		Node *subR = parent->_right;
		Node *subRL = subR->_left;
		parent->_right = subRL;
		if (subRL)
		{
			subRL->_parent = parent;
		}
		subR->_left = parent;
		Node *ppNode = parent->_parent;
		parent->_parent = subR;
		if (ppNode == NULL)
		{
			_root = subR;
			subR->_parent = NULL;
		}
		else
		{
			if (ppNode->_left == parent)
			{
				ppNode->_left = subR;
				subR->_parent = ppNode;
			}
			else
			{
				ppNode->_right = subR;
				subR->_parent = ppNode;
			}
		}
		parent->_bf = subR->_bf = 0;
	}
	void RotateRL(Node *parent)//右左双旋
	{
		Node *subR = parent->_right;
		Node *subRL = subR->_left;
		int bf = subRL->_bf;

		RotateR(parent->_right);
		RotateL(parent);
		if (bf == 1)
		{
			parent->_bf = -1;
		}
		else if (bf==-1)
		{
			subR->_bf = 1;
		}
		else
		{
			subRL->_bf = 0;
		}
	}
	void RotateLR(Node *parent)
	{
		Node *subL = parent->_left;
		Node *subLR = subL->_right;
		int bf = subLR->_bf;

		RotateL(parent->_left);
		RotateR(parent);
		if (bf==-1)
		{
			subL->_bf = 1;
		}
		else if (bf==-1)
		{
			parent->_bf = -1;
		}
		else
		{
			;
		}
	}
	void _InOrder(Node *root)
	{
		if (root == NULL)
		{
			return;
		}
		_InOrder(root->_left);
		cout << root->_key << " ";
		_InOrder(root->_right);
	}

private:
	Node *_root;
};

void test()
{
	int arr[] = { 4, 2, 6, 1, 3, 5, 15, 8, 16, 7 };
	int len = sizeof(arr) / sizeof(arr[0]);
	AVLTree<int, int> avl;
	for (int i = 0; i < len; i++)
	{
		avl.Insert(arr[i], i);
	}
	avl.InOrder();
	cout << endl;
	AVLTreeNode<int, int>* root = avl.retRoot();
	bool result = avl.IsBalance(root);
	if (result == true)
	{
		cout << "平衡！！！" << endl;
	}
	else
	{
		cout << "不平衡" << endl;
	}
	AVLTreeNode<int, int>* ret = avl.Find(4); 
	if (ret == NULL)
	{
		cout << "不存在" << endl;
	}
	else
	{
		cout << ret->_key;
	}
	cout << endl;
	
}