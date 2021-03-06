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

	template<typename Key, typename Value, class Compare = less<Key>>
	class rbTree;

	template <typename Key, typename Value>
	struct rbNode
	{
		friend class rbTree<Key, Value>;

	private:
		typedef Value        value_type;
		typedef Value*       pointer;
		typedef const Value* const_pointer;
		typedef Value&       reference;
		typedef const Value& const_reference;

		enum Color
		{
			RED = true,
			BLACK = false
		};

	private:
		Key					key;
		Value				value;

		bool				color;

		rbNode<Key, Value>*	left;
		rbNode<Key, Value>*	right;
		rbNode<Key, Value>*	parent;

	public:
		rbNode<Key, Value>()
			: key(0), value(0), left(nullptr), right(nullptr), parent(nullptr), color(false)
		{}
		rbNode<Key, Value>(Key& _key, Value& _value)
			: key(_key), value(_value), left(nullptr), right(nullptr), parent(nullptr), color(false)
		{}
		~rbNode<Key, Value>() = default;

	public:
		bool operator <  (const_reference node) { return this->key <  node.key; }
		bool operator <= (const_reference node) { return this->key <= node.key; }
		bool operator >	 (const_reference node) { return this->key >  node.key; }
		bool operator >= (const_reference node) { return this->key >= node.key; }

	public:
		Key GetKey() const
		{
			return key;
		}
		Value GetValue() const
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
		node_pointer pRoot;
		node_pointer pNull;

		int iSize;

	private:
		Compare comp;

	public:
		rbTree<Key, Value, Compare>()
			: pRoot(nullptr), pNull(new rbNode<Key, Value>()), comp(Compare()), iSize(0)
		{}
		~rbTree<Key, Value, Compare>()
		{
			if (nullptr != pRoot)
				delete pRoot;
			if (nullptr != pNull)
				delete pNull;
		}

	public:
		bool empty() const noexcept;
		int size() const noexcept;
	
		node_pointer insert(node_reference _rNode);
		node_pointer insert(Key& _key, Value& _value);
		node_pointer insert(Key&& _key, Value&& _value);

		void erase(Key& _key);
		void erase(Key&& _key);

		void clear();
		Value find(Key& _key);

	private:
		node_pointer grandparent(node_pointer _pNode);
		node_pointer uncle(node_pointer _pNode); 
		node_pointer sibling(node_pointer _pNode);

		void rotateLeft(node_pointer _pNode);
		void rotateRight(node_pointer _pNode);
		void replace_node(node_pointer _pNode, node_pointer _pChild);

		void insert_recurse(node_pointer _pRoot, node_pointer _pNode);
		void insert_repair_tree(node_pointer _pNode);
		void insert_case1(node_pointer _pNode); // if root
		void insert_case2(node_pointer _pNode); // if parent == black
		void insert_case3(node_pointer _pNode); // if parent == red and uncle == red
		void insert_case4(node_pointer _pNode); // if parent == red and uncle == black and i'm left child // need rotation
		void insert_case5(node_pointer _pNode); // if parent == red and uncle == black and i'm right child // need rotation

		void remove(node_pointer _pNode);
		void remove_case1(node_pointer _pNode);
		void remove_case2(node_pointer _pNode);
		void remove_case3(node_pointer _pNode);
		void remove_case4(node_pointer _pNode);
		void remove_case5(node_pointer _pNode);
		void remove_case6(node_pointer _pNode);
		
		node_pointer find_node(Key& _key);

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
	inline rbNode<Key, Value>* rbTree<Key, Value, Compare>::insert(node_reference _rNode)
	{
		(&_rNode)->left = pNull;
		(&_rNode)->right = pNull;
		if (pRoot == nullptr)
		{
			pRoot = &_rNode;
			pRoot->color = BLACK;
			return pRoot;
		}

		insert_recurse(pRoot, &_rNode);

		insert_repair_tree(&_rNode);

		pRoot = &_rNode;
		while (pRoot->parent != nullptr)
			pRoot = pRoot->parent;
		return pRoot;
	}

	template<typename Key, typename Value, class Compare>
	inline rbNode<Key, Value>* rbTree<Key, Value, Compare>::insert(Key& _key, Value& _value)
	{
		node_pointer pNode = new rbNode<Key, Value>(_key, _value);
		pNode->left = pNull;
		pNode->right = pNull;
		if (pRoot == nullptr)
		{
			pRoot = pNode;
			pRoot->color = BLACK;
			return pRoot;
		}
		insert_recurse(pRoot, pNode);

		insert_repair_tree(pNode);

		pRoot = pNode;
		while (pRoot->parent != nullptr)
			pRoot = pRoot->parent;
		return pRoot;
	}

	template<typename Key, typename Value, class Compare>
	inline rbNode<Key, Value>* rbTree<Key, Value, Compare>::insert(Key && _key, Value && _value)
	{
		node_pointer pNode = new rbNode<Key, Value>(_key, _value);
		pNode->left = pNull;
		pNode->right = pNull;
		if (pRoot == nullptr)
		{
			pRoot = pNode;
			pRoot->color = BLACK;
			return pRoot;
		}
		insert_recurse(pRoot, pNode);

		insert_repair_tree(pNode);

		pRoot = pNode;
		while (pRoot->parent != nullptr)
			pRoot = pRoot->parent;
		return pRoot;
	}

	template<typename Key, typename Value, class Compare>
	inline void rbTree<Key, Value, Compare>::erase(Key& _key)
	{
		node_pointer pRet = find_node(_key);
		if (pRet == pNull)
			return;
		remove(pRet);
	}

	template<typename Key, typename Value, class Compare>
	inline void rbTree<Key, Value, Compare>::erase(Key && _key)
	{
		node_pointer pRet = find_node(_key);
		if (pRet == pNull)
			return;
		remove(pRet);
	}

	template<typename Key, typename Value, class Compare>
	inline void rbTree<Key, Value, Compare>::clear()
	{
	}

	template<typename Key, typename Value, class Compare>
	inline Value rbTree<Key, Value, Compare>::find(Key& _key)
	{
		node_pointer pRet = find_node(_key);
		if (pRet == pNull)
			return nullptr;
		return pRet->value;
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
	inline rbNode<Key, Value>* rbTree<Key, Value, Compare>::sibling(node_pointer _pNode)
	{
		if (_pNode == _pNode->parent->left)
			return _pNode->parent->right;
		else
			return _pNode->parent->left;
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
	inline void rbTree<Key, Value, Compare>::replace_node(node_pointer _pNode, node_pointer _pChild)
	{
		_pChild->parent = _pNode->parent;
		if (_pNode == _pNode->parent->left)
			_pNode->parent->left = _pChild;
		else
			_pNode->parent->right = _pChild;
	}

	template<typename Key, typename Value, class Compare>
	inline void rbTree<Key, Value, Compare>::insert_recurse(node_pointer _pRoot, node_pointer _pNode)
	{
		if (_pRoot != nullptr && _pNode->key < _pRoot->key) {
			if (_pRoot->left != pNull) {
				insert_recurse(_pRoot->left, _pNode);
				return;
			}
			else
				_pRoot->left = _pNode;
		}
		else if (_pRoot != nullptr) {
			if (_pRoot->right != pNull) {
				insert_recurse(_pRoot->right, _pNode);
				return;
			}
			else
				_pRoot->right = _pNode;
		}

		_pNode->parent = _pRoot;
		_pNode->left = pNull;
		_pNode->right = pNull;
		_pNode->color = RED;
	}

	template<typename Key, typename Value, class Compare>
	inline void rbTree<Key, Value, Compare>::insert_repair_tree(node_pointer _pNode)
	{
		if (_pNode->parent == nullptr) 
		{
			insert_case1(_pNode);
		}
		else if (_pNode->parent->color == BLACK) 
		{
			insert_case2(_pNode);
		}
		else if (uncle(_pNode)->color == RED) 
		{
			insert_case3(_pNode);
		}
		else 
		{
			insert_case4(_pNode);
		}
	}

	template<typename Key, typename Value, class Compare>
	inline void rbTree<Key, Value, Compare>::insert_case1(node_pointer _pNode)
	{
		if (_pNode->parent == nullptr)
		{
			_pNode->color = BLACK;
		}
	}

	template<typename Key, typename Value, class Compare>
	inline void rbTree<Key, Value, Compare>::insert_case2(node_pointer _pNode) 
	{
		return; // valied
	}

	template<typename Key, typename Value, class Compare>
	inline void rbTree<Key, Value, Compare>::insert_case3(node_pointer _pNode)
	{
		node_pointer pUNode = uncle(_pNode), gPNode = grandparent(_pNode);

		_pNode->parent->color = BLACK;
		pUNode->color = BLACK;
		gPNode->color = RED;
		insert_repair_tree(gPNode);
	}

	template<typename Key, typename Value, class Compare>
	inline void rbTree<Key, Value, Compare>::insert_case4(node_pointer _pNode)
	{
		node_pointer pGNode = grandparent(_pNode);
		if (_pNode == pGNode->left->right) // parent's right child, parent is grandparent's left child
		{
			rotateLeft(_pNode->parent);
			_pNode = _pNode->left;// rotated node(parent -> child)
		}
		else if (_pNode == pGNode->right->left) // parent's left child, parent is grandparent's right child
		{
			rotateRight(_pNode->parent);
			_pNode = _pNode->right;// rotated node(parent -> child)
		}
		insert_case5(_pNode);
	}

	template<typename Key, typename Value, class Compare>
	inline void rbTree<Key, Value, Compare>::insert_case5(node_pointer _pNode)
	{
		node_pointer pGNode = grandparent(_pNode);

		if (_pNode == _pNode->parent->left)
			rotateRight(pGNode);
		else
			rotateLeft(pGNode);

		_pNode->parent->color = BLACK;
		pGNode->color = RED;
	}

	template<typename Key, typename Value, class Compare>
	inline void rbTree<Key, Value, Compare>::remove(node_pointer _pNode)
	{
		if (_pNode == pNull)
			return;
		node_pointer pChild = pNull == _pNode->right ? _pNode->left : _pNode->right;

		replace_node(_pNode, child);

		if (_pNode->color == BLACK)
		{
			if (pChild->color == RED)
			{
				pChild->color = BLACK;
			}
			else
			{
				remove_case1(pChild);
			}
		}
		free(_pNode);
	}

	template<typename Key, typename Value, class Compare>
	inline void rbTree<Key, Value, Compare>::remove_case1(node_pointer _pNode)
	{
		if (_pNode->parent != nullptr)
		{
			remove_case2(_pNode);
		}
	}

	template<typename Key, typename Value, class Compare>
	inline void rbTree<Key, Value, Compare>::remove_case2(node_pointer _pNode)
	{
		node_pointer pSibling = sibling(_pNode);

		if (pSibling->color == RED) {
			_pNode->parent->color = RED;
			pSibling->color = BLACK;
			if (_pNode == _pNode->parent->left)
				rotate_left(_pNode->parent);
			else
				rotate_right(_pNode->parent);
		}
		remove_case3(_pNode);
	}

	template<typename Key, typename Value, class Compare>
	inline void rbTree<Key, Value, Compare>::remove_case3(node_pointer _pNode)
	{
		node_pointer pSibling = sibling(_pNode);

		if ((_pNode->parent->color == BLACK) &&
			(pSibling->color == BLACK) &&
			(pSibling->left->color == BLACK) &&
			(pSibling->right->color == BLACK)) {
			pSibling->color = RED;
			delete_case1(_pNode->parent);
		}
		else
			remove_case4(_pNode);
	}

	template<typename Key, typename Value, class Compare>
	inline void rbTree<Key, Value, Compare>::remove_case4(node_pointer _pNode)
	{
		node_pointer pSibling = sibling(_pNode);

		if ((_pNode->parent->color == RED) &&
			(pSibling->color == BLACK) &&
			(pSibling->left->color == BLACK) &&
			(pSibling->right->color == BLACK)) {
			pSibling->color = RED;
			_pNode->parent->color = BLACK;
		}
		else
			remove_case5(_pNode);
	}

	template<typename Key, typename Value, class Compare>
	inline void rbTree<Key, Value, Compare>::remove_case5(node_pointer _pNode)
	{
		node_pointer pSibling = sibling(_pNode);

		if (pSibling->color == BLACK) {  
			if ((_pNode == _pNode->parent->left) &&
				(pSibling->right->color == BLACK) &&
				(pSibling->left->color == RED)) {  
				pSibling->color = RED;
				pSibling->left->color = BLACK;
				rotate_right(pSibling);
			}
			else if ((_pNode == _pNode->parent->right) &&
				(pSibling->left->color == BLACK) &&
				(pSibling->right->color == RED)) { 
				pSibling->color = RED;
				pSibling->right->color = BLACK;
				rotate_left(pSibling);
			}
		}
		remove_case6(_pNode);
	}

	template<typename Key, typename Value, class Compare>
	inline void rbTree<Key, Value, Compare>::remove_case6(node_pointer _pNode)
	{
		node_pointer pSibling = sibling(_pNode);

		pSibling->color = _pNode->parent->color;
		_pNode->parent->color = BLACK;

		if (_pNode == _pNode->parent->left) {
			pSibling->right->color = BLACK;
			rotate_left(_pNode->parent);
		}
		else {
			pSibling->left->color = BLACK;
			rotate_right(_pNode->parent);
		}
	}

	template<typename Key, typename Value, class Compare>
	inline rbNode<Key, Value>* rbTree<Key, Value, Compare>::find_node(Key& _key)
	{
		node_pointer pRet = pRoot;

		while (true)
		{
			if (pRet == pNull)
			{
				break;
			}
			if (pRet->key == _key)
			{
				break;
			}
			else if (pRet->key > _key)
			{
				pRet = pRet->left;
			}
			else
			{
				pRet = pRet->right;
			}
		}

		return pRet;
	}

}