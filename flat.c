/*
 * Writen by wangyanpeng@gmail.com on 2011.05.29
 * Add comments on 2016.11.18
 */

#include "stdio.h"
#include "string.h"

/*
 * For each number, we define a data structure Node as below.
 * Node has several links pointing to two trees, one is
 * sequence tree, another is avl tree.
 *
 * There will be many seperated sequence trees. 
 * Each sequence tree is a subset of nodes which meet the relationship
 * of k-flat. Only insert operation will occur on sequence tree.
 *
 * There will be many binary trees (the nodes of each avl tree 
 * have same cur_length), while they are connected with 
 * each other via avl_nexttree and avl_previoustree as a 
 * bidirection link list. 
 * The avl trees are sorted by root's data_val in descending order.
 */
typedef struct Node
{
	int data_val;
    int cur_length;
    int degree; 
    int myindex;                           /* sequence number, from 1 to n */
    int min_k;                             /* left boundary = data_val - k */
    int max_k;                             /* right bounddary = data_val + k */
    int bf;                                /* balance factor */
	struct Node * seqTree_leftchild;
    struct Node * seqTree_rightchild;
    struct Node * seqTree_parent;
    struct Node * nextprint;
    struct Node * avl_leftchild;
    struct Node * avl_rightchild;
    struct Node * avl_nexttree;
    struct Node * avl_previoustree;
} *pMyNode, MyNode;


pMyNode AVLTreeList_Head = new MyNode;	 

void BBT_L_Rotate(pMyNode & root)            
{
 pMyNode rc=root->avl_rightchild;                 
 if ( rc == 0) return;
 root->avl_rightchild=rc->avl_leftchild;                 
 rc->avl_leftchild=root;                         
 root=rc;                                 
}


void BBT_R_Rotate(pMyNode & root)             
{
 pMyNode lc=root->avl_leftchild;                  
 if (lc == 0 ) return;
 root->avl_leftchild=lc->avl_rightchild;                  
 lc->avl_rightchild =root;                         
 root=lc;                                  
}


void LeftBalance(pMyNode & root)     
{

	if (root->avl_leftchild == 0)
		return;
 pMyNode lc=root->avl_leftchild,rc=NULL;          
 if(lc->bf==1)                             
 {
  root->bf=lc->bf=0;                    
  BBT_R_Rotate(root);                   
 }
 else if(lc->bf==-1)                       
 {
  rc=lc->avl_rightchild;                        
  if ( rc == 0 ) return;
  if(rc->bf==1)                         
  {
   root->bf=-1;
   lc->bf=0;
  }
  else if(rc->bf==0)
  {
   root->bf=0;
   lc->bf=0;
  }
  else
  {
   root->bf =0;
   lc->bf =1;
  }
  rc->bf=0;
  BBT_L_Rotate(root->avl_leftchild);               
  BBT_R_Rotate(root);                       
 }
 else if (lc->avl_rightchild != 0) 
 {
  rc=lc->avl_rightchild;
  if(rc->bf==1)
  {
   root->bf=-1;
   lc->bf=1;
   rc->bf=1;
  }
  else if(rc->bf==0)
  {
   root->bf=1;
   lc->bf=1;
   rc->bf=1;
  }
  else
  {
   root->bf =0;
   lc->bf =2;
   rc->bf=0;
  }
  
  BBT_L_Rotate(root->avl_leftchild);
  BBT_R_Rotate(root);
  if(root->avl_leftchild->bf==2)        
  {
   root->avl_leftchild->bf=0;
   if (root->avl_leftchild->avl_leftchild != 0)
	 root->avl_leftchild->avl_leftchild->bf=0;
   BBT_R_Rotate(root->avl_leftchild);
  }
 }
}


void RightBalance(pMyNode & root)       
{

 if ( root->avl_rightchild == 0 ) 
	 return;
 pMyNode rc=root->avl_rightchild,lc=NULL;
 if(rc->bf==-1)                     
 {
	  rc->bf=root->bf=0;
	  BBT_L_Rotate(root);
 }
 else if ((rc->bf==1) && (rc->avl_leftchild != 0))                 
 {
	  lc=rc->avl_leftchild;
	  if(lc->bf==1)
	  {
	   rc->bf=0;
	   root->bf =-1;
	  }
	  else if(lc->bf==0)
	  {
	   root->bf=rc->bf=0;
	  }
	  else
	  {
	   root->bf =1;
	   rc->bf =0;
	  }
	  lc->bf=0;
	  BBT_R_Rotate(root->avl_rightchild);
	  BBT_L_Rotate(root);
 }
 else if (rc->avl_leftchild != 0)
 {
	  lc=rc->avl_leftchild;
	  if(lc->bf==1)                       
	  {
	   rc->bf=-2;
	   root->bf =0;
	   lc->bf=0;
	  }
	  else if(lc->bf==0)
	  {
	   root->bf=0;
	   rc->bf=-1;
	   lc->bf=-1;
	  }
	  else
	  {
	   root->bf =1;
	   rc->bf =-1;
	  }
  
	  BBT_R_Rotate(root->avl_rightchild);
	  BBT_L_Rotate(root);
	  if(root->avl_rightchild->bf==-2)
	  {
	   root->avl_rightchild->bf=0;
	   if(root->avl_rightchild->avl_rightchild != 0)
		   root->avl_rightchild->avl_rightchild->bf=0;
	   BBT_L_Rotate(root->avl_rightchild);
	  }
 }
 
}


