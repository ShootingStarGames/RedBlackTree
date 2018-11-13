#pragma once
#include "stdafx.h"

namespace LysLibrary
{
	template <class T>
	struct less {
		bool operator() (const T& x, const T& y) const { return x<y; }
		typedef T first_argument_type;
		typedef T second_argument_type;
		typedef bool result_type;
	};

	template <typename T1, typename T2>
	struct pair
	{
		typedef T1 first_value;
		typedef T2 second_value;

	public:
		pair(first_value _val1, second_value _val2)
			: first(_va1), second(_val2)
		{}

	public:
		//for operator

	public:
		first_value first;
		second_value second;
	};

	template <typename Key, typename T>
	struct rbNode
	{
	private:
		typedef T        value_type;
		typedef T*       pointer;
		typedef const T* const_pointer;
		typedef T&       reference;
		typedef const T& const_reference;

		enum Color
		{
			RED = true,
			BLACK = false
		};

	private:
		Key					key;
		T					value;

		bool				color;

		rbNode<Key, T>*		left;
		rbNode<Key, T>*		right;
		rbNode<Key, T>*		parent;

	public:
		rbNode(Key& _key, T& _value)
			: key(_key), value(_value), left(nullptr), right(nullptr), parent(nullptr), color(false)
		{}
		~rbNode() = default;

		Key GetKey() const
		{
			return key;
		}
		T GetValue() const
		{
			return value;
		}
	};

	template <typename Key, typename Value, class Compare = less<Key>>
	class rbTree
	{
	private:
		typedef Value			 value_type;
		typedef Value*			 pointer;
		typedef const Value*	 const_pointer;
		typedef Value&			 reference;
		typedef const Value&	 const_reference;

		typedef rbNode<Key, Value>  node;
		typedef rbNode<Key, Value>* node_pointer;
		typedef rbNode<Key, Value>& node_reference;

		enum Color
		{
			RED = true,
			BLACK = false
		};

	private:
		rbNode<Key, Value> *pRoot;
		int iSize;

	private:
		Compare comp;

	public:
		rbTree()
			: pRoot(new rbNode<Key, Value>()), comp(Compare()) , size(0)
		{}
		~rbTree()
		{
			if (nullptr != pRoot)
				delete pRoot;
		}
	public:
		//operator

	public:
		bool empty() const noexcept;
		int size() const noexcept;
	
		rbNode<Key, Value> insert(node_reference _rNode);
		rbNode<Key, Value> insert(Key _key, Value _value);
		void erase(Key _key);
		void clear();
		Value find(Key _key);
		void count();

	private:
		node_pointer grandparent(node_pointer _pNode);
		node_pointer uncle(node_pointer _pNode); 
		void rotateLeft(node_pointer _pNode);
		void rotateRight(node_pointer _pNode);
		void insertcase1(node_pointer _pNode); // if root
		void insertcase2(node_pointer _pNode); // if parent == black
		void insertcase3(node_pointer _pNode); // if parent == red and uncle == red
		void insertcase4(node_pointer _pNode); // if parent == red and uncle == black and i'm left child // need rotation
		void insertcase5(node_pointer _pNode); // if parent == red and uncle == black and i'm right child // need rotation
	};

	template<typename Key, typename Value, class Compare>
	inline bool rbTree<Key, Value, Compare>::empty() const noexcept
	{
		return size() == 0;
	}

	template<typename Key, typename Value, class Compare>
	inline int rbTree<Key, Value, Compare>::size() const noexcept
	{
		return iSize;
	}

	template<typename Key, typename Value, class Compare>
	inline rbNode<Key, Value> rbTree<Key, Value, Compare>::insert(node_reference _rNode)
	{
		if (pRoot == nullptr)
		{
			pRoot = &_rNode;
			pRoot->color = BLACK;
		}
		else
		{

		}
		return _node;
	}

	template<typename Key, typename Value, class Compare>
	inline rbNode<Key, Value> rbTree<Key, Value, Compare>::insert(Key _key, Value _value)
	{
		return rbNode<Key, Value>();
	}

	template<typename Key, typename Value, class Compare>
	inline void rbTree<Key, Value, Compare>::erase(Key _key)
	{
	}

	template<typename Key, typename Value, class Compare>
	inline void rbTree<Key, Value, Compare>::clear()
	{
	}

	template<typename Key, typename Value, class Compare>
	inline Value rbTree<Key, Value, Compare>::find(Key _key)
	{
		return Value();
	}

