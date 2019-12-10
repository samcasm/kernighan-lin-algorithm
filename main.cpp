#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

struct v_l { 
    int v; 
    struct v_l * next; 
} ;
typedef struct v_l vertex_list;
// vertex_list * graph_part( int n, int k, vertex_list * edges[], int r );

void initNode(struct v_l *&head, int n){
    head->v = n;
    head->next = NULL;
}

void addNode(struct v_l *&head, int n){
    struct v_l *NewNode = new v_l;
    NewNode-> v = n;
    NewNode -> next = head;
    head = NewNode;
}


void swap(vector<int> &partition1, vector<int> &partition2, int pos1, int pos2)
{
	int temp = partition1[pos1];
	partition1[pos1] = partition2[pos2];
	partition2[pos2] = temp;
}

void partition(vector<int> &partition1, vector<int> &partition2, int s1, int s2, vector<int> &permanent_lock, int nodes)
{
	int i;
	
  	for( i=0; i<nodes; ++i)
	{
		if(permanent_lock[i]==false)
		{
  			if((rand() + i) % 2 == 1) 
  			{
  				if((int )partition1.size() == s1)
		  			partition2.push_back(i);	
				else if((int )partition2.size() == s2)
	  				partition1.push_back(i);
	  			else
	  				partition1.push_back(i);
			}
			else
			{
				if((int )partition1.size() == s1)
		  			partition2.push_back(i);	
		  		else if((int )partition2.size() == s2)
		  			partition1.push_back(i);
		  		else
		  			partition2.push_back(i);
			}
		}
	}

  // cout << "\n" << "This is first partition: " << endl;
  // for (int i=0;i<partition1.size();i++){
  //   if(partition1[i]!=-1)
	//     		cout<<partition1[i]<<" ";
	//     	else
	//     		cout << " this is the end "<<endl;
  // }
}

void initializeDValue(vector<int> &D, int nodes)
{
	int i;
	for(i=0;i<nodes;++i)
	{
		D[i]=0;
	}
}

int container(int node, vector<int> &partition1, vector<int> &partition2)
{
	if (find(partition1.begin(), partition1.end(), node) != partition1.end() )
		return 1;
	else
		return 2;
}

void findDValue(vector<int> &D, map< pair<int, int>, int> &edgeSet, vector<int> &partition1, vector<int> &partition2)
{
  	pair<int, int> p;
  	int node1, node2;
  	int container_node1, container_node2;
  	int edgeWeight;
  	
  	for(map< pair<int, int>, int>::iterator it=edgeSet.begin();it!=edgeSet.end();it++)
  	{
  		p = it->first;
  		edgeWeight = it->second;

  		node1 = p.first;
  		container_node1 = container(node1, partition1, partition2);

  		node2 = p.second;
		container_node2 = container(node2, partition1, partition2);

		if(container_node1 == container_node2)
		{
			D[node1] = D[node1] - edgeWeight;
			D[node2] = D[node2] - edgeWeight;
		}
  		else
  		{
  			D[node1] = D[node1] + edgeWeight;
  			D[node2] = D[node2] + edgeWeight;
		}
	}
}

void changeDValue(vector<int> &D, map< pair<int, int>, int> &edgeSet, int a, int b, vector<int> &partition1, vector<int> &partition2)
{
	int i, c1, c2, x, y;
	for(i=0;i<partition1.size();++i)
	{
		x=partition1[i];

		if(x != a && x != b)
		{
			if(edgeSet.find(make_pair(x, a)) != edgeSet.end())
				c1 = edgeSet[make_pair(x, a)];
			else if(edgeSet.find(make_pair(a, x)) != edgeSet.end())
				c1 = edgeSet[make_pair(a, x)];
			else
				c1 = 0;
			
			if(edgeSet.find(make_pair(x, b)) != edgeSet.end())
				c2 = edgeSet[make_pair(x, b)];
			else if(edgeSet.find(make_pair(b, x)) != edgeSet.end())
				c2 = edgeSet[make_pair(b, x)];
			else
				c2 = 0;
			
			D[x] = D[x] + 2*c1 - 2*c2;				
		}
	}
	
	for(i=0; i < partition2.size(); ++i)
	{
		y = partition2[i];
		
		if(y != a && y != b)
		{
			if(edgeSet.find(make_pair(y, a)) != edgeSet.end())
				c1 = edgeSet[make_pair(y, a)];
			else if(edgeSet.find(make_pair(a, y)) != edgeSet.end())
				c1 = edgeSet[make_pair(a, y)];
			else
				c1 = 0;
			
			if(edgeSet.find(make_pair(y, b)) != edgeSet.end())
				c2 = edgeSet[make_pair(y, b)];
			else if(edgeSet.find(make_pair(b, y)) != edgeSet.end())
				c2 = edgeSet[make_pair(b, y)];
			else
				c2 = 0;
			
			D[y] = D[y] + 2*c2 - 2*c1;				
		}
	}
}


