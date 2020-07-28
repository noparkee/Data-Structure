//#define DEBUG 1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "trie.h"

// 역색인 헤더 정보에 대한 구조체
typedef struct {
	int		index;	// starting position in posting.idx
	int		df;		// document frequency
} tHEADER;



static char *rtrim( char *str);
static char *ltrim( char *str);
static char *trim( char *str);
////////////////////////////////////////////////////////////////////////////////
// 헤더 정보가 저장된 파일(예) "header.idx")을 읽어 메모리에 저장한다.
// 헤더 구조체 메모리를 할당하고 그 주소를 반환
// 실패시 NULL을 반환
tHEADER *load_header( char *filename){
	int size;
	tHEADER *head = (tHEADER*)malloc(sizeof(tHEADER)*25700);
	
	FILE *header = fopen(filename, "rb");
	
	fread(head, sizeof(tHEADER), 25700, header);
	fclose(header);
	
	return head;
};

// 포스팅 리스트가 저장된 파일(예) "posting.idx")를 읽어 메모리에 저장한다.
// 포스팅 리스트(int arrary) 메모리를 할당하고 그 주소를 반환
// 실패시 NULL을 반환
int *load_posting( char *filename){
	int size;
	int *pos = (int*)malloc(sizeof(int)*410000);
	
	FILE *posting = fopen(filename, "rb");
	
	fread(pos, sizeof(int), 410000, posting);
	fclose(posting);
	
	return pos;
};

// 문서 집합을 화면에 출력한다.
void showDocuments( int *docs, int numdocs){
	for(int i = 0; i<numdocs; i++){
		printf(" %d", docs[i]);
	}
	printf("\n");
};

// 두 문서 집합의 교집합을 구한다.
// 교집합을 위한 메모리를 할당하고 그 주소를 반환
// 실패시 NULL을 반환
// 교집합의 문서 수는 newnumdocs에 저장한다.
int *intersectDocuments( int *docs, int numdocs, int *docs2, int numdocs2, int *newnumdocs){
	int *adr = (int*)malloc(sizeof(int)*9100);
	int i = 0;
	int j = 0;
	int k = 0;
	
	while(i<numdocs && j<numdocs2){
		if(docs[i] == docs2[j]){
			adr[k] = docs[i];
			i++;
			j++;
			k++;
		}
		else if(docs[i] > docs2[j]){
			j++;
		}
		else
			i++;
	}
	
	*newnumdocs = k;
	
	return adr;
};

// 두 문서 집합의 합집합을 구한다.
// 합집합을 위한 메모리를 할당하고 그 주소를 반환
// 실패시 NULL을 반환
// 합집합의 문서 수는 newnumdocs에 저장한다.
int *unionDocuments( int *docs, int numdocs, int *docs2, int numdocs2, int *newnumdocs){
	int *adr = (int*)malloc(sizeof(int)*9100);
	int i = 0;
	int j = 0;
	int k = 0;
	
	while(i<numdocs || j<numdocs2){
		if(i<numdocs && j<numdocs2){
			if(docs[i] == docs2[j]){
				adr[k] = docs[i];
				i++;
				j++;
				k++;
			}
			else if(docs[i] > docs2[j]){
				adr[k] = docs2[j];
				j++;
				k++;
			}
			else{
				adr[k] = docs[i];
				i++;
				k++;
			}
		}
		
		else if(i<numdocs && j>=numdocs2){
			adr[k] = docs[i];
			i++;
			k++;
		}
		else if(i>=numdocs && j<numdocs2){
			adr[k] = docs2[j];
			j++;
			k++;
		}
	}
	
	*newnumdocs = k;
	
	return adr;
};

// 단일 텀(single term)을 검색하여 문서를 찾는다.
// 문서 집합을 위한 메모리를 할당하고 그 주소를 반환
// 실패시 NULL을 반환
// 검색된 문서 수는 newnumdocs에 저장한다.
int *getDocuments( tHEADER *header, int *posting, TRIE *trie, char *term, int *numdocs){
	int *adr = (int*)malloc(sizeof(int)*9100);
	int dic_index;
	int df;
	int start;
	
	dic_index = trieSearch(trie, term);
	
	if(dic_index == -1){
		return NULL;
	}
	
	df = header[dic_index].df;
	start = header[dic_index].index;
	
	*numdocs = df;
	
	for(int i = 0; i<df; i++){
		adr[i] = posting[start];
		start++;
	}
	
	return adr;
};

