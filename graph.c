#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STACK_SIZE	121
int stack[MAX_STACK_SIZE];
int top = -1;

int queue[MAX_STACK_SIZE];
int head = 0;
int tail = 0;

int push(int num){
	
	if(top >= MAX_STACK_SIZE - 1){
		printf("stack overflow\n");
		return -1;
	}
	
	top ++;
	stack[top] = num;
	
	return 1;
}

int pop(){
	int result;
	
	if(top < 0){
		printf("stack underflow\n");
		return -1;
	}
	
	else{
		result = stack[top];
		top --;
		printf("%d ", result);
		return result;
	}
}

int enqueue(int num){
	if(tail == MAX_STACK_SIZE - 1){
		printf("queue overflow\n");
		return -1;
	}
	
	queue[tail] = num;
	tail ++;
	
	return 1;
}

int dequeue(){
	int result;
	
	if(head == tail){
		printf("queue is underflow\n");
		return -1;
	}
	else{
		result = queue[head];
		head++;
		printf("%d ", result);
		return result;
	}
}

int ** inVertex (int num){
	int ** graph;
	
	graph = (int**)malloc(sizeof(int*)*(num));
	
	for(int i = 0; i<num; i++){
		graph[i] = (int*)malloc(sizeof(int)*(num));
	}	//graph n x n 행렬 메모리 할당.
	
	for(int i = 0; i<num; i++){
		for(int j = 0; j<num; j++){
			graph[i][j] = 0;
		}
	}
	
	return graph;
}

int** makeGraph (char* file, int* nodenum){			//여기서 error 4개
	int **graph;
	char *pp = (char*)malloc(sizeof(char));
	int num, from, to;
	FILE *fp = fopen(file, "r");
	
	fscanf(fp, "%s", pp);
	fscanf(fp, "%d", &num);
	fscanf(fp, "%s", pp);
	
	free(pp);

	*nodenum = num;		//vertex 갯수
	
	graph = inVertex(num);
	
	while(1){
		fscanf(fp, "%d %d", &from, &to);
		if(feof(fp))
			break;
		
		graph[from-1][to-1] = 1;
		graph[to-1][from-1] = 1;
	}
	
	fclose(fp);
	
	return graph;
}

void depthFirst (int** graph, int num, int start, int check[]){
	int m, cnt;
	
	if(graph == NULL)
		return;
	
	push(start);
	check[start-1] = 1;
	m = pop();
	check[m-1] = 2;
	
	while(1){
		for(int i = 0; i<num; i++){
			if(graph[m-1][i] == 1 && graph[i][m-1] == 1 && check[i] == 0){
				push(i+1);
				check[i] = 1;
			}
		}
		
		if(top > -1){
			m = pop();
			check[m-1] = 2;
		}
		
		cnt = 0;
		
		for(int i = 0; i<num; i++){
			if(graph[m-1][i] == 1 && check[i] == 0)
				cnt++;
		}
		
		if(cnt == 0){	//연결된게 없으면
			if(top > -1){
				m = pop();
				check[m-1] = 2;
			}
			else
				break;
		}
		
	}
	
	while(top > -1){
		m=pop();
		check[m-1] = 2;
	}
	
	return;
}

void breadthFirst (int** graph, int num, int start, int check[]){
	int m, cnt;
	
	if(graph == NULL)
		return;
	
	enqueue(start);
	check[start-1] = 1;
	m = dequeue();
	check[m-1] = 2;
	
	while(1){
		for(int i = 0; i<num; i++){
			if(graph[m-1][i] == 1 && graph[i][m-1] == 1 && check[i] == 0){
				enqueue(i+1);
				check[i] = 1;
			}
		}
		
		if(head != tail){
			m = dequeue();
			check[m-1] = 2;
		}
		
		cnt = 0;
		
		for(int i = 0; i<num; i++){
			if(graph[m-1][i] == 1 && check[i] == 0)
				cnt++;
		}
		
		if(cnt == 0){	//연결된게 없으면
			if(tail != head){
				m = dequeue();
				check[m-1] = 2;
			}
			else
				break;
		}
	}
	
	while(head != tail){
		m=dequeue();
		check[m-1] = 2;
	}
	
	return;
}

void destroyGraph(int** graph, int num){
	for(int i = 0; i<num; i++){
		free(graph[i]);
	}
	free(graph);
}



int main(int argc, char** argv){
	int** graph;
	int num;		//num은 vertex 갯수.
	int start;
	
	graph = makeGraph (argv[1], &num);
	
	int check[num];
	int check2[num];
	
	for(int i = 0; i<num; i++)
		check[i] = 0;
	
	for(int i = 0; i<num; i++)
		check2[i] = 0;
	
	if (argc != 2)
	{
		printf( "Usage: %s FILE\n", argv[0]);
		return 2;
	}
	
	printf("DFS: ");
	while(1){
		int i;
		for (i = 0; i < num; i++){
			if (check[i] == 0){
				start = i + 1;
				break;
			}
		}
		if (i == num)
			break;
		depthFirst(graph, num, start, check);
	}
	printf("\n");
	
	printf("BFS: ");
	while(1){
		int j;
		for(j = 0; j<num; j++){
			if(check2[j] == 0){
				start = j + 1;
				break;
			}
		}
		if(j==num)
			break;
		breadthFirst(graph, num, start, check2);
	}
	printf("\n");
	
	destroyGraph(graph, num);
	
	return 0;
}