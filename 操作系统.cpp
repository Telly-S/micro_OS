#include <stdio.h> 
#include <stdlib.h>
#include <tchar.h>
#include <stdio.h>
#include <string.h> 
#include <windows.h>
#define OK 0
#define OVERFLOW 1
#define L 10 

/*
��ҵ���� 
*/ 

typedef struct JCB {     //������̿��ƿ�
	char num[2];   //��ҵ��
	char name[10];   //��ҵ��
	char state;   //����״̬
	int tijiaotime;  //�ύ��ҵʱ��
	int starttime;  //��ҵ��ʼʱ��
	int finishtime;  //����ʱ��
	int needtime;   //������Ҫʱ��
	struct JCB *next;  //ָ���¸���ҵ
}jcb;
int time=10000,n;       //��ʱ��
           
jcb *head=NULL,*p,*q;
void run_fcfo(jcb *p1) {
	time = p1->tijiaotime > time? p1->tijiaotime:time;
	p1->starttime=time;
	printf("\n����ʱ����%d,��ʼ������ҵ%s\n",time,p1->name);
	time+=p1->needtime;
	p1->state='F';
	p1->finishtime=time;
	printf("��ҵ�� ��ʼʱ�� ����ʱ�� ����ʱ��\n");
	printf("%s,%d,%d,%d",p1->name,p1->starttime,p1->needtime,p1->finishtime);
}

void fcfo() {
	int i,j,t;
	for(j=0;j<n;j++) {
		p=head;
		t=10000;
		for(i=0;i<n;i++) {  //�ҵ���ǰδ��ɵ���ҵ
   			if(p->tijiaotime<t && p->state=='W') {
    			t=p->tijiaotime;
    			q=p;    //��ǵ�ǰδ��ɵ���ҵ
			}
   		p=p->next;
  		}
	run_fcfo(q);
	}
}

void getInfo() {         //��������
	int num;
	printf("\n��ҵ����:");
	scanf("%d",&n);
	for(num=0;num<n;num++) {
		p=(jcb *)malloc(sizeof(jcb));
  		if(head==NULL) {head=p;q=p;}
	printf("�������룺\n��ҵ�ţ���ҵ�����ύʱ�䣬����CPUʱ��\n");
	scanf("%s\t%s\t%d\t%d",&p->num,&p->name,&p->tijiaotime,&p->needtime);
	if(p->tijiaotime < time) time=p->tijiaotime;
	q->next=p;
	p->starttime=0;
	p->finishtime=0;
	p->next=NULL;
	p->state='W';
	q=p;        
	}
}

void jobS() {
	printf("�����ȷ����㷨ģ��......");
	getInfo();
	fcfo();
}


/*
���̵��� 
*/ 

char pro[20] ;    //���� 
int processNum;     //������ 
int timeSlice = 0;   //ʱ��Ƭ 
typedef char QlemTypeChar;
typedef int QlemTypeInt;
typedef int Status;
typedef struct QNode {
	QlemTypeChar data;
	QlemTypeInt timeArrive = 0;
	QlemTypeInt timeService = 0;
	QlemTypeInt timeCount = 0;
	QlemTypeInt runCount = 0;
	QlemTypeInt timeFinal = 0;   //���ʱ��
	QlemTypeInt timeRound = 0;  //��תʱ��
	float timeRightRound = 0;  //��Ȩ��תʱ��
	QlemTypeChar proState = 'W';     //���̵�״̬��W--����̬��R--ִ��̬��F--���̬   
	struct 	QNode *next;   //����ָ��   
}QNode, *QueuePtr;

typedef struct {
	QueuePtr front;    //��ͷָ��
	QueuePtr rear;     //��βָ��
}LinkQueue;

Status InitQueue(LinkQueue &Q) {
	Q.front = Q.rear = (QueuePtr)malloc(sizeof(QNode));
	if(!Q.front) exit(OVERFLOW);
	Q.front->next = NULL;
	return OK;
}

