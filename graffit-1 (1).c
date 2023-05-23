/**
 * CSC A48 - Intro to Computer Science II, Summer 2021
 * 
 * Assignment 3 - Graffit
 * 
 * Graphs &
 * Recursion
 * Assignment
 * For
 * Freshmen
 * In
 * Toronto
 *
 * (I am so proud of that initialism.)
 * 
 * This is the program file where you will implement your solution for
 * Assignment 3. Please make sure you read through this file carefully
 * and that you understand what is provided and what you need to complete.
 * 
 * You will also need to have read the handout carefully. 
 * 
 * Parts where you have to implement functionality are clearly labeled TODO.
 * 
 * Be sure to test your work thoroughly, our testing will be extensive
 * and will check that your solution is *correct*, not only that it
 * provides functionality.
 * 
 * (c) 2020 William Song, Mustafa Quraish
 * (c) 2021 Charles Xu
 **/

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_STR_LEN 1024

#ifndef MAT_SIZE
#define MAT_SIZE 3 // A small graph
#endif

typedef struct user_struct
{
  char name[MAX_STR_LEN];
  struct friend_node_struct *friends;
  struct brand_node_struct *brands;
  bool visited;
  //struct friend_node_struct* userList;
} User;

typedef struct friend_node_struct
{
  User *user;
  bool vis;
  int dis;
  struct friend_node_struct *next;
} FriendNode;

typedef struct brand_node_struct
{
  char brand_name[MAX_STR_LEN];
  struct brand_node_struct *next;
} BrandNode;
typedef struct q
{
	User* from;
	User* to;
	int dis;
	bool vis;
	struct q *next;
}Q;
typedef struct list
{
	char name[MAX_STR_LEN];
	struct list *next;
}list;
FriendNode *allUsers = NULL;

int brand_adjacency_matrix[MAT_SIZE][MAT_SIZE];
char brand_names[MAT_SIZE][MAX_STR_LEN];

/**
 * Given the head to a FriendNode linked list, returns true if a
 * given user's name exists in the list. Returns false otherwise.
 */
bool in_friend_list(FriendNode *head, User *node)
{
  for (FriendNode *cur = head; cur != NULL; cur = cur->next)
  {
    if (strcmp(cur->user->name, node->name) == 0)
    {
      return true;
    }
  }
  return false;
}

/**
 * Given the head to a BrandNode linked list, returns true if a
 * given brand's name exists in the list. Returns false otherwise.
 */
bool in_brand_list(BrandNode *head, char *name)
{
  for (BrandNode *cur = head; cur != NULL; cur = cur->next)
  {
    if (strcmp(cur->brand_name, name) == 0)
    {
      return true;
    }
  }
  return false;
}

/**
 * Given the head to a FriendNode linked list, inserts a given user
 * into the linked list and returns the head of the new linked list.
 * The insertion is done in alphabetical order. If the user already
 * exists, no modifications are made and the list is returned as is.
 */
FriendNode *insert_into_friend_list(FriendNode *head, User *node)
{
  if (node == NULL)
    return head;

  if (in_friend_list(head, node))
  {
    printf("User already in list\n");
    return head;
  }

  FriendNode *fn = calloc(1, sizeof(FriendNode));
  fn->user = node;
  fn->next = NULL;

  if (head == NULL)
    return fn;

  if (strcmp(head->user->name, node->name) > 0)
  {
    fn->next = head;
    return fn;
  }

  FriendNode *cur;
  for (cur = head; cur->next != NULL && strcmp(cur->next->user->name, node->name) < 0;
       cur = cur->next)
    ;

  fn->next = cur->next;
  cur->next = fn;

  return head;
}

/**
 * Given the head to a BrandNode linked list, inserts a given brand into
 * the linked list and returns the head of the new linked list. The
 * insertion is done in alphabetical order. If the brand already exists,
 * no modifications are made and the list is returned as is.
 */
