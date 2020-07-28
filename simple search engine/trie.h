
#define MAX_DEGREE	27 // 'a' ~ 'z' and EOW
#define EOW			'$' // end of word		//이거 원래 없는거

// TRIE type definition
typedef struct trieNode {
	int 			index; // 0, 1, 2, ...
	struct trieNode	*subtrees[MAX_DEGREE];
} TRIE;

#define getIndex(x)		(((x) == EOW) ? MAX_DEGREE-1 : ((x) - 'a'))		//이거 원래 없는 거

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a trie node and returns its address to caller
	return	node pointer
			NULL if overflow
*/
TRIE *trieCreateNode(void){
	TRIE *pTrie = (TRIE*)malloc(sizeof(TRIE));
	
	for(int i = 0; i<MAX_DEGREE; i++)
		pTrie -> subtrees[i] = NULL;
	
	pTrie -> index = -1;
	
	if(pTrie == NULL)
		return NULL;
	else
		return pTrie;
};

/* Deletes all data in trie and recycles memory
*/
void trieDestroy( TRIE *root){
	if(root){
		for(int i = 0; i<MAX_DEGREE; i++){
			trieDestroy(root -> subtrees[i]);
		}
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
int trieInsert( TRIE *root, char *str, int dic_index){
	TRIE* ptr;
	ptr = root;
	
	for(int i = 0; i<strlen(str); i++){
		if(ptr -> subtrees[getIndex(str[i])] == NULL){
			ptr -> subtrees[getIndex(str[i])] = trieCreateNode();
		}
		ptr = ptr -> subtrees[getIndex(str[i])];
		
	}
	
	if(ptr -> index != -1)
		return 0;
	
	ptr -> index = dic_index;
	
	return 1;
};

/* Retrieve trie for the requested key
	return	index in dictionary (trie) if key found
			-1 key not found
*/
int trieSearch( TRIE *root, char *str){
	TRIE* ptr;
	char* find;
	ptr = root;
	int num;
	
	if(root == NULL)
		return -1;
	
	find = strchr(str, '\n');
		if(find != NULL)
			*find = '\0';
	
	for(int i = 0; i<strlen(str); i++){
		
		if(ptr -> subtrees[getIndex(str[i])] == NULL)
			return -1;
		else{
			ptr = ptr -> subtrees[getIndex(str[i])];
		}
	}
	
	if(ptr -> index == -1){
		return -1;
	}
	else{
		return ptr -> index;
	}
};

/* prints all entries in trie using preorder traversal
*/
void trieList( TRIE *root){
	if(root -> index != -1)
		printf("%d\n", root -> index);
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
/*int make_permuterms( char *str, char *permuterms[]){
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
};*/

/* recycles memory for permuterms
*/
/*void clear_permuterms( char *permuterms[], int size){
	for(int i = 0; i<size; i++)
		free(permuterms[i]);
};*/

/* wildcard search
	ex) "ab*", "*ab", "a*b", "*ab*"
	using triePrefixList function
*/
/*void trieSearchWildcard( TRIE *root, char *str){
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
		sch = strtok(str, "*");
		triePrefixList(root, sch);
	}
};*/

/* makes a trie for given dictionary file
	return	trie head node pointer
			NULL failure
*/ 
TRIE *dic2trie( char *dicfile){
	TRIE *trie;
	char str[100];
	//char *permuterms[100];
	int dic_index = 0;
	int ret;
	FILE *fp = fopen(dicfile, "rt");
	
	trie = trieCreateNode();
	
	printf( "Inserting to trie...\t");
	while (fscanf( fp, "%s", str) == 1) // words file
	{	
		trieInsert( trie, str, dic_index);
		dic_index++;
	}
	
	printf( "[done]\n"); // Inserting to trie
	
	fclose( fp);
	
	return trie;
};


/* makes a permuterm trie for given dictionary file
	return	trie head node pointer
			NULL failure
*/ 
/*TRIE *dic2permute_trie( char *dicfile){
	TRIE *permute_trie;
	char str[100];
	char *permuterms[100];
	int num_p;
	int ret;
	FILE *fp = fopen(dicfile, "rt");
	
	printf( "Inserting to trie...\t");
	while (fscanf( fp, "%s", str) == 1) // words file
	{	
		//ret = trieInsert( trie, str);
		
		if (ret)
		{
			num_p = make_permuterms( str, permuterms);		
		
			for (int i = 0; i < num_p; i++)
				//trieInsert( permute_trie, permuterms[i]);			
			
			clear_permuterms( permuterms, num_p);
		}
	}
	
	printf( "[done]\n"); // Inserting to trie

	fclose( fp);
};*/
