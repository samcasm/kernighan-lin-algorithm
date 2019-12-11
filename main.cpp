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

// to create result linked list

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

// to swap elements of the two partitions

void swap(vector<int> &partition1, vector<int> &partition2, int pos1, int pos2)
{
	int temp = partition1[pos1];
	partition1[pos1] = partition2[pos2];
	partition2[pos2] = temp;
}

// create initial partitions of k and n-k
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

	//to print both partitions
  // cout << "\n" << "This is first partition: " << endl;
  // for (int i=0;i<partition1.size();i++){
  //   if(partition1[i]!=-1)
	//     		cout<<partition1[i]<<" ";
	//     	else
	//     		cout << " this is the end "<<endl;
  // }
}

// initialize a DValues vector
void initializeDValue(vector<int> &D, int nodes)
{
	int i;
	for(i=0;i<nodes;++i)
	{
		D[i]=0;
	}
}

// check which partition the vertice lies in
int in_which_container(int node, vector<int> &partition1, vector<int> &partition2)
{
	if (find(partition1.begin(), partition1.end(), node) != partition1.end() )
		return 1;
	else
		return 2;
}

// find initial D values
void findDValue(vector<int> &D, map< pair<int, int>, int> &edgeSet, vector<int> &partition1, vector<int> &partition2)
{
	// edgeSet: vector<pair (vertice1, vertice2)> , <pair (vertice2, vertice1)> as graph is undirected

  	pair<int, int> p;
  	int node1, node2;
  	int container_node1, container_node2;
  	int edgeWeight;
  	
  	for(map< pair<int, int>, int>::iterator it=edgeSet.begin();it!=edgeSet.end();it++)
  	{
  		p = it->first;
  		edgeWeight = it->second;

  		node1 = p.first;
  		container_node1 = in_which_container(node1, partition1, partition2);

  		node2 = p.second;
		container_node2 = in_which_container(node2, partition1, partition2);

		// if both nodes in same container

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

// update D values for all vertices in both partitions except a and b
// where a from partition A and b from partition B, and g = D[a] + D[b] – 2*c(a, b) is maximal
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

// permanent_lock: vector that keeps track of vertices to remove from further considerations
// K1: number of partitions
// n_vertices: Total vertices
// rem_vertices: Remaining vertices left to be considered in the next iteration

void KLAlgo(map< pair<int, int>, int> &edgeSet, vector<int> &answer, vector<int> &permanent_lock, vector<int> &D, int K1, int n_vertices, int rem_vertices)
{	

	int i, j, k;
	int s1 = rem_vertices-(rem_vertices/K1), s2 = rem_vertices/K1;
	vector<int> partition1, partition2;
	vector<int> lock(n_vertices, false);		

	partition(partition1, partition2, s1, s2, permanent_lock, n_vertices);
	initializeDValue(D, n_vertices);
	findDValue(D, edgeSet, partition1, partition2);

	int g, ig;
	int vertice1, vertice2;
	vector<int> gval;
	vector<int> pos1, pos2;
	bool gfound;
	int c;

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
						
						vertice1 = i;
						vertice2 = j;
						gfound = true;
 					}
 					else
 					{
 						ig = D[partition1[i]] + D[partition2[j]] - 2*(c);
 						
 						if(ig>g)
 						{
 							g = ig;
							vertice1 = i;
							vertice2 = j;
						}
					}
				}
			}
		}
		
		lock[ partition1[vertice1] ] = true;
		pos1.push_back(vertice1);
		
		pos2.push_back(vertice2);
		lock[ partition2[vertice2] ] = true;
			
		gval.push_back(g);

		changeDValue(D, edgeSet, partition1[vertice1], partition2[vertice2], partition1, partition2);
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
			answer.push_back(partition1[i]);
			permanent_lock[partition1[i]]=1;
		}
		answer.push_back(-1);
	}

	for(i=0;i<partition2.size();++i)
	{
		answer.push_back(partition2[i]);
		permanent_lock[partition2[i]]=1;
	}
	answer.push_back(-1);

}



vertex_list * graph_part( int n, int k, vertex_list * edges[], int r ){
    int i, j, rem_vertices;
    int K1 = n/k;
    int n_vertices = n;
    rem_vertices = n_vertices;

    map<pair<int,int>,int> edgeSet;
    std::vector<int> vertices;
    std::vector<int> answer;
    
    // permanent lock vector init
    std::vector<int> permanent_lock(n_vertices, false);

    // D values vector init
    std::vector<int> D(n_vertices, 0);

    // making graph edgeset where 
	//<vector> : pair(vertice1, vertice2) = 1, pair(vertice2, vertice1) = 1
	// as the graph is undirected and unweighted
    for(int i=0; i<n; i++){
      vertex_list * new_node = edges[i];
      // cout << " "<< i << " " << new_node->v << " " << new_node->next->v << endl;
      vertices.push_back(i);
      edgeSet[make_pair(new_node->v, new_node->next->v)] = 1;
      edgeSet[make_pair(new_node->next->v, new_node->v)] = 1;

    }

	if(n_vertices == K1)
	{
		cout<<"Given graph is already divided into "<<K1<<" partitions"<<endl;
	}
	else
	{
		while(K1 != 1)
		{
			KLAlgo(edgeSet, answer, permanent_lock, D, K1, n_vertices, rem_vertices);
			rem_vertices = rem_vertices - (rem_vertices/K1);
			--K1;
		}			
		
	}

    struct v_l *head = new v_l;
    
    initNode(head, answer[0]);


    for (int i=1; i<answer.size();i++){
      if (answer[i] != -1){
        addNode(head, answer[i]);
      }else{
        break;
      }
    }
    // cout << ans.size() << " size" << endl;
    return head;

    // struct v_l* cur  = head;
    // do {
    //     printf("Node @ %p : %i\n",(void*)cur, cur->v );
    // } while ( ( cur = cur->next ) != NULL );

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
    result = graph_part(900, 300, graph, 100);
    for(i=0; i<900; i++)
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