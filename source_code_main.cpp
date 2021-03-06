#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <errno.h>
#include <cstring>
#include <algorithm>
#include <unistd.h>
#include "color.h"
using namespace std;

int sort_array_length = 0; //	The number of the members
int sum_member = 0;
int pass_number[5]; //pass_number[i] represent total passing subject i number;
const int N = 1e2 + 10;
struct member_List
{
	char id[20];
	char name[20];
	int score[5];
	bool pass_or_not;
	struct member_List *next_address_point;
};
typedef struct member_List* Mem_List_Ptr;
struct member_List array_sort[N];

struct admin_List
{
	char name[20];
	struct admin_List *next_address_point;
};
typedef struct admin_List* Adm_List_Ptr;

struct coach_List
{
	char name[20];
	struct coach_List *next_address_point;
};
typedef struct coach_List* Coa_List_Ptr;

//检查文件是否打开成功
void open_File_Check(FILE *fp);
//加载学员的名单
Mem_List_Ptr file_Load_Member(Mem_List_Ptr member_head_list);
//加载管理员的名单
Adm_List_Ptr file_Load_Admin(Adm_List_Ptr admin_head_list);
//加载教练的名单
Coa_List_Ptr file_Load_Coach(Coa_List_Ptr coach_head_list);
//创建学员名单(list)
Mem_List_Ptr create_Member_List(const Mem_List_Ptr incept_from_txt_pointer, Mem_List_Ptr member_head_list);
//创建管理员名单(list)
Adm_List_Ptr create_Admin_List(const Adm_List_Ptr incept_from_txt_pointer, Adm_List_Ptr admin_head_list);
//创建教练名单(list)
Coa_List_Ptr create_Coach_List(const Coa_List_Ptr incept_from_txt_pointer, Coa_List_Ptr coach_head_list);

//插入新学员
Mem_List_Ptr insert_Member_List(const Mem_List_Ptr incept_from_txt_pointer, Mem_List_Ptr member_head_list);
//删除新学员
Mem_List_Ptr remove_Member_List(const char *aim_string, Mem_List_Ptr member_head_list);
//查询学员
void find_Member_List(const char aim_string[], const Mem_List_Ptr member_head_list);
//修改学员信息
void alter_Member_List(const char aim_string[], const Mem_List_Ptr member_head_list);

//use quick sort for array
void sort_Member_List(const Mem_List_Ptr member_head_list);
//print sorted array
void print_Array_Sort(const struct member_List array[], int flag);

//user login
int User_Login(char name[], const Mem_List_Ptr member_head_list, const Adm_List_Ptr admin_head_list, const Coa_List_Ptr coach_head_list);
//write updated information of member to member.txt
void data_Write(Mem_List_Ptr member_head_list);

//释放三根链表
void destroy_member_List(Mem_List_Ptr member_head_list);
void destroy_Admin_List(Adm_List_Ptr admin_head_list);
void destroy_Coach_List(Coa_List_Ptr coach_head_list);
void progress_Bar_Print();
//打印彩色进度条 
void progress_Bar_Print()
{
   char buf[102] = "#";
   char sym[] = "/|-\\";
   for(int i = 1;i <= 100;i++)
   {
      buf[i] = '#';
      //设置进度条为蓝色，百分比进度为绿色，\r为不换行，光标移动到行首输出
      //printf("\033[34m[%-100s]\033[32m[%d%]\033[0m[%c]\r",buf,i,sym[i%4]);
      //因为没有遇到\n因此需要刷新缓冲区立刻输出printf的内容，，否则会先执行usleep
      printf( BLUE "[%-100s]" L_GREEN "[%d%%]" NONE "[%c]\r",buf,i,sym[i%4]);
      fflush(stdout);
      //Linux下usleep是按照毫秒计算的
      usleep(30000);
   }
   printf("\n");
}
//whether a is front of b 
bool cmp_Positive_Order_Id(struct member_List &a, struct member_List &b)
{
	if(strlen(a.id) != strlen(b.id))
		return strlen(a.id) < strlen(b.id);
	else if(strcmp(a.id, b.id) <= 0)
		return true;
	return false;
}
bool cmp_Positive_Order_Name(struct member_List &a, struct member_List &b)
{
	if(strcmp(a.name, b.name) <= 0)
		return true;
	return false;
}
/*
	检查文件是否打开成功
*/
void open_File_Check(FILE *fp)
{
	if(fp == NULL)
	{
		printf("fopen():%s\n", strerror(errno));
		exit(1);
	}
}