BrandNode *insert_into_brand_list(BrandNode *head, char *node)
{
  if (node == NULL)
    return head;

  if (in_brand_list(head, node))
  {
    printf("Brand already in list\n");
    return head;
  }

  BrandNode *fn = calloc(1, sizeof(BrandNode));
  strcpy(fn->brand_name, node);
  fn->next = NULL;

  if (head == NULL)
    return fn;

  if (strcmp(head->brand_name, node) > 0)
  {
    fn->next = head;
    return fn;
  }

  BrandNode *cur;
  for (cur = head; cur->next != NULL && strcmp(cur->next->brand_name, node) < 0;
       cur = cur->next)
    ;

  fn->next = cur->next;
  cur->next = fn;

  return head;
}

/**
 * Given the head to a FriendNode linked list, removes a given user from
 * the linked list and returns the head of the new linked list. If the user
 * does not exist, no modifications are made and the list is returned as is.
 */
FriendNode *delete_from_friend_list(FriendNode *head, User *node)
{
  if (node == NULL)
    return head;

  if (!in_friend_list(head, node))
  {
    printf("User not in list\n");
    return head;
  }

  if (strcmp(head->user->name, node->name) == 0)
  {
    FriendNode *temp = head->next;
    free(head);
    return temp;
  }

  FriendNode *cur;
  for (cur = head; cur->next->user != node; cur = cur->next)
    ;

  FriendNode *temp = cur->next;
  cur->next = temp->next;
  free(temp);

  return head;
}

/**
 * Given the head to a BrandNode linked list, removes a given brand from
 * the linked list and returns the head of the new linked list. If the brand
 * does not exist, no modifications are made and the list is returned as is.
 */
BrandNode *delete_from_brand_list(BrandNode *head, char *node)
{
  if (node == NULL)
    return head;

  if (!in_brand_list(head, node))
  {
    printf("Brand not in list\n");
    return head;
  }

  if (strcmp(head->brand_name, node) == 0)
  {
    BrandNode *temp = head->next;
    free(head);
    return temp;
  }

  BrandNode *cur;
  for (cur = head; strcmp(cur->next->brand_name, node) != 0; cur = cur->next)
    ;

  BrandNode *temp = cur->next;
  cur->next = temp->next;
  free(temp);

  return head;
}

/**
 * Given a user, prints their name, friends, and liked brands.
 */
void print_user_data(User *user)
{
  printf("User name: %s\n", user->name);

  printf("Friends:\n");
  for (FriendNode *f = user->friends; f != NULL; f = f->next)
  {
    printf("   %s\n", f->user->name);
  }

  printf("Brands:\n");
  for (BrandNode *b = user->brands; b != NULL; b = b->next)
  {
    printf("   %s\n", b->brand_name);
  }
}

/**
 * Given a brand, returns the index of the brand inside the brand_names array.
 * If it doesn't exist in the array, return -1
 */
int get_brand_index(char *name)
{
  for (int i = 0; i < MAT_SIZE; i++)
  {
    if (strcmp(brand_names[i], name) == 0)
    {
      return i;
    }
  }

  printf("Brand '%s' not found\n", name);
  return -1; // Not found
}

/**
 * Given a brand, prints their name, index (inside the brand_names
 * array), and the names of other similar brands.
 */
void print_brand_data(char *brand_name)
{
  int idx = get_brand_index(brand_name);
  if (idx < 0)
  {
    printf("Brand '%s' not in the list.\n", brand_name);
    return;
  }

  printf("Brand name: %s\n", brand_name);
  printf("Brand idx: %d\n", idx);

  printf("Similar brands:\n");
  for (int i = 0; i < MAT_SIZE; i++)
  {
    if (brand_adjacency_matrix[idx][i] == 1 && strcmp(brand_names[i], "") != 0)
    {
      printf("   %s\n", brand_names[i]);
    }
  }
}

/**
 * Read from a given file and populate a the brand list and brand matrix.
 **/