void KLAlgo(map< pair<int, int>, int> &edgeSet, vector<int> &ans, vector<int> &permanent_lock, vector<int> &D, int K1, int nodes, int inodes)
{	

	int i, j, k;
	int s1 = inodes-(inodes/K1), s2 = inodes/K1;
	vector<int> partition1, partition2;
	vector<int> lock(nodes, false);		

	partition(partition1, partition2, s1, s2, permanent_lock, nodes);
	initializeDValue(D, nodes);
	findDValue(D, edgeSet, partition1, partition2);

	int g, ig;
	int pnode1, pnode2;
	vector<int> gval;
	vector<int> pos1, pos2;
	bool gfound;
	int c;

  cout << partition1.size() << " " << partition2.size() << " " << s2;
	for( k=0; k < s2/2; ++k)
	{
		gfound=false;
		for(i=0; i<partition1.size(); ++i)
		{
			for(j=0; j<partition2.size(); ++j)
			{
				if(lock[partition1[i]]==false && lock[partition2[j]]==false)
				{
					if(edgeSet.find(make_pair(partition1[i], partition2[j])) != edgeSet.end())
						c = edgeSet[make_pair(partition1[i], partition2[j])];
					else if(edgeSet.find(make_pair(partition2[j], partition1[i])) != edgeSet.end())
						c = edgeSet[make_pair(partition2[j], partition1[i])];
					else
						c=0;
							
					if(gfound == false)
					{
						g = D[partition1[i]] + D[partition2[j]] - 2*(c);
						
						pnode1 = i;
						pnode2 = j;
						gfound = true;
 					}
 					else
 					{
 						ig = D[partition1[i]] + D[partition2[j]] - 2*(c);
 						
 						if(ig>g)
 						{
 							g = ig;
							pnode1 = i;
							pnode2 = j;
						}
					}
				}
			}
		}
		
		lock[ partition1[pnode1] ] = true;
		pos1.push_back(pnode1);
		
		pos2.push_back(pnode2);
		lock[ partition2[pnode2] ] = true;
			
		gval.push_back(g);

		changeDValue(D, edgeSet, partition1[pnode1], partition2[pnode2], partition1, partition2);
	}
	
	int maxg=gval[0], imaxg=gval[0], K=1;
	for( i=1; i < gval.size(); ++i)
	{
		imaxg = imaxg+gval[i];
		if(imaxg>maxg)
		{
			maxg = imaxg;
			K = i;
		}
	}
		
	if(maxg<0 && K==1)
		K = -1;
		
	for(i=0;i<K;++i)
	{
		swap(partition1, partition2, pos1[i], pos2[i]);
	}
	
	if((K1-1) == 1)
	{
		for(i = 0; i < partition1.size(); ++i)
		{
			ans.push_back(partition1[i]);
			permanent_lock[partition1[i]]=1;
		}
		ans.push_back(-1);
	}

	for(i=0;i<partition2.size();++i)
	{
		ans.push_back(partition2[i]);
		permanent_lock[partition2[i]]=1;
	}
	ans.push_back(-1);

}