Status EnQueue(LinkQueue &Q, QlemTypeChar e) {
	QueuePtr p;
	p = (QueuePtr)malloc(sizeof(QNode));
	if (!p) exit(OVERFLOW);
	p->data = e;
	p->next = NULL;
	Q.rear->next = p;
	Q.rear = p;
	return OK;
}

Status DeQueue(LinkQueue &Q, QlemTypeChar &e) {
	QueuePtr p;
	if (Q.front == Q.rear) return ERROR;
	p = Q.front->next;
	e = p->data;
	Q.front->next = p->next;
	if (Q.rear == p) Q.rear = Q.front;
	free(p);
	return OK;
}

LinkQueue QPro;
QNode qq[10];

void ProGetFirst() {          //ȡ���������ж��׽���
	InitQueue(QPro);
	printf("������Ҫ�����Ľ�������:\n");
	for (int i = 0; i < processNum-1; i++) {
		fflush(stdin);
		scanf("%c", &pro[i]);
	}
	fflush(stdin);
	for (int i = 0; i<processNum-1; i++) {
		qq[i].data = pro[i];
		EnQueue(QPro, qq[i].data);
	}
}

void scanfData() {
	printf("������Ҫ�����Ľ�����Ŀ:");
	scanf("%d", &processNum);
	processNum++;
	fflush(stdin);
 	printf("\n");
	ProGetFirst();
	printf("�������̵���ʱ�䣺\n");
	int time_Arr[10];
	for (int i = 0; i < processNum-1; i++) {
		scanf("%d", &time_Arr[i]);
	}
	for (int i =0; i<processNum-1; i++) {
		qq[i].timeArrive = time_Arr[i];
		EnQueue(QPro, qq[i].timeArrive);
	}
	printf("�������̷���ʱ�䣺\n");
	int time_Ser[10];
	for (int i = 0; i < processNum-1; i++) {
		scanf("%d", &time_Ser[i]);
	}
	for (int i = 0; i<processNum-1; i++) {
		qq[i].timeService = time_Ser[i];
		EnQueue(QPro, qq[i].timeService);
	}
	printf("������ʱ��Ƭ��С��:");
	scanf("%d", &timeSlice);
	printf("\n");
}

void ProOutPut1(){           //��ȡ������Ϣ
	printf("������\t     ����ʱ��\t   ����ʱ��\t   ����״̬\t   ִ�д���\n");
	for (int i = 0; i < processNum - 1; i++){
			printf("%c\t\t%d\t\t%d\t\t%c\t\t%d\n", qq[i].data, qq[i].timeArrive, qq[i].timeService, qq[i].proState, qq[i].runCount);
	}
}

void CalculatetimeFinal() {         //�������ʱ��
	int timecou=0;
	int countTemp = 0;
	QlemTypeChar ee;
	for (int i = 0; i < processNum - 1; i++) {
		countTemp += qq[i].timeService;
	}
	while (timecou < countTemp) {
		for (int i = 0; i < processNum - 1; i++) {
			if (qq[i].timeFinal == 0) {
				if (qq[i].timeService - qq[i].timeCount >= timeSlice) {
					timecou += timeSlice;
				}
				else {
					timecou += (qq[i].timeService - qq[i].timeCount);
					//DeQueue(QPro, ee);
				}
				if (timeSlice < qq[i].timeService) { //ʱ��Ƭ��С< ����ʱ��
					int timetemp = timeSlice > qq[i].timeService ? qq[i].timeService : timeSlice;
					if ((qq[i].timeCount + timetemp) < qq[i].timeService) {
						if (qq[i].timeService - qq[i].timeCount >= timeSlice) {
							qq[i].timeCount += timeSlice;
						}
						else {
							qq[i].timeCount += (qq[i].timeService - qq[i].timeCount); 
						}
					}
					else {
						if (qq[i].timeFinal == 0) {
							qq[i].timeFinal = timecou;
						}
					}
				}
				else {   //ʱ��Ƭ��С>= ����ʱ��
					qq[i].timeFinal = timecou;   //�ý��̵����ʱ��=count
				}
			}
		}
	}
	for (int i = 0; i < processNum - 1; ++i) {
		qq[i].timeRound = qq[i].timeFinal - qq[i].timeArrive;
		qq[i].timeRightRound = (float)qq[i].timeRound / qq[i].timeService;
	}
}