void populate_brand_matrix(char *file_name)
{
  // Read the file
  char buff[MAX_STR_LEN];
  FILE *f = fopen(file_name, "r");
  fscanf(f, "%s", buff);
  char *line = buff;
  // Load up the brand_names matrix
  for (int i = 0; i < MAT_SIZE; i++)
  {
    if (i == MAT_SIZE - 1)
    {
      strcpy(brand_names[i], line);
      break;
    }
    int index = strchr(line, ',') - line;
    strncpy(brand_names[i], line, index);
    line = strchr(line, ',') + sizeof(char);
  }
  // Load up the brand_adjacency_matrix
  for (int x = 0; x < MAT_SIZE; x++)
  {
    fscanf(f, "%s", buff);
    for (int y = 0; y < MAT_SIZE; y++)
    {
      int value = (int)buff[y * 2];
      if (value == 48)
      {
        value = 0;
      }
      else
      {
        value = 1;
      }
      brand_adjacency_matrix[x][y] = value;
    }
  }
}

/**
 * TODO: Complete this function
 * Creates and returns a user.
 */
User *create_user(char *name)
{
	User *new=NULL;
	new=(User*)calloc(1,sizeof(User));
	strcpy(new->name,name);
	new->visited=false;
	if(in_friend_list(allUsers,new)==true)
	{
		free(new);
		return NULL;
	}
	allUsers=insert_into_friend_list(allUsers, new);
	//new->userList = allUsers;
	return new;
}

/**
 * TODO: Complete this function
 * Deletes a given user.
 */

int delete_user(User *user)
{
	if(in_friend_list(allUsers,user)==false)
	{
		return -1;
	}
	BrandNode*q = user->brands;
	while(q!=NULL)
	{
		BrandNode *temp = q->next;
		free(q);
		q= temp;
	}
	FriendNode*o=user->friends;
	while(o!=NULL)
	{
		FriendNode* temp = o->next;
		free(o);
		o=temp;
	}
	allUsers=delete_from_friend_list(allUsers, user);
	FriendNode* p=NULL;
	p=allUsers;
	while(p!=NULL)
	{
	
		p->user->friends=delete_from_friend_list(p->user->friends, user);
		
		
		p=p->next;
	}
	return 0;
}

/**
 * TODO: Complete this function
 * Create a friendship between user and friend.
 */
int add_friend(User *user, User *friend)
{
	//do I assume two users are already in allUsers?
	// FriendNode* p=NULL;
	// p=allUsers;
	// int count=0;
	// while(p!=NULL)
	// {
		// if(strcmp(p->user->name,user->name)==0)
		// {
			// if(in_friend_list(p->user->friends, friend)==true)
			// {
				// count++;
			// }
			// else
			// {
				
				// p->user->friends=insert_into_friend_list(p->user->friends, friend);
				
			// }
		// }
		// p=p->next;
	// }
	// FriendNode* q=NULL;
	// q=allUsers;
	// while(q!=NULL)
	// {
		// if(strcmp(q->user->name,friend->name)==0)
		// {
			// if(in_friend_list(q->user->friends, user)==true)
			// {
				// count++;
			// }
			// else
			// {
				// q->user->friends=insert_into_friend_list(q->user->friends,user);
				
			// }
		// }
		// q=q->next;
	// }
	// if(count==2)
	// {
		// return -1;
	// }
	// return 0;
	if(user==friend) return -1;
	if(in_friend_list(user->friends,friend)==true&&in_friend_list(friend->friends,user)==true)
	{
		return -1;
	}
	user->friends=insert_into_friend_list(user->friends,friend);
	friend->friends=insert_into_friend_list(friend->friends,user);
	return 0;
}
void printAll(FriendNode* s)
{
	FriendNode* a=NULL;
	a=allUsers;
	while(a!=NULL)
	{
		printf("*****%s\n", a->user->name);
		a=a->next;
	}
}
/**
 * TODO: Complete this function
 * Removes a friendship between user and friend.
 */
