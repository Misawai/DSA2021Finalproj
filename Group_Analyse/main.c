#include<api.h>
#include<stdio.h>
int main()
{
	int n_mails, n_queries;
	mail *mails;
	query queries;
	void api.init(int *n_mails, int *n_queries, mail **mails, query **queries);//init
	//using disjoinnt set to implememt
	//For two different people, if A sends a mail to B in the mail set, then A is related to B and B is related to A.
	//struct int len(size of the set) int mids[512](id's)
	
}