#include<stdio.h>
#include<string.h>
#include<stdlib.h>


typedef struct
{
	int c_custkey;    	   //�˿ͱ��
	char c_mkgsegment[20]; //��Ӧ��ĳ���г�����
}customer;				   //�˿ͽṹ�� 

typedef struct
{
	int o_orderkey;    	 //������ 
	int o_custkey;    	 //�˿ͱ��
	char o_orderdate[10];//�������� 
}orders;				 //����

typedef struct
{
	int l_orderkey;//������
	double l_extendedprice;//����۸�
	char l_shipdate[10];//�������� 
}lineitem; //��Ʒ��Ϣ 

typedef struct
{
	int l_orderkey;//������
	char o_orderdate[10];//�������� 
	double l_extendedprice;//����۸�
}select_result;


customer * read_customer_txt() //��ȡcustomer��txt���� 
{
	FILE * fp;
	customer *a=NULL;
	a = (customer *)malloc(101*sizeof(customer));
	int i=0;
	char b;
	fp = fopen("customer.txt","r");
	if(NULL==fp)
	{
		printf("cannot open customer.txt!");
		return NULL;
	}
	while(!feof(fp))
	{	
		
		fscanf(fp,"%d%c%s",&a[i].c_custkey,&b,&a[i].c_mkgsegment);
		
		i++;
	}
	fclose(fp);
	return a;
}
orders * read_orders_txt()//��ȡorders.txt���� 
{
	int i =0; 
	orders * a=NULL;
	a = (orders * )malloc(4001*sizeof(orders));
	char b,c;
	long long d;
	FILE *fp;
	fp = fopen("orders.txt","r");
	if(fp == NULL)
	{
		printf("cannot open orders.txt!");
		return NULL;
	}
	while(!feof(fp))
	{	
		
		fscanf(fp,"%d%c%lld%c%s",&a[i].o_orderkey,&b,&d,&c,&a[i].o_orderdate);
		a[i].o_custkey=d%100;
		
		i++;
	}
	fclose(fp);
	return a;
}

lineitem * read_lineitem_txt()//��ȡlineitem.txt����
{
	FILE * fp;
	lineitem * l=NULL;
	l = (lineitem *)malloc(1001*sizeof(lineitem));
	int i=0;
	char b,c;
	fp = fopen("lineitem.txt","r");
	if(fp==NULL)
	{
		printf("cannot open lineitem.txt!");
		return NULL;
	}
	while(!feof(fp))
	{
		
		fscanf(fp,"%d%c%lf%c%s",&l[i].l_orderkey,&c,&l[i].l_extendedprice,&b,&l[i].l_shipdate);
		
		i++;
	}
	fclose(fp);
	return l; 
}

void bubbleSort(select_result *arr, int n) 
{
    for (int i = 0; i< n; i++)
        for (int j = 0; j < n - i - 1; j++)
        {
            //���ǰ������Ⱥ���С�����н���
            if (arr[j].l_extendedprice < arr[j + 1].l_extendedprice)
			{
                select_result  temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
            }
        }
}

select_result * Select(customer * cus,orders * ord,lineitem * item,char * order_date,char * ship_date,char * mktsegment)//����ѡ�� 
{
	select_result * result=NULL;
	result = (select_result *)malloc(1001*sizeof(select_result));
	int i=0,j=0,k=0,count=0;
	for(i=0;i<100;i++)
		if(strcmp(cus[i].c_mkgsegment,mktsegment)==0)
			for(j=0;j<4000;j++)
				if((ord[j].o_custkey==cus[i].c_custkey) && strcmp(order_date,ord[j].o_orderdate)==1)
					for(k=0;k<1000;k++)
						if(item[k].l_orderkey==ord[j].o_orderkey && strcmp(item[k].l_shipdate,ship_date)==1 )
							{
								result[count].l_orderkey = item[k].l_orderkey;
								strcpy(result[count].o_orderdate,ord[j].o_orderdate);
								result[count].l_extendedprice = item[k].l_extendedprice;
								count++;
							}
	//printf("%d\n",count);
	for(i=0;i<count-1;i++)
		for(j=i+1;j<count;j++)
		{
			if(result[i].l_orderkey == result[j].l_orderkey && strcmp(result[i].o_orderdate,result[j].o_orderdate)==0)
				{
					result[i].l_extendedprice = result[i].l_extendedprice + result[j].l_extendedprice;
					result[j].l_extendedprice = 0;
				}
		}
	bubbleSort(result,count);
	
	return result;
	



	
}
int change_argv_to_number(char s[])//�������������������ַ���ת��Ϊ�������� 
{
	int i=0;
	int number=0;
	while(s[i]!='\0')
	{
		if(i==0)
			number = (s[i]-48);
		else
			number = number*10 + (s[i]-48);
		
		i++;
	}
	return number;
}

int main(int argc,char * argv[])//argc��ʾ�������ݵ��ܸ�����argv[]�ڱ�������������� 
{
	int i,j;
	int num;
	
	int limit=3;
	char order_date[10];
	char ship_date[10];
	char mktsegment[20];
	select_result *result=NULL;
	customer * cus = NULL;//ָ��ͻ�����ָ�� 
	orders * ord = NULL;//ָ�򶩵�����ָ�� 
	lineitem * item = NULL;//ָ�� ��Ʒ����ָ�� 
	cus = read_customer_txt();//��ȡcustomer.txt������ ������ͻ��� 
	ord = read_orders_txt();//��ȡorders.txt������ �����붩���� 
	item = read_lineitem_txt();//��ȡlineitem.txt������ �������Ʒ�� 
	num = change_argv_to_number(argv[4]);//�ܹ�����Ĵ���
	
	for(i=0;num>0;num--,i=i+4)
	{
		strcpy(mktsegment,argv[5+i]);
		strcpy(ship_date,argv[7+i]);
		strcpy(order_date,argv[6+i]);
		limit = change_argv_to_number(argv[8+i]);
		//printf("%d	mktsegment:%s	order_date:%s	ship_date:%s	limit:%d\n",num,mktsegment,order_date,ship_date,limit);
		//strcpy(order_date,argv[6+i]);
		result=Select(cus,ord,item,order_date,ship_date,mktsegment);
		printf("l_orderkey|o_orderdate|revenue\n");
		for(j=0;j<limit;j++)
		{
			if(result[j].l_extendedprice==0 || result[j].l_orderkey<=0)
				printf("null      |null       |null   \n");
			else
				printf("%-10d|%-11s|%-20.2lf\n",result[j].l_orderkey,result[j].o_orderdate,result[j].l_extendedprice);
		}
		free(result);
		result = NULL;
	} 
	
	
	return 0;
}