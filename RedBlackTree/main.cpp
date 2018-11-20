#include "stdafx.h"
#include <iostream>
#include "RedBlackTree.hpp"
#include <map>

using namespace std;
 
void main()
{
	using namespace LysLibrary;

	rbTree<int, int> *t = new rbTree<int, int>();

	t->insert(1, 0);
	t->insert(2, 0);
	t->insert(3, 0);

	return;
}
