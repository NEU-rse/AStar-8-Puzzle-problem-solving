#include<stdio.h>
#include<malloc.h>
#include<string.h>

//定义节点的数据结构
typedef struct node_array
{
    int a[3][3];
    int depth;
    int f;
    char text[11];
    struct node_array *ch1;
    struct node_array *ch2;
    struct node_array *ch3;
    struct node_array *ch4;
    struct node_array *parent;
    struct node_array *next;
} node;

node *front=NULL,*rear=NULL,*x,*front_list=NULL,*rear_list=NULL;

int up[3][3],dn[3][3],rt[3][3],lt[3][3],goal[3][3],depth_flag=0,f_count=0;

void set_zero(int a[3][3])
{
    int i,j;
    for(i=0; i<3; i++)
        for(j=0; j<3; j++)
            a[i][j]=0;
}

void copy_array(int a[3][3],int b[3][3])//copy b to a
{
    int i,j;
    for(i=0; i<3; i++)
        for(j=0; j<3; j++)
            a[i][j]=b[i][j];
}
//比较两个棋盘是否一致
int is_equal(int a[3][3],int b[3][3])
{
    int i,j,flag=1;
    for(i=0; i<3; i++)
        for(j=0; j<3; j++)
            if(a[i][j]!=b[i][j])
                flag=0;
    return flag;
}
//交换棋盘中棋子位置
void swap(int *a,int *b)
{
    int temp;
    temp=*a;
    *a=*b;
    *b=temp;
}
//如题
void print_array(int a[3][3])
{
    int i,j;
    for(i=0; i<3; i++)
    {
        for(j=0; j<3; j++)
            printf("%d ",a[i][j]);
        printf("\n");
    }
}

int check_zero(int a[3][3])//returns 1 if 0
{
    int i,j,flag=1;
    for(i=0; i<3; i++)
    {
        for(j=0; j<3; j++)
            if(a[i][j]!=0)
                flag=0;
    }
    return flag;
}
//判断列表内是否还有剩余节点
int check_list(node *nd)//returns 0 if it doesnt match the list
{
    node *new_node=NULL;
    if(front_list==NULL)
        return 0;
    else
    {
        new_node=front_list;
        while(new_node!=NULL)
        {
            if(is_equal(new_node->a,nd->a)==1)
                return 1;
            new_node=new_node->next;
        }
        return 0;
    }
}

//向队列插入子节点
void insert_queue(node *nd)
{
    nd->next=NULL;
    if(front==NULL)
    {
        front=nd;
        rear=nd;
    }
    else
    {
        rear->next=nd;
        rear=nd;
        rear->next=NULL;
    }
}
//向列表插入子节点函数
void insert_list(node *nd)
{
    nd->next=NULL;
    if(front_list==NULL)
    {
        front_list=nd;
        rear_list=nd;
    }
    else
    {
        rear_list->next=nd;
        rear_list=nd;
        rear_list->next=NULL;
    }
}


int mod(int a)
{
    if(a>=0)
        return a;
    else
        return (-1*a);
}
//计算代价函数值
int calc_heuristic(node *nd)
{
    int i,j,k,l,h=0;
    for(i=0; i<=2; i++)
    {
        for(j=0; j<=2; j++)
        {
            if(nd->a[i][j]!=0)
            {
                for(k=0; k<=2; k++)
                {
                    for(l=0; l<=2; l++)
                    {
                        if(nd->a[i][j]==goal[k][l])
                            goto xy;
                    }
                }
xy:
                h+=(mod(i-k)+mod(j-l));
            }
        }
    }
    return h;
}


