#include <stdlib.h> // malloc
#include <stdio.h>
#include <string.h> // strdup, strcmp

// User structure type definition
typedef struct 
{
	char	*token;
	int		freq;
} tTOKEN;

////////////////////////////////////////////////////////////////////////////////
// LIST type definition
typedef struct node
{
	tTOKEN		*dataPtr;
	struct node	*link;
} NODE;

typedef struct
{
	int		count;
	NODE	*pos;
	NODE	*head;
	NODE	*rear;
} LIST;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a list head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
LIST *createList( void){
	LIST *p = (LIST *)malloc(sizeof(LIST));
	
	p -> count = 0;
	p -> pos = NULL;
	p -> head = NULL;
	p -> rear = NULL;
		
	return p;
};

/* Deletes all data in list and recycles memory
	return	NULL head pointer
*/
LIST *destroyList( LIST *pList){
	while(emptyList(pList)==0)	{
		pList -> pos = pList -> head;
		pList -> head = pList -> head -> link;

		free(pList -> pos -> dataPtr -> token);
		free(pList -> pos -> dataPtr);
		free(pList -> pos);
		
		if(pList -> head == NULL)
			break;
	}
	pList -> count = 0;
	
	free(pList);
};

/* Inserts data into list
	return	-1 if overflow
			0 if successful
			1 if duplicated key
*/
int addNode( LIST *pList, tTOKEN *dataInPtr) {
	tTOKEN *pDataOut;
	
	int found = searchList(pList, dataInPtr -> token, pDataOut);
	
	pList -> pos = pList -> head;
	if(pList!=NULL){
		if(found == 0){		//not found
			NODE *pNew = (NODE*)malloc(sizeof(NODE));
			
			if(pNew == NULL)
				return -1;
			
			NODE *pPre;
			pNew -> dataPtr = dataInPtr;
			//pNew -> dataPtr -> token = strdup(dataInPtr -> token);
		
			if(emptyList(pList)==1){
				pPre = NULL;
			}
			else{
				if(strcmp((pList -> pos -> dataPtr -> token), (dataInPtr -> token)) > 0){
					pPre = NULL;
				}
				else{
					while(strcmp((pList -> pos -> dataPtr -> token), (dataInPtr -> token)) < 0){
						pPre = pList -> pos;
						pList -> pos = pList -> pos -> link;
			
						if(pList -> pos == NULL)	
							break;
					}
				}
			}
			if(pPre == NULL){
				pNew -> link = pList -> head;
				pList -> head = pNew;
			}
			else{
				pNew -> link = pPre -> link;
				pPre -> link = pNew;	
			}
			pNew -> dataPtr -> freq ++;
			pList -> count ++;
			
			return 0;
		}
		else {		//found same
			while(strcmp((pList -> pos -> dataPtr -> token), (dataInPtr -> token)) != 0){
				pList -> pos = pList -> pos -> link;
				if(pList -> pos == NULL)
					break;
			}
			pList -> pos -> dataPtr -> freq ++;
			pList -> count ++;
			
			
			
			return 1;
		}
	}
	else
		return -1;
};

/* Removes data from list
	return	0 not found
			1 deleted
*/
int removeNode( LIST *pList, char *keyPtr, tTOKEN **dataOut){
	//키가 같은 노드를 지움.
	pList -> pos = NULL;					// pPre
	pList -> rear = pList -> head; 			// pLoc
	int found;
	
	found = searchList(pList, keyPtr, dataOut);
	
	if(found == 1){
		while(pList -> rear != NULL && strcmp((pList -> rear -> dataPtr -> token), keyPtr) < 0){
			pList -> pos = pList -> rear;
			pList -> rear = pList -> rear -> link;
		}
		
		if(pList -> pos == NULL)
			pList -> head = pList -> rear -> link;
		else
			pList -> pos -> link = pList -> rear -> link;
		
		free(pList -> rear -> dataPtr -> token);
		free(pList -> rear -> dataPtr);
		free(pList -> rear);
		
		pList -> count--;
		
		return 1;
	}
	else
		return 0;
};

/* interface to search function
	Argu	key being sought  찾아야하는것
	dataOut	contains found data
	return	1 successful
			0 not found
*/
int searchList( LIST *pList, char *pArgu, tTOKEN **pDataOut){
	pList -> pos = NULL;					//pPre
	pList -> rear = pList -> head;			//pLoc
	
	while(pList -> rear != NULL && strcmp((pList -> rear -> dataPtr -> token), pArgu) < 0){		//뒤의 문자가 더 큼
		pList -> pos = pList -> rear;
		pList -> rear = pList -> rear -> link;
	}
	
	if(pList -> rear == NULL){
		return 0;
	}
	else{
		if(strcmp(pArgu, pList -> rear -> dataPtr -> token) == 0){
			pDataOut = pList -> rear -> dataPtr;
			return 1;
		}
		else{
			return 0;
		}
	}
};

/* returns number of nodes in list
*/
int listCount( LIST *pList){
	if(emptyList(pList)==1)
		return 0;
	else
		return pList -> count;
};

/* returns	1 empty
			0 list has data
*/
int emptyList( LIST *pList){
	if(pList -> count == 0)
		return 1;
	else
		return 0;
};

//int fullList( LIST *pList);

/* prints data from list
*/
void printList( LIST *pList){
	if(emptyList(pList) == 0){
		pList -> pos = pList -> head;
		while(pList -> pos != NULL){
			printf("%s\t%d\n", pList -> pos -> dataPtr -> token, pList -> pos -> dataPtr -> freq);
			pList -> pos = pList -> pos -> link;
		}
		pList -> pos = NULL;
	}
};

/* internal insert function
	inserts data into a new node
	return	1 if successful
			0 if memory overflow
*/
static int _insert( LIST *pList, NODE *pPre, tTOKEN *dataInPtr);

/* internal delete function
	deletes data from a list and saves the (deleted) data to dataOut
*/
static void _delete( LIST *pList, NODE *pPre, NODE *pLoc, tTOKEN **dataOutPtr);

/* internal search function
	searches list and passes back address of node
	containing target and its logical predecessor
	return	1 found
			0 not found
*/
static int _search( LIST *pList, NODE **pPre, NODE **pLoc, char *pArgu);

/* Allocates dynamic memory for a token structure, initialize fields(token, freq) and returns its address to caller
	return	token structure pointer
			NULL if overflow
*/
tTOKEN *createToken( char *str){
	tTOKEN *T;
	T = (tTOKEN*)malloc(sizeof(tTOKEN));
	T -> token = strdup(str);
	T -> freq = 0;
	
	return T;
};

/* Deletes all data in token structure and recycles memory
	return	NULL head pointer
*/
tTOKEN *destroyToken( tTOKEN *pToken){
	pToken -> freq = 0;
	
	free(pToken -> token);
	free(pToken);
	
	return NULL;
};

////////////////////////////////////////////////////////////////////////////////
int main( void)
{
	LIST *list;
	char str[1024];
	tTOKEN *pToken;
	int ret;
	
	// creates a null list
	list = createList();
	if (!list)
	{
		printf( "Cannot create list\n");
		return 100;
	}
	
	while(scanf( "%s", str) == 1)
	{
		pToken = createToken( str);
		// insert function call
		ret = addNode( list, pToken);			
		if (ret == 1) // duplicated 
			destroyToken( pToken);
	}
	// print function call
	printList( list);
	
	destroyList( list);
	
	return 0;
}