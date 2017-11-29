#include "structureDefine.h"
ofstream output("DecisionTree//carResult.txt");
class DecisionTree {
private :
	ifstream fileData;
	ifstream fileAttributes;
	ifstream fileInput;//��ȡĿ�Ļ��ֵı�ǩ����
	Instance* testList;
	DTree* root;//�õ�����������ͷ
	attribute* spiltAttributes;
	attribute aim;
	double correctRate;
	double timeCost;
	int n;
	Instance* inputData(attribute* spiltAttributes, attribute* aim, Instance* head);
	attribute * inputSpiltAttributes(); //�õ���������
	int chooseSpiltAttributes(attribute* spiltAttribute, attribute aim);
	void printMatrix(attribute* spiltAttributes, attribute aim);
	bool getAttributeMatrix(attribute* spiltAttributes, attribute* aim, Instance* head);//����ǰû�����Կ�ѡ�ˣ�����true
	DTree* createDecisionTree(attribute* spiltAttributes, attribute* aim, Instance* head, double purity);//���ɾ�����
	int purityJudge(Instance * head, attribute aim, double purity);//�����жϣ�����ǰ�ڵ�ĳ��ǩ��Ŀռ�ȵ�һ���̶ȣ��ͽ�����Ϊ�սڵ�ֹͣ����
	char* classJudge(Instance ins, DTree* root, attribute* spiltAttributes);//���ݽ����ľ������жϵ�ǰԪ�������ĸ���ǩ
public :
	DecisionTree(string data, string attribute, string classValue);
	double getCorrectRate();
	attribute* getAttributes();
	double getTimeCost();
};

Instance * DecisionTree::inputData(attribute * spiltAttributes, attribute * aim, Instance * head)
{
	Instance* testData = createNewInstanceList();
	while (!fileData.eof())//��ȡ���ݣ�ÿ��ȡһ�����ݣ�����һ��ʵ��
	{
		Instance * ins = new Instance();
		int random = rand() % 10;//��С�����򽫸�ʵ����������Լ������������򻮷���ѵ����
		char temp[N][N] = { 0 };
		for (int i = 0; i < n; i++)
		{
			if (spiltAttributes[i].partition != 0)
			{
				fileData >> temp[i];
				//cout << temp[i]<< " ";////////////////
				strcpy(ins->spiltAttr[i], temp[i]);//�������ֵ
			}
			else {
				fileData >> (ins->spiltAttrNum[i]);//��Ϊ���������ԣ���ȡ��ֵ������
			}
			strcpy(ins->spiltAttrName[i], spiltAttributes[i].chr);//�����Ӧ��������
		}
		fileData >> temp[n];
		strcpy(ins->categorization, temp[n]);
		//cout << ins->categorization << " ";/////////////
		//cout << endl;
		if (random >= 3) addNewInstance(head, ins);
		else addNewInstance(testData, ins);
	}
	return testData;
}

attribute * DecisionTree::inputSpiltAttributes()
{
	fileAttributes >> n;
	attribute* spiltAttributes = new attribute[n];
	for (int i = 0; i < n; i++)
	{
		fileAttributes >> spiltAttributes[i].chr;
		int partition;
		fileAttributes >> partition;
		spiltAttributes[i].partition = partition;
		for (int j = 0; j < spiltAttributes[i].partition; j++)
			fileAttributes >> spiltAttributes[i].partitionName[j];
	}
	return spiltAttributes;
}

int DecisionTree::chooseSpiltAttributes(attribute * spiltAttribute, attribute aim)
{
	double aimEntropy = Entropy(aim.partition, aim.list[0]);
	double infoExpection[N] = { 0 };
	double gain[N] = { 0 };//��Ϣ����
	double spilt_info[N] = { 0 };//������Ϣ
	double gain_ratio[N] = { 0 };//������
	for (int i = 0; i < n; i++)
	{
		if (isvisited[i]) continue;
		if (spiltAttribute[i].partition != 0)//��ɢ�ͱ���
		{
			infoExpection[i] = info(spiltAttribute[i], aim.partition);
			gain[i] = aimEntropy - infoExpection[i];
			spilt_info[i] = Entropy(aim.partition, spiltAttribute[i].list[0]);
			gain_ratio[i] = gain[i] / spilt_info[i];
		}
		else {//�����ͱ���

		}
	}
	int p = searchMax(gain_ratio, n);
	//printf("Spilt:%s\n", spiltAttribute[p].chr);
	return p;
}

