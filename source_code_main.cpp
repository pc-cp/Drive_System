#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <errno.h>
#include <cstring>

using namespace std;

struct member_List
{
	int id;
	char  name[20];
	int score[5];
	bool pass_or_not;
	struct member_List *next_address_point;
};
typedef struct member_List* Mem_List_Ptr;
//检查文件是否打开成功
void open_File_Check(FILE *fp);
//加载管理员的名单
void file_Load_Admin();
//加载教练的名单
void file_Load_Jiaolian();
//加载学员的名单 
void file_Load_Member();

void open_File_Check(FILE *fp)
{
	if(fp == NULL)
	{
		printf("fopen():%s\n", strerror(errno));
		exit(1);
	}
}
void file_Load_Member()
{
	FILE *fp = NULL;
	fp = fopen("/root/20210830/Txt_Doc/member.txt", "r");
	open_File_Check(fp);
	struct member_List incept_from_txt;
	while(~fscanf(fp, "%d	%s	%d	%d	%d	%d", &incept_from_txt.id, incept_from_txt.name, &incept_from_txt.score[1], &incept_from_txt.score[2], &incept_from_txt.score[3], &incept_from_txt.score[4]))
	{
	printf("%d	%s	%d 	%d	%d	%d\n", incept_from_txt.id, incept_from_txt.name, incept_from_txt.score[1], incept_from_txt.score[2], incept_from_txt.score[3], incept_from_txt.score[4]);
	}
	fclose(fp);
}
Mem_List_Ptr create_Member_List(const Mem_List_Ptr incept_from_txt_pointer, Mem_List_Ptr member_head_list)
{
	Mem_List_Ptr head_list = NULL, index = NULL, tail_list = NULL;
	//head_list = (Mem_List_Ptr)malloc(sizeof(struct member_List));
	//memset(head_list, 0, sizeof(struct member_List));
	
	tail_list = head_list;
	
	index = (Mem_List_Ptr)malloc(sizeof(struct member_List));
	memset(index, 0, sizeof(struct member_List));
	
	
	index->id = incept_from_txt_pointer->id;
	strcpy(incept_from_txt_pointer->name, index->name);
	index->next_address_point = NULL;
	
	
	tail_list = index;	
/**
 *
 *
 *struct member_List
{
	int id;
	char  name[20];
	int score[5];
	bool pass_or_not;
};
 */	
	if(member_head_list != NULL)
		tail_list->next_address_point = member_head_list->next_address_point;
	return head_list;
}		
int main()
{
	file_Load_Member();	
//	Mem_List_Ptr a, b;
//	printf("%d %d\n", sizeof(a), sizeof(b));
	exit(0);
}
