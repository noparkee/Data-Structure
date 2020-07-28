#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // isupper, tolower

#define MAX_DEGREE	27 // 'a' ~ 'z' and EOW
#define EOW			'$' // end of word

// used in the following functions: trieInsert, trieSearch, triePrefixList
#define getIndex(x)		(((x) == EOW) ? MAX_DEGREE-1 : ((x) - 'a'))

// TRIE type definition
typedef struct trieNode {
	char 			*entry;
	struct trieNode	*subtrees[MAX_DEGREE];
} TRIE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

TRIE *trieCreateNode(void);
void trieDestroy( TRIE *root);
int trieInsert( TRIE *root, char *str);
int trieSearch( TRIE *root, char *str);
void trieList( TRIE *root);
void triePrefixList( TRIE *root, char *str);
int make_permuterms( char *str, char *permuterms[]);
void clear_permuterms( char *permuterms[], int size);
void trieSearchWildcard( TRIE *root, char *str);

////////////////////////////////////////////////////////////////////////////////

/* Allocates dynamic memory for a trie node and returns its address to caller
	return	node pointer
			NULL if overflow
*/
TRIE *trieCreateNode(void){
	TRIE *pTrie = (TRIE*)malloc(sizeof(TRIE));
	
	for(int i = 0; i<MAX_DEGREE; i++)
		pTrie -> subtrees[i] = NULL;
	
	pTrie -> entry = NULL;
	
	if(pTrie == NULL)
		return NULL;
	else
		return pTrie;
};

/* Deletes all data in trie and recycles memory
*/
void trieDestroy( TRIE *root){		//Destroy??		그리고 마지막에 Aborted (core dumped)

	if(root){
		for(int i = 0; i<MAX_DEGREE; i++){
			trieDestroy(root -> subtrees[i]);
		}
		free(root -> entry);
		free(root);
	}
	
	return;
};

/* Inserts new entry into the trie
	return	1 success
			0 failure
*/
// 주의! 엔트리를 중복 삽입하지 않도록 체크해야 함
// 대소문자를 소문자로 통일하여 삽입
// 영문자 외 문자를 포함하는 문자열은 삽입하지 않음
int trieInsert( TRIE *root, char *str){
	TRIE* ptr;
	ptr = root;

	for(int i = 0; i<strlen(str); i++){
		if(isupper(str[i]))
			str[i] = tolower(str[i]);
		
		if(getIndex(str[i]) > 26 || getIndex(str[i]) < 0){
			return 0;
		}
	}
	
	for(int i = 0; i<strlen(str); i++){
		if(ptr -> subtrees[getIndex(str[i])] == NULL){
			ptr -> subtrees[getIndex(str[i])] = trieCreateNode();
		}	
		ptr = ptr -> subtrees[getIndex(str[i])];
	}
	
	if(ptr -> entry != NULL)
		return 0;
	
	ptr -> entry = (char*)malloc(sizeof(char)*(strlen(str) + 1));		//여기도 +1 해야하나?
	strcpy(ptr -> entry, str);
	
	return 1;
};

/* Retrieve trie for the requested key
	return	1 key found
			0 key not found
*/
int trieSearch( TRIE *root, char *str){
	TRIE* ptr;
	
	ptr = root;
	
	for(int i = 0; i<strlen(str); i++){
		if(isupper(str[i]))
			str[i] = tolower(str[i]);
		if(getIndex(str[i]) > 25 || getIndex(str[i]) < 0)
			return 0;
	}
	
	for(int i = 0; i<strlen(str); i++){
		if(ptr -> subtrees[getIndex(str[i])] == NULL){
			return 0;
		}
		else
			ptr = ptr -> subtrees[getIndex(str[i])];
	}
	
	if(ptr -> entry == NULL)
		return 0;
	
	if(strcmp(ptr -> entry, str)==0)
		return 1;
	else
		return 0;
};

/* prints all entries in trie using preorder traversal
*/
void trieList( TRIE *root){		
	if(root -> entry != NULL)
		printf("%s\n", root -> entry);
	for(int i = 0; i<MAX_DEGREE; i++){
		if(root -> subtrees[i] != NULL)
			trieList(root -> subtrees[i]); 
	}
};

/* prints all entries starting with str (as prefix) in trie
   ex) "abb" -> "abbas", "abbasid", "abbess", ...
	using trieList function
*/
void triePrefixList( TRIE *root, char *str){
	TRIE* ptr = root;
	
	for(int i = 0; i<strlen(str); i++){
		ptr = ptr -> subtrees[getIndex(str[i])];
		if(ptr == NULL)
			return ;
	}
	if(ptr == NULL)
		return ;
	
	trieList(ptr);
};

