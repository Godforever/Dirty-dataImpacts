#include"kmeans.h"
int  main()
{
	int flag;
	std::cout << "1.���������ݼ����б�ǩ���ұ�ǩ��0��ʼ\n2.���������ݼ����б�ǩ���ұ�ǩ��1��ʼ\n3.���������ݼ������б�ǩ\n";
	std::cin >> flag;
	if (flag == 3)
	{
		Kmeans x = Kmeans();
		x.K_means();
	}
	else 
	Kmeans x = Kmeans(flag);
	system("pause");
}