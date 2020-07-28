#include <stdio.h>
#include <stdlib.h> // malloc, rand
#include <time.h> // time
#include <assert.h>

#define MAX_ELEM	21

typedef struct
{
	int *heapArr;
	int	last;
	int	capacity;
} HEAP;


HEAP *heapCreate( int capacity);
void heapDestroy( HEAP *heap);
int heapInsert( HEAP *heap, int data);
static void _reheapUp( HEAP *heap, int index);
int heapDelete( HEAP *heap, int* data);
static void _reheapDown( HEAP *heap, int index);
void heapPrint( HEAP *heap);


HEAP *heapCreate( int capacity){
	HEAP *H = (HEAP*)malloc(sizeof(HEAP));
	if(H == NULL)
		return NULL;
	
	//int arr[MAX_ELEM];
	
	H -> heapArr = (int*)malloc(sizeof(int)*MAX_ELEM);
	H -> last = -1;
	H -> capacity = capacity;
	
	return H;
};

void heapDestroy( HEAP *heap){
	free(heap -> heapArr);
	free(heap);
};

static void _reheapUp( HEAP *heap, int index){	//index가 끝의 last.
	if(index > 0){
		int par = (index - 1) / 2;
		
		if(heap -> heapArr[index] > heap -> heapArr[par]){
			int tem;
			tem = heap -> heapArr[index];
			heap -> heapArr[index] = heap -> heapArr[par];
			heap -> heapArr[par] = tem;
			_reheapUp(heap, par);
		}
	}
};

int heapInsert( HEAP *heap, int data){
	heap -> last ++;
	heap -> heapArr[heap -> last] = data;
	_reheapUp(heap, heap -> last);

	return 1;
};

static void _reheapDown( HEAP *heap, int index){	//index를 root로 하자.
	int left, right, tem, root;
	root = index;	
	
	if(2*index + 1 >= 0 && 2*index + 1 <= heap -> last){
		left = heap -> heapArr[2*index + 1];
		if(2*index + 2 >= 0 && 2*index + 2 <= heap -> last){
			right = heap -> heapArr[2*index + 2];
		}
		else
			right = NULL;
		
		if(left > right)
			index = 2*index + 1;
		else
			index = 2*index + 2;
		
		if(heap -> heapArr[root] < heap -> heapArr[index]){
			tem = heap -> heapArr[root];
			heap -> heapArr[root] = heap -> heapArr[index];
			heap -> heapArr[index] = tem;
			
			_reheapDown(heap, index);
		}		
	}
};

int heapDelete( HEAP *heap, int* data){		//main에서 delete 부분 while(heqp -> last > 0) 이어야 하는거 아닌가?
	*data = heap -> heapArr[0];
	
	heap -> heapArr[0] = heap -> heapArr[heap -> last];
	heap -> last --;
	
	_reheapDown(heap, 0);
	
	return 1;
};

void heapPrint( HEAP *heap){
	for(int i = 0; i <= heap -> last; i++)
		printf("\t%d", heap -> heapArr[i]);
	printf("\n");
};

int main(void)
{
	HEAP *heap;
	int data;
	
	heap = heapCreate(MAX_ELEM);
	
	srand( time(NULL));
	
	for (int i = 1; i < MAX_ELEM; i++)
	{
		data = rand() % MAX_ELEM * 3 + 1; // 1 ~ MAX_ELEM*3 random number
				
		fprintf( stdout, "Inserting %d: ", data);
		
		// insert function call
		heapInsert( heap, data);
		
		heapPrint( heap);
 	}

	while (heap->last >= 0)
	{
		// delete function call
		heapDelete( heap, &data);

		fprintf( stdout, "Deleting %d: ", data);

		heapPrint( heap);
 	}
	
	heapDestroy( heap);
	
	return 0;
}