void DecisionTree::printMatrix(attribute * spiltAttributes, attribute aim)
{
	for (int i = 0; i < n; i++)
	{
		std::cout << spiltAttributes[i].chr << "����ֵ����Ϊ:" << endl;
		if (spiltAttributes[i].partition != 0) {
			for (int j = 0; j <= spiltAttributes[i].partition; j++)//����
			{
				for (int k = 0; k <= aim.partition; k++)
				{
					std::cout << spiltAttributes[i].list[j][k] << "\t";
				}
				std::cout << endl;
			}
		}
		else {
			for (int j = 0; j <= 2; j++)//����
			{
				for (int k = 0; k <= aim.partition; k++)
				{
					std::cout << spiltAttributes[i].list[j][k] << "\t";
				}
				std::cout << endl;
			}
		}
	}
}

bool DecisionTree::getAttributeMatrix(attribute * spiltAttributes, attribute * aim, Instance * head)
{
	for (int i = 0; i < n; i++)//��ʼ��ÿһ���������ԵĴ洢��
		if (spiltAttributes[i].partition != 0)
		{
			for (int j = 0; j <= spiltAttributes[i].partition; j++)
				for (int k = 0; k <= N; k++)
					spiltAttributes[i].list[j][k] = 0;
		}
		else {
			for (int j = 0; j <= 2; j++)
				for (int k = 0; k <= N; k++)
					spiltAttributes[i].list[j][k] = 0;
		}
	for (int i = 0; i <= n; i++) aim->list[0][i] = 0;//��ʼ��Ŀ��������ֵ���󣬸þ���ֻ���õ���һ��
	int count = 0;//������¼֮ǰ�ѷ����˵����Ը���
	//��ѵ������������k�Σ�kΪδ���ʵ����������Եĸ���������������������Եķ��ѵ�
	for (int i = 0; i < n; i++) {
		if (isvisited[i]||spiltAttributes[i].partition!=0) continue;
		else {
			int *wholenum = insertSort(head, i,*aim);
			double spiltPoints = head->next->spiltAttrNum[i];//��ָ��Ϊ��һ��
			double max = -100000;
			Instance* p = head->next; int countInstance = 1;
			int* counts = new int[aim->partition];
			for (int j = 0; j < aim->partition; j++) counts[j] = 0;
			while (p != NULL) {
				for(int j=0;j<aim->partition;j++) 
					if (strcmp(aim->partitionName[j], p->categorization) == 0) {
						counts[j]++;
						break;
					}
				int list[3];
				list[0] = wholenum[0];
				list[1] = countInstance;
				list[2] = wholenum[0] - countInstance;
				double entropy = Entropy(2, list);
				//std::cout << entropy << " ";
				//����������Ϣ
				double info = 0;
				int *listtemp = new int[aim->partition + 1];
				listtemp[0] = countInstance;
				for (int k = 1; k <= aim->partition; k++) listtemp[k] = counts[k - 1];
				info += (((double)countInstance) / wholenum[0])*Entropy(aim->partition, listtemp);
				listtemp[0] = wholenum[0] - countInstance;
				for (int k = 1; k <= aim->partition; k++) listtemp[k] = wholenum[k] - counts[k - 1];
				info += (((double)(wholenum[0]-countInstance)) / wholenum[0])*Entropy(aim->partition, listtemp);
				//std::cout << info << " ";
				double info_gain = entropy - info;
				//std::cout << info_gain << " ";
				//std::cout << p->spiltAttrNum[i] << endl;
				if (info_gain > max)
				{
					max = info_gain;
					spiltPoints = p->spiltAttrNum[i];
				}
				p = p->next;
				countInstance++;
			}
			spiltAttributes[i].spiltPoint = spiltPoints;
			//std::cout << spiltPoints << endl;
			delete[] counts;
		}
	}
	/*std::cout << endl;
	for (int i = 0; i < aim->partition; i++) std::cout << spiltAttributes[i].spiltPoint << "-";
	std::cout << spiltAttributes[3].spiltPoint;
	std::cout << endl;*/
	//��ʱÿ�����������Ե�ֵ�Ա��������
	while (head->next != NULL)
	{
		count = 0;
		for (int i = 0; i < n; i++)
		{
			if (isvisited[i]) {
				count++;
				continue;//����������֮ǰ�Ѿ����ѹ��ˣ��Ͳ����ظ�������
			}
			if (spiltAttributes[i].partition != 0)
			{
				for (int j = 0; j < spiltAttributes[i].partition; j++)
				{
					if (strcmp(head->next->spiltAttr[i], spiltAttributes[i].partitionName[j]) == 0)
					{
						spiltAttributes[i].list[j + 1][0]++;
						spiltAttributes[i].list[0][j + 1]++;
						for (int k = 0; k < aim->partition; k++)
							if (strcmp(head->next->categorization, aim->partitionName[k]) == 0)
							{
								spiltAttributes[i].list[j + 1][k + 1]++;
								aim->list[0][k + 1]++;
								break;
							}
						break;
					}
				}
			}
			else {
					if (head->next->spiltAttrNum[i] < spiltAttributes[i].spiltPoint) {
						spiltAttributes[i].list[1][0]++;
						spiltAttributes[i].list[0][1]++;
						for (int k = 0; k < aim->partition; k++)
							if (strcmp(head->next->categorization, aim->partitionName[k]) == 0)
							{
								spiltAttributes[i].list[1][k + 1]++;
								aim->list[0][k + 1]++;
								break;
							}
					}
					else {
						spiltAttributes[i].list[2][0]++;
						spiltAttributes[i].list[0][2]++;
						for (int k = 0; k < aim->partition; k++)
							if (strcmp(head->next->categorization, aim->partitionName[k]) == 0)
							{
								spiltAttributes[i].list[2][k + 1]++;
								aim->list[0][k + 1]++;
								break;
							}
					}
			}
			spiltAttributes[i].list[0][0]++;
			aim->list[0][0]++;
		}
		if (n == count) return true;
		head = head->next;
	}
	for (int i = 0; i <= aim->partition; i++) aim->list[0][i] /= (n - count);
	return false;
}//**

