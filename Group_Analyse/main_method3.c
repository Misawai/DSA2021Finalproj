#include<api.h>
#include<stdio.h>
#define MAX 10000
int main()
{
	int n_mails, n_queries;
	mail *mails;
	query queries;
	api.init(&n_mails, &n_queries, &mails, &queries);//init
	//using disjoinnt set to implememt
	//For two different people, if A sends a mail to B in the mail set, then A is related to B and B is related to A.
	//struct int len(size of the set) int mids[512](id's)
	
}
typedef struct node{
	struct headtail * head;
	char name[256];
	struct node *next;
}Node;

typedef struct headtail{
	struct node head;
	struct node tail;
}HeadTail;

HeadTail list[MAX];

int hash(char input)
{
	
}

mail make_set(char in)
{
	if ()
	//have to find a proper hash function inorder to sort names properly
	//maybe using linear hashing is a good way, however it might be time consuming
	
}
mail find_set(const mail *data)
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

mail link(const mail *ra,const char *rb)
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
void Group_Analysis(int len,const mail *mids[])
{
	//disjoint set noneed to compressed since only query once or....
	//if many queries impelemented, path compression is needed.
	//I think I need to decleare a hash table to do a "correspond table of ID and index.
	for(int i=0;i<sizeof(mids);i++)
	{
		makeset(mids[i]->from);
	}
	
	
	
}
int probing()
{
	
}