int remove_friend(User *user, User *friend)
{
	if(in_friend_list(user->friends,friend)==false&&in_friend_list(friend->friends,user)==false)
	{
		return -1;
	}
	user->friends=delete_from_friend_list(user->friends,friend);
	friend->friends=delete_from_friend_list(friend->friends, user);
	return 0;
}

/**
 * TODO: Complete this function
 * Creates a follow relationship, the user follows the brand.
 */
int follow_brand(User *user, char *brand_name)
{
	if(get_brand_index(brand_name)==-1)
	{
		return -1;
	}
	if(brand_name==NULL)
	{
		return -1;
	}
	if(in_brand_list(user->brands, brand_name)==true)
	{
		return -1;
	}

	user->brands=insert_into_brand_list(user->brands, brand_name);
	
	return 0;
}

/**
 * TODO: Complete this function
 * Removes a follow relationship, the user unfollows the brand.
 */
int unfollow_brand(User *user, char *brand_name)
{
	if(get_brand_index(brand_name)==-1)
	{
		return -1;
	}
	//invalid brand name?
	if(in_brand_list(user->brands, brand_name)==false)
	{
		return -1;
	}
	user->brands= delete_from_brand_list(user->brands, brand_name);
	return 0;
}

/**
 * TODO: Complete this function
 * Return the number of mutual friends between two users.
 */
int get_mutual_brands(User* a, User* b)
{
	BrandNode* p=NULL;
	BrandNode* q=NULL;
	p=a->brands;
	q=b->brands;
	int count=0;
	for(p=a->brands;p!=NULL;p=p->next)
	{
		for(q=b->brands;q!=NULL;q=q->next)
		{
			if(strcmp(q->brand_name, p->brand_name)==0)
			{
				count++;
			}
			///q=q->next;
		}
		
		
		//p=p->next;
		
	}
	return count;
}
int get_mutual_friends(User *a, User *b)
{
	FriendNode* p=NULL;
	FriendNode* q=NULL;
	p=a->friends;
	q=b->friends;
	int count=0;
	//printf("***********%d\n", count);
	for(p=a->friends;p!=NULL;p=p->next)
	{
		
		for(q=b->friends;q!=NULL;q=q->next)
		{
			
			if(strcmp(p->user->name,q->user->name)==0)
			{
				count=count+1;
				//continue;
				//printf("***********%d\n", count);
			}
			//q=q->next;
		}
		
		//p=p->next;
	}
	
	return count;
}

/**
 * TODO: Complete this function
 * A degree of connection is the number of steps it takes to get from
 * one user to another. Returns a non-negative integer representing
 * the degrees of connection between two users.
 */
Q* new_q()
{
	Q* new = NULL;
	new = (Q*)calloc(1,sizeof(Q));
	new->from=NULL;
	new->to=NULL;
	new->dis=-1;
	new->vis=false;
	new->next=NULL;
	return new;
}
void print(Q* head)
{
	Q* new =NULL;
	new=head;
	while(new!=NULL)
	{
		printf("****from: %s****\n", new->from->name);
		printf("****to: %s****\n", new->to->name);
		new=new->next;
	}
	
}

