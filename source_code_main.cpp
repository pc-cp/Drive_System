#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <errno.h>
#include <cstring>
#include <algorithm>
using namespace std;

int sum = 0; //	The number of the members
const int N = 1e2 + 10;
struct member_List
{
	char id[10];
	char name[10];
	int score[5];
	bool pass_or_not;
	struct member_List *next_address_point;
};
typedef struct member_List* Mem_List_Ptr;
struct member_List array_sort[N];

//检查文件是否打开成功
void open_File_Check(FILE *fp);
//加载管理员的名单
void file_Load_Admin();
//加载教练的名单
void file_Load_Jiaolian();
//whether a is front of b
bool cmp_Positive_Order_Id(struct member_List &a, struct member_List &b)
{
	if(strcmp(a.id, b.id) <= 0)
		return true;
	return false;
}
bool cmp_Positive_Order_Name(struct member_List &a, struct member_List &b)
{
	if(strcmp(a.name, b.name) <= 0)
		return true;
	return false;
}
//加载学员的名单 
Mem_List_Ptr file_Load_Member(Mem_List_Ptr member_head_list);
//write updated information of member to member.txt
void Data_Write(Mem_List_Ptr member_head_list);
//插入新学员
Mem_List_Ptr insert_Member_List(const Mem_List_Ptr incept_from_txt_pointer, Mem_List_Ptr member_head_list);
//删除新学员
Mem_List_Ptr remove_Member_List(const char *aim_string, Mem_List_Ptr member_head_list);
//查询学员
void find_Member_List(const char aim_string[], const Mem_List_Ptr member_head_list);
//修改学员信息
void alter_Member_List(const char aim_string[], const Mem_List_Ptr member_head_list);
//创建学员名单
Mem_List_Ptr create_Member_List(const Mem_List_Ptr incept_from_txt_pointer, Mem_List_Ptr member_head_list);
//use quick sort for array
void sort_Member_List(const Mem_List_Ptr member_head_list);
//print sorted array
void print_Array_Sort(const struct member_List array[]);
void open_File_Check(FILE *fp)
{
	if(fp == NULL)
	{
		printf("fopen():%s\n", strerror(errno));
		exit(1);
	}
}

/**
 * 读取已经存在的成员名单，并且串入链表中
 */
Mem_List_Ptr file_Load_Member(Mem_List_Ptr member_head_list)
{
	int return_fscanf;
	FILE *fp = NULL;
	fp = fopen("/root/20210830/Txt_Doc/member.txt", "r");
	open_File_Check(fp);
	struct member_List incept_from_txt;
	while((return_fscanf =  fscanf(fp, "%s	%s	%d	%d	%d	%d", incept_from_txt.id, incept_from_txt.name, &incept_from_txt.score[1], &incept_from_txt.score[2], &incept_from_txt.score[3], &incept_from_txt.score[4])) != EOF)
	{
		member_head_list = create_Member_List(&incept_from_txt, member_head_list);
	}
	fclose(fp);
	return member_head_list;
}

void Data_Write(Mem_List_Ptr member_head_list)
{
	int return_fscanf;
	FILE *fp = NULL;
	fp = fopen("/root/20210830/Txt_Doc/member.txt", "r");
	open_File_Check(fp);
	
	Mem_List_Ptr index = NULL;
	while(index)
	{
		fprintf(fp, );


		index = index->next_address_point;
	}
	
	fclose(fp);
}

/**
 * 修改学员信息
 */
void alter_Member_List(const char aim_string[], const Mem_List_Ptr member_head_list)
{
	Mem_List_Ptr index = NULL;
	index = member_head_list;
	while(index)
	{
		if((strcmp(index->id, aim_string) == 0) || (strcmp(index->name, aim_string) == 0))
		{
			printf("%s	%s	%d 	%d	%d	%d", index->id, index->name, index->score[1], index->score[2], index->score[3], index->score[4]);
			if(index->pass_or_not)
				printf("	PASS!\n");
			else
				printf("	NOT PASS\n");
			
			printf("---重新录入成绩----\n");

			index->pass_or_not = true;
			for(int i = 1; i <= 4; ++i)
			{
				scanf("%d", &index->score[i]);

				index->pass_or_not = index->pass_or_not && (index->score[i] >= 90);
			}
			printf("---更新成功！----\n");
			
			break;
		}
		index = index->next_address_point;	
	}
}
/*
 * find the member who you want
 * you can find him use his name or id
 */
