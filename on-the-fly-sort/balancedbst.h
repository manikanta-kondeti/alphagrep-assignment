//balancedbst.h
/*
    Program: onTheFlySort 
    Author: Manikanta 
*/
#ifndef BALANCEDBST
#include <iostream>
using namespace std;

struct TreeNode {
	long long int val; 
	TreeNode * left;
	TreeNode * right;
	int height;
}; 

TreeNode * create_node(long long int val) {
	TreeNode * new_node = (TreeNode *)malloc(sizeof(TreeNode));
	new_node->val = val;
	new_node->left = NULL;
	new_node->right = NULL;
	return new_node;
}

void inorder(TreeNode * root) {
	if(root == NULL) return;
	inorder(root->left);
	cout << root->val << endl;
	inorder(root->right);
	return;
}

int height(TreeNode *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}
 
int max(int a, int b)
{
    return (a > b)? a : b;
}
 
TreeNode* create_node(int val)
{
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->val   = val;
    node->left   = NULL;
    node->right  = NULL;
    node->height = 1; 
    return(node);
}
 
TreeNode *rightRotate(TreeNode *y)
{
    TreeNode *x = y->left;
    TreeNode *T2 = x->right;
    x->right = y;
    y->left = T2;
 
    y->height = max(height(y->left), height(y->right))+1;
    x->height = max(height(x->left), height(x->right))+1; 
    return x;
}

TreeNode *leftRotate(TreeNode *x)
{
    TreeNode *y = x->right;
    TreeNode *T2 = y->left;
 
    y->left = x;
    x->right = T2;
 
    x->height = max(height(x->left), height(x->right))+1;
    y->height = max(height(y->left), height(y->right))+1;
 

    return y;
}
 
int getBalance(TreeNode *N)
{
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}
 
TreeNode* insertTreeNode(TreeNode* node, int val)
{
    if (node == NULL)
        return(create_node(val));
 
    if (val < node->val)
        node->left  = insertTreeNode(node->left, val);
    else if (val > node->val)
        node->right = insertTreeNode(node->right, val);
    else 
        return node;
 
    node->height = 1 + max(height(node->left),
                           height(node->right));
    int balance = getBalance(node);
 
    if (balance > 1 && val < node->left->val)
        return rightRotate(node);
 
    if (balance < -1 && val > node->right->val)
        return leftRotate(node);
 
    if (balance > 1 && val > node->left->val) {
        node->left =  leftRotate(node->left);
        return rightRotate(node);
    }
 
    if (balance < -1 && val < node->right->val) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}
#endif 
