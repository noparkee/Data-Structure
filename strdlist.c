			#include <stdlib.h> // malloc
			#include <stdio.h>
			#include <string.h> // strdup, strcmp
			#include <ctype.h> // toupper

			#define QUIT			1
			#define FORWARD_PRINT	2
			#define BACKWARD_PRINT	3
			#define DELETE			4

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
				struct node	*llink;
				struct node	*rlink;
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
					pList -> head = pList -> head -> rlink;

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
			int addNode( LIST *pList, tTOKEN *dataInPtr){
				tTOKEN *pDataOut;
				
				int found = searchList(pList, dataInPtr -> token, pDataOut);
				pList -> pos = pList -> head;
				if(pList != NULL){
					NODE *pPre;
						if(found == 0){		//not found same
							NODE *pNew = (NODE*)malloc(sizeof(NODE));
							
							if(pNew == NULL)
								return -1;
							
							pNew -> dataPtr = dataInPtr;
							if(emptyList(pList)==1){		//pList is empty
							pPre = NULL;
							}
					else{
				
							if(strcmp((pList -> pos -> dataPtr -> token), (dataInPtr -> token)) > 0){
								pPre = NULL;

							}
							else{
								while(strcmp((pList -> pos -> dataPtr -> token), (dataInPtr -> token)) < 0){
									pPre = pList -> pos;
									pList -> pos = pList -> pos -> rlink;
								
									if(pList -> pos == NULL)	
										break;
								}
							}
							///////insert
							
						}
						if(pPre == NULL){
								pNew -> llink = NULL;
								pNew -> rlink = pList -> head;
								pList -> head = pNew;
								if(pNew -> rlink != NULL)
									pNew -> rlink -> llink = pNew;
							}
							else{
								pNew -> rlink = pPre -> rlink;
								pPre -> rlink = pNew;
								pNew -> llink = pPre;
								
								if(pNew -> rlink != NULL)
									pNew -> rlink -> llink = pNew;
							}
							
						pList -> count ++;
						pNew -> dataPtr -> freq =+1;
					
						return 0;
						}
						else{		//found		
							pList -> pos = pList -> head;
							while(strcmp((pList -> pos -> dataPtr -> token), (dataInPtr -> token)) != 0){
								pPre = pList -> pos;
								pList -> pos = pList -> pos -> rlink;
								if(pList -> pos == NULL)
									break;
							}
							pList -> pos -> dataPtr -> freq ++;
				
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
				pList -> pos = NULL;					// pPre
				pList -> rear = pList -> head; 			// pLoc
				int found;
				
				found = searchList(pList, keyPtr, dataOut);
				if(found == 1){
					while(pList -> rear != NULL && strcmp((pList -> rear -> dataPtr -> token), keyPtr) < 0){
						pList -> pos = pList -> rear;
						pList -> rear = pList -> rear -> rlink;
					}
					
					*dataOut = pList -> rear -> dataPtr;
					
					if(pList -> pos == NULL){		//처음 지우기
						pList -> head = pList -> rear -> rlink;
						pList -> rear -> rlink -> llink = NULL;
					}
					else if(pList -> rear -> rlink == NULL){	//마지막
						pList -> pos -> rlink = pList -> rear -> rlink;
						pList -> rear -> llink = NULL;			
					}
					else{
						pList -> pos -> rlink = pList -> rear -> rlink;
						pList -> rear -> rlink -> llink = pList -> pos;
					}
					
					free(pList -> rear);
					
					return 1;
				}
				else
					return 0;
			}

			/* interface to search function
				Argu	key being sought
				dataOut	contains found data
				return	1 successful
						0 not found
			*/
			int searchList( LIST *pList, char *pArgu, tTOKEN **pDataOut){
				pList -> pos = NULL;					//pPre
				pList -> rear = pList -> head;			//pLoc
				
				while(pList -> rear != NULL && strcmp((pList -> rear -> dataPtr -> token), pArgu) < 0){		//뒤의 문자가 더 큼
					pList -> pos = pList -> rear;
					pList -> rear = pList -> rear -> rlink;
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

			/* prints data from list (forward)
			*/
			void printList( LIST *pList){
				if(emptyList(pList) == 0){
					pList -> pos = pList -> head;
					while(pList -> pos != NULL){
						printf("%s\t%d\n", pList -> pos -> dataPtr -> token, pList -> pos -> dataPtr -> freq);
						pList -> pos = pList -> pos -> rlink;
					}
					pList -> pos = NULL;
				}
			};

			/* prints data from list (backward)
			*/
			void printListR( LIST *pList){
				pList -> rear = pList -> head;
				while(pList -> rear -> rlink != NULL){
					pList -> rear = pList -> rear -> rlink;
					if(pList -> rear -> rlink == NULL)
						break;
				}
				
				if(emptyList(pList) == 0){
					while(pList -> rear != NULL){
						printf("%s\t%d\n", pList -> rear -> dataPtr -> token, pList -> rear -> dataPtr -> freq);
						pList -> rear = pList -> rear -> llink;
					}
					
				}
			};

			/* internal insert function
				inserts data into a new node
				return	1 if successful
						0 if memory overflow
			*/
			//static int _insert( LIST *pList, NODE *pPre, tTOKEN *dataInPtr);

			/* internal delete function
				deletes data from a list and saves the (deleted) data to dataOut
			*/
			//static void _delete( LIST *pList, NODE *pPre, NODE *pLoc, tTOKEN **dataOutPtr);

			/* internal search function
				searches list and passes back address of node
				containing target and its logical predecessor
				return	1 found
						0 not found
			*/
			//static int _search( LIST *pList, NODE **pPre, NODE **pLoc, char *pArgu);

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
				//pToken -> token = NULL;
				pToken -> freq = 0;
				
				free(pToken -> token);
				free(pToken);
				
				return NULL;
			};

			/* gets user's input
			*/
			int get_action()
			{
				char ch;
				scanf( "%c", &ch);
				ch = toupper( ch);
				switch( ch)
				{
					case 'Q':
						return QUIT;
					case 'F':
						return FORWARD_PRINT;
					case 'B':
						return BACKWARD_PRINT;
					case 'D':
						return DELETE;
				}
				return 0; // undefined action
			}

			////////////////////////////////////////////////////////////////////////////////
			int main( int argc, char **argv)
			{
				LIST *list;
				char str[1024];
				tTOKEN *pToken;
				int ret;
				FILE *fp;
				
				if (argc != 2)
				{
					fprintf( stderr, "usage: %s FILE\n", argv[0]);
					return 1;
				}
				
				fp = fopen( argv[1], "rt");
				if (!fp)
				{
					fprintf( stderr, "Error: cannot open file [%s]\n", argv[1]);
					return 2;
				}
				
				// creates a null list
				list = createList();
				if (!list)
				{
					printf( "Cannot create list\n");
					return 100;
				}
				
				while(fscanf( fp, "%s", str) == 1)
				{
					pToken = createToken( str);
					
					// insert function call
					ret = addNode( list, pToken);
					if (ret == 1) // duplicated 
						destroyToken( pToken);
				}
				
				fclose( fp);
				
				fprintf( stdout, "Select Q)uit, F)orward print, B)ackward print, D)elete: ");
				
				while (1)
				{
					int action = get_action();
					
					switch( action)
					{
						case QUIT:
							destroyList( list);
							return 0;
						
						case FORWARD_PRINT:
							printList( list);
							break;
						
						case BACKWARD_PRINT:
							printListR( list);
							break;
						
						case DELETE:
							fprintf( stdout, "Input a string to delete: ");
							fscanf( stdin, "%s", str);
							int ret = removeNode( list, str, &pToken);
							if (ret)
							{	
								fprintf( stdout, "%s deleted\n", pToken->token);
								destroyToken( pToken);
							}
							else fprintf( stdout, "%s not found\n", str);
							break;
						}
					
					if (action) fprintf( stdout, "Select Q)uit, F)orward print, B)ackward print, D)elete: ");
				}
				return 0;
			}