Q* add(Q* head, FriendNode* fr, User* rf)
{
	//print(head);
	Q* p = NULL;
	//Q* q = NULL;
	while(fr!=NULL)
	{
		p=head;
		while(p->next!=NULL)
		{
			p=p->next;
		}
		//printf("%s\n", p->from->name);
		p->next=new_q();
		p->next->from = rf;
		p->next->to = fr->user;
		p->next->dis=1;
		fr=fr->next;
	}
	
	return head;
}
int get_degrees_of_connectionn(User*a, User*b, int depth)
{

	a->visited=true;
	if(strcmp(a->name,b->name)!=0)
	{

		for(FriendNode* temp=a->friends; temp!=NULL; temp=temp->next)
		{
			//printf("%s\n", temp->user->name);
			if(temp->user->friends!=NULL)
			{
				
				if(temp->user->visited==false)
				{
					get_degrees_of_connectionn(temp->user,b,depth++);
					//a->visited=false;
				}
				
			}
		}
	}
		
	//printf("%d\n", depth);
	return depth;
}
void printTable(FriendNode* a)
{
	for(int i=0; a[i].user!=NULL;i++)
	{
		printf("\n");
		printf("*******************\n");
		printf("name:%s\n", a[i].user->name);
		if(a[i].vis==false)
		{
			printf("status:%s\n", "false");
		}
		else
		{
			printf("status:%s\n", "true");
		}
		printf("dis:%d\n", a[i].dis);
		printf("*******************\n");
		printf("\n");
		
	}
}
int index(FriendNode* a, char* name)
{
//	FriendNode* p=a;
	int i=0;
	while(a[i].user!=NULL)
	{
		if(strcmp(a[i].user->name, name)==0)
		{
			return i;
		}
		i++;
	}
	return -1;
	
}
FriendNode* addQ(FriendNode* head, User* a)
{
	if(head==NULL)
	{
		FriendNode*new=(FriendNode*)calloc(1,sizeof(FriendNode));
		new->user=a;
		head=new;
		return head;
	}
	FriendNode* p =NULL;
	p=head;
	while(p->next!=NULL)
	{
		p=p->next;
	}
	FriendNode*new=(FriendNode*)calloc(1,sizeof(FriendNode));
	new->user=a;
	p->next=new;
	return head;
}
void printqq(FriendNode*head)
{
	FriendNode* p=NULL;
	p=head;
	printf("------------------\n");
	while(p!=NULL)
	{
		
		printf("%s\n",p->user->name);
		p=p->next;
	}
	printf("------------------\n");
}
int get_degrees_of_connection(User *a, User *b)
{
	if(strcmp(a->name, b->name)==0)
	{
		return 0;
	}
	FriendNode* p=NULL;
	p=allUsers;
	FriendNode* c = NULL;
	c=allUsers;
	int counttt=0;
	while(c!=NULL)
	{
		counttt++;
		c=c->next;
	}
	FriendNode d[counttt];
	int i=0;
	//printf("****%s\n",allUsers->user->name);
	while(p!=NULL)
	{
		d[i].user=p->user;
		d[i].dis=0;
		d[i].vis=false;
		//printf("********\n");
		i=i+1;
		p=p->next;
	}
	
	d[index(d, a->name)].dis=0;
	d[index(d, a->name)].vis=true;
	// printTable(d);
	// printf("**********************done****************\n");
	FriendNode* head=NULL;
	head=(FriendNode*)calloc(1,sizeof(FriendNode));
	head->user=a;
	// addQ(head, b);
	// printf("%s\n", head->user->name);
	// printf("%s\n", head->next->user->name);
	while(head!=NULL)
	{
		//printqq(head);
		FriendNode* temp = head->next;
		FriendNode* tem=head->user->friends;
		int te=d[index(d, head->user->name)].dis;
		free(head);
		head=temp;
		//FriendNode* i=head->user->friends;
		for(FriendNode* i=tem; i!=NULL; i=i->next)
		{
			
			//printf("*****\n");
			if(d[index(d, i->user->name)].vis==false)
			{
				//printf("%s\n", d[index(d, i->user->name)].user->name);
				d[index(d, i->user->name)].dis=te+1;
				d[index(d, i->user->name)].vis=true;
				
				head=addQ(head, i->user);
				
				//printf("*******\n");
			}
		}
		//head=NULL;
		//i->dis=0;
		
	}
	//printTable(d);
	int x=0;
	while(d[x].user!=NULL)
	{
		if(strcmp(d[x].user->name, b->name)==0)
		{
			if(d[x].dis==0)
			{
				return -1;
			}
			return d[x].dis;
		}
		x++;
	}
	return -1;
}