//加载管理员的名单
Adm_List_Ptr file_Load_Admin(Adm_List_Ptr admin_head_list)
{
	int return_fscanf;
	FILE *fp = NULL;
	fp = fopen("/root/20210830/Txt_Doc/admin.txt", "r");
	open_File_Check(fp);
	struct admin_List incept_from_txt;
	
	while((return_fscanf =  fscanf(fp, "%s", incept_from_txt.name) != EOF))
	{
	//	printf("return_fscanf_= %d\n", return_fscanf);
	//	printf("%s\n", incept_from_txt.name);
		admin_head_list = create_Admin_List(&incept_from_txt, admin_head_list);
	}
	fclose(fp);
	return admin_head_list;
}
//将admin中的数据元素插入list
Adm_List_Ptr create_Admin_List(const Adm_List_Ptr incept_from_txt_pointer, Adm_List_Ptr admin_head_list)
{
	Adm_List_Ptr index = NULL;
	index = (Adm_List_Ptr)malloc(sizeof(struct admin_List));
	if(index == NULL)
		printf("error!\n failed!\n");
	
	memset(index, 0, sizeof(struct admin_List));

	
	strcpy(index->name, incept_from_txt_pointer->name);
	
	index->next_address_point = admin_head_list;
	admin_head_list = index;	
	return admin_head_list;
}

//加载教练的名单
Coa_List_Ptr file_Load_Coach(Coa_List_Ptr coach_head_list)
{
	int return_fscanf;
	FILE *fp = NULL;
	fp = fopen("/root/20210830/Txt_Doc/coach.txt", "r");
	open_File_Check(fp);
	struct coach_List incept_from_txt;
	
	while((return_fscanf =  fscanf(fp, "%s", incept_from_txt.name) != EOF))
	{
	//	printf("return_fscanf_= %d\n", return_fscanf);
	//	printf("%s\n", incept_from_txt.name);
		coach_head_list = create_Coach_List(&incept_from_txt, coach_head_list);
	}
	fclose(fp);
	return coach_head_list;
}
//将coach中的数据元素插入list
Coa_List_Ptr create_Coach_List(const Coa_List_Ptr incept_from_txt_pointer, Coa_List_Ptr coach_head_list)
{
	Coa_List_Ptr index = NULL;
	index = (Coa_List_Ptr)malloc(sizeof(struct coach_List));
	if(index == NULL)
		printf("error!\n failed!\n");
	
	memset(index, 0, sizeof(struct coach_List));

	
	strcpy(index->name, incept_from_txt_pointer->name);
	
	index->next_address_point = coach_head_list;
	coach_head_list = index;	
	return coach_head_list;
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
	char pass_or_not_string[20];
	while((return_fscanf =  fscanf(fp, "%s	%s	%d	%d	%d	%d	%s", incept_from_txt.id, incept_from_txt.name, &incept_from_txt.score[1], &incept_from_txt.score[2], &incept_from_txt.score[3], &incept_from_txt.score[4], pass_or_not_string)) != EOF)
	{
	//	printf("return_fscanf_= %d\n", return_fscanf);
	//	printf("%s	%s	4 = %d	%s\n", incept_from_txt.id, incept_from_txt.name, incept_from_txt.score[4], pass_or_not_string);
		member_head_list = create_Member_List(&incept_from_txt, member_head_list);
	}
	fclose(fp);
	return member_head_list;
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
}

#if 1
void data_Write(Mem_List_Ptr member_head_list)
{
	//int return_fscanf;
	FILE *fp = NULL;
	//对于member.txt 有则清空，无则创建
	fp = fopen("/root/20210830/Txt_Doc/member.txt", "w");
	open_File_Check(fp);
	
	Mem_List_Ptr index = NULL;
	index = member_head_list;
	while(index)
	{
		fprintf(fp,"%s	%s	%d	%d	%d	%d", index->id, index->name, index->score[1], index->score[2], index->score[3], index->score[4]);
		
		if(index->pass_or_not)
			fprintf(fp, "	PASS!\n");
		else
			fprintf(fp, "	NOT_PASS\n");

		index = index->next_address_point;
	}
	fflush(fp);	
	fclose(fp);
}
#endif
/*
* 采用头插入法插入新的节点
*/
Mem_List_Ptr insert_Member_List(const Mem_List_Ptr incept_from_txt_pointer, Mem_List_Ptr member_head_list)
{
	return create_Member_List(incept_from_txt_pointer, member_head_list);
}