bool BBT_Insert(pMyNode & now,pMyNode & nowparent,bool & taller,pMyNode & pNewNode)   
{                                                       
 bool result=false;                               
 if(!now)                                         
 {
  now = pNewNode;                                
  now->bf=0;                                   
  if (now->data_val <= nowparent->data_val)
	 nowparent->avl_leftchild = now;
  else
	 nowparent->avl_rightchild = now;
  taller=true;                                 
  return true;                                 

 }
 else if(pNewNode->data_val < now->data_val)                          
 {
  result=BBT_Insert(now->avl_leftchild,now,taller,pNewNode);   
  if(taller)                                    
  {                                             
   if(now->bf==-1)                           
   {
    now->bf=0;                            
    taller=false;                         
   }
   else if(now->bf==0)
   {
    now->bf =1;                            
    taller=true;                           
   }
   else
   {
    LeftBalance(now);                      
    taller=false;                          
   }
  }
 }
 else if(pNewNode->data_val > now->data_val)                             
 {
  result=BBT_Insert(now->avl_rightchild,now,taller,pNewNode);     
  if(taller)
  {
   if(now->bf==-1)
   {
    RightBalance(now);
    taller=false;
   }
   else if(now->bf==0)
   {
    now->bf=-1;
    taller=true;
   }
   else
   {
    now->bf=0;
    taller=false;
   }
  }
 }
 return result;                                       
}

void BBT_Del(pMyNode & root,pMyNode & del_Node,bool & shorter,bool & suc,bool & del,bool & leaf)
{
 pMyNode p,f;
 if(!root)                                         
	 suc=false;     
 else if(root->data_val==del_Node->data_val)                         
 {
			  if(root->avl_leftchild==NULL&&root->avl_rightchild==NULL)    
			  {
					   leaf=del=true;                            
					   shorter=true;                             
			  }
			  else                             
			  {
					   if(root->avl_leftchild==NULL)
					   {
							p=root;                  
							root=root->avl_rightchild;       
							shorter=true;            
					   }
					   else 
					   {
							p=f=root->avl_leftchild;         
							while(p->avl_rightchild)          
							{
									 f=p;                  
									 p=p->avl_rightchild;          
							}
							if(p==f)                      
							{
									 p=root;                   
									 root=f;                   
									 root->avl_rightchild=p->avl_rightchild;   
									 if(p->bf==0)
									 {
										  shorter=false;        
										  root->bf=-1;          
									 }
									 else if(p->bf==1)         
									 {
										  shorter=true;         
										  root->bf=0;           
									 }
									 else
									 {
										  root->bf=p->bf-1;     
										  RightBalance(root);   
										  shorter=true;         
									 }

							}
							else
							{

									 f->avl_rightchild=p->avl_leftchild;      
									 p->avl_leftchild = root->avl_leftchild;
									 p->avl_rightchild = root->avl_rightchild;
									 root = p;

									 if(f->bf==0)              
									 {
										  shorter=false;        
										  f->bf=1;              
									 }
									 else if(f->bf==1)
									 {
										  LeftBalance(root->avl_leftchild);
										  shorter=true;
									 }
									 else
									 {
										  shorter=true;           
										  f->bf=0;                
									 }
									 if(shorter)                 
									 {
											  if(root->bf==0)
											  {
												   shorter=false;
												   root->bf=-1;
											  }
											  else if(root->bf==1)
											  {
												   shorter=true;
												   root->bf=0;
											  }
											  else
											  {
												   RightBalance(root);
												   shorter=true;
											  }
									 }
							}
					   }
			  }	
}
 else if(root->data_val > del_Node->data_val)        
 {
				  BBT_Del(root->avl_leftchild,del_Node,shorter,suc,del,leaf);   
				  if(del&&leaf)                       
				  {
					   root->avl_leftchild=NULL;              
					   del=false;                      
				  }
				  if(shorter)                        
				  {
					   if(root->bf==0)
					   {
							root->bf=-1;
							shorter=false;
					   }
					   else if(root->bf==1)
					   {
							root->bf=0;
							shorter=true;
					   }
					   else
					   {
							RightBalance(root);
							shorter=true;
					   }
				 }
 }
 else
 {
		  BBT_Del(root->avl_rightchild,del_Node,shorter,suc,del,leaf);
		  if(del&&leaf)
		  {
				   del=false;
				   root->avl_rightchild=NULL;
		  }
		  if(shorter)
		  {
			   if(root->bf==0)
			   {
					root->bf=1;
					shorter=false;
			   }
			   else if(root->bf==1)
			   {
					LeftBalance(root);
					shorter=true;
			   }
			   else
			   {
					root->bf=0;
					shorter=true;
			   }
			}
}
}

