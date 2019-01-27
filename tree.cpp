#include <iostream>

#include <string>

#include <windows.h>

#include <fstream>

#include <conio.h>

using namespace std;



void gotoxy(int x, int y)//given code, move cursor to a location

{

	COORD pos = { x, y };

	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleCursorPosition(output, pos);

}

int tx = 0, ty = 8;

class node //node class that makes up the tree

{

public:

	int c;

	node* left, *right, *up;

	node(int n = 0)

	{

		c = n;

		left = nullptr;

		right = nullptr;

		up = nullptr;

	}

};

void print(node* n, int code) //print all the elements of a tree in different order

{

	if (code == 1)

		cout << n->c << " ";

	if (n->left != nullptr)

		print(n->left, code);

	if (code == 0)

		cout << n->c << " ";

	if (n->right != nullptr)

		print(n->right, code);

	if (code == 2)

		cout << n->c << " ";



}

void deleteAll(node* n) //delete all the node in a tree using recursion

{

	if (n->left != nullptr)

		deleteAll(n->left);

	if (n->right != nullptr)

		deleteAll(n->right);

	if (n->up != nullptr)

		delete n;

}

void displayTree(node*n) //display the tree in graphic format

{

	if (n->left != nullptr)

	{

		ty += 2;

		displayTree(n->left);

		gotoxy(++tx, --ty);

		cout << "/";

		tx++; ty--;

	}

	gotoxy(tx, ty);

	cout << n->c;

	if (n->right != nullptr)

	{

		gotoxy(++tx, ++ty);

		cout << "\\";

		tx++; ty++;

		displayTree(n->right);

		ty -= 2;

	}



}

class tree

{

public:

	bool isEmpty; //true if is empty, false if not

	node* root; // address of the root node

	tree()

	{

		isEmpty = true;

		root = new node();

	}

	int findsize(node*n)// return the size of the tree, using recursion

	{

		if (n == nullptr || isEmpty)

			return 0;

		int s = 1;

		if (n->left != nullptr)

			s += findsize(n->left);

		if (n->right != nullptr)

			s += findsize(n->right);

		return s;

	}

	void insert(int n)// insert a node into the tree

	{

		if (isEmpty)

		{

			root->c = n;

			isEmpty = false;

		}

		else

		{

			node *t = root;

			while (true)

				if (n == t->c)

					break; // break if element already exist

				else if (n > t->c) // go left if the element is smaller

					if (t->right == nullptr) //insert if reach leaves

					{

				t->right = new node(n);

				t->right->up = t;

				break;

					}

					else

						t = t->right;

				else if (n < t->c) // go right if bigger

					if (t->left == nullptr)//insert if reach leaf

					{

					t->left = new node(n);

					t->left->up = t;

					break;

					}

					else

						t = t->left;

		}

	}

	int deleteNode(int n)//search the node and delete it

	{

		node*t1, *t2;

		t1 = root;

		while (t1->c != n)// locate the node been deleted as t1

		{

			if (n > t1->c)

				if (t1->right == nullptr)

					return -1;

				else

					t1 = t1->right;

			else if (n < t1->c)

				if (t1->left == nullptr)

					return -1;

				else

					t1 = t1->left;

		}

		if (t1->left == nullptr && t1->right == nullptr)//if the node been deleted is a leaf

		{

			if (t1 == root)// if node is root

				isEmpty = true;

			else //if node is not root

			{

				if (t1->up->c < n)

					t1->up->right = nullptr;

				else t1->up->left = nullptr;

				delete t1;

			}

		}

		else if (findsize(t1->left) > findsize(t1->right)) // if node have more element on the left than the right, than find a replacement node on the left with the closest value

		{

			t2 = t1->left;

			if (t2->right == nullptr)

			{

				t1->c = t2->c;

				t1->left = t2->left;

			}

			else

			{

				while (t2->right != nullptr)

					t2 = t2->right;

				t1->c = t2->c;

				t2->up->right = t2->left;

			}

			delete t2;

		}

		else// if node have more element on the right than the left, than find a replacement node on the right with the closest value

		{

			t2 = t1->right;

			if (t2->left == nullptr)

			{

				t1->c = t2->c;

				t1->right = t2->right;

			}

			else

			{

				while (t2->left != nullptr)

					t2 = t2->left;

				t1->c = t2->c;

				t2->up->left = t2->right;

			}

			delete t2;

		}

		return 0;





	}