void next_move(node *nd)
{
    int i,j,x,y;
    for(i=0; i<3; i++)
    {
        for(j=0; j<3; j++)
        {
            if(nd->a[i][j]==0)
                goto label;
        }
    }
// 此处判断记录0出现在0~8的位置后该和哪些位置交换
label:
    x=i;
    y=j;
    if(y+1 >2)
        nd->ch4=NULL;
    else
    {
        nd->ch4=(node*)malloc(sizeof(node));
        copy_array(nd->ch4->a,nd->a);
        swap(&nd->ch4->a[x][y],&nd->ch4->a[x][y+1]);
        if(check_list(nd->ch4)==1)
        {
            nd->ch4=NULL;
            free(nd->ch4);
        }
        else
        {
            nd->ch4->parent=nd;
            nd->ch4->depth=nd->depth+1;
            strcpy(nd->ch4->text,"空格右移");
            nd->ch4->f=nd->depth+calc_heuristic(nd->ch4)+1;
            insert_list(nd->ch4);
            insert_queue(nd->ch4);
        }
    }
    if(y-1 <0)
        nd->ch3=NULL;
    else
    {
        nd->ch3=(node*)malloc(sizeof(node));
        copy_array(nd->ch3->a,nd->a);
        swap(&nd->ch3->a[x][y],&nd->ch3->a[x][y-1]);
        if(check_list(nd->ch3)==1)
        {
            nd->ch3=NULL;
            free(nd->ch3);
        }
        else
        {
            nd->ch3->parent=nd;
            strcpy(nd->ch3->text,"空格左移");
            nd->ch3->depth=nd->depth+1;
            nd->ch3->f=nd->depth+calc_heuristic(nd->ch3)+1;
            insert_list(nd->ch3);
            insert_queue(nd->ch3);
        }
    }
    if(x+1 >2)
        nd->ch2=NULL;
    else
    {
        nd->ch2=(node*)malloc(sizeof(node));
        copy_array(nd->ch2->a,nd->a);
        swap(&nd->ch2->a[x][y],&nd->ch2->a[x+1][y]);
        if(check_list(nd->ch2)==1)
        {
            nd->ch2=NULL;
            free(nd->ch2);
        }
        else
        {
            nd->ch2->parent=nd;
            strcpy(nd->ch2->text,"空格下移");
            nd->ch2->depth=nd->depth+1;
            nd->ch2->f=nd->depth+calc_heuristic(nd->ch2)+1;
            insert_list(nd->ch2);
            insert_queue(nd->ch2);
        }
    }
    if(x-1 <0)
        nd->ch1=NULL;
    else
    {
        nd->ch1=(node*)malloc(sizeof(node));
        copy_array(nd->ch1->a,nd->a);
        swap(&nd->ch1->a[x][y],&nd->ch1->a[x-1][y]);

        if(check_list(nd->ch1)==1)
        {
            nd->ch1=NULL;
            free(nd->ch1);
        }
        else
        {
            nd->ch1->parent=nd;
            strcpy(nd->ch1->text,"空格上移");
            nd->ch1->depth=nd->depth+1;
            nd->ch1->f=nd->depth+calc_heuristic(nd->ch1)+1;
            insert_list(nd->ch1);
            insert_queue(nd->ch1);
        }
    }
}
//判断是否达到目标状态，是则返回1
int is_goal(node *nd)
{
    return is_equal(nd->a,goal);
}
//弹出队列中最靠前的叶子节点
node *pop_queue()
{
    node *nd;
    nd=front;
    front=front->next;
    if(front==NULL)
        rear=NULL;
    return nd;
}
//最后输出具体移动过程到屏幕
void final_print(node *nd)
{
    while(nd!=NULL)
    {
        printf("\n");
        print_array(nd->a);
        printf("\n\n%s\n",nd->text);
        nd=nd->parent;

        f_count++;
    }
    printf("\n总共所需的步数为： %d",f_count-1);
}

int count_queue()
{
    int count=0;
    node *temp;
    temp=front;
    while(temp!=NULL)
    {
        temp=temp->next;
        count++;
    }
    return count;
}
//对各子节点的估计函数值进行排序
void arrange()
{
    node *t1,*t2,*t3;
    int i,j;
    i=count_queue();
    j=i;
    for(int k=0; k<i; k++)
    {
        t1=front;
        t2=front->next;

        for(int l=1; l<j; l++)
        {
            if(t1->next->f>t2->next->f)
            {
                t3=t2;
                t1->next=t2->next;
                t3->next=t3->next->next;
                t1->next->next=t2;
            }
            j--;
            t1=t1->next;
            t2=t2->next;
        }
    }
    t1=front;
    while(t1->next!=NULL)
        t1=t1->next;
    rear=t1;
    rear_list=t1;
}
//A*算法的主程序
void A_star(node *root)
{
    {
        node *nd;
        insert_queue(root);
        insert_list(root);
        nd=root;
        while(1)
        {
            nd=front;
            if(is_goal(nd)==1)
            {
                final_print(nd);
                break;
            }
            next_move(nd);
            arrange();
            nd=pop_queue();
        }
    }
}
//主函数
int main()
{
    node *n;
    int i,j;
    n=(node*)malloc(sizeof(node));
    x=(node*)malloc(sizeof(node));
    printf("============八数码问题求解器（A*）==============\n\n");
    printf("请输入棋子的目标状态：（数码为0-8，空位用0表示）\n");
    for(i=0; i<3; i++)
        for(j=0; j<3; j++)
            scanf("%d",&n->a[i][j]);
//初始化该节点
    n->parent=NULL;
    n->next=NULL;
    n->ch1=NULL;
    n->ch2=NULL;
    n->ch3=NULL;
    n->ch4=NULL;
    n->depth=0;


    printf("请输入当前棋子状态：\n");
    for(i=0; i<3; i++)
        for(j=0; j<3; j++)
            scanf("%d",&goal[i][j]);
//调用启发式搜索算法函数计算第一个f(n)
    n->f=calc_heuristic(n);
    printf("\n棋子的状态转移顺序为：\n",f_count);
//A*搜索调用，并显示状态转移具体过程
    A_star(n);
    printf("\n\n请按任意键后按ENTER键退出程序\n");
//此处避免命令行窗口直接退出
    scanf("%d",&i);
    return 0;
}
