#include <stdlib.h> // malloc, atoi
#include <string.h>	//strlen
#include <stdio.h>
#include <ctype.h> // isdigit
#include <assert.h> // assert

#define MAX_STACK_SIZE	50

////////////////////////////////////////////////////////////////////////////////
// LIST type definition
typedef struct node
{
	char		data;
	struct node	*left;
	struct node	*right;
} NODE;

typedef struct
{
	NODE	*root;
} TREE;



///////////////////////////////////////////////////
NODE* stack[MAX_STACK_SIZE];
float floatstack[MAX_STACK_SIZE];
int top = -1;
int floattop = -1;

int push(NODE* pNew){
	
	if(top >= MAX_STACK_SIZE - 1){
		printf("stack overflow\n");
		return -1;
	}
	
	top ++;
	stack[top] = pNew;
	
	return 1;
}

NODE* pop(){
	NODE *dataOut;
	
	if(top < 0){
		printf("stack underflow\n");
		return NULL;
	}
	
	else{
		dataOut = stack[top];
		stack[top] = NULL;
		top --;
		return dataOut;
	}
}

int floatpush(float num){
	
	if(floattop >= MAX_STACK_SIZE - 1){
		printf("stack overflow\n");
		return -1;
	}
	
	floattop ++;
	floatstack[floattop] = num;
	
	return 1;
}

float floatpop(){
	float result;
	
	if(floattop < 0){
		printf("stack underflow\n");
		return -1;
	}
	
	else{
		result = floatstack[floattop];
		floattop --;
		return result;
	}
}

//////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

TREE *createTree( void);
TREE *destroyTree( TREE *pTree);
static void _destroy( NODE *root);
static NODE *_makeNode( char ch);
int postfix2tree( char *expr, TREE *pTree);
void traverseTree( TREE *pTree);
static void _traverse( NODE *root);
void printTree( TREE *pTree);
static void _infix_print( NODE *root, int level);
float evalPostfix( char *expr);



TREE *createTree( void){
	TREE *pTree = (TREE*)malloc(sizeof(TREE));
	if(pTree == NULL)
		return NULL;
	
	pTree -> root = NULL;

 	return pTree;
};


static void _destroy( NODE *root){
	if(root != NULL){
		_destroy(root -> left);
		_destroy(root -> right);
		free(root);
	}
};

static NODE *_makeNode( char ch){
	NODE *n = (NODE*)malloc(sizeof(NODE));
	if(n == NULL)
		return NULL;
	else {
		n -> data = ch;
		n -> left = NULL;
		n -> right = NULL;
	}
		return n;
};

int postfix2tree( char *expr, TREE *pTree){		//왜 return이 아니라 break 하면 안 되지?  \t 
	for(int i = 0; i< strlen(expr); i++){
		if(expr[i] >= '0' && expr[i] <= '9'){
			NODE* pNew = _makeNode(expr[i]);
			push(pNew);
		}
		else if(expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/'){
			NODE* pNew = _makeNode(expr[i]);
			if(top <= 0){
				if(top == 0)
					_destroy(pop());
					//free(pop());
				free(pNew);
				free(pTree);
				return 0;
			}
			else{
				pNew -> right = pop();
				pNew -> left = pop();
				push(pNew);	
			}
		}
		else{
			free(pTree);
			return 0;
		}
	}
	
	if(top == 0)
		pTree -> root = pop();
	else if(top > 0) {
		while(top > -1){
			_destroy(pop());
		}
		free(pTree);
		return 0;
	}
	
	return 1;
};


static void _traverse( NODE *root){
	if(root != NULL){
		if(root -> data >= '0' && root -> data <= '9'){
			printf("%c", root -> data);
		}
		else{
			printf("(");
			_traverse(root -> left);
			printf("%c", root -> data);
			_traverse(root -> right);
			printf(")");
		}
	}
};


static void _infix_print( NODE *root, int level){
	if(root != NULL){
		
		_infix_print(root -> right, level+1);	
		
		for(int i = 0; i < level; i++)
			printf("\t");
		
		printf("%c\n", root -> data);
		
		_infix_print(root -> left, level+1);
	}
};


float evalPostfix( char *expr){		
	float num1, num2;
	float result;
	
	for(int i = 0; i< strlen(expr); i++){
		if(expr[i] >= '0' && expr[i] <= '9'){
			floatpush(expr[i] - 48);
		}
		else if(expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/'){
			switch(expr[i]){
				case '+':
					num2 = floatpop();
					num1 = floatpop();
					result = num1 + num2;
					
					floatpush(result);
					
					break;
				case '-':
					num2 = floatpop();
					num1 = floatpop();
					result = num1 - num2;
					
					floatpush(result);
					
					break;
				case '*':
					num2 = floatpop();
					num1 = floatpop();
					result = num1 * num2;
					
					floatpush(result);
					break;
				case '/':
					num2 = floatpop();
					num1 = floatpop();
					result = num1 / num2;
					
					floatpush(result);
					break;
			}
		}
	}
	
	return (float)(floatpop());
};

////////////////////////////////////////////////////////////////////////////////
TREE *destroyTree( TREE *pTree)
{
	if (pTree)
	{
		_destroy( pTree->root);
	}
		
	free( pTree);
	
	return NULL;	
}

////////////////////////////////////////////////////////////////////////////////
void printTree( TREE *pTree)
{
	_infix_print(pTree->root, 0);
	
	return;
}

////////////////////////////////////////////////////////////////////////////////
void traverseTree( TREE *pTree)
{
	_traverse (pTree->root);
	
	return;
}

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	TREE *tree;
	char expr[1024];
	
	fprintf( stdout, "\nInput an expression (postfix): ");
	
	while (fscanf( stdin, "%s", expr) == 1)				//한 글자 씩 받고 있음.
	{
		// creates a null tree
		tree = createTree();
		
		if (!tree)
		{
			printf( "Cannot create tree\n");
			return 100;
		}
		
		// postfix expression -> expression tree
		int ret = postfix2tree( expr, tree);
		if (!ret)
		{
			fprintf( stdout, "invalid expression!\n");
			continue;
		}
		
		// expression tree -> infix expression
		fprintf( stdout, "\nInfix expression : ");
		traverseTree( tree);
		
		// print tree with right-to-left infix traversal
		fprintf( stdout, "\n\nTree representation:\n");
		printTree(tree);
		
		// evaluate postfix expression
		float val = evalPostfix( expr);
		fprintf( stdout, "\nValue = %f\n", val);
		
		// destroy tree
		destroyTree( tree);
		
		fprintf( stdout, "\nInput an expression (postfix): ");
	}
	return 0;
}