	bool readTree(string fileName)//read tree from file

	{

		int n;

		fileName += ".txt";

		ifstream getTree(fileName);

		if (getTree.is_open())

		{

			deleteAll(root);

			isEmpty = true;

			root->left = root->right = nullptr;

			while (getTree >> n)

				insert(n);

			return true;

		}

		return false;

	}

};

int printOption(tree n)//display printing meun

{

	int m = 0;

	char inp;

	gotoxy(0, 0);

	cout << "Choose print order with arrow and enter key :  LNR  NLR  LRN (Esc to go back)";

	gotoxy(m * 5 + 46, 0);

	while ((inp = _getch()) != 27)//only allow user to input up, down, enter, and esc

	{

		switch (inp)

		{

		case -32:

			switch (_getch())

			{

			case 75://when user input left

				if (m != 0)

					m--;

				break;

			case 77://when user press right

				if (m != 2)

					m++;

				break;

			}

			break;

		case 13:

			return m;

			break;

		}

		gotoxy(m * 5 + 46, 0);

	}

	return 0;

}

bool isint(string s)//check if a string is also int

{

	for (char c : s)

		if (!isdigit(c))

			return false;

	return true;

}

int main()// display main meun

{

	int i;

	tree a;

	string t, message = "";

	string mode = "insert";

	cout << "Tree project 11/19/2018 Austin Kao 900764774\n";

	cout << "Enter file name to load your first tree!(enter skip if you don't have one)\n";

	cout << "(also don't include .txt when you input your file name, we will add it for you)\n:";

	cin >> t;

	system("cls");

	if (a.readTree(t))

	{

		displayTree(a.root);

		message = "your file went through and here is your first tree, enjoy!";

	}

	else message = "you have a empty tree, enter the first number to create a root!";

	while (true)

	{

		gotoxy(0, 0);

		cout << "Enter the number you wanna to " << mode << ": ";

		if (mode == "delete")

			cout << "        " << "(enter clear to delete all node)";

		cout << endl << "or Enter P to print the tree, R to read tree from file,\n";

		if (mode == "insert") cout << "D to enter delete mode ";

		else cout << "I to enter insert mode ";



		cout << "and Q to quit" << endl << "Tree size : " << a.findsize(a.root) << "        message: " << message;

		message = "";

		gotoxy(37, 0);

		cin >> t;

		if ((t == "clear" || t == "Clear") && mode == "delete")

		{

			deleteAll(a.root);

			a.isEmpty = true;

			a.root->left = a.root->right = nullptr;

		}

		else if (t[0] == 'Q' || t[0] == 'q')//q for quiz

		{

			system("cls");

			return 0;

		}

		else if (t[0] == 'D' || t[0] == 'd')//d for going to delete mode

		{

			mode = "delete";

			message = "switch to delete mode";

		}

		else if (t[0] == 'I' || t[0] == 'i')//i for going to insert mode

		{

			mode = "insert";

			message = "switch to insert mode";

		}

		else if (t[0] == 'R' || t[0] == 'r')//r for reading files

		{

			system("cls");

			cout << "Enter file name: " << endl << endl << "massage: ";

			gotoxy(16, 0);

			cin >> t;

			if (a.readTree(t))

				message = "file loaded";

			else

				message = "fail to load file";

		}

		else if (t[0] == 'P' || t[0] == 'p')// p for printing trees

		{

			i = printOption(a);

		}

		else

		{

			if (isint(t))

				if (mode == "insert")

					a.insert(stoi(t));

				else

					a.deleteNode(stoi(t));

			else

				message = "invalid input";

		}

		system("cls");

		if (t[0] == 'P' || t[0] == 'p')

		{

			gotoxy(0, 6);

			print(a.root, i);

		}

		tx = 0;

		if (!a.isEmpty)

			displayTree(a.root);

	}



	return 0;

}