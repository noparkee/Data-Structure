#include <stdlib.h> // malloc, rand
#include <stdio.h>
#include <time.h> // time				

#define MAX_ELEM 20										
#define max(x, y)	(((x) > (y)) ? (x) : (y))			//부모 노드의 height를 구할 때.

////////////////////////////////////////////////////////////////////////////////
// AVL_TREE type definition
typedef struct node
{
	int			data;
	struct node	*left;
	struct node	*right;
	int			height;
} NODE;

typedef struct
{
	NODE	*root;
	int		count;  // number of nodes
} AVL_TREE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

AVL_TREE *AVL_Create( void);
AVL_TREE *AVL_Destroy( AVL_TREE *pTree);
static void _destroy( NODE *root);
int AVL_Insert( AVL_TREE *pTree, int data);
static NODE *_insert( NODE *root, NODE *newPtr);
static NODE *_makeNode( int data);
int *AVL_Retrieve( AVL_TREE *pTree, int key);
static NODE *_retrieve( NODE *root, int key);
void AVL_Traverse( AVL_TREE *pTree);
static void _traverse( NODE *root);
void printTree( AVL_TREE *pTree);
static void _infix_print( NODE *root, int level);
static int getHeight( NODE *root);
static NODE *rotateRight( NODE *root);
static NODE *rotateLeft( NODE *root);

////////////////////////////////////////////////////////////////////////////////


/* Allocates dynamic memory for a AVL_TREE head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
AVL_TREE *AVL_Create( void){
	AVL_TREE* pTree = (AVL_TREE*)malloc(sizeof(AVL_TREE));
	
	if(pTree == NULL)
		return NULL;
	
	else{
		pTree -> root = NULL;
		pTree -> count = 0;
		return pTree;
	}
};

/* Deletes all data in tree and recycles memory
	return	NULL head pointer
*/
AVL_TREE *AVL_Destroy( AVL_TREE *pTree){
	if (pTree)
	{
		_destroy( pTree->root);
	}
		
	free( pTree);
	
	return NULL;
};

static void _destroy( NODE *root){
	if(root != NULL){
		_destroy(root -> left);
		_destroy(root -> right);
		free(root);
	}
	else
		return;
};

/* Inserts new data into the tree
	return	1 success
			0 overflow
*/
int AVL_Insert( AVL_TREE *pTree, int data){
	NODE* newPtr;
	newPtr = _makeNode(data);
	if(newPtr == NULL)
		return 0;
	
	pTree -> root = _insert(pTree -> root, newPtr);
	
	pTree -> count ++;
	
	return 1;
};

/* internal function
	This function uses recursion to insert the new data into a leaf node
	return	pointer to new root
*/
static NODE *_insert( NODE *root, NODE *newPtr){
	if(root == NULL){
		root = newPtr;
		
		return root;
	}
	
	if(root -> data > newPtr -> data){
		root -> left = _insert(root -> left, newPtr);
		
		if(getHeight(root -> left) > getHeight(root -> right)+1){
			if(getHeight(root -> left -> left) > getHeight(root -> left -> right)){			//left of left			
				root = rotateRight(root);
			}
			else{		//right of left
				root -> left = rotateLeft(root -> left);
				root = rotateRight(root);
			}
		}
	}
	else{
		root -> right = _insert(root -> right, newPtr);
		
		if(getHeight(root -> right) > getHeight(root -> left)+1){
			if(getHeight(root -> right -> right) > getHeight(root -> right -> left)){
				root = rotateLeft(root);
			}
			else{
				root -> right = rotateRight(root -> right);
				root = rotateLeft(root);
			}
		}
	}
	
	root -> height = max(getHeight(root -> left), getHeight(root -> right)) + 1;
	
	return root;
};

static NODE *_makeNode( int data){
	NODE *pNew = (NODE*)malloc(sizeof(NODE));
	if(pNew == NULL)
		return NULL;
	else{
		pNew -> left = NULL;
		pNew -> right = NULL;
		pNew -> data = data;
		pNew -> height = 1;
	}
};

/* Retrieve tree for the node containing the requested key
	return	address of data of the node containing the key
			NULL not found
*/
int *AVL_Retrieve( AVL_TREE *pTree, int key);

/* internal function
	Retrieve node containing the requested key
	return	address of the node containing the key
			NULL not found
*/
static NODE *_retrieve( NODE *root, int key);

/* Prints tree using inorder traversal
*/
void AVL_Traverse( AVL_TREE *pTree){
	if(pTree -> root != NULL)
		_traverse(pTree -> root);
	
	return;
}

static void _traverse( NODE *root){
	if(root != NULL){
		_traverse (root -> left);
		printf("%d ", root -> data);
		_traverse (root -> right);
	}
	return ;
};

/* Prints tree using inorder right-to-left traversal
*/
void printTree( AVL_TREE *pTree){

	_infix_print(pTree->root, 0);
	
	return;
};

/* internal traversal function
*/
static void _infix_print( NODE *root, int level){
	if(root != NULL){
		
		_infix_print(root -> right, level+1);	
		
		for(int i = 0; i < level; i++)
			printf("\t");
		
		printf("%d\n", root -> data);
		
		_infix_print(root -> left, level+1);
	}
};

/* internal function
	return	height of the (sub)tree from the node (root)
*/
static int getHeight( NODE *root){
	if(root == NULL)
		return 0;
	else
		return root -> height;
};

/* internal function
	Exchanges pointers to rotate the tree to the right
	updates heights of the nodes
	return	new root
*/
static NODE *rotateRight( NODE *root){			//root는 균형이 안 맞는 부분의 root 
	NODE *ptr;
	ptr = root -> left;
	
	root -> left = ptr -> right;
	ptr -> right = root;
	
	root -> height = max(getHeight(root -> left), getHeight(root -> right)) + 1;
	ptr -> height = max(getHeight(ptr -> left), getHeight(ptr -> right)) + 1;
	
	return ptr;
};

/* internal function
	Exchanges pointers to rotate the tree to the left
	updates heights of the nodes
	return	new root
*/
static NODE *rotateLeft( NODE *root){
	NODE *ptr;
	ptr = root -> right;
	
	root -> right = ptr -> left;
	ptr -> left = root;
	
	root -> height = 1;
	
	
	root -> height = max(getHeight(root -> left), getHeight(root -> right)) + 1;
	ptr -> height = max(getHeight(ptr -> left), getHeight(ptr -> right)) + 1;
	
	return ptr;
};

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	AVL_TREE *tree;
	int data;
	
	// creates a null tree
	tree = AVL_Create();
	
	if (!tree)
	{
		printf( "Cannot create tree\n");
		return 100;
	}
	fprintf( stdout, "Inserting: ");
	
	srand( time(NULL));
	for (int i = 0; i < MAX_ELEM; i++)
	{
		data = rand() % (MAX_ELEM * 3) + 1; // random number
		// data = i+1; // sequential number
		
		fprintf( stdout, "%d ", data);

		// insert function call
		AVL_Insert( tree, data);
	}
	
	fprintf( stdout, "\n");
	
	
	// inorder traversal
	fprintf( stdout, "Inorder traversal: ");
	AVL_Traverse( tree);
	fprintf( stdout, "\n");
	
	// print tree with right-to-left infix traversal
	fprintf( stdout, "Tree representation:\n");
	printTree(tree);
	
	fprintf( stdout, "Height of tree: %d\n", tree->root->height);
	fprintf( stdout, "# of nodes: %d\n", tree->count);
	
	AVL_Destroy( tree);

	return 0;
}