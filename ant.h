#pragma once
#include<iostream>
#include<cstdio>
#include<vector>
#include"set.h"
#include<time.h>


//���͹޳���صĺ궨��
#define COMP_COUNT	 Car[0]												//������
#define COMP_Q		Car[1]												//����������L��
#define HMIN  Car[2]													//����������
#define HMAX  Car[3]													//����������


using namespace std;

const double DB_MAX = 10e9;


class ant {
public:
	ant(void) {};
	~ant(void) {};
public:
	//vector<int> m_npath;//����һ������������
	//��Ҫ��ʼ��
	int ant_NO;									//���ϱ��
	float cost;			//���ϵ�ǰ����
	int Qall;//��������
	float tran_cost;							//����ɱ�
	float unload_cost;							//ж�ͳɱ�
	float delay_cost;							//����ɱ�

	float next_delay_cost;						//��һ�ڳ�ȱ���ɱ�
	//int tankallsati;						    //�����͹�����������ʱ=1
	int car_useable[CAR_COUNT];					//��¼���Ƿ����
	int cur_tank_NO;							//��ǰ�͹ޱ��
	//int cur_tank_r;						    //��ǰ�͹����ʹ������
	int cur_car_NO;
	int cur_car_r;							    //��ǰ�������������
	double cur_time;							//���ߵĵ�ǰʱ��
	float capacity;								//��ǰ���Ŀ��ø���
	tank c_info[N_NODE_COUNT];
	tour tour_info[CAR_COUNT];
	
	int tour_sub[CAR_COUNT][33][3];				//[node][q][r]��iֻ���ϵĵ�k̨��tourk�ĵ�n���ڵ�����,�����ϴ�0��ʼ��tour����1��ʼ���ڵ�����1��ʼ��
	float zzl;
	float delay_time;
	float next_delay_time;
	int order_all;
	int lap_all;
	float iter_cost[500];

	//int sati_tank[N_NODE_COUNT];				//�����tank��Ӧλ��Ϊ1
	//int sati_num;								//��¼������͹޸���
	

	//vector<once_move> m_npath1;					//��¼·��

	//once_move last_move;								//��¼��һ���ƶ�����Ϣ
	//once_move next_move;								//��¼��һ���ƶ�����Ϣ
	//int ant_num_;

public:
	void init(int i);
	//once_move cal_cost(int cityNO);				//���㵽��һ���еĻ��ѣ�����Ϊ���б��
	int choose_next_city();						//����ת�ƹ���ѡ����һ����
	int move();								//���һ���ƶ�
	void cal_plan_cost();
	bool cal_next_move(int cityNO);				//������һ���ƶ�����Ϣ

	void return_last_node();					//������һ�ڵ㣬���ҽ��з��زֿ����վ����
	bool change_stasus(int next_city,int q);						//���ƶ������н���״̬�ı仯���ڴ˻��жϵ���������Լ���Ƿ�����

	//�������ٶ�v������һ��������Ҫ�����������Լ�
	bool CalculateCostOFV(int city_num, float v);
	void init_befor_calcul();
	void make_gc_reasonable(int tour);
	int insert(int ins, int tour_ins, int out, int tour_out);
	int scheduling_con();
	int fin_test();
	float Exchange_order1();         //�ֲ��Ż�����1_���ѡ��ͬһ��·�����ͻ����н���
	float Exchange_order2();         //�ֲ��Ż�����2_���ѡ��ͬһtour�����ͻ�����
	float Exchange_order3();         //�ֲ��Ż�����3_���ѡ��ͬtour�����ͻ����н���
	float Short1();					 //�ֲ��Ż�����4_���ѡ��һ��tour��ȱ���ɱ����Ķ���������tour������λ�ã�����λ������ɱ���С�������
	float Short2();					 //�ֲ��Ż�����5_���ѡ��һ��tour��ȱ���ɱ����Ķ����������ѡ��һ��tour�������ж�������������λ������ɱ���С�Ľ�������
	float Station();				 //�ֲ��Ż�����6_�ҵ�����ʱ�������С������ͬ����վ�Ķ����ԣ����ʱ����С�������У����β����Ż�
	float Unneed_forshort();         //�ֲ��Ż�����7_���ѡ��һ��tour�����ѡ��һ�����еķǱ�Ҫ���Ͷ��������˶�����������ת�Ƶ��������ڳ�ȱ���ɱ����͹���
	float Unneed_exchange();         //�ֲ��Ż�����8_���ѡ������tour�������еķǱ�Ҫ���Ͷ����ҳ����������������ҵ��ɱ���С������ʽ
	float Unneed_forH();			 //�ֲ��Ż�����9_���ѡ��һ��/����Ǳ�Ҫ���Ͷ�����ȡ���󣬰���װ�����ɵ͵��߷������������
};


												//�û���Ϣ��
extern short int phe_table[N_NODE_COUNT][N_NODE_COUNT][R_MAX][R_MAX][Q_CANBE_COUNT][Q_CANBE_COUNT];										//��Ϣ�ر�
extern tour tour_info_ini[CAR_COUNT];
//extern int num_of_station;
//extern int num_of_tank;
extern float Car[4];
extern double dis[N_NODE_COUNT][N_NODE_COUNT];
extern int q_canbe[27];