void ProOutPut2() {           //��ȡ���̴�������Ϣ
	printf("������\t   ����ʱ��     ����ʱ��     ���ʱ��     ��תʱ��     ��Ȩ��ת\n");
	for (int i = 0; i < processNum - 1; i++) {
		printf("%c\t\t%d\t%d\t%d\t%d\t%.2f\n", 
			qq[i].data, qq[i].timeArrive, qq[i].timeService, qq[i].timeFinal, qq[i].timeRound, qq[i].timeRightRound);
	}
}

int courseS() {
	scanfData();
	ProOutPut1();
	CalculatetimeFinal();
	printf("\n");
	printf("CPU������......\n");
	printf("���ʱ�䣺");
	for (int i = 0; i < processNum - 1; i++) {
		printf("%d,", qq[i].timeFinal);
	}
	printf("\n");
	printf("��תʱ�䣺");
	for (int i = 0; i < processNum - 1; i++) {
		printf("%d,",qq[i].timeRound);
	}
	printf("\n");
	printf("��Ȩ��תʱ�䣺");
	for (int i = 0; i < processNum - 1; i++) {
		printf("%.2f,", qq[i].timeRightRound);
	}
	printf("\n");
	ProOutPut2();
	return 0;
}


/*
�ڴ���� 
*/

typedef struct LNode  {
	int startaddress; 
    int size; 
    int state; 
}LNode; 
LNode 
P[L]={{0,128,0},{200,256,0},{500,512,0},{1500,1600,0},{5000,150,0}}; 
int N=5; int f=0; 
void print() {
	int i; 
    printf("��ʼ��ַ  ����  ״̬\n"); 
    for(i=0;i<N;i++) 
		printf("%3d  %8d  %4d\n",P[i].startaddress,P[i].size,P[i].state);
} 

void First() {
	int i,l=0,m; 
	printf("\n���������������Ĵ�С��"); 
	scanf("%d",&m); 
	for(i=0;i<N;i++) {
	   if(P[i].size<m) 
           continue; 
        else if(P[i].size==m) {
			P[i].state=1; 
        	l=1; 
        	break;
		} 
        else {         
			P[N].startaddress=P[i].startaddress+m; 
       		P[N].size=P[i].size-m; 
       		P[i].size=m;P[i].state=1; 
       		l=1;N++; 
       		break;
		}
	} 
   if(l==1||i<N) {
   		printf("��ַ�ɹ�����\n\n"); 
   		printf("��ַ����ɹ����״̬��\n"); 
		print();
	} 
   else  
    	printf("û�п��Է���ĵ�ַ�ռ�\n");
	} 

void CirFirst() {
	int l=0,m,t=0; 
	printf("\n���������������Ĵ�С��"); 
   	scanf("%d",&m); 
   	while(f<N) {
	    if(P[f].size<m) {
			f=f+1; 
       		if(f%N==0) {
				f=0;t=1;
			} 
       continue;
	   } 
    	if(P[f].size==m && P[f].state!=1) {
			P[f].state=1; 
     		l=1;f++; 
     		break;
		} 
    	if(P[f].size>m && P[f].state!=1) {
			P[N].startaddress=P[f].startaddress+m; 
			P[N].size=P[f].size-m; 
    		P[f].size=m; 
    		P[f].state=1; 
    		l=1;N++;f++;
			break;
		}
	} 
   if(l==1) {
		printf("��ַ�ɹ�����\n\n"); 
      	printf("��ַ����ɹ����״̬��\n");print();
	} 
    else printf("û�п��Է���ĵ�ַ�ռ�\n");
} 