/**
 * TODO: Complete this function
 * Marks two brands as similar.
 */
void connect_similar_brands(char *brandNameA, char *brandNameB)
{
	if(get_brand_index(brandNameA)!=-1&&get_brand_index(brandNameB)!=-1)
	{
		brand_adjacency_matrix[get_brand_index(brandNameA)][get_brand_index(brandNameB)]=1;
		brand_adjacency_matrix[get_brand_index(brandNameB)][get_brand_index(brandNameA)]=1;

		//printf("*****************%d\n",get_brand_index(brandNameB) );
	}
}

/**
 * TODO: Complete this function
 * Marks two brands as not similar.
 */
void remove_similar_brands(char *brandNameA, char *brandNameB)
{
	if(get_brand_index(brandNameA)!=-1&&get_brand_index(brandNameB)!=-1)
	{
		brand_adjacency_matrix[get_brand_index(brandNameA)][get_brand_index(brandNameB)]=0;
		brand_adjacency_matrix[get_brand_index(brandNameB)][get_brand_index(brandNameA)]=0;
		//printf("*****************%d\n",get_brand_index(brandNameB) );
	}
}

/**
 * TODO: Complete this function
 * Returns a suggested friend for the given user.
 */
User *get_suggested_friend(User *user)
{
	int max=0;
	User* temp=NULL;
	FriendNode* p=NULL;
	p=allUsers;
	//printf("**********\n");
	while(p!=NULL)
	{
		//printf("******%s\n", p->user->name);
		//printf("**********\n");
		if(strcmp(p->user->name,user->name)!=0)
		{
			//printf("**********%s\n",p->user->name);
			if(in_friend_list(user->friends, p->user)==false)
			{
				
				if(max<=get_mutual_brands(user, p->user))
				{
					max=get_mutual_brands(user, p->user);
					
					temp=p->user;
					
				}
			}
		}
		p=p->next;
	}
	//printf("done\n");
	return temp;
}
// User *get_suggested_friendd(User *user)
// {
	// int max=0;
	// User* temp=NULL;
	// FriendNode* p=allUsers;
	// while(p!=NULL)
	// {
		// if(strcmp(p->user->name,user->name)!=0)
		// {
			// if(in_friend_list(user->friends, p->user)==false)
			// {
				// if(max<=get_mutual_brands(user, p->user))
				// {
					// if(p->user->visited==false)
					// {
						// max=get_mutual_brands(user, p->user);
						// p->user->visited=true;
						// temp=p->user;
					// }
				// }
			// }
		// }
		// p=p->next;
	// }
	// return temp;
// }
/**
 * TODO: Complete this function
 * Adds n suggested friends for the given user.
 * Returns how many friends were successfully followed.
 */
int add_suggested_friends(User *user, int n)
{
	int count=0;
	//printf("*********%s\n",get_suggested_friend(user)->name);
	//print_user_data(user);
	//printf("*******\n");
	for(int i=0;i<n;i++)
	{
		//printf("*********%s\n",get_suggested_friend(user)->name);
		//printf("**********\n");
		if(get_suggested_friend(user)!=NULL)
		{
			//printf("**********\n");
			//if(in_friend_list(user->friends, get_suggested_friend(user))==false)
			//{
			int check = add_friend(get_suggested_friend(user),user);
			//add_friend(get_suggested_friend(user),user);
			//add_friend(get_suggested_friend(user),user);
			//if(check==0)
			if(check==0)
			{
			//user->friends=insert_into_friend_list(user->friends,get_suggested_friend(user));
				count++;
			}
			//}

		}
		//printf("*********\n");
	}
	//print_user_data(user);
	return count;
}

/**
 * TODO: Complete this function
 * Follows n suggested brands for the given user.
 * Returns how many brands were successfully followed.
 */
