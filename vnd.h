#pragma once
#include<iostream>
#include"set.h"
#include"ant.h"
#include<time.h>

#define MAX_ITE	  500			//����������500��ȫ�֣�500
#define MAX_ITE_NOIPV 20			//����޸Ľ�������ȫ�֣�20
#define MAX_LOC_ITE	   60 		    //�������������ֲ���40
#define VNS_MAXITER 10             //VND�������� 10
#define PMAX	100					//������Ϣ�����ֵ
#define PMIN	10					//������Ϣ����Сֵ
#define ITE_PHE 50000				//һ�ε����н�����Ϣ�ظ��µ�����50000 is better


using namespace std;


class vnd
{
public:
	vnd() {};
	~vnd() {};
public:
	//��������
	ant antarray[ANT_COUNT];
	//ȫ����������
	ant Global_Best_Ant;
	//���ε�����������
	ant Ite_Best_Ant;
	//��ʱ����
	ant Temp_Ant;

	float ITER_COST[MAX_ITE];
	float tour_out[CAR_COUNT][33][7];
	float gante[CAR_COUNT][5][25][3];

	//���б����������ϵ�·��Ѱ��
	void search_best_ant();

	int iter_all=0;
	int iter = 0;
	int best_ite;
	clock_t best_time;//���Ž�ʱ��

	//������Ϣ�صķֲ����,������һ�ε���֮��
	void update_phe();
	void cal_someone();
	float keep2(float a);

	//����Ӧ���
	ant VND_search();
	int better_EX1_num = 0; //�ֲ�1����ȫ�����Ž����
	int better_EX2_num = 0; //�ֲ�2����ȫ�����Ž����
	int better_EX3_num = 0; //�ֲ�3����ȫ�����Ž����	
	int better_S1_num = 0;  //�ֲ�4����ȫ�����Ž����
	int better_S2_num = 0;  //�ֲ�5����ȫ�����Ž����
	int better_Station_num = 0;  //�ֲ�6����ȫ�����Ž����
	int better_UN1_num = 0; //�ֲ�1����ȫ�����Ž����
	int better_UN2_num = 0; //�ֲ�2����ȫ�����Ž����
	int better_UN3_num = 0; //�ֲ�3����ȫ�����Ž����	

	clock_t EX1_time; //�ֲ�1����ȫ�����Ž����
	clock_t EX2_time; //�ֲ�2����ȫ�����Ž����
	clock_t EX3_time; //�ֲ�3����ȫ�����Ž����	
	clock_t S1_time;  //�ֲ�4����ȫ�����Ž����
	clock_t S2_time;  //�ֲ�5����ȫ�����Ž����
	clock_t Station_time;  //�ֲ�6����ȫ�����Ž����
	clock_t UN1_time; //�ֲ�7����ȫ�����Ž����
	clock_t UN2_time; //�ֲ�8����ȫ�����Ž����
	clock_t UN3_time; //�ֲ�9����ȫ�����Ž����	

	

	//int optimize_num_ = 0;//ÿֻ���ϲ��þֲ��Ż����Գɹ�����
	/*
	//�������ܷ���ָ��

	int EX1_num = 0;//�ֲ�1ʹ�ô���
	int EX2_num = 0;//�ֲ�2ʹ�ô���
	int EX3_num = 0;//�ֲ�3ʹ�ô���
	int TW_num = 0; //�ֲ�4ʹ�ô���
	int WV_num = 0; //�ֲ�5ʹ�ô���
	int WR_num = 0; //�ֲ�6ʹ�ô���

	int best_EX1_num = 0; //�ֲ�1����ȫ�����Ž����
	int best_EX2_num = 0; //�ֲ�2����ȫ�����Ž����
	int best_EX3_num = 0; //�ֲ�3����ȫ�����Ž����	
	int best_TW_num = 0;  //�ֲ�4����ȫ�����Ž����
	int best_WV_num = 0;  //�ֲ�5����ȫ�����Ž����
	int best_WR_num = 0;  //�ֲ�6����ȫ�����Ž����
	int best_num = 0;

	clock_t EX1_time; //�ֲ�1����ȫ�����Ž����
	clock_t EX2_time; //�ֲ�2����ȫ�����Ž����
	clock_t EX3_time; //�ֲ�3����ȫ�����Ž����	
	clock_t TW_time;  //�ֲ�4����ȫ�����Ž����
	clock_t WV_time;  //�ֲ�5����ȫ�����Ž����
	clock_t WR_time;  //�ֲ�6����ȫ�����Ž����

	



	//������Ϣ�صķֲ����,������һ�ε���֮��
	void update_phe();

	//����Ӧ���
	ant Adapt_search();

	//���ݹ̶��ڵ�Ľ������оֲ��Ż�
	ant ExchangeNode1();         //�ֲ��Ż�����1_���ѡ��ͬһ��·�����ͻ����н���
	ant ExchangeNode2();         //�ֲ��Ż�����2_���ѡ��ͬһ��·�����ͻ������ͻ������пͻ�˳����(�������ͻ���
	ant ExchangeNode3();         //�ֲ��Ż�����3_���ѡ��ͬ��·�����ͻ����н���

	//����ʱ�䴰���оֲ��Ż�
	ant TimeWindow();            //�ֲ��Ż�����4_ѡ��ʱ�䴰Υ�������Ŀͻ����뵽�ɱ���͵�λ��
	ant TimeEarly();             //�ֲ��Ż�����5_ѡ��ʱ�䴰�絽�����Ŀͻ��������ÿͻ�֮ǰ��ĳ���ͻ���ǰһ�λ����ٶ��Լ�С��ǰ�ͻ����絽��
	ant TimeDelay();             //�ֲ��Ż�����6_ѡ��ʱ�䴰���������Ŀͻ��������ÿͻ�֮ǰ��ĳ���ͻ���ǰһ�λ����ٶ��Լ�С��ǰ�ͻ���������

	//�����ٶȽ��оֲ��Ż�
	ant WorstV();                //�ֲ��Ż�����7_ѡ���������ٶȼ�϶����һ�λ����ڶ����ڵ�Ϊ�ͻ��������ÿͻ����뵽�ɱ���͵�λ��

	//���ݳ��վ���оֲ��Ż�
	ant WorstRecharge();         //�ֲ��Ż�����8_ѡ����ɱ����ĳ��վ�Ƴ����ڸ�λ�ò���ʹ�ɱ���С�ĳ��վ���������У�������ͬ��ʽ������֮ǰ�Ļ�
*/
};
