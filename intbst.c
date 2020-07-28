#include <stdlib.h> // malloc, atoi, rand
#include <stdio.h>
#include <ctype.h> // isdigit
#include <assert.h> // assert
#include <time.h> // time

////////////////////////////////////////////////////////////////////////////////
// TREE type definition
typedef struct node
{
	int			data;
	struct node	*left;
	struct node	*right;
} NODE;

typedef struct
{
	NODE	*root;
} TREE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

TREE *BST_Create( void);
TREE *BST_Destroy( TREE *pTree);
static void _destroy( NODE *root);
int BST_Insert( TREE *pTree, int data);
static NODE *_makeNode( int data);
int BST_Delete( TREE *pTree, int dltKey);
NODE *_delete( NODE *root, int dltKey, int *success);
int *BST_Retrieve( TREE *pTree, int key);
NODE *_retrieve( NODE *root, int key);
void BST_Traverse( TREE *pTree);
static void _traverse( NODE *root);
void printTree( TREE *pTree);
static void _infix_print( NODE *root, int level);


TREE *BST_Create( void){
	TREE *pTree = (TREE*)malloc(sizeof(TREE));
	
	if(pTree == NULL)
		return NULL;
	else{
		pTree -> root  = NULL;
		return pTree;
	}
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

TREE *BST_Destroy( TREE *pTree)
{
	if (pTree)
	{
		_destroy( pTree->root);
	}
		
	free( pTree);
	
	return NULL;	
}

static NODE *_makeNode( int data){
	NODE *n = (NODE*)malloc(sizeof(NODE));
	
	if(n == NULL)
		return NULL;
	
	else{
		n -> data = data;
		n -> left = NULL;
		n -> right = NULL;
	
		return n;
		}
}

int BST_Insert( TREE *pTree, int data){
	NODE *p;
	NODE *t;
	
	p = pTree -> root;
	
	NODE *pNew = _makeNode(data);
	if(pNew == NULL)
		return 0;
	
	if(p == NULL){
		pTree -> root = pNew;	
		return 1;
	}
	
	while(p != NULL){
		if(data < p -> data){
			t = p;
			p = p -> left;
		}
		else{
			t = p;
			p = p -> right;
		}	
	}
	
	if(data < t -> data)
		t -> left = pNew;
	else
		t -> right = pNew;
	
	return 1;
};

NODE *_delete( NODE *root, int dltKey, int *success){	
	NODE* ptr;
	
	if(root == NULL){
		*success = 0;
		return NULL;
	}
	
	if(dltKey < root -> data)
		root->left = _delete (root -> left, dltKey, success);
	else if(dltKey > root -> data)
		root->right = _delete (root -> right, dltKey, success);
	
	else{		//root와 dltKey가 같을 때. root를 지움.
		if(root -> left == NULL){
			ptr = root -> right;
			free(root);
			*success = 1;
			root = ptr;
		}
		else if(root -> right == NULL){
			ptr = root -> left;
			free(root);
			*success = 1;
			root = ptr;
		}
		else{		//left, right 둘 다 있을 때
			NODE* smallest;
			smallest = root -> right;
			
			while(smallest -> left != NULL)
				smallest = smallest -> left;
		
			root -> data = smallest -> data;
			root -> right = _delete (root -> right, smallest -> data, success);
		}
	}
	
	return root;
};

int BST_Delete( TREE *pTree, int dltKey){
	int success;
	NODE *ptr;
	
	if(pTree -> root != NULL){
		if(pTree -> root -> data == dltKey && pTree -> root -> left == NULL){
			ptr = pTree -> root -> right;
			free(pTree -> root);
			pTree -> root = ptr;
			success = 1;
		}
		else if(pTree -> root -> data == dltKey && pTree -> root -> right == NULL){
			ptr = pTree -> root -> left;
			free(pTree -> root);
			pTree -> root = ptr;
			success = 1;
		}

		else{
			_delete(pTree -> root, dltKey, &success);
		}
	}
	
	return success;
};

NODE *_retrieve( NODE *root, int key){		
	if(root == NULL || root -> data == key){
		return root;
	}
	
	if(key < root -> data)
		return _retrieve (root -> left, key);
	else if(key > root -> data)
		return _retrieve (root -> right, key);
};

int *BST_Retrieve( TREE *pTree, int key){
	NODE* fid;
	
	if(pTree != NULL)
		fid = _retrieve (pTree -> root, key);
	
	if(fid == NULL)
		return NULL;
	
	return &(fid -> data);		//????
};

static void _traverse( NODE *root){
	if(root != NULL){
		_traverse (root -> left);
		printf("%d ", root -> data);
		_traverse (root -> right);
	}
	return ;
};

void BST_Traverse( TREE *pTree)
{
	_traverse(pTree -> root);
	
	return;
};

static void _infix_print( NODE *root, int level){
	if(root != NULL){
		
		_infix_print(root -> right, level+1);	
		
		for(int i = 0; i < level; i++)
			printf("\t");
		
		printf("%d\n", root -> data);
		
		_infix_print(root -> left, level+1);
	}
};

void printTree( TREE *pTree)
{
	_infix_print(pTree->root, 0);
	
	return;
}
////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	TREE *tree;
	int data;
	
	// creates a null tree
	tree = BST_Create();
	if (!tree)
	{
		printf( "Cannot create tree\n");
		return 100;
	}

	fprintf( stdout, "Inserting: ");
	
	srand( time(NULL));
	for (int i = 1; i < 20; i++)
	{
		data = rand() % 100 + 1; // 1 ~ 100 random number
		
		fprintf( stdout, "%d ", data);
		
		// insert funtion call
		BST_Insert( tree, data);
 	}
	fprintf( stdout, "\n");
	
	// inorder traversal
	fprintf( stdout, "Inorder traversal: ");
	BST_Traverse( tree);
	fprintf( stdout, "\n");
	
	// print tree with right-to-left infix traversal
	fprintf( stdout, "Tree representation:\n");
	printTree(tree);
	
	int ret;
	do
	{
		fprintf( stdout, "Input a number to delete: "); 
		int num;
		ret = scanf( "%d", &num);
		if (ret != 1) break;
		
		ret = BST_Delete( tree, num);
		if (!ret) fprintf( stdout, "%d not found\n", num);
		
		// print tree with right-to-left infix traversal
		fprintf( stdout, "Tree representation:\n");
		printTree(tree);
		
	} while(1);
	
	BST_Destroy( tree);

	return 0;
}