/*
 * For a given avl tree, check if the new Node could 
 * find its k-flat precedent i.e parent of sequence tree
 * and insert the new node into the sequence tree if found.
 *
 */
bool SearchBST(pMyNode &T,pMyNode &key)
{
    if(T==NULL) return false;
	
    /* new node is in current Node's range */
	if(key->data_val <= T->max_k && key->data_val >= T->min_k )
	{
		
        /* new node is on the right side */
		if (key->data_val > T->data_val)
		{
		    /* 
             * let new node to be left seq child of current node's right avl child.
             * This is because the known requirement for more than 1 solution.
             *
             *                    T (data_val=10, myindex=8, <7, 13>)
             *                     \
             *                      \ avl tree's right child
             *                       \
             *                      avl-rightchild (data_val=14, myindex=6, <11, 17>) 
             *                     //
             *                    // seqTree_leftchild
             *                   //
             *                  key (data_val=13, myindex=21, <10, 16> ) 
             */	
			if ( T->avl_rightchild!=0 && key->data_val >= T->avl_rightchild->min_k 
				&& T->myindex > T->avl_rightchild->myindex)
			{
				T->avl_rightchild->seqTree_leftchild = key;
				T->avl_rightchild->degree += 1;
				key->cur_length = T->avl_rightchild->cur_length + 1;
				key->degree = 0;
				key->seqTree_parent = T->avl_rightchild;

			}
			else
			{
				T->seqTree_rightchild = key;
				T->degree += 2;
				key->cur_length = T->cur_length + 1;
				key->degree = 0;
				key->seqTree_parent = T;

			}
		}
		
		else 
		{
		    /* for the similar reason as avl right child case above */	
			if ( T->avl_leftchild!=0 && key->data_val <= T->avl_leftchild->max_k 
				&& T->myindex > T->avl_leftchild->myindex)
			{
				T->avl_leftchild->seqTree_rightchild = key;
				T->avl_leftchild->degree += 2;
				key->cur_length = T->avl_leftchild->cur_length + 1;
				key->degree = 0;
				key->seqTree_parent = T->avl_leftchild;

			}
			
			else
			{
				T->seqTree_leftchild = key;
				T->degree += 1;
				key->cur_length = T->cur_length + 1;
				key->degree = 0;
				key->seqTree_parent = T;

			}

		}

		return true;
	}
    else if(key->data_val <= T->data_val) return SearchBST(T->avl_leftchild,key);
    else return SearchBST(T->avl_rightchild,key);
}

int n,k;

void AdjustAVLroot(pMyNode & newroot, pMyNode & oldroot)
{

	pMyNode prenode = oldroot->avl_previoustree;
	pMyNode nextnode = oldroot->avl_nexttree;
	prenode -> avl_nexttree = newroot;
	if ( nextnode != 0 )
		nextnode -> avl_previoustree = newroot;
	newroot -> avl_nexttree = nextnode;
	newroot -> avl_previoustree = prenode;

}