	template<typename Key, typename Value, class Compare>
	inline void rbTree<Key, Value, Compare>::count()
	{
	}

	template<typename Key, typename Value, class Compare>
	inline rbNode<Key, Value>* rbTree<Key, Value, Compare>::grandparent(node_pointer _pNode)
	{
		if ((_pNode != nullptr) && (_pNode->parent != nullptr))
			return _pNode->parent->parent;
		else
			return nullptr;
	}

	template<typename Key, typename Value, class Compare>
	inline rbNode<Key, Value>* rbTree<Key, Value, Compare>::uncle(node_pointer _pNode)
	{
		node_pointer pGNode = grandparent(_pNode);
		if (pGNode == nullptr)
			return nullptr; // no grandparent means no uncle

		if (_pNode->parent == pGNode->left) // _pNode의 parent가 grandparent의 왼쪽 자식 일 때
			return pGNode->right;
		else
			return pGNode->left;
	}

	template<typename Key, typename Value, class Compare>
	inline void rbTree<Key, Value, Compare>::rotateLeft(node_pointer _pNode)
	{
		node_pointer c = _pNode->right;
		node_pointer p = _pNode->parent;

		if (c->left != nullptr)
			c->left->parent = _pNode;

		_pNode->right = c->left;
		_pNode->parent = c;
		c->left = _pNode;
		c->parent = p;

		if (p != nullptr) {
			if (p->left == _pNode)
				p->left = c;
			else
				p->right = c;
		}
	}

	template<typename Key, typename Value, class Compare>
	inline void rbTree<Key, Value, Compare>::rotateRight(node_pointer _pNode)
	{
		node_pointer c = _pNode->left;
		node_pointer p = _pNode->parent;

		if (c->right != nullptr)
			c->right->parent = _pNode;

		_pNode->left = c->right;
		_pNode->parent = c;
		c->right = _pNode;
		c->parent = p;

		if (p != nullptr) {
			if (p->right == _pNode)
				p->right = c;
			else
				p->left = c;
		}
	}

	template<typename Key, typename Value, class Compare>
	inline void rbTree<Key, Value, Compare>::insertcase1(node_pointer _pNode)
	{
		if (_pNode->parent == nullptr)
		{
			_pNode->color = BLACK;
			pRoot = _pNode;
		}
		else
		{
			insertcase2(_pNode);
		}
	}

	template<typename Key, typename Value, class Compare>
	inline void rbTree<Key, Value, Compare>::insertcase2(node_pointer _pNode) 
	{
		if (_pNode->parent->color == BLACK)
		{
			return; // valied
		}
		else
		{
			insertcase3(_pNode);
		}
	}

	template<typename Key, typename Value, class Compare>
	inline void rbTree<Key, Value, Compare>::insertcase3(node_pointer _pNode)
	{
		node_pointer pUNode = uncle(_pNode), gPNode;

		if ((u != nullptr) && (u->color == RED))
		{
			_pNode->parent->color = BLACK;
			u->color = BLACK;
			gPNode = grandparent(_pNode);
			gPNode->color = RED;
			insertcase1(gPNode);
		}
		else
		{
			insertcase4(_pNode);
		}
	}

	template<typename Key, typename Value, class Compare>
	inline void rbTree<Key, Value, Compare>::insertcase4(node_pointer _pNode)
	{
		node_pointer pGNode = grandparent(_pNode);
		if ((_pNode == _pNode->parent->right) && (_pNode->parent == pGNode->left)) // parent's right child, parent is grandparent's left child
		{
			rotateLeft(_pNode->parent);
			_pNode = _pNode->left;// rotated node(parent -> child)
		}
		else if ((_pNode == _pNode->parent->left) && (_pNode->parent == pGNode->right)) // parent's left child, parent is grandparent's right child
		{
			rotateRight(_pNode->parent);
			_pNode = _pNode->right;// rotated node(parent -> child)
		}
		insertcase5(_pNode);
	}

	template<typename Key, typename Value, class Compare>
	inline void rbTree<Key, Value, Compare>::insertcase5(node_pointer _pNode)
	{
		node_pointer pGNode = grandparent(_pNode);

		_pNode->parent->color = BLACK;
		pGNode->color = RED;
		if (_pNode == _pNode->parent->left)
			rotateRight(pGNode);
		else
			rotateLeft(pGNode);
	}

}