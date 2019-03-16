#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<omp.h>

#define NUM_THREADS 4

int occ[256];

int*  lastocc(char str[])
{
	int i;
	char a;
	for(i=0;i<128;i++)
		occ[i]=-1;
	int len=strlen(str);
	for(i=0;i<len-1;i++)
	{
		a=str[i];
		occ[a]=i;
	}
	return occ;
}

void bmh(char *t,int start,int end,char *p)
{
	int *locc;
	int i0,j,m,n;
	n=end-start+1;
	m=strlen(p);
	locc=lastocc(p);
	i0=start;
	while(i0<=end-m+1)
	{
	 
		j=m-1;
		while(j>=0 && p[j]==t[i0+j])
			j--;
		if(j<0)
			printf("Pattern found at %d\n",i0);
		i0+=m-1;
		i0-=locc[t[i0]];
	}
}

int main()
{
	char pat[10];
	char *text;
	int n,m,i=0;
	size_t size = 0;

	/* Open your_file in read-only mode */
	FILE *fp = fopen("gene.txt", "r");

	/* Get the buffer size */
	fseek(fp, 0, SEEK_END); /* Go to end of file */
	size = ftell(fp); /* How many bytes did we pass ? */

	/* Set position of stream to the beginning */
	rewind(fp);

	/* Allocate the buffer (no need to initialize it with calloc) */
	text = malloc((size + 1) * sizeof(*text)); /* size + 1 byte for the \0 */

	/* Read the file into the buffer */
	fread(text, size, 1, fp); /* Read 1 chunk of size bytes from fp into buffer */

	/* NULL-terminate the buffer */
	text[size] = '\0';

	scanf("%s",pat);

	int lenp=strlen(pat);
	printf("Length of pattern: %d\n",lenp);
	printf("Length of pattern: %d\n",strlen(text));

	int bs=strlen(text)/NUM_THREADS;
	int rem=strlen(text)%NUM_THREADS;
	printf("bs: %d rem: %d\n",bs,rem);
	printf("num of threads %d\n",NUM_THREADS);
	int tid,start,end;

	#pragma omp parallel num_threads(NUM_THREADS) private(tid,start,end) shared(text,pat,rem,bs,m)
	{
		tid=omp_get_thread_num();
		printf("tid  %d\n",tid);
		if(tid==0)
		{
			#pragma omp critical (part1)
			{
				start=tid;
				end=bs-1;
				printf("start: %d end: %d\n",start,end);
				printf("tid= %d  text block : %d ... %d\n",tid,start,end);
				bmh(text,start,end,pat);
			}
		}
		else
		{
			#pragma omp critical (part2)
			{
				start=(tid*bs)-lenp;
				end=(tid*bs)+bs-1;
				printf("start: %d end: %d\n",start,end);
				printf("tid= %d  text block : %d ... %d\n",tid,start,end);
				bmh(text,start,end,pat);
			}
		}
	}

	if(rem!=0)
		bmh(text,(NUM_THREADS+1)*bs,strlen(text),pat);
	return 0;
}