DTree * DecisionTree::createDecisionTree(attribute * spiltAttributes, attribute * aim, Instance * head, double purity)
{
	if (head->next == NULL) return NULL;
	Instance* p = head;
	DTree* node = new DTree();
	int flag = purityJudge(head, *aim, purity);
	if (flag != -1)//˵�����սڵ�
	{
		node->brands = 0;
		strcpy(node->endName, aim->partitionName[flag]);
	}
	else {//���򣬵ݹ����
		bool isAllVisited = getAttributeMatrix(spiltAttributes, aim, head);
		if (isAllVisited == true) {
			node->brands = 0;
			int* classNum = new int[aim->partition];
			for (int i = 0; i < aim->partition; i++) classNum[i] = 0;
			Instance* p = head;
			while (p->next != NULL)
			{
				for (int i = 0; i < aim->partition; i++) {
					if (strcmp(aim->partitionName[i], p->next->categorization) == 0) {
						classNum[i]++;
						break;
					}
				}
				p = p->next;
			}
			int k = 0;
			for (int i = 0; i < aim->partition; i++)
			{
				if (classNum[k] < classNum[i]) k = i;
			}
			strcpy(node->endName, aim->partitionName[k]);//�����������Զ����궼��û�е��﴿��Ҫ�󣬾Ͱѵ�ǰ��������ս�㣬��ǩΪ��ǰ��ѵ���������ı�ǩ
			return node;
		}
		int maxGain_info = chooseSpiltAttributes(spiltAttributes, *aim);
		Instance* headList = NULL;
		if(spiltAttributes[maxGain_info].partition!=0)//����ɢ�����ԣ��Ӽ�����Ϊ���ָ���
			headList = createNewInstanceLists(spiltAttributes[maxGain_info].partition);
		else headList = createNewInstanceLists(2);//���������ԣ��Ӽ�����Ϊ2
		strcpy(node->decisionName, spiltAttributes[maxGain_info].chr);
		node->decisionSubscript = maxGain_info;

		if (spiltAttributes[maxGain_info].partition != 0)
		node->brands = spiltAttributes[maxGain_info].partition;
		else node->brands = 2;

		isvisited[maxGain_info] = true;
		p = head;
		while (p->next != NULL)
		{
			Instance* ins = new Instance();
			strcpy(ins->categorization, p->next->categorization);
			for (int i = 0; i < n; i++)
			{
				strcpy(ins->spiltAttrName[i], p->next->spiltAttrName[i]);
				strcpy(ins->spiltAttr[i], p->next->spiltAttr[i]);
				ins->spiltAttrNum[i] = p->next->spiltAttrNum[i];
			}
			if (spiltAttributes[maxGain_info].partition != 0) {//��ɢ��
				for (int i = 0; i < spiltAttributes[maxGain_info].partition; i++)
				{
					if (strcmp(ins->spiltAttr[maxGain_info], spiltAttributes[maxGain_info].partitionName[i]) == 0)
					{
						addNewInstance(&headList[i], ins);
						break;
					}
				}
			}
			else {//������
				if (ins->spiltAttrNum[maxGain_info] < spiltAttributes[maxGain_info].spiltPoint) {
					addNewInstance(&headList[0], ins);
				}
				else addNewInstance(&headList[1], ins);
			}
			p = p->next;
		}
		freeList(head);
		for (int i = 0; i < node->brands; i++)
			node->child[i] = createDecisionTree(spiltAttributes, aim, &headList[i], purity);
		isvisited[maxGain_info] = false;
	}
	return node;
}

