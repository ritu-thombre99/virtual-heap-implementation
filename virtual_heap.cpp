#include<iostream>
using namespace std;
#include<string.h>
#include<bits/stdc++.h>
typedef struct Node
{
	int size,start_address;
	//#size# units of memory is allocated pointed by "name"
	char name[100];
	struct Node *next;
} alloc_node;
typedef struct head
{
	int start,size,next;
} header_node;
//deque header simulates the entire heap
deque<header_node> header;
//head and tail simulate allocated list
alloc_node *head,*tail;
bool sortByIndex(header_node a,header_node b)
{
	return (a.start < b.start);
}
void call_coalesce(deque<header_node> &temp_header)
{
	int done = false;
	for(int i=0;i<temp_header.size()&&!done;i++)
	{
		if(temp_header[i].start + temp_header[i].size == temp_header[i+1].start)
		{
			temp_header[i].size+=temp_header[i+1].size;
			temp_header.erase(temp_header.begin()+i+1);
			done = true;
		}
	}
}
void coalesce()
{
	deque<header_node> temp_header;
	for(auto itr = header.begin();itr!=header.end();itr++)
	{
		temp_header.push_back(*itr);
	}
	//sorts free memory according to the starting index
	sort(temp_header.begin(),temp_header.end(),sortByIndex);
	bool fragmented = true;
	//while loop executes untill there are fragments in the memory i.e two adjacent memory chunks are fragmented
	while(fragmented)
	{
		bool done = true;
		//for loop to check adjacent memory chunks
		for(int i=0;i<temp_header.size()&&done;i++)
		{
			if(temp_header[i].start + temp_header[i].size == temp_header[i+1].start)
				done = false;
		}
		if(done == true)
			fragmented = false;
		else
			call_coalesce(temp_header);
	}
	//adjusting "next" field in temporary header list
	for(int i=0;i<temp_header.size()-1;i++)
	{
		temp_header[i].next = temp_header[i+1].start;
	}
	int i;
	temp_header[temp_header.size()-1].next = -1;
	cout<<"\n====================COALESCED MEMORY=============================\n";
	for(auto itr = temp_header.begin();itr!=temp_header.end();itr++)
	{
		cout<<"Start:"<<(*itr).start<<endl;
		cout<<"Size:"<<(*itr).size<<endl;
		cout<<"next:"<<(*itr).next<<endl;
		cout<<endl;
	}
	//clears eveything in the originak header and copies coalesced memory header
	header.clear();
	for(auto itr = temp_header.begin();itr!=temp_header.end();itr++)
	{
		header.push_back(*itr);
	}
}
void Malloc(char name[],int size)
{
	if(header.empty())
		cout<<"NO FREE MEMORY\n";
	else
	{
		bool already_exists = false;
		alloc_node *ptr = head;
		//to check if pointer with same name already exists
		while(ptr != NULL && !already_exists)
		{
			if(strcmp(ptr->name,name)==0)
				already_exists = true;
			ptr = ptr->next;
		}
		if(already_exists)
			cout<<"Pointer with this name already exits in the memory\n\n";
		else
		{
			int i=0;
			while(i<header.size() && header[i].size < size)
			{
				i++;
			}
			if(i != header.size())
			{
				int t = 0;
				alloc_node *temp = (alloc_node*)malloc(sizeof(alloc_node));
				temp -> start_address = header[i].start;
				temp -> size = size;
				strcpy(temp -> name,name);
				temp->next = NULL;
				header[i].size-=size;
				header[i].start+=size;
				if(header[i].size == 0)
				{
					header.erase(header.begin() + i);
				}
				if(head==NULL && tail == NULL)
				{
					head = temp;
					tail = temp;
				}
				else if(head == tail)
				{
					tail = temp;
					head -> next = tail;
				}
				else
				{
					tail -> next = temp;
					tail = temp;
				}
			}
			else
				cout<<"THERE IS NOT ENOUGH FOR YOUR REQUEST\n";
		}
	}
}
void Free(char name[])
{	
	if(head != NULL)
	{
		if(head -> next != NULL)
		{
			alloc_node *ptr = head,*prev = NULL;
			while(ptr != NULL && strcmp(ptr->name,name)!=0)
			{
				prev = ptr;
				ptr = ptr->next;
			}
			if(ptr == NULL)
				cout<<"MEMORY WITH THIS NAME NOT FOUND\n\n";
			else if(prev == NULL)
			{
				alloc_node *temp_alloc_list = head;
				head = head->next;
				header_node x;
				x.start = temp_alloc_list->start_address;
				x.size = temp_alloc_list->size;
				auto itr = header.begin();
				x.next = (*itr).start;
				free(temp_alloc_list);
				header.push_front(x);
			}
			else 
			{
				alloc_node *temp_alloc_list = ptr;
				prev -> next = ptr->next;
				header_node x;
				x.start = temp_alloc_list->start_address;
				x.size = temp_alloc_list->size;
				auto itr = header.begin();
				x.next = (*itr).start;
				free(temp_alloc_list);
				header.push_front(x);
			}
		}
		else
		{
			if(strcmp(head->name,name)!=0)
				cout<<"\nMemory not found\n";
			else
			{
				alloc_node *temp_alloc_list = head;
				header_node x;
				x.start = temp_alloc_list->start_address;
				x.size = temp_alloc_list->size;
				auto itr = header.begin();
				x.next = (*itr).start;
				free(temp_alloc_list);
				header.push_front(x);
				head = NULL;
				tail = NULL;
			}
		}
		/*cout<<"Do you want to coalesce the memory?\nType yes or no:";
		char c[4];
		scanf("%s",c);
		if(strcmp("yes",c)==0)
			coalesce();*/
	}
	else
		cout<<"Pointer:"<<name<<" does not exist\nAlloc list is empty\nPlease assign memory first\n\n";
}
void display_heap()
{
	int i;
	if(!header.empty())
	{
		int free_memory = 0;
		cout<<"=======================HEADER==================================\n\n";
		cout<<"Header:"<<header[0].start<<endl<<endl;
		cout<<"=======================FREE LIST==================================\n\n";
		for(i=0;i<header.size();i++)
		{
			cout<<"Start address:"<<header[i].start<<endl;	
			cout<<"Size:"<<header[i].size<<endl;
			cout<<"Next address:"<<header[i].next;
			cout<<"\n\n";
		}
	}
	else
		cout<<"NO FREE MEMORY\n\n";
	cout<<"=======================ALLOCATED LIST==================================\n\n";
	alloc_node *ptr = head;	
	while(ptr != NULL)
	{
		cout<<"Name:"<<ptr->name<<endl;
		cout<<"Start address:"<<ptr->start_address<<endl;	
		cout<<"Size:"<<ptr->size<<endl;
		alloc_node *temp;
		if(ptr->next != NULL)
		{
			temp = ptr->next;
			cout<<"Next address:"<<temp->start_address<<endl;
		}
		else
			cout<<"Next address:"<<"-1"<<endl;
		ptr = ptr->next;
		cout<<"\n\n";
	}
	if(head == NULL)
		cout<<"MEMORY IS EMPTY\n\n";
	cout<<"\n\n";
}
int reverse(int size)
{
	int rev = 0;
	while(size > 0)
	{
		rev = rev*10 + (size%10);
		size = size/10;
	}
	return rev;
}
bool coalesced()
{
	deque<header_node> temp_header;
	for(auto itr = header.begin();itr!=header.end();itr++)
	{
		temp_header.push_back(*itr);
	}
	//sorts free memory according to the starting index
	sort(temp_header.begin(),temp_header.end(),sortByIndex);
	bool continuous = true;
	for(int i=0;i<temp_header.size()&&continuous;i++)
	{
		if(temp_header[i].start + temp_header[i].size == temp_header[i+1].start)
			continuous = false;
	}
	return continuous;
}
int main()
{
	int choose_option = 1;
	//Initially entire 
	int i;
	header_node temp;
	temp.start = 0;
	temp.size = 100;
	temp.next = -1;
	header.push_front(temp);
	head = NULL;
	tail = NULL;
	while(1)
	{
		cout<<"ENTER YOUR COMMAND FROM THE FOLLOWING:";
		char command[50];
		scanf("\n");	
		scanf("%[^\n]s",command);
		if(strstr(command,"malloc"))
		{
			cout<<command<<endl;
			char name[50];
			int size = 0;
			int i=0;
			while(command[i] != ' ')
			{
				name[i] = command[i];
				i++;
			}
			i = strlen(command)-2;
			while(command[i] != '(')
			{
				size = size*10 + (command[i] - '0');
				i--;
			}
			size = reverse(size);
			i = strlen(command)-2;
			while(command[i] == '0')
			{
				size*=10;
				i--;
			}
			cout<<"Size:"<<size<<endl;
			Malloc(name,size+2);
		}
		else if(strstr(command,"free"))
		{
			cout<<command<<endl;
			char name[50];
			int i = 5;
			while(command[i] != ')')
			{
				name[i-5] = command[i];
				i++;
			}
			cout<<"Pointer name:"<<name<<endl;
			Free(name);
		}
		else if(strcmp(command,"coalesce")==0)
		{
			cout<<command<<endl;
			if(!coalesced())
				coalesce();
			else
				cout<<"Memory not coalesced, no adjacent free memory chunks found."<<endl;
		}
		else if(strcmp(command,"display status")==0)
		{
			
			display_heap();
		}
		else if(strcmp(command,"exit")==0)
		{
			break;
		}
		else
		{
			cout<<"\nEnter valid command\n";
			cout<<"Type exit to exit\n\n";
		}
	}
	return 0;
}