bool checkk(list* head, char* name)
{
	list* p=NULL;
	p=head;
	while(p!=NULL)
	{
		if(strcmp(p->name, name)==0)
		{
			return true;
		}
		p=p->next;
	}		
	return false;
}
char* suggested_brands(User* user,list*head)
{
	// int max=0;
	char* temp=NULL;
	//strcpy(temp, "");
	//printf("**********************\n");
	// BrandNode* p=NULL;
	// p=user->brands;
	// while(p!=NULL)
	// {
		// int index = get_brand_index(p->brand_name);
		// for(int w=0;w<MAT_SIZE;w++)
		// {
			// if(in_brand_list(user->brands, brand_names[w])==false)
			// {
				// if(max<=brand_adjacency_matrix[w][index])
				// {
					// max = brand_adjacency_matrix[w][index];
					// temp=brand_names[w];
				// }
			// }
		// }
		
		// p=p->next;
	// }
	//return temp;
	int count =0;
	int max=0;
	for(int i=0; i<MAT_SIZE;i++)
	{
		if(checkk(head, brand_names[i])==true)
		{
			continue;
		}

		//printf("******%d\n");
		count=0;
		if(in_brand_list(user->brands, brand_names[i])==false)
		{

			for(int w=0;w<MAT_SIZE;w++)
			{
				if(in_brand_list(user->brands, brand_names[w])==true)
				{
					
					
					if(brand_adjacency_matrix[i][w]!=0)
					{
						// printf("********\n");
						// printf("^^^^%s\n",brand_names[i]);
						// printf("%d\n",i);
						// printf("%d\n", w);
						// printf("********\n");
						count++;
						//printf("***************%d\n",count);
					}
				}
			}
			

			if(max==count)
			{
				if(temp==NULL)
				{
					temp=brand_names[i];
				}
				else if(strcmp(temp,brand_names[i])<0)
				{
					max = count;
					temp=brand_names[i];
				}
			}
			if(max<count)
			{
				//printf("******************%s\n", brand_names[i]);
				max=count;
				temp=brand_names[i];
				//printf("**%s\n", temp);
			}
			
			count=0;
		}
		//printf("%s\n", temp);
		
	}
	//printf("%s\n", temp);
	// if(strcmp("",temp)==0)
	// {
		// return NULL;
	// }
	return temp;
}
// void printlist(char** a)
// {
	// int i=0;
	// printf("**********\n");
	// while(a[i]!=NULL)
	// {
		// printf("%s\n", a[i]);
		// i++;
	// }
	// printf("**********\n");
// }	
void printlis(list*head)
{
	printf("**********\n");
	list* p=head;
	while(p!=NULL)
	{
		printf("%s\n", p->name);
		p=p->next;
	}
	printf("**********");

}
list* headd(list*head, char*name)
{
	
	list* new = NULL;
	new=(list*)calloc(1,sizeof(list));
	strcpy(new->name,name);
	new->next = NULL;
	if(head==NULL)
	{
		return new;
	}
	
	new->next = head;
	return new;
}
void del(list* head)
{
	list* p=head;
	list* q=NULL;
	while(p!=NULL)
	{
		q=p->next;
		free(p);
		p=q;
	}
	//return NULL;
}
int follow_suggested_brands(User *user, int n)
{
	int count=0;
	// char* list[MAT_SIZE]={NULL};
	// int m=0;
	list *head=NULL;
	head = (list*)calloc(1, sizeof(list));
	
	for(int i=0;i<n;i++)
	{
		char* nam = suggested_brands(user,head);
		//printf("%s\n", nam);
		if(nam!=NULL)
		{
			// strcpy(list[m],nam);
			// m++;
			head = headd(head, nam);
			
			count++;
		}
		
	}
	//printlis(head);
	list* p =NULL;
	p=head;

	while(p->next!=NULL)
	{
		//printlis(head);

		user->brands=insert_into_brand_list(user->brands, p->name);
		p=p->next;
	}
	
	//printlis(head);
	//printf("%s\n",list[0]);
	del(head);
	return count;
}