int DecisionTree::purityJudge(Instance * head, attribute aim, double purity)
{
	int* department = new int[aim.partition];
	for (int i = 0; i < aim.partition; i++) department[i] = 0;
	while (head->next != NULL)
	{
		for (int i = 0; i < aim.partition; i++)
			if (strcmp(head->next->categorization, aim.partitionName[i]) == 0) { department[i]++; break; }
		head = head->next;
	}
	int sum = 0;
	for (int i = 0; i < aim.partition; i++) sum += department[i];
	for (int i = 0; i < aim.partition; i++)
		if (((double)department[i] / sum)>purity) return i;
	delete[] department;
	return -1;
}

char * DecisionTree::classJudge(Instance ins, DTree * root, attribute * spiltAttributes)
{
	DTree*  p = root;
	if (p == NULL) return NULL;
	if (root->brands == 0) return root->endName;
	do {
		int flag = 0;
		if (spiltAttributes[p->decisionSubscript].partition != 0) {
			for (int i = 0; i < spiltAttributes[p->decisionSubscript].partition; i++)
				if (strcmp(ins.spiltAttr[p->decisionSubscript], spiltAttributes[p->decisionSubscript].partitionName[i]) == 0)
				{
					flag = 1;
					p = p->child[i];
					break;
				}
		}
		else {
			flag = 1;
			if (ins.spiltAttrNum[p->decisionSubscript] < spiltAttributes[p->decisionSubscript].spiltPoint) p = p->child[0];
			else p = p->child[1];
		}
		if (p == NULL) return NULL;
		else if (flag == 0) {//����ǰ��ֵ����ѽڵ���������һֵ����ƥ��Ļ�������Ԫ�ظ������Ľڵ㴦
			return NULL;//�����Ȳ�����
		}
	} while (p->brands != 0);
	return p->endName;
}

