/*
�����ù淶��
���ຯ��������Jtree_process��
����ֵΪ�����ʣ�
���ڹ����и�error_sub��ֵ
*/
#include "data.h"
#include<set>

class adaBoost_function {
private:
	int data_count;//���ݸ���
	int classifier_number;//���շ���������
	float alpha[max_container];//����ÿ����������Ҫ��
	int *error_sub;
public:
	adaBoost_function(int data_count1) {
		this->data_count = data_count1;
	}
	//���캯��
	float* getalpha() {
		return alpha;
	}
	//���ط�������Ҫ��
	void adaBoost_process(Node *classifier[max_container]);
	//adaBoost�㷨
	void sampling(float *W, int *sample_subscript);
	//����
	int search(float *s, float x);
	//���ң����ڳ���
	void resetWeight(float *w, float alpha1, int *error_subscript);
	//��������Ȩֵ
	void initialWeight(float *weight_array, int array_length);
	//��ʼ��Ȩֵ
	float adaBoost_function::jointClassifier(Node * classifier[max_container],int opt);
};

#define AttributeKinds 20//���Ը�������
//#define N 20//������������
#define e 2.71828
#define infinite 10000
float W[upper_limit];
int sample_subscript[upper_limit];
float s[upper_limit];

void adaBoost_function::initialWeight(float * weight_array, int array_length) {
	if (array_length == 0)
		return;
	for (int i = 0; i < array_length; i++)
		weight_array[i] = 1 / (float)array_length;
}

void adaBoost_function::adaBoost_process(Node *classifier[max_container]) {
	float error_rate;
	classifier_number = 0;
	initialWeight(W, data_count);
	int height, k, count = 0, flag = 0, error_subscript[max_container + 1];
	sample_subscript[0] = data_count;
	cout << "�����������������" << endl;
	cin >> k;
	sample_subscript[0] = sample.size();
	do {
		height = 0;//��ʼ������
		error_subscript[0] = 0;//��ʼ����������
		if (flag)
			initialWeight(W, data_count);
		flag = 0;
		int attribute_array[AttributeKinds];
		sampling(W, sample_subscript);
		//ѵ������
		classifier[count] = new Node;
		error_subscript[0] = 0;
		error_rate = Jtree_process(sample_subscript, this->error_sub,classifier[count]);
		//error_rate = calculateErrorRate(classifier[count], error_subscript);
		if (error_rate >= 0.5) {
			flag = 1;
			continue;
		}
		else if (error_rate == 0||error_sub[0] == 0) {
			if (classifier_number == 0) {
				alpha[count] = infinite;
				classifier_number++;
			}
			break;
		}
		else {
			alpha[count] = 0.5*log((1 - error_rate) / error_rate) / log(e);
			resetWeight(W, alpha[count], error_sub);
			count++;
		}
		classifier_number++;
	} while (count < k);
}

void adaBoost_function::sampling(float *W, int *sample_subscript) {
	float random;
	for (int i = 0; i < data_count; i++) {
		if (i == 0)
			s[i] = 0;
		else
			s[i] = s[i - 1] + W[i];
	}
	s[data_count] = 1;
	int count = 0;
	int subscript;
	do {
		random = rand() % 999 / (float)1000;
		if (random == 0)
			continue;
		count++;
		subscript = search(s, random);
		sample_subscript[count] = subscript;
	} while (count < data_count);
}

int adaBoost_function::search(float *s, float x) {
	int h = data_count + 1, l = 0, m = (h + l) / 2;
	while (!(x > s[m - 1] && x <= s[m])) {
		if (x > s[m])
			l = m;
		else if (x < s[m])
			h = m;
		else
			break;
		m = (h + l) / 2;
	}
	return m - 1;
}

void adaBoost_function::resetWeight(float * w, float alpha1, int *error_sub) {
	int test1 = error_sub[0];
	int test2 = error_sub[1];
	int subscript, count = 0;
	float total = 0;
	set<int> error;
	for (int i = 1; i <= error_sub[0]; i++) {
		subscript = error_sub[i];
		w[subscript] = w[subscript] * pow(e, alpha1);
		total += w[subscript];
		error.insert(subscript);
	}
	for (int j = 0; j < data_count; j++) {
		if (error.find(j) != error.end())
			continue;
		w[j] = w[j] = w[j] * pow(e, -alpha1);
		total += w[j];
	}
	for (int z = 0; z < data_count; z++)
		w[z] = w[z] / total;
}

float adaBoost_function::jointClassifier(Node *classifier[max_container],int opt)
{
	int x, y, max, classifier_code, subscript;
	string class_label;
	float error = 0, total = (opt == 1)?test_data.size():all_data.size(), count[Class_upper_limit];
	for (int k = 0; k < total; k++) {
		for (int j = 0; j < class_count; j++)
			count[j] = 0;
		for (int i = 0; i < classifier_number; i++) {
			classifier_code = judge(classifier[i], k, opt);
			count[classifier_code] += alpha[i];
		}
		max = count[0];
		subscript = 0;
		for (int n = 0; n < class_count; n++) {
			if (max < count[n]) {
				max = count[n];
				subscript = n;
			}
		}
		class_label = (opt == 1)?test_data[k][attr_count]:all_data[k][attr_count];
		if (class_label != ClassName[subscript])
			error++;
		//��ʼ���ж�����
	}
	return 1- error / total;
}