//删除操作
Mem_List_Ptr remove_Member_List(const char aim_string[], Mem_List_Ptr member_head_list)
{
	Mem_List_Ptr index_front = member_head_list, index = member_head_list, index_behind = member_head_list->next_address_point;
	
	//int cnt = 1;
	//printf("remove----\n", );
	//该字符串为姓名或编号	
	while(index != NULL)
	{
		//printf("cnt = %d\n", cnt++);
		//if(cnt >= 20)
			//break;
		if((strcmp(index->name, aim_string) == 0) || (strcmp(index->id, aim_string) == 0))
		{
			if(index_front == index)  //头部就是要删除的节点
			{
				index_front = index_behind; //直接删除第一个节点
				member_head_list = index_front;
				break;
			}
			else if(index->next_address_point == NULL) //尾部才是要删除的节点
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
		if(index != NULL)
		{
			index_behind = index->next_address_point;
		}
		if(index_front->next_address_point != index)
		{
			index_front = index_front->next_address_point;
		}	
		
	}
	//释放已经删除节点的内存
	free(index);
	return member_head_list;
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
					printf("	NOT_PASS\n");
				
				break;
			}
		index = index->next_address_point;	
	}
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
				printf("	NOT_PASS\n");
			
			printf("---重新录入成绩----\n");

			index->pass_or_not = true;
			for(int i = 1; i <= 4; ++i)
			{
				printf("科目[ %d ] : ", i);
				scanf("%d", &index->score[i]);

				index->pass_or_not = index->pass_or_not && (index->score[i] >= 90);
			}
			printf("---更新成功！----\n");
			
			break;
		}
		index = index->next_address_point;	
	}
}

//print the member'list
void print_Member_List(const Mem_List_Ptr member_head_list)
{
	//初始化
	for(int i = 0; i <= 4; ++i)
		pass_number[i] = 0;
	
	Mem_List_Ptr index = NULL;
	index = member_head_list;
	sum_member = 0; // 初始化
	while(index)
	{
		sum_member++;
		printf("%-3s	%-10s	%d 	%d	%d	%d", index->id, index->name, index->score[1], index->score[2], index->score[3], index->score[4]);
		for(int i  = 1; i <= 4; ++i)
		{
			if(index->score[i] >= 90)
				pass_number[i]++;
		}
		if(index->pass_or_not)
		{
			pass_number[0]++;  //如果该学员通过则pass_number[0] + 1	
			printf("	PASS!\n");
		}
		else
			printf("	NOT_PASS\n");
		index = index->next_address_point;	
	}
}

#if 1
//printed sorted array use name or id of student
void print_Array_Sort(const struct member_List array[], int flag)
{
	int incept_from_input;
	printf("\n[1]正序 	[2]逆序 !\n");
	scanf("%d", &incept_from_input);
	if(incept_from_input == 1)
	{
		if(flag == 1)
		{
			for(int i = 1; i <= sort_array_length; ++i)
			{
				printf(YELLOW "%-3s" NONE "	%-10s	%d 	%d	%d	%d", array[i].id,  array[i].name, array[i].score[1], array[i].score[2], array[i].score[3], array[i].score[4]);
				if(array[i].pass_or_not)
					printf("	PASS!\n");
				else
					printf("	NOT_PASS\n");
			}

		}
		else
		{
			for(int i = 1; i <= sort_array_length; ++i)
			{
				printf("%-3s	" YELLOW "%-10s" NONE "	%d 	%d	%d	%d", array[i].id,  array[i].name, array[i].score[1], array[i].score[2], array[i].score[3], array[i].score[4]);
				if(array[i].pass_or_not)
					printf("	PASS!\n");
				else
					printf("	NOT_PASS\n");
			}


		}
	}
	else if(incept_from_input == 2)
	{
		if(flag == 1)
		{
			for(int i = sort_array_length; i >= 1; --i)
			{
				printf(YELLOW "%-3s" NONE "	%-10s	%d 	%d	%d	%d", array[i].id,  array[i].name, array[i].score[1], array[i].score[2], array[i].score[3], array[i].score[4]);
				if(array[i].pass_or_not)
					printf("	PASS!\n");
				else
					printf("	NOT_PASS\n");
			}

		}
		else
		{
			for(int i = sort_array_length; i >= 1; --i)
			{
				printf("%-3s	" YELLOW "%-10s" NONE "	%d 	%d	%d	%d", array[i].id,  array[i].name, array[i].score[1], array[i].score[2], array[i].score[3], array[i].score[4]);
				if(array[i].pass_or_not)
					printf("	PASS!\n");
				else
					printf("	NOT_PASS\n");
			}


		}
	}
}
#endif