DecisionTree::DecisionTree(string data, string attribute, string classValue) {
	srand((int)time(0));
	fileData.open(data,ios::in);
	if (!fileData) {
		std::cout << "��ʧ�ܣ�" << endl;
		return;
	}
	fileAttributes.open(attribute, ios::in);
	if (!fileAttributes) {
		std::cout << "��ʧ�ܣ�" << endl;
		return;
	}
	fileInput.open(classValue, ios::in);
	if (!fileInput) {
		std::cout << "��ʧ�ܣ�" << endl;
		return;
	}
	spiltAttributes = inputSpiltAttributes();	
	//��ȡ��������
	/*std::cout << "��������������ʾ��ÿһ��Ϊһ�ַ������ԣ���һ���ֶ�Ϊ���������ڶ���Ϊ�����Ի��ָ�������������Ϊ�����ֵ����ƣ�" << endl;
	for (int i = 0; i < n; i++)
	{
		std::cout << spiltAttributes[i].chr << " " << spiltAttributes[i].partition << " ";
		for (int j = 0; j < spiltAttributes[i].partition; j++)
		{
			std::cout << spiltAttributes[i].partitionName[j] << " ";
		}
		std::cout << endl;
	}*/
	//
	//��ȡ��ǩ����
	char c[N];
	fileInput >> aim.partition;
	for (int i = 0; !fileInput.eof(); i++)
	{
		fileInput >> c;
		strcpy(aim.partitionName[i], c);
	}
	//
	LARGE_INTEGER litmp;
	long long QPstart, QPend;
	double dfMinus, dfFreq;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;//���CPU����Ƶ��
	QueryPerformanceCounter(&litmp);//����
	QPstart = litmp.QuadPart;
	//����������
	Instance* head = createNewInstanceList();
	Instance* testList = inputData(spiltAttributes, &aim, head);
	DTree* root = createDecisionTree(spiltAttributes, &aim, head, 0.9);
	//���˽����˾�����
	//
	//���㵱ǰ�������ڲ��Լ��µ���ȷ��
	int denaminator = 0;//��¼�ܲ���Ԫ�ظ���
	int molecule = 0;//��¼��ȷ����Ԫ�ظ���
	correctRate = 0;//�ڵ�ǰ���Լ���ѵ�����µ���ȷ��
	Instance* p = testList;
	while (p->next != NULL)
	{
		char* judge = classJudge(*(p->next), root, spiltAttributes);
		if (judge != NULL&&strcmp(judge, p->next->categorization) == 0) molecule++;
		denaminator++;
		p = p->next;
	}
	correctRate = (double)molecule / denaminator;//�õ���ȷ��
	output << "��ȡ������ȷ��Ϣ��������" << molecule << endl;
	output << "��ȡ��������Ϣ��������" << denaminator << endl;
	QueryPerformanceCounter(&litmp);//����
	QPend = litmp.QuadPart;
	dfMinus = (double)(QPend - QPstart);
	timeCost = dfMinus / dfFreq;//��������/����Ƶ��
	fileAttributes.close();
	fileData.close();
	fileInput.close();
}

double DecisionTree::getCorrectRate(void)
{
	return correctRate;
}

attribute * DecisionTree::getAttributes(void)
{
	return spiltAttributes;
}

double DecisionTree::getTimeCost(void)
{
	return timeCost;
}

int main()
{
	srand((int)time(0));
	int count = 10;
	double times[10] = { 0 };
	for (int i = 0; i < count; i++) {
		output << "��" << i + 1 << "�Σ�" << endl;
		DecisionTree tree("DecisionTree//car//car_data.txt", "DecisionTree//car//car_attributes.txt", "DecisionTree//car//class.txt");
		double correctRate = tree.getCorrectRate();
		output << "��ǰ����������ȷ��Ϊ(���Լ���ѵ����Ԫ�ظ�������Ϊ3:7):" << correctRate << endl;
		output << "�ٻ���Ϊ1������֤��������Ԫ�ض������˲��ԣ�" << endl;
		output << "�ʵ�F1-measureֵΪ:" << (2 * correctRate / (correctRate + 1)) << endl;
		times[i] = tree.getTimeCost();
		output <<"��"<<i+1<< "�ι�������������Լ����Ե���ʱ�仨��Ϊ:" << times[i] << "��" << endl<<endl;
	}
	double average = 0;
	for (int i = 0; i < 10; i++) {
		average += times[i];
	}
	average /= 10;
	output << "ʮ�������ƽ��ʱ��Ϊ:" << average << "��" << endl;
	output.close();
	return 0;
}