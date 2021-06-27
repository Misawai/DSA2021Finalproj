#include "api.h"
#include <stdio.h>
#include<stdbool.h>
#define MAX 10000
// The testdata only contains the first 100 mails (mail1 ~ mail100)
// and 2000 queries for you to debug.

int n_mails, n_queries;
mail *mails;
query *queries;

typedef struct disjoint_tree{
	struct disjoint_tree *parent;
	int rank;
}Tree;

Tree ds[MAX];
bool set[MAX];
int largecount[MAX];
char com[MAX][32]={};
char find_set(const char *data);
char find_set(const char *data);
int hash(const char* s)
{
	for(int i=0;i<MAX;i++)
	{
		if (strcmp(s,com[i])==0)
		{
			return i;
		}
		
	}
}

//source from TA's JUDGE PID 50//START
inline void static init(const char* data) {
    int i = hash(data);
    if (!set[i]) {
        makeset(data);
        set[i] = true;
    }
}
//source from TA's JUDGE PID 50//END
//Method 2 
char make_set(char *data)
{
	int i=hash(data);//TBC
	ds[i].parent=i;
	ds[i].rank=0;
	//have to find a proper hash function inorder to sort names properly
	//maybe using linear hashing is a good way, however it might be time consuming
	
}
bool only(const int i)
{
	if (ds[i].parent==i)
	{
		return true;
	}
}
char find_set(const char *data)
{
	init(data);
	int i=hash(data);
	if (ds[i].parent==i)
	{
		return ds[i].parent;
	}
	else
	{
		return cset(i);
	}
	
}
//SOURCE FROM TA's DSAJUDGE PID50
int cset(int i){
	if (ds[i].parent==i)
	{
		return ds[i].parent;
	}
	else{
		ds[i].parent=cset(ds[i].parent);
	}
   return ds[i].parent;
}
//SOURCE FROM TA's DSAJUDGE PID50

char linkage(const char *ra,const char *rb)
{
	int a=find_set(ra), b=findset(rb);
	if (ds[a].rank>ds[b].rank)
	{
		ds[b].parent=a;
	}
	else{
		ds[a].parent=b;
		if(ds[a].rank==ds[b].rank)
		{
			ds[b].rank++;
		}
	}
}
int count_group()
{
	int count=0;
	for(int i=0;i<sizeof(ds);i++)
	{
		if (set[i])//if this set exist
		{
			if(only(i))//if this set is the parent
			{
				count++;//++
			}
		}
	}
	return count;
}
int largest_group()
{
	int temp=0;
	for(int i=0;i<sizeof(ds);i++)
	{
		//traverse all elements, count their findset parents and add them into a hash table.
		largecount[findset(ds)]=largecount[findset(ds)]+1;
	}
	for (int i=0;i<sizeof(largecount);i++)
	{
		if (temp<largecount[i])
		{
			temp=largecount[i];
		}
	}
	return temp;
}
void Group_Analysis(int len,const mail *mids[],int ans[])
{
	//disjoint set noneed to compressed since only query once or....
	//if many queries impelemented, path compression is needed.
	//I think I need to decleare a hash table to do a "correspond table of ID and index.
	for(int i=0;i<sizeof(mids);i++)
	{
		makeset(mids[i]->from);
		makeset(mids[i]->to);
		linkage(mids[i]->from,mids[i]->to);
	}	
	//Method 3 should be better when calculating the number of nodes in each group.
	//Method 2 should be OK but code might be troublesome.
	//however, if we change the value in array become the index of root, than counting will be more faster. 
	//But overall, Method 3 has more stability and better to maintain, since we are going to do path compression.
	//path compression is necessary?
	//actually for 10000 mails and 20000 maximum possible names, it seems be necessary since it get makeset O(1) and findset O(1)
	ans[0]=count_group();
	ans[1]=largest_group();
	
}

int main(void) {
	api.init(&n_mails, &n_queries, &mails, &queries);

	for(int i = 0; i < n_queries; i++)
		if(queries[i].type == expression_match)
		  api.answer(queries[i].id, NULL, 0);

  return 0;
}