void find_Member_List(const char aim_string[], const Mem_List_Ptr member_head_list)
{
	Mem_List_Ptr index = NULL;
	index = member_head_list;
	while(index)
	{
		if((strcmp(index->id, aim_string) == 0) || (strcmp(index->name, aim_string) == 0))
		{
			printf("%s	%s	%d 	%d	%d	%d", index->id, index->name, index->score[1], index->score[2], index->score[3], index->score[4]);
			if(index->pass_or_not)
				printf("	PASS!\n");
			else
				printf("	NOT PASS\n");
			
			break;
		}
		index = index->next_address_point;	
	}
}


//删除操作
Mem_List_Ptr remove_Member_List(const char aim_string[], Mem_List_Ptr member_head_list)
{
	Mem_List_Ptr index_front = member_head_list, index = member_head_list, index_behind = member_head_list->next_address_point;
 	
	int cnt = 1;
	printf("remove----\n");
	//该字符串为姓名或编号	
	while(index != NULL)
	{
		printf("cnt = %d\n", cnt++);
		if(cnt >= 10)
			break;
		if((strcmp(index->name, aim_string) == 0) || (strcmp(index->id, aim_string) == 0))
		{
			if(index_front == index)  //头部就是要删除的节点
			{
				index_front = index_behind; //直接删除第一个节点
				member_head_list = index_front;
				break;
			}
			else if(index == index_behind) //尾部才是要删除的节点
			{
				index_front->next_address_point = NULL; //直接删除最后一个节点
				break;
			}
			else	//中间某个节点是要删除的节点 
			{
				index_front->next_address_point = index_behind; //直接删除中间节点
				break;	
				
			}
		}
		//index_front 始终在指向index的前面一个节点，index_behind始终指向index的后面一个节点
		//除非一开始状态是front和index一样，指向最后一个节点时候，behind和index一样
		index = index_behind;
		if(index->next_address_point != NULL)
			index_behind = index->next_address_point;
		if(index_front->next_address_point != index)
		{
			index_front = index_front->next_address_point;
		}	

	}
	//释放已经删除节点的内存
	free(index);
	return member_head_list;
}
	
void print_Member_List(const Mem_List_Ptr member_head_list)
{
	Mem_List_Ptr index = NULL;
	index = member_head_list;
	//printf("print===");
	while(index)
	{
		printf("%s	%s	%d 	%d	%d	%d", index->id, index->name, index->score[1], index->score[2], index->score[3], index->score[4]);
		if(index->pass_or_not)
			printf("	PASS!\n");
		else
			printf("	NOT PASS\n");
		index = index->next_address_point;	
	}
}

#if 1
//printed sorted array use name or id of student
void print_Array_Sort(const struct member_List array[])
{
	int incept_from_input;
	printf("\n1 2 Positive or Reverse !\n");
	scanf("%d", &incept_from_input);
	if(incept_from_input == 1)
	{
		for(int i = 1; i <= sum; ++i)
		{
			printf("%s	%s	%d 	%d	%d	%d", array[i].id,  array[i].name, array[i].score[1], array[i].score[2], array[i].score[3], array[i].score[4]);
			if(array[i].pass_or_not)
				printf("	PASS!\n");
			else
				printf("	NOT PASS\n");

		}
	}
	else if(incept_from_input == 2)
	{
		for(int i = sum; i >= 1; --i)
		{
			printf("%s	%s	%d 	%d	%d	%d", array[i].id,  array[i].name, array[i].score[1], array[i].score[2], array[i].score[3], array[i].score[4]);
			if(array[i].pass_or_not)
				printf("	PASS!\n");
			else
				printf("	NOT PASS\n");

		}
	}
}
#endif

