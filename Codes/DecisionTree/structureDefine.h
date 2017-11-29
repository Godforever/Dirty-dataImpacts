#ifndef STRUCTUREDEFINE_H
#define  STRUCTUREDEFINE_H
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <time.h>
#include <fstream>
#include <windows.h>
#include <winnt.h>
#define N 100
using namespace std;
typedef struct instance
{
	char spiltAttrName[N][N];
	char spiltAttr[N][N];
	double spiltAttrNum[N];
	char categorization[N];//Ŀ�����Է���
	struct instance* next;
	struct instance* pre;
}Instance;
typedef struct attr
{
	char chr[N];//������
	int partition;//�÷��������¶�ѵ��Ԫ�����ֵĸ���,��partitionΪ0����Ϊ����������
	char partitionName[N][N];
	double spiltPoint;//���������Եķ��ѵ�
	int list[N][N];//����0����ÿ�д���һ�����֣�ÿ�е�һ��Ԫ�ؼ�¼�û�����Ԫ�ص��ܸ���������Ԫ�ؼ�¼��Ŀ�Ļ���D��ͬ���ּ��µ�Ԫ�ظ���,��0�У�0�ŵ�Ԫ�洢ѵ�����ܸ������������δ洢�����ԵĲ�ͬ���ּ���Ԫ�ظ�����
}attribute;
ifstream fileData;
ifstream fileAttributes("DecisionTree//attribute.txt");
ifstream fileInput("DecisionTree//classValue.txt");//��ȡĿ�Ļ��ֵ��������
int n;
bool isvisited[N] = { 0 };//�������ƽ���������ĳ�����Ƿ��ѷ���
typedef struct node
{
	struct node* child[N];
	int brands;//˵����֧�ĸ�������Ϊ0��Ϊ�սڵ�
	Instance* head;
	char decisionName[N];
	int decisionSubscript;//���±�����ָ���������ĸ�����
	char endName[N];//�սڵ�ı�ǩ��
}DTree; 

double Entropy(int partition, int list[])//�������Ե���,numΪ���ϵĻ���
{
	int n = partition;
	double entropy = 0;
	for (int i = 1; i <= n; i++)
	{
		double temp = ((double)(list[i]) / list[0]);
		if (temp == 0) continue;
		entropy += temp*(log10(temp) / log10(2));
	}
	if (entropy == 0) return 0;
	return -entropy;
}

double info(attribute spilt, int partition)//����ĳ�������Ի����µ�������Ϣ��spiltΪ������������partition��Ŀ�Ļ��ֵĻ�����Ŀ����aim�Ļ�����Ŀ��
{
	int spiltPartition = spilt.partition;
	double info = 0;
	if (spilt.partition != 0) {
		for (int i = 1; i <= spiltPartition; i++)
		{
			double temp = ((double)(spilt.list[0][i]) / spilt.list[0][0]);
			temp *= Entropy(partition, spilt.list[i]);
			info += temp;

		}
	}
	else {
		for (int i = 1; i <= 2; i++)
		{
			double temp = ((double)(spilt.list[0][i]) / spilt.list[0][0]);
			temp *= Entropy(partition, spilt.list[i]);
			info += temp;

		}
	}
	return info;
}

int searchMax(double array[], int n)
{
	int k = 0;
	for (int i = 0; i < n; i++)
		if (!isvisited[i]) {
			k = i; break;
		}
	for (int i = k; i < n; i++)
	{
		if (isvisited[i]) continue;
		if (array[i] > array[k]) k = i;
	}
	return k;
}

void freeList(Instance* head)
{
	Instance* p = head;
	if (p == NULL) return;
	else {
		while (p->next != NULL) {
			Instance* q = p->next;
			p->next = q->next;
			delete q;
		}
	}
}

int lengthOfList(Instance * head)
{
	int count = 0;
	while (head->next != NULL)
	{
		count++;
		head = head->next;
	}
	return count;
}

void addNewInstance(Instance * head, Instance * v)
{
	while (head->next != NULL) head = head->next;
	head->next = v;
	v->next = NULL;
}

Instance * createNewInstanceList()
{
	Instance* head = new Instance;
	head->next = NULL;
	return head;
}

Instance * createNewInstanceLists(int n)
{
	Instance* headList = new Instance[n];
	for (int i = 0; i < n; i++)
		headList[i].next = NULL;
	return headList;
}

int* insertSort(Instance* trainingList,int i,attribute aim) {//��ѵ�������ڵ�i�����Խ��в�������,���ص���һ�����飬0��ԪΪ��Ԫ�ظ���������n����ԪΪ��Ӧ��ǩ�ĸ���
	Instance *p = trainingList;
	Instance*q = trainingList->next;
	p->pre = NULL;
	int* count = new int[aim.partition+1];
	for (int i = 0; i <=aim.partition; i++) count[i] = 0;
	while (q != NULL) {
		for (int i = 0; i < aim.partition; i++) {
			if (strcmp(aim.partitionName[i], q->categorization) == 0) {
				count[i + 1]++;
			}
		}
		q->pre = p;
		q = q->next;
		p = p->next;
		count[0]++;
	}
	p = trainingList->next;
	while (p != NULL) {
		Instance* temp = p->next;
		q = p->pre;
		while (q->pre != NULL) {
			if (q->spiltAttrNum[i] >= p->spiltAttrNum[i]) break;
			else q = q->pre;
		}//ѭ��������pӦ�ò���q�ĺ���
		if(p->next!=NULL) p->next->pre = p->pre;
		p->pre->next = p->next;
		if(q->next!=NULL) q->next->pre = p;
		p->pre = q;
		p->next = q->next;
		q->next = p;
		p = temp;
	}
	/*Instance* pttt = trainingList;
	while (pttt->next != NULL) {
		for (int i = 0; i < 3; i++) cout << pttt->next->spiltAttrNum[i] << "-";
		cout << pttt->next->spiltAttrNum[3] << endl;
		pttt = pttt->next;
	}*/
	return count;
}
#endif 