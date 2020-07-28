#define DEBUG 0

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

// 토큰-문서 구조체
typedef struct {
	char	*token;	// 토큰
	int		docid;	// 문서번호(document ID)
} tTokenDoc;

typedef struct {
	int		index;	// starting position in posting.idx
	int		df;		// 문서 빈도(document frequency)
} tHEADER;

////////////////////////////////////////////////////////////////////////////////

void invertedIndex( tTokenDoc *tokens, int num_tokens, 
					char *dicfilename, char *headerfilename, char *postingfilename);
tTokenDoc *get_tokens(char *filename, int *num_tokens);
static int _compare(const void *n1, const void *n2);

// 토큰 구조체로부터 역색인 파일을 생성한다.
void invertedIndex( tTokenDoc *tokens, int num_tokens, char *dicfilename, char *headerfilename, char *postingfilename){		//fwrite 에서 반복 횟수를 몇 번으로?	
	
	tHEADER *header = (tHEADER*)malloc(sizeof(tHEADER)*num_tokens);
	int *posting = (int*)malloc(sizeof(int)*num_tokens);
	int j = 0;
	int k = 0;
	
	FILE *fp = fopen(dicfilename, "w");
	int dic = 1;
	if(fp==NULL){
		printf("File Open Error\n");
		return;
	}
	
	fprintf(fp, "%s\n", tokens[0].token);
	
	for(int i = 1; i<num_tokens; i++){
		if(strcmp(tokens[i].token, tokens[i-1].token) != 0){
			fprintf(fp, "%s\n", tokens[i].token);
			dic++;
		}
	}
	
	fclose(fp);
	
	//////////////////////////////////////////////////////////////////////////
	
	FILE *fpp = fopen(headerfilename, "wb");
	if(fpp==NULL){
		printf("File Open Error\n");
		return;
	}
	
	header[0].df = 1;
	header[0].index = 0;
	for(int i = 1; i<num_tokens; i++){
		if (strcmp(tokens[i].token, tokens[i-1].token) != 0){
			j ++;
			header[j].df = 1;
			header[j].index = header[j-1].df + header[j-1].index;
		}
		else{
			if(tokens[i].docid != tokens[i-1].docid){
				header[j].df ++;
			}
		}
	}
	
	fwrite(header, sizeof(tHEADER), j+1, fpp);		//j=25610
	fclose(fpp);

	
	
	
	FILE *fppp = fopen(postingfilename, "wb");
	if(fppp == NULL){
		printf("File Open Error\n");
		return;
	}
	
	posting[0] = tokens[0].docid;
	for(int i = 1; i<num_tokens; i++){
		if (strcmp(tokens[i].token, tokens[i-1].token) != 0){
			k++;
			posting[k] = tokens[i].docid;
		}
		else{
			if(tokens[i].docid != tokens[i-1].docid){
				k++;
				posting[k] = tokens[i].docid;
			}
		}
	}
	
	fwrite(posting, sizeof(int), k+1, fppp);		//k=403328	
	fclose(fppp);
	
	/*FILE *asdf = fopen("asdf.txt", "w");
	for(int i = 0; i<j+1; i++){
		fprintf(asdf, "%d\t%d\n", header[i].index, header[i].df);
	}
	fclose(asdf);*/
	
	//////////////////////////////////////////////////////////////////////////
	
	
	/*FILE *post = fopen("post.txt", "w");
	for(int i = 0; i<k+1; i++){
		fprintf(post, "%d\n", posting[i]);
	}
	fclose(post);*/
	
	free(header);
	free(posting);
}

// 입력 파일로부터 토큰-문서 구조체를 생성한다.
tTokenDoc *get_tokens(char *filename, int *num_tokens){		//여기에 자꾸 공백문자 들어감.
	char tmp[5000];
	char *ptr;
	char *find;
	tTokenDoc *t = (tTokenDoc*)malloc(sizeof(tTokenDoc)*624457);
	
	int i = 0;
	int j = 1;
	int dou;
	
	FILE *fp = fopen(filename, "r");
	if(fp==NULL){
		printf("File Open Error\n");
		return NULL;
	}
	
	while(!feof(fp)){
		fgets(tmp, sizeof(tmp), fp);
		int a = strlen(tmp);
		
		find = strchr(tmp, '\n');
		if(find != NULL)
			*find = '\0';
		
		
		ptr = strtok(tmp, " ");
		
		while(ptr != NULL){
			if(strcmp(ptr, " ") != 0){
			t[i].token = strdup(ptr);
			t[i].docid = j;
			
			i++;
			}
			ptr = strtok(NULL, " ");
		}
		j++;
	}
	
	*num_tokens = i;
	fclose(fp);

	return t;
}

// qsort를 위한 비교함수 (첫번째 정렬 기준: 토큰 문자열, 두번째 정렬 기준: 문서 번호)
static int _compare(const void *n1, const void *n2){
	tTokenDoc *a = (tTokenDoc*)n1;
	tTokenDoc *b = (tTokenDoc*)n2;
	
	if(strcmp(a->token, b->token)==0){
		if(a->docid < b->docid)
			return -1;
		else if(a->docid > b-> docid)
			return 1;
		else
			return 0;
	}
	else
		return strcmp(a->token, b->token);
}

////////////////////////////////////////////////////////////////////////////////
static void print_tokens( tTokenDoc *tokens, int num_tokens)
{
	int i;
	
	for (i = 0; i < num_tokens; i++)
	{
		printf( "%s\t%d\n", tokens[i].token, tokens[i].docid);
	}
}

////////////////////////////////////////////////////////////////////////////////
void destroy_tokens( tTokenDoc *tokens, int num_tokens)
{
	int i;
	
	for (i = 0; i < num_tokens; i++)
	{
		free( tokens[i].token);
	}
	free( tokens);
}

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	tTokenDoc *tokens;
	int num_tokens = 0;

	if (argc != 2)
	{
		printf( "Usage: %s FILE\n", argv[0]);
		return 2;
	}
	
	tokens = get_tokens( argv[1], &num_tokens);			//토큰을 잘라! 만들어!
	
	assert( tokens != NULL && num_tokens > 0);

#if DEBUG
	print_tokens( tokens, num_tokens);
#endif

	// 정렬 (첫번째 정렬 기준: 토큰 문자열, 두번째 정렬 기준: 문서 번호)
	qsort( tokens, num_tokens, sizeof( tTokenDoc), _compare);		//토큰들을 정렬해!
	FILE *token_file = fopen("token.txt", "w");
	for(int i = 0; i<num_tokens; i++){
		fprintf(token_file, "%d\t%s\n", tokens[i].docid, tokens[i].token);
	}
	fclose(token_file);
	invertedIndex( tokens, num_tokens, "dic.txt", "header.idx", "posting.idx");		//역색인을 만들어! 즉, 파일들을 생성!

#if DEBUG	
	print_tokens( tokens, num_tokens);
#endif	

	destroy_tokens( tokens, num_tokens);
	
	return 0;
}