void Worst() {
	int i,t=0,l=0,m; 
	int a[L]; 
	printf("\n���������������Ĵ�С��"); 
	scanf("%d",&m); 
	for(i=0;i<N;i++) {
		a[i]=0;  
    	if(P[i].size<m) 
           continue; 
        else if(P[i].size==m) {
			P[i].state=1; 
        	l=1;break;
		} 
        else a[i]=P[i].size-m;
	} 
   if(l==0) {
    	for(i=0;i<N;i++) {
			if(a[i]!=0) 
        	t=i;
		} 
		for(i=0;i<N;i++) {
			if(a[i]!=0 && a[i]>a[t]) 
				t=i;
			}
     	P[N].startaddress=P[t].startaddress+m; 
  		P[N].size=P[t].size-m; 
  		P[t].size=m;P[t].state=1; 
  		l=1;N++;
	} 
    if(l==1||i<N) {
		printf("��ַ�ɹ�����\n\n"); 
		printf("��ַ����ɹ����״̬��\n"); 
   		print();
	} 
   else printf("û�п��Է���ĵ�ַ�ռ�\n");
} 

void Best() {
	int i,t=0,l=0,m; 
   	int a[L]; 
   	printf("\n���������������Ĵ�С��"); 
   	scanf("%d",&m); 
   	for(i=0;i<N;i++) {
	    a[i]=0;  
    	if(P[i].size<m) 
        	continue; 
        else if(P[i].size==m) {
			P[i].state=1; 
        	l=1; 
        	break; 
  		} 
        else a[i]=P[i].size-m;   
	} 
   if(l==0) {
       for(i=0;i<N;i++) {
	    	if(a[i]!=0) 
        	t=i;
		} 
       for(i=0;i<N;i++) {
			if(a[i]!=0 && a[i]<a[t]) 
        	t=i;
		}  
     	P[N].startaddress=P[t].startaddress+m; 
  		P[N].size=P[t].size-m; 
  		P[t].size=m;P[t].state=1; 
  		l=1;N++;
	} 
   if(l==1||i<N) {
		printf("��ַ�ɹ�����\n\n"); 
   		printf("��ַ����ɹ����״̬��\n"); 
   		print();
	} 
   else printf("û�п��Է���ĵ�ַ�ռ�\n");
} 
   
void pr() {
   	int k=0;
   	printf("��̬���������㷨��"); 
    while(k!=5) {
		printf("\n~~~~~~~~���˵�~~~~~~~~~"); 
		printf("\n1���״���Ӧ�㷨\n2��ѭ���״���Ӧ�㷨"); 
     	printf("\n3�����Ӧ�㷨\n4�������Ӧ�㷨"); 
     	printf("\n5���˳�\n"); 
      	printf("��ѡ���㷨��"); 
      	scanf("%d",&k); 
      	switch(k) {
			case 1: 
  				printf("\n��ʼ״̬Ϊ��\n"); 
            	print(); 
      			First(); 
      			continue; 
  			case 2: 
         		printf("\n��ʼ״̬Ϊ��\n"); 
		 		print(); 
         		CirFirst(); 
         		continue; 
            case 3: 
        		printf("\n��ʼ״̬Ϊ��\n"); 
              	print(); 
           		Worst(); 
        		continue; 
            case 4: 
         		printf("\n��ʼ״̬Ϊ��\n"); 
               	print(); 
            	Best(); 
         		continue; 
            case 5: 
        		break; 
 			default:printf("ѡ�����������ѡ��\n"); 
        }        
	} 
}
   
int memoryM() {    
	pr();
  	return 0;
}


void OS() {
	printf("------------------------------------------------------------------------------------\n");
	printf("******************************�򵥵Ĳ���ϵͳ****************************************\n");
	printf("------------------------------------------------------------------------------------\n");
	printf("\n");
	printf("��ѡ������Ҫ�Ĺ���\n");
	printf("\n");
	printf("1.��ҵ����\n");
	printf("2.���̵���\n");
	printf("3.�ڴ����\n");
	printf("\n");
	scanf("%d",&n);
	switch(n) {
		case 1:
			jobS();
			break;
		case 2:
			courseS();
			break;
		case 3:
			memoryM();
			break;
		default:
			printf("��������Ч����\n");
	}
}

int main() {	
	OS();
}

