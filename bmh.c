#include<stdio.h>
#include<string.h>
#include<stdlib.h>

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

void bmh(char t[],char p[])
{
	int *locc;
	int i0,j,m,n;
	n=strlen(t);
	m=strlen(p);
	locc=lastocc(p);
	i0=0;
	while(i0<=(n-m))
	{
		j=m-1;
		while(j>=0 && p[j]==t[i0+j])
			j--;

		if(j<0)
		{
			printf("Pattern found at %d\n",i0);
		}

		i0+=m-1;
		i0-=locc[t[i0]];
	}
}


int main()
{
	char pat[10],text[256];
	int n,m;
	scanf("%s",text);
	scanf("%s",pat);
	printf("text : %s pattern : %s\n",text,pat);
	bmh(text,pat);
	return 0;
}