void sort_Member_List(const Mem_List_Ptr member_head_list)
{	
	Mem_List_Ptr index = NULL;
	index = member_head_list;
	//printf("print===");
	sum = 0;
	for(int i = 1; index; ++i)
	{
		strcpy(array_sort[i].id, index->id);	
		strcpy(array_sort[i].name, index->name);
		for(int j = 1; j <= 4; ++j)
		{
			array_sort[i].score[j] = index->score[j];
		}
		array_sort[i].pass_or_not = index->pass_or_not;
		++sum;
/*	
	struct member_List
{
	char id[10];
	char name[10];
	int score[5];
	bool pass_or_not;
	struct member_List *next_address_point;
};
*/
	//	printf("%s	%s	%d 	%d	%d	%d", index->id, index->name, index->score[1], index->score[2], index->score[3], index->score[4]);
	//	if(index->pass_or_not)
	//		printf("	PASS!\n");
	//	else
	//		printf("	NOT PASS\n");
		index = index->next_address_point;	
	}
	int incept_from_input;
	printf("\n	1 2	name or id\n");
	scanf("%d", &incept_from_input);
	
	if(incept_from_input == 1)
		sort(array_sort+1, array_sort+sum+1, cmp_Positive_Order_Name);	
	else if(incept_from_input == 2)
		sort(array_sort+1, array_sort+sum+1, cmp_Positive_Order_Id);
	else
		printf("\nerror, do not have this opion!\n");
	print_Array_Sort(array_sort);
}
/*
 * 采用头插入法插入新的节点
 */
Mem_List_Ptr insert_Member_List(const Mem_List_Ptr incept_from_txt_pointer, Mem_List_Ptr member_head_list)
{
	return create_Member_List(incept_from_txt_pointer, member_head_list);
}

/*
 * 采用头插入法录入从文件中读取的节点
 */
Mem_List_Ptr create_Member_List(const Mem_List_Ptr incept_from_txt_pointer, Mem_List_Ptr member_head_list)
{
	Mem_List_Ptr index = NULL;
	index = (Mem_List_Ptr)malloc(sizeof(struct member_List));
	if(index == NULL)
		printf("error!\n failed!\n");
	
	memset(index, 0, sizeof(struct member_List));

	
	strcpy(index->id, incept_from_txt_pointer->id);
	strcpy(index->name, incept_from_txt_pointer->name);
	index->pass_or_not = true;
	for(int i = 1; i <= 4; ++i)
	{
		index->pass_or_not = index->pass_or_not && (incept_from_txt_pointer->score[i] >= 90);
		index->score[i] = incept_from_txt_pointer->score[i];
	}

	index->next_address_point = member_head_list;
	member_head_list = index;	
	return member_head_list;
/**
 *struct member_List
{
	char id[10];
	char  name[20];
	int score[5];
	bool pass_or_not;
};
 */	
}
void destroy_Member_List(Mem_List_Ptr member_head_list)
{
		
	Mem_List_Ptr index = NULL;
	index = member_head_list;
	while(member_head_list)
	{
		index = member_head_list;
		member_head_list = member_head_list->next_address_point;
		free(index);
	}
}		
int main()
{
	Mem_List_Ptr member_head_list = NULL;
	member_head_list = file_Load_Member(member_head_list);	
	//打印链表
	print_Member_List(member_head_list);
	
//Mem_List_Ptr remove_Member_List(const char *aim_string, Mem_List_Ptr member_head_list)
	member_head_list = remove_Member_List("lp", member_head_list);
	print_Member_List(member_head_list);
	

//void find_Member_List(cconst char aim_string[], const Mem_List_Ptr member_head_list)
	find_Member_List("pc", member_head_list);
	//printf("hello!\n");	
	find_Member_List("4", member_head_list);

	printf("\n\n");
	
	struct member_List demo_mem;
	strcpy(demo_mem.id, "9");
	strcpy(demo_mem.name, "zl");
	
//Mem_List_Ptr insert_Member_List(const Mem_List_Ptr incept_from_txt_pointer, Mem_List_Ptr member_head_list)
	member_head_list = insert_Member_List(&demo_mem, member_head_list);
//member_head_list = remove_Member_List("5", member_head_list);
	//print_Member_List(member_head_list);
	
	//member_head_list = remove_Member_List("1", member_head_list);
	print_Member_List(member_head_list);
	

//void alter_Member_List(const char aim_string[], const Mem_List_Ptr member_head_list);
	
	printf("\n\n");

	alter_Member_List("pc", member_head_list);
	print_Member_List(member_head_list);
	
	printf("\n");
	//void sort_Member_List(const Mem_List_Ptr member_head_list)
	while(1)
	{
		sort_Member_List(member_head_list);
		printf("\n");
	}
	destroy_Member_List(member_head_list);
	exit(0);
}