void Process(MyNode * pCur_Node)
{

	pMyNode curActiveAVLTree = AVLTreeList_Head -> avl_nexttree;
	pMyNode tmppreviousAVLTree= AVLTreeList_Head,beforeAdjustAVLroot = AVLTreeList_Head;

	while ((curActiveAVLTree!=NULL) && !SearchBST(curActiveAVLTree, pCur_Node))
	{
		tmppreviousAVLTree = curActiveAVLTree;
		curActiveAVLTree = curActiveAVLTree->avl_nexttree;
	}
	
    /* 
     * we have gotten the corresponding avl tree and inserted 
     * cur node into sequence tree.  
     */
	if (curActiveAVLTree!=NULL)
	{
		
            /* 
             * if new node's sequence tree parent already has both right 
             * and left child within sequence trees, we will remove it 
             * from the avl tree.
             *  
             */
			if (pCur_Node->seqTree_parent->degree == 3)
			{
			    /* remove current avl tree if it only has 1 node */	
				if (curActiveAVLTree->avl_leftchild==0 && curActiveAVLTree->avl_rightchild==0)
				{
					tmppreviousAVLTree->avl_nexttree = curActiveAVLTree->avl_nexttree;
					if (curActiveAVLTree->avl_nexttree != 0)
						curActiveAVLTree->avl_nexttree->avl_previoustree = tmppreviousAVLTree;
				}
				else
				{
					bool shorter,suc,del,leaf;
					beforeAdjustAVLroot = curActiveAVLTree;
                    /* remove current node's seq parent from current avl tree */
					BBT_Del(curActiveAVLTree,pCur_Node->seqTree_parent,shorter,suc,del,leaf);
					if (curActiveAVLTree != beforeAdjustAVLroot)
                        /* fix the links of current root */
						AdjustAVLroot(curActiveAVLTree,beforeAdjustAVLroot);
				}
			}
 	
            /* build a new avl tree with pCur_Node's cur_length */
			if (pCur_Node->cur_length != tmppreviousAVLTree->cur_length)
			{
				pCur_Node->avl_nexttree = curActiveAVLTree;
				pCur_Node->avl_previoustree = tmppreviousAVLTree;
				tmppreviousAVLTree->avl_nexttree = pCur_Node;
				curActiveAVLTree->avl_previoustree = pCur_Node;

			}
			else  
			{	
                /* add pCur_Node into previous avl tree */
				bool taller;
				beforeAdjustAVLroot = tmppreviousAVLTree;
				BBT_Insert(tmppreviousAVLTree,tmppreviousAVLTree,taller,pCur_Node);
				if (tmppreviousAVLTree != beforeAdjustAVLroot)
					AdjustAVLroot(tmppreviousAVLTree,beforeAdjustAVLroot);
			}

	}
	else 
	{
            /* fail to find a precedent buddy */
			pCur_Node->cur_length = 1;
			if (tmppreviousAVLTree->cur_length == 1)
			{
                /* insert current node into the avl tree of length = 1 */
				bool taller;
				beforeAdjustAVLroot = tmppreviousAVLTree;
				BBT_Insert(tmppreviousAVLTree,tmppreviousAVLTree,taller,pCur_Node);
				if (tmppreviousAVLTree != beforeAdjustAVLroot)
					AdjustAVLroot(tmppreviousAVLTree,beforeAdjustAVLroot);
			}
			else
			{
                /* create a new avl tree at the very end */
				pCur_Node->avl_nexttree = 0;
				pCur_Node->avl_previoustree = tmppreviousAVLTree;
				tmppreviousAVLTree->avl_nexttree = pCur_Node;
			}

	}
					
}


void Display (pMyNode & LastNode)
{
	
   if (LastNode == NULL)
	   return;
	 
	 while ( LastNode -> seqTree_parent != NULL)
	 {
	 	LastNode->seqTree_parent->nextprint = LastNode;
	 	LastNode = LastNode->seqTree_parent;
	 }
	 	 
   while ( LastNode != NULL)
   {
	   printf ("%d ", LastNode->data_val);
	   LastNode = LastNode -> nextprint;
   }
   printf ("\n");
}


int minindex ;
pMyNode lastprintnode;
void FindMinIndex(pMyNode T)
{
	if (!T) return;
	if (T->myindex < minindex)
	{
		minindex = T->myindex;
		lastprintnode = T;
	}
	FindMinIndex(T->avl_leftchild);
	FindMinIndex(T->avl_rightchild);
	
}


void ZeroNode(pMyNode & tmpAVLTreeList_Head)
{
    memset(tmpAVLTreeList_Head, 0, sizeof(MyNode));
}


int main() 
{

	scanf("%d %d", &n, &k);
	
	pMyNode NodeList = new MyNode[n];
	pMyNode tmpNodeList = NodeList;
	int i = 0;
	for( i=0; i < n; i++)
	{
		ZeroNode(tmpNodeList);
		scanf("%d", &NodeList[i].data_val);
		NodeList[i].min_k = NodeList[i].data_val - k;
		NodeList[i].max_k = NodeList[i].data_val + k;
		tmpNodeList++;
	}

	ZeroNode(AVLTreeList_Head);

	if (n > 0)
		{
			NodeList[0].myindex = 0;
			NodeList[0].cur_length = 1;
			NodeList[0].degree = 0;	
			AVLTreeList_Head -> avl_nexttree = &NodeList[0]; 
			NodeList[0].avl_previoustree = AVLTreeList_Head;
		}

	for ( i=1; i<n; i++)
	{	
		NodeList[i].myindex = i;
		Process(&NodeList[i]);
	}

	if (AVLTreeList_Head != NULL && (AVLTreeList_Head->avl_nexttree) != NULL)
	{
		minindex = AVLTreeList_Head->avl_nexttree->myindex;
		lastprintnode = AVLTreeList_Head->avl_nexttree;
		FindMinIndex(AVLTreeList_Head->avl_nexttree);
		Display(lastprintnode);  
	}
	
	delete []NodeList;
	delete AVLTreeList_Head;
	return 0;  
}