void sort_Member_List(const Mem_List_Ptr member_head_list)
{	
	Mem_List_Ptr index = NULL;
	index = member_head_list;
	
	sort_array_length = 0;
	for(int i = 1; index; ++i)
	{
		strcpy(array_sort[i].id, index->id);	
		strcpy(array_sort[i].name, index->name);
		for(int j = 1; j <= 4; ++j)
		{
			array_sort[i].score[j] = index->score[j];
		}
		array_sort[i].pass_or_not = index->pass_or_not;
		++sort_array_length;

		index = index->next_address_point;	
	}
	int incept_from_input;
	printf("\n [1] 编号 		[2] 姓名\n");
	scanf("%d", &incept_from_input);
	
	if(incept_from_input == 1)
		sort(array_sort+1, array_sort+sort_array_length+1, cmp_Positive_Order_Id);	
	else if(incept_from_input == 2)
		sort(array_sort+1, array_sort+sort_array_length+1, cmp_Positive_Order_Name);
	else
		printf("\nerror, do not have this option!\n");
	print_Array_Sort(array_sort, incept_from_input);
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
void destroy_Admin_List(Adm_List_Ptr admin_head_list)
{
	
	Adm_List_Ptr index = NULL;
	index = admin_head_list;
	while(admin_head_list)
	{
		index = admin_head_list;
		admin_head_list = admin_head_list->next_address_point;
		free(index);
	}
}
void destroy_Coach_List(Coa_List_Ptr coach_head_list)
{
	
	Coa_List_Ptr index = NULL;
	index = coach_head_list;
	while(coach_head_list)
	{
		index = coach_head_list;
		coach_head_list = coach_head_list->next_address_point;
		free(index);
	}
}

int find_User_Member(const char aim_string[], const Mem_List_Ptr member_head_list)
{
	Mem_List_Ptr index = NULL;
	index = member_head_list;
	while(index)
	{
		//printf("find_user");
		if(strcmp(index->name, aim_string) == 0)
		{
			return 1; //represent member	
			break;
		}
		index = index->next_address_point;	
	}
	return -1;
}

int find_User_Coach(const char aim_string[], const Coa_List_Ptr coach_head_list)
{
	Coa_List_Ptr index = NULL;
	index = coach_head_list;
	while(index)
	{
		if(strcmp(index->name, aim_string) == 0)
		{
			return 2; //represent coach
			break;
		}
		index = index->next_address_point;	
	}
	return -1;
}

int find_User_Admin(const char aim_string[], const Adm_List_Ptr admin_head_list)
{
	Adm_List_Ptr index = NULL;
	index = admin_head_list;
	while(index)
	{
		if(strcmp(index->name, aim_string) == 0)
		{
			return 3; //represent admin
			break;
		}
		index = index->next_address_point;	
	}
	return -1;
}

int User_Login(char name[], const Mem_List_Ptr member_head_list, const Adm_List_Ptr admin_head_list, const Coa_List_Ptr coach_head_list)
{
	int flag = 0;
	
	flag = find_User_Member(name, member_head_list);
	if(flag == 1)
	{
		
		progress_Bar_Print();
		printf("Welcome %s " L_RED "Student!\n" NONE, name);
		return flag;
	}
	
	flag = find_User_Coach(name, coach_head_list);
	if(flag == 2)
	{
		progress_Bar_Print();
		printf("Welcome %s " L_RED "Coach!\n" NONE, name);
		return flag;
	}
	
	flag = find_User_Admin(name, admin_head_list);
	if(flag == 3)
	{	
		progress_Bar_Print();
		printf("Welcome %s " L_RED "Admin!\n" NONE, name);
		return flag;
	}

	return flag;  //no such man "-1"
}
int main()
{
#if 0
	Mem_List_Ptr member_head_list = NULL;
	member_head_list = file_Load_Member(member_head_list);	
	Adm_List_Ptr admin_head_list  = NULL;
	admin_head_list  = file_Load_Admin(admin_head_list);
	Coa_List_Ptr coach_head_list  = NULL;
	coach_head_list  = file_Load_Coach(coach_head_list);

	//user Login
	int flag = User_Login(member_head_list, admin_head_list, coach_head_list);
	if(flag < 0)
	{
		printf("无此人记录!\n");
		exit(1);
	}
#endif
	Mem_List_Ptr member_head_list = NULL;
	Adm_List_Ptr admin_head_list  = NULL;
	Coa_List_Ptr coach_head_list  = NULL;
	int option = 8;
	char aim_string[20], name[20];
	int flag;
	while(1)
	{
		if(option == 8)
		{
			member_head_list = NULL;
			admin_head_list  = NULL;
			coach_head_list  = NULL;

			member_head_list = file_Load_Member(member_head_list);	
			admin_head_list  = file_Load_Admin(admin_head_list);
			coach_head_list  = file_Load_Coach(coach_head_list);
			//user Login
			//	1. 学员
			//	2. coach
			//	3、admin
			memset(name, '\0', 20*sizeof(char));
			printf("\n--------------登陆窗口用户名:------------\n");
			scanf("%s", name);
			flag = User_Login(name, member_head_list, admin_head_list, coach_head_list);
			if(flag < 0)
			{
				printf("\n无此人记录!\n");
				break;
			}
		}
		printf("\n------******--------\n");
		printf("+1	查找功能     +\n");
		printf("+2	修改功能     +\n");
		printf("+3	插入功能     +\n");
		printf("+4	删除功能     +\n");
		printf("+5	打印功能     +\n");
		printf("+6	排序功能     +\n");
		printf("+7	保存文件功能 +\n");
		printf("+8	退出登陆功能 +\n");
		printf("\n------******--------\n");
		printf("请选择: ");
		scanf("%d", &option);
		getchar();
		switch(option)
		{
			case 1:
			   	if(flag == 1)
				{	
					printf(L_RED);
					find_Member_List(name, member_head_list);
					printf(NONE);
					break;
				}
				printf("\ninput the id or name which you want search:\n");
				memset(aim_string, '\0', 20*sizeof(char));
				cin.getline(aim_string, 20);
				printf(YELLOW);
				find_Member_List(aim_string, member_head_list);
				printf(NONE);
				break;
			case 2:
				if(flag != 2 && flag != 3)
				{
					printf("\nNo such permission\n");
					break;
				}
				memset(aim_string, '\0', 20*sizeof(char));
				printf("\ninput the id or name which you want to alter:\n");
				cin.getline(aim_string, 20);

				alter_Member_List(aim_string, member_head_list);
				break;
			case 3:
				if(flag != 3)
				{
					printf("\nNo such permission\n");
					break;
				}	
				struct member_List aim_member;
				printf("\ninput the information which you want to insert:\n");
				printf("input his id: ");
				cin.getline(aim_member.id, 20); 
				printf("input his name: ");
				cin.getline(aim_member.name, 20);
				for(int i = 1; i <= 4; ++i)
				{
					printf("\n科目[ %d ] : ", i);
					scanf("%d", &aim_member.score[i]);
				}
				member_head_list = insert_Member_List(&aim_member, member_head_list);
				break;
			case 4:
				if(flag != 3)
				{
					printf("\nNo such permission\n");
					break;
				}	
				memset(aim_string, '\0', 20*sizeof(char));
				printf("\ninput the id or name which you want to remove:\n");
				cin.getline(aim_string, 20);

				member_head_list = remove_Member_List(aim_string, member_head_list);
				break;
			case 5:
				if(flag != 2 && flag != 3)
				{
					printf("\nNo such permission\n");
					break;
				}	
				printf(YELLOW);
				print_Member_List(member_head_list);
				printf(NONE);//清除颜色

				if(flag == 3)
				{
					printf(L_RED);
					for(int i = 1; i <= 4; ++i)
					{
						printf("科目[ %d ] 的通过率为:  %d/%d\n", i, pass_number[i], sum_member);
					}
					printf("\n一共通过%d个学员, 通过率为: %d/%d!\n", sum_member, pass_number[0], sum_member);
					printf(NONE);
					break;
				}

				break;
			case 6:
				sort_Member_List(member_head_list);
				break;
			case 7:
				
				progress_Bar_Print();
				data_Write(member_head_list);
				printf("\nwrite Success!\n");	
				break;
			case 8:
				destroy_Member_List(member_head_list);
				destroy_Admin_List(admin_head_list);
				destroy_Coach_List(coach_head_list);
				break;
			default:
				printf("\nnot have this option\n");
				break;
		}
	}
	exit(0);
}