/* makes permuterms for given str
	ex) "abc" -> "abc$", "bc$a", "c$ab", "$abc"
	return	number of permuterms
*/
int make_permuterms( char *str, char *permuterms[]){
	char* per = (char*)malloc(sizeof(char) * (strlen(str)+2));		//strlen: NULL문자가 위치한 index값을 알려주는 느낌적인 느낌.
	char ds;
	
	for(int i = 0; str[i]; i++){
		per[i] = str[i];
	}
	per[strlen(str)] = '$';
	per[strlen(str)+1] = 0;
	
	
	//permuterms[0] 은 처음 $붙인거
	permuterms[0] = strdup(per);
	
	
	for(int i = 1; i<strlen(str)+1; i++){
		ds = per[0];
		for(int j = 0; j<strlen(str); j++){
			per[j] = per[j+1];
		}
		per[strlen(str)] = ds;
		permuterms[i] = strdup(per);
	}
	
	free(per);
	
	return (strlen(str)+1);
};

/* recycles memory for permuterms
*/
void clear_permuterms( char *permuterms[], int size){
	for(int i = 0; i<size; i++)
		free(permuterms[i]);
};

/* wildcard search
	ex) "ab*", "*ab", "a*b", "*ab*"
	using triePrefixList function
*/
void trieSearchWildcard( TRIE *root, char *str){
	char* sch;
	int num = 0;
	
	for(int i = 0; i<strlen(str); i++){
		if(isupper(str[i]))
			str[i] = tolower(str[i]);
		
		if(str[i] == '*')
			num++;
		
		if((getIndex(str[i]) > 26 || getIndex(str[i]) < 0) && str[i] != '*')
			return;
	}
	
	if(num == 1){	//*이 1개
		char* per = (char*)malloc(sizeof(char)*(strlen(str)+1));
		
		if(str[0] == '*'){		//맨 앞에 *
			for(int i = 0; i<strlen(str) - 1; i++)
				per[i] = str[i+1];
			per[strlen(str)-1] = '$';
			per[strlen(str)] = 0;
			sch = strdup(per);
		
			triePrefixList(root, sch);
		
			free(sch);
			free(per);
		}
		
		else if(str[strlen(str)-1] == '*'){		//맨 뒤에 *
			per[0] = '$';
			for(int i = 0; i<strlen(str) - 1; i++)
				per[i+1] = str[i];
			per[strlen(str)] = 0;
			sch = strdup(per);
		
			triePrefixList(root, sch);
		
			free(sch);
			free(per);
		}
		
		else{		//중간에 *
			char p;
		
			for(int i = 0; i<strlen(str); i++)
				per[i] = str[i];
		
			per[strlen(str)] = '$';
	
			while(per[strlen(str)] != '*'){
				p = per[0];
				for(int j = 0; j<strlen(str); j++){
					per[j] = per[j+1];
				}
				per[strlen(str)] = p;
			}
		
			per[strlen(str)] = 0;
			sch = strdup(per);
		
			triePrefixList(root, sch);
		
			free(sch);
			free(per);
		}
	}
	
	else if(num >= 2){		//별이 두 개 이상
		
		/*if (strstr( str, "**")){
			triePrefixList(root, sch);
		}
			
		else{
		sch = strtok(str, "*");
		triePrefixList(root, sch);
		}*/
		
		sch = strtok(str, "*");
		triePrefixList(root, sch);
	}
};

///////////////////////////////////////////////////////





int main(int argc, char **argv)
{
	TRIE *trie;
	TRIE *permute_trie;
	int ret;
	char str[100];
	FILE *fp;
	char *permuterms[100];
	int num_p;
	
	if (argc != 2)
	{
		fprintf( stderr, "Usage: %s FILE\n", argv[0]);
		return 1;
	}
	
	fp = fopen( argv[1], "rt");
	if (fp == NULL)
	{
		fprintf( stderr, "File open error: %s\n", argv[1]);
		return 1;
	}
	
	trie = trieCreateNode(); // original trie
	permute_trie = trieCreateNode(); // trie for permuterm index
	
	printf( "Inserting to trie...\t");
	while (fscanf( fp, "%s", str) == 1) // words file
	{	
		ret = trieInsert( trie, str);
		
		if (ret)
		{
			num_p = make_permuterms( str, permuterms);		
		
			for (int i = 0; i < num_p; i++)
				trieInsert( permute_trie, permuterms[i]);			
			
			clear_permuterms( permuterms, num_p);
		}
	}
	
	printf( "[done]\n"); // Inserting to trie

	fclose( fp);
	
	printf( "\nQuery: ");
	while (fscanf( stdin, "%s", str) == 1)
	{
		if (strchr( str, '*')) // wildcard search term
		{
			trieSearchWildcard( permute_trie, str);
		}
		else // search term
		{
			ret = trieSearch( trie, str);
			printf( "[%s]%s found!\n", str, ret ? "": " not");
		}
		printf( "\nQuery: ");
	}

	trieDestroy( trie);
	trieDestroy( permute_trie);
	
	return 0;
}