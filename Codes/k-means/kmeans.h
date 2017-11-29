#ifndef KMEANS_H
#define KMEANS_H

#include<iostream>
#include<ctime>
class Kmeans {
public:
	Kmeans();//���캯����ȡ�Ǳ�ǩ�ļ�����
	Kmeans(int y);//���캯�������ǩ�ļ�����
	double**InitArray(int m, int n);
	void ZeroneNormal();//���ݵ�0-1���
	double GetDistance(double* s, double* t);//����ŷ����þ���
	void Cluster();//��N�����ݵ���࣬���ÿ���������ĸ�����
	double GetDifference();//�������о�������ĵ��������ݵ�ľ���֮��
	void GetCenter();
	void Output();
	void K_means();
	void PrintData()
	{
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < D; j++)
			{
				std::cout << DATA[i][j] << "\t";
			}
			std::cout << std::endl;
		}
	}
private:
	clock_t start;
	clock_t end;
	int K;  //�������Ŀ
	int D;	//���ݵ�ά��
	int N;	//���ݸ���
	double* maxdata;//��������е����ֵ
	double* mindata;//��������е���Сֵ
	double** data;//���0-1���򻯵��ļ�����
	double** DATA;//����ļ�����
	double** cluster_center;//������ĵ�
	double* in_cluster;//���ÿ�����ݵ���������ı�־����
};
double** Kmeans::InitArray(int m, int n)
{
	double** Array;
	Array = new double*[m];
	for (int i = 0; i < m; i++)
		Array[i] = new double[n];
	return Array;
}
void Kmeans::ZeroneNormal()
{
	int i, j;
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < D; j++)
		{
			data[i][j] = (DATA[i][j] - mindata[j]) / (maxdata[j] - mindata[j]);
		}
	}
}
double Kmeans::GetDistance(double* s, double* t)
{
	double r = 2;
	double sum = 0.0;
	for (int i = 0; i<D; i++)
		sum += pow(s[i] - t[i], r);
	return pow(sum, 1.0 / r);
}
void Kmeans::Cluster()
{
	int i, j;
	double min;
	double** distance = InitArray(N, K);

	for (i = 0; i<N; ++i)
	{
		min = 9999.9;
		for (j = 0; j<K; ++j)
		{
			distance[i][j] = GetDistance(data[i], cluster_center[j]);
			if (distance[i][j]<min)
			{
				min = distance[i][j];
				this->in_cluster[i] = j;
			}
		}
	}
	for (int i = 0; i < N; i++)
		delete[] distance[i];
	delete[] distance;
}
double Kmeans::GetDifference()
{
	int i, j;
	double sum = 0.0;
	for (i = 0; i<K; ++i) {
		for (j = 0; j<N; ++j) {
			if (i == in_cluster[j])
				sum += GetDistance(data[j], cluster_center[i]);
		}
	}
	return sum;
}
void Kmeans::GetCenter()
{
	double** sum;
	sum = InitArray(K, D);
	int i, j, q, count;
	for (i = 0; i<K; i++)
		for (j = 0; j<D; j++)
			sum[i][j] = 0;
	for (i = 0; i<K; i++)
	{
		count = 0;
		for (j = 0; j<N; j++)
		{
			if (in_cluster[j] == i)
			{
				for (q = 0; q<D; q++)
					sum[i][q] += this->data[j][q];
				count++;
			}
		}
		if (count != 0)
			for (q = 0; q<D; q++)
				this->cluster_center[i][q] = sum[i][q] / count;
	}
	for (int i = 0; i < K; i++)
		delete[] sum[i];
	delete[] sum;
}
void Kmeans::Output()
{
	int i, j;
	FILE *fp[100];
	char sz[32];
	std::cout<<"��ʼ���ݵĴ��ļ����..............\n";
	for (i = 1; i <= K; i++)
	{
		sprintf(sz, "��%d����.csv", i);
		fp[i] = fopen(sz, "a+");
	}
	for (i = 0; i < K; i++)
	{
		for (j = 0; j < N; j++)
		{
			if (i == in_cluster[j])
			{
				int p;
				for (p = 0; p < D - 1; p++)
				{
					fprintf(fp[i + 1], "%lf,", this->DATA[j][p]);
				}
				fprintf(fp[i + 1], "%lf\n", this->DATA[j][p]);
			}
		}
	}
	for (i = 1; i <= K; i++)
	{
		sprintf(sz, "��%d����.txt", i);
		fclose(fp[i]);
	}
}
Kmeans::Kmeans()
{
	int i, j;
	long double x;
	char ch[32];
	char junk;
	FILE *fp;
	std::cout << "�������ļ����ƣ�";
	std::cin >> ch;
	std::cout << "������ص���Ŀ��";
	std::cin >> K;
	start = clock();
	maxdata = new double[100];
	mindata = new double[100];
	if ((fp = fopen(ch, "r")) == NULL)
		fprintf(stderr, "cannot open data.txt!\n");
	D = 0;
	int flag = 1;
	while (flag)
	{
		if (fscanf(fp, "%lf%c", &x, &junk)) 
		{
			maxdata[D] = x;
			mindata[D] = x;
			D++;
		}
		if ( '\n'==junk||'\r'==junk)
			flag = 0;
	}
	fclose(fp);
	fp = fopen(ch, "r");
	while (!feof(fp))
	{
		for (j = 0; j < D; j++)
		{
			if (!fscanf(fp, "%lf%c", &x, &junk)) 
			{
				break;
			}
		}
		if (j == D)
			N++;
	}
	fclose(fp);
	std::cout << "\n��������Ϊ��" << N << std::endl;
	this->data = InitArray(N, D);
	this->DATA = InitArray(N, D);
	in_cluster = new double[N];
	cluster_center = InitArray(K, D);
	fp = fopen(ch, "r");
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < D; j++)
		{
			if (fscanf(fp, "%lf%c", &x, &junk))  
			{
				this->DATA[i][j] = x;
				if (x > maxdata[j])
					this->maxdata[j] = x;
				if (x < mindata[j])
					this->mindata[j] = x;
			}
		}
	}
	std::cout << std::endl;
	fclose(fp);
	std::cout << "\n��ʼ����ing........................................................";
}
Kmeans::Kmeans(int y)
{
	int i, j;
	double x; 
	char ch[32];
	char junk;//��ȡ���ݼ��е���Ч�ַ�
	FILE *fp;
	std::cout << "�������ļ����ƣ�";
	std::cin >> ch;
	std::cout << "������ص���Ŀ��";
	std::cin >> K;
	int **c;//�����������ڼ���׼ȷ��
	c = new int*[K];
	for (i = 0; i < K; i++)
		c[i] = new int[K] {0};
	start = clock();
	if ((fp = fopen(ch, "r")) == NULL)
		fprintf(stderr, "cannot open data.txt!\n");
	D = 0;
	int flag = 1;
	maxdata = new double[100]{ 0 };
	mindata = new double[100]{ 0 };
	while (flag)
	{
		if (fscanf(fp, "%lf%c", &x, &junk)) //��ȡ���ݵ�
		{	
			maxdata[D] = x;
			mindata[D] = x;
			D++;
		}
		if ('\n' == junk || '\r' == junk)
			flag = 0;
	}
	fclose(fp);
	fp = fopen(ch, "r");
	while (!feof(fp))
	{
		for (j = 0; j < D; j++)
		{
			if (!fscanf(fp, "%lf%c", &x, &junk)) //��ȡ���ݵ�
			{
				break;
			}
		}
		if (j == D)
			N++;
	}
	D = D - 1;
	fclose(fp);
	std::cout << "��������Ϊ��" << N << std::endl;
	int train_num= (N % 10 >= 7) ?  7 : (N % 10);
	int count = N / 10 * 7 + train_num;
	int datanum = N;
	N = count;
	data = InitArray(count, D);
	DATA = InitArray(count, D);
	in_cluster = new double[count];
	cluster_center = InitArray(K, D);
	int FLAG;
	int *lag = new int[datanum - N];//��Ų������ݵ�ʵ�ʱ�ǩ
	double **testdata = InitArray(datanum - N, D);
	fp = fopen(ch, "r");
	for (i = 0; i < datanum; i++)
	{
		for (j = 0; j < D + 1; j++)
		{
			if (i % 10 >= 0 && i % 10 <= 6) 
			{
				if (fscanf(fp, "%lf%c", &x, &junk))  
				{
					if (j < D)
					{
						DATA[i / 10 * 7 + i % 10][j] = x;
						if (x > maxdata[j])
							maxdata[j] = x;
						if (x < mindata[j])
							mindata[j] = x;
					}
				}
			}
			else
			{
				if (fscanf(fp, "%lf%c", &x, &junk)) 
				{
					if (j < D)
					{
						testdata[i / 10 * 3 + i % 10 - 7][j] = x;
					}
					else
					{
						lag[i / 10 * 3 + i % 10 - 7] = x;
					}
				}
			}
		}
	}
	fclose(fp);
	double temp1, temp2;
	int count_ = 0;
	this->ZeroneNormal();
	int randnum;
	srand((unsigned int)(time(NULL)));  
	for (i = 0; i < K; i++)
	{
		randnum= fabs(((int)((double)(N*rand() / (RAND_MAX + 1.0)))));

		for (j = 0; j < D; j++)
		{
			cluster_center[i][j] =data[randnum][j];
		}
	}
	Cluster();
	temp1 = GetDifference();
	count_++;
	GetCenter();
	Cluster(); 
	temp2 = GetDifference();
	count_++;
	while (fabs(temp2 - temp1) != 0) {   
		temp1 = temp2;
		GetCenter();
		Cluster();
		temp2 = GetDifference();
		count_++;
	}
	std::cout << "��" << count_ << "������ƽ�����Ϊ:" << temp2 << "\n";
	for (i = 0; i < K; i++)
	{
		for (j = 0; j < D; j++)
		{
			cluster_center[i][j] = cluster_center[i][j] * (maxdata[j] - mindata[j]) + mindata[j];
			std::cout << cluster_center[i][j] << " ";
		}
		std::cout << std::endl;
	}
	FILE *FP[100];
	char sz[32];
	printf("��ʼ�������ݵľ����ļ����..............");
	for (i = 1; i <= K; i++)
	{
		sprintf(sz, "��%d����.csv", i);
		FP[i] = fopen(sz, "a+");
	}
	for (i = 0; i < datanum - N; i++)
	{
		int min = GetDistance(cluster_center[0], testdata[i]);
		FLAG = 0;
		for (int k = 0; k < K; k++)
		{
			if (min > GetDistance(cluster_center[k], testdata[i]))
			{
				FLAG = k;
				min = GetDistance(cluster_center[k], testdata[i]);
			}
		}
		for (int k = 0; k < K; k++)
		{
			if (k == FLAG)
			{
				if (y == 1)
					c[k][lag[i]]++;
				else
				c[k][lag[i]-1]++;
				int p;
				for (p = 0; p < D; p++)
				{
					fprintf(FP[k + 1], "%lf,", testdata[i][p]);
				}
				fprintf(FP[k + 1], "%d\n", lag[i]);
			}

		}
	}
	for (i = 1; i <= K; i++)
	{
		sprintf(sz, "��%d����.txt", i);
		fclose(FP[i]);
	}
	std::cout << std::endl;
	end = clock();
	double P, R, F=0;
	int row, col;
	count = 0;
	int num;
	for (i = 0; i < K; i++)
	{
		col = 0;
		row = 0;
		num = c[i][i];
		for (j = 0; j < K; j++)
		{
			col += c[j][i];
			row += c[i][j];
			num = (num > c[i][j]) ? num : c[i][j];
		}
		P = (double)num/ (double)col;
		R = (double)num / (double)row;
		if (P != 0 && R != 0)
		{
			F += 2 * P*R / (P + R);
			count++;
		}
	}
	F = F / count;
	std::cout << "׼ȷ��Ϊ:" << F << std::endl;
	std::cout << "����ʱ��Ϊ" << (end - start) << "ms" << std::endl;
}
void  Kmeans::K_means()
{
	int i, j, count = 0;
	double temp1, temp2;
	this->ZeroneNormal();
	int randnum;
	srand((unsigned int)(time(NULL)));  //�����ʼ��k�����ĵ�
	for (i = 0; i < K; i++)
	{
		randnum = fabs(((int)((double)(N*rand() / (RAND_MAX + 1.0)))));
		for (j = 0; j < D; j++)
		{
			this->cluster_center[i][j] = this->data[randnum][j];
		}
	}
	Cluster();
	temp1 = GetDifference();
	count++;
	GetCenter();
	Cluster();  //���µ�k�����ĵ���еڶ��ξ���
	temp2 = GetDifference();
	count++;
	while (fabs(temp2 - temp1) != 0) {   //�Ƚ�ǰ�����ε�����������ȼ�������
		temp1 = temp2;
		GetCenter();
		Cluster();
		temp2 = GetDifference();
		count++;
	}
	std::cout << std::endl;
	std::cout << "��" << count << "������ƽ�����Ϊ:" << temp2 << "\n\n";
	std::cout << "��������Ϊ:" << count << "\n\n";  //ͳ�Ƶ�������
	Output();
	std::cout << "���������\n";
	end = clock();
	std::cout << "����ʱ��Ϊ" << (end - start) <<"ms" << std::endl;
}
#endif