// 질의(query)를 검색하여 문서를 찾는다.
// 질의는 단일 텀 또는 하나 이상의 불린 연산자('&' 또는 '|')를 포함한 질의가 될 수 있다.
// 문서 집합을 위한 메모리를 할당하고 그 주소를 반환
// 실패시 NULL을 반환
// 검색된 문서 수는 newnumdocs에 저장한다.
int *searchDocuments( tHEADER *header, int *posting, TRIE *trie, char *query, int *numdocs){
	//char *clean;
	query = trim(query);
	char* str;
	int j = 0;
	int k = 0;
	
	int num1;
	int num2;
	
	int *result;
	
	int *bol1;
	int *bol2;
	int *bol3;
	
	/*int **bol = (int**)malloc(sizeof(int*)*(50));
	for(int i = 0; i<50; i++){
		bol[i] = (int*)malloc(sizeof(int)*9100);
	}*/
	
	int q[50];		//불린 연산자 저장용
	for(int i = 0; i<50; i++)
		q[i] = 0;
	
	if(strchr(query, '&')!= NULL || strchr(query, '|') != NULL){	//불린 연산자가 포함되어 있을 때
		for(int i = 0; i<strlen(query); i++){
			if(query[i] == '&'){
				q[j] = 1;
				j++;
			}
			else if(query[i] == '|'){
				q[j] = 2;
				j++;
			}
		}		//불린 연산자 저장
		
		str = strtok(query, "&|");
		str = trim(str);
		bol1 = getDocuments(header, posting, trie, str, numdocs);
		num1 = *numdocs;
		/*printf("%s\t%d\n", str, num1);
		for(int i = 0; i<num1; i++)
			printf(" %d", bol1[i]);
		
		printf("\n");*/
		
		str = strtok(NULL, "&|");
		str = trim(str);
		bol2 = getDocuments(header, posting, trie, str, numdocs);
		num2 = *numdocs;
		/*printf("%s\t%d\n", str, num2);
		for(int i = 0; i<num2; i++)
			printf(" %d", bol2[i]);*/
		
		if(q[k] == 1){
			bol3 = intersectDocuments( bol1, num1, bol2, num2, numdocs);
		}
		else if(q[k] ==2){
			bol3 = unionDocuments( bol1, num1, bol2, num2, numdocs);
		}
		
		k++;
		
		bol1 = bol3;
		num1 = *numdocs;
		//printf("%d\n", num1);
		
		str = strtok(NULL, "&|");
		while(str != NULL){
			str = trim(str);
			bol2 = getDocuments(header, posting, trie, str, numdocs);
			num2 = *numdocs;
			
			if(q[k] == 1){
				bol3 = intersectDocuments( bol1, num1, bol2, num2, numdocs);
			}
			else if(q[k] ==2){
				bol3 = unionDocuments( bol1, num1, bol2, num2, numdocs);
			}
			
			bol1 = bol3;
			num1 = *numdocs;
			
			str = strtok(NULL, "&|");
		}
		
		return bol1;
		
	}
	else
		result = getDocuments(header, posting, trie, query, numdocs);		//단일 텀
	
	
	return result;
};

////////////////////////////////////////////////////////////////////////////////
static char *rtrim( char *str)
{
	char *p = str + strlen( str) - 1;
	
	while (p >= str)
	{
		if (*p == '\n' || *p == ' ' || *p == '\t') *p = 0;
		else return str;
		p--;
	}
}

static char *ltrim( char *str)
{
	char *p = str;
	
	while (*p)
	{
		if (*p == '\n' || *p == ' ' || *p == '\t') p++;
		else 
		{
			return p;
		}
	}
	return NULL;
}

static char *trim( char *str)
{
	if( str == NULL || *str == 0) return str;
	
	return rtrim(ltrim(str));
}

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	tHEADER *header;
	int *posting;
	TRIE *trie;
	char query[100];
	int index;
	
	header = load_header( "header.idx");
	if (header == NULL) return 1;
	
	posting = load_posting( "posting.idx");
	if (posting == NULL) return 1;
		
	trie = dic2trie( "dic.txt");
	
	printf( "\nQuery: ");
	while (fgets( query, 100, stdin) != NULL)
	{
		int numdocs;
		int *docs = searchDocuments( header, posting, trie, query, &numdocs);
		
		if (docs == NULL) printf( "not found!\n");
		else 
		{
			showDocuments( docs, numdocs);
			free( docs);
		}
		printf( "\nQuery: ");
	}
	
	free( header);
	free( posting);
	trieDestroy( trie);
	
	return 0;
}