vertex_list * graph_part( int n, int k, vertex_list * edges[], int r ){

    int i, j, start_s, stop_s, inodes;
    int K1 = 2;
    int nodes = n;
    inodes = nodes;

    map<pair<int,int>,int> edgeSet;
    std::vector<int> vertices;
    std::vector<int> ans;
    
    // permanent lock
    std::vector<int> permanent_lock(nodes, false);

    // D values
    std::vector<int> D(nodes, 0);

    // making graph edgeset
    for(int i=0; i<1000; i++){
      vertex_list * new_node = edges[i];
      // cout << " "<< i << " " << new_node->v << " " << new_node->next->v << endl;
      vertices.push_back(i);
      edgeSet[make_pair(new_node->v, new_node->next->v)] = 1;
      edgeSet[make_pair(new_node->next->v, new_node->v)] = 1;

    }

	if(nodes == K1)
	{
		cout<<"Given graph is already divided into "<<K1<<" partitions"<<endl;
	}
	else
	{
		while(K1 != 1)
		{
			KLAlgo(edgeSet, ans, permanent_lock, D, K1, nodes, inodes);
			inodes = inodes - (inodes/K1);
			--K1;
		}
		
		
		
	}

    struct v_l *head = new v_l;
    
    initNode(head, ans[0]);


    for (int i=1; i<ans.size();i++){
      if (ans[i] != -1){
        addNode(head, ans[i]);
      }else{
        break;
      }
    }

    return head;
    // struct v_l* cur  = head;
    // do {
    //     printf("Node @ %p : %i\n",(void*)cur, cur->v );
    // } while ( ( cur = cur->next ) != NULL );

    // vertex_list * new_list = edges[5];

    // cout << new_list->v << " the vertice" << endl;
    // cout << new_list ->next->v << " next vertice" << endl;
    // // cout << new_list ->next->next<< " next vertice" << endl;

    // cout << "Here now";

    // return new_list;
}

int main()
{   vertex_list * graph[1000], *result, *e, *tmp;
    int i,j, cross;
    int partition[1000];

    /* make empty graph */    
    e = (vertex_list *) malloc( 10000* sizeof(vertex_list) );
    for(i=0; i<1000; i++)
      graph[i] = NULL;
    printf("Test 1: A cycle with 1000 vertices\n");
    /* now fill it as a cycle */
    for(i=0,j=0; i<1000; i++)
      {  graph[i] = e+j; j+=1; /*next available edge node */
	(graph[i])->v = (i+999)%1000;
        (graph[i])->next = e+j; j+=1; /*next available edge node */
        (graph[i])->next->v = (i+1)%1000;
        (graph[i])->next->next = NULL;
      }
    printf("Made a cycle, now try to partition it\n"); fflush(stdout);
    result = graph_part(1000, 500, graph, 100);
    for(i=0; i<1000; i++)
       partition[i]=0;
    for( i=0, tmp=result; tmp != NULL; tmp=tmp->next )
    {   partition[tmp->v]=1; i+=1; }
    if( i != 500 )
      printf("the partition has %d vertices, not 500\n", i);fflush(stdout);
    for(i=0, cross = 0; i< 1000; i++)
    {  for( tmp = graph[i]; tmp != NULL; tmp = tmp-> next )  
       {  j = tmp->v;
	  if(partition[i] != partition[j])
             cross +=1;
       }
    }
    printf("Partition has %d crossing edges\n\n", cross/2); fflush(stdout);
    printf("Test 2: 300 cycles with 3 vertices each\n");
    /* now fill the graph*/
    for(i=0,j=0; i<300; i++)
    {    graph[3*i] = e+j; j+=1; /*next available edge node */
	(graph[3*i])->v = 3*i+1;
        (graph[3*i])->next = e+j; j+=1; /*next available edge node */
        (graph[3*i])->next->v = 3*i+2;
        (graph[3*i])->next->next = NULL;
         graph[3*i+1] = e+j; j+=1; /*next available edge node */
	(graph[3*i+1])->v = 3*i;
        (graph[3*i+1])->next = e+j; j+=1; /*next available edge node */
        (graph[3*i+1])->next->v = 3*i+2;
        (graph[3*i+1])->next->next = NULL;
         graph[3*i+2] = e+j; j+=1; /*next available edge node */
	(graph[3*i+2])->v = 3*i;
        (graph[3*i+2])->next = e+j; j+=1; /*next available edge node */
        (graph[3*i+2])->next->v = 3*i+1;
        (graph[3*i+2])->next->next = NULL;
    }
    printf("Made 300 K_, now try to partition it\n"); fflush(stdout);
    result=graph_part(900, 300, graph, 100);
    for(i=0; i<9000; i++)
       partition[i]=0;
    for( i=0, tmp=result; tmp != NULL; tmp=tmp->next )
    {   partition[tmp->v]=1; i+=1; }
    if( i != 300 )
      printf("the partition has %d vertices, not 300\n", i);fflush(stdout);
    for(i=0, cross = 0; i< 900; i++)
    {  for( tmp = graph[i]; tmp != NULL; tmp = tmp-> next )  
       {  j = tmp->v;
	  if(partition[i] != partition[j])
             cross +=1;
       }
    }
    printf("Partition has %d crossing edges\n", cross/2); fflush(stdout);

    exit(0);
} 