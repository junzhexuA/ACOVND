#pragma once
#include<vector>
#include<iostream>
#define ANT_COUNT 50														//���ϸ�����50better
#define N_NODE_COUNT 83                                                    //���нڵ��������Ϳ�+�͹ޣ���ʵ�ʰ���176
#define N_STATION_COUNT 50                                                 //���м���վ������ʵ�ʰ���72
#define CAR_COUNT 16                                                     //�͹޳�����
#define SERVICE_RATE 60000.0                                               //ж������ L/h
#define INIT_PHE 20.0													   //��ʼ��Ϣ��
#define CAR_V	50.0													   //�͹޳���ʻ�ٶ�
#define B_START 0                                                          //�����ڿ�ʼ
#define B_END 16                                                           //�����ڽ�����ʵ��ȡ24
#define LOAD_T 0.5                                                         //װ�͹̶�ʱ��
#define R_MAX 6															   //�͹������������ֵ
#define Q_CANBE_COUNT 27	
#define pi 3.1415926535897932384626433832795
#define EARTH_RADIUS 6378.137 //����뾶 KM
#define Max_tour_per_car 4												// ��������������

//һ��15�����ܵ�q
//�ͻ�����صĺ궨��
#define drive_c		15.0														//��ʻ�ɱ�����λΪԪ/km
#define unload_c    1.8													//ж�ͳɱ�����λΪԪ/L														//92�ųͷ��ɱ�,��λΪԪ/L
#define short_c		0.9													//�ͷ��ɱ�,��λΪԪ/L


typedef struct Node
{
	int id;
	char t;									//�ڵ����࣬d���ͳ���t���͹�
	char oil;								//�͹���Ʒ����a/b/c
	int station;						    //��������վ���
	double x;
	double y;
	double ini_stock;                      //��
	//double fin_stock;                   //��
	double tank_cap;                   //��
	double sell_v;                     //��/Сʱ
	double recent_left_time;			//���һ�����͵��뿪ʱ��
	float S_time;						//ȱ����
	int send_all;						//һ�������˶�����
	int r_now;                         //һ�������˶��ٴ�
	float dtime_all;					//ȱ����ʱ��
	float dtime_cost;
	float next_dtime;					//����ĩ��ȱ����ʱ��
	float next_d_time_cost;
	//int sati;							//������ĩ��档����1.������0
	int rmax;							//�����ʹ�������=��tank_cap-ini_stock+B_END*sell_v��/0.8*Car[2],floor()
	float order[10][8];					//���������Ϣ
	//order[0]:[r][qir][air][a_time][Uir][Sir][d_time][d_cost];
}tank;

typedef struct Tour
{
	int car_id;
	int cur_node_id;
	int cur_q;                               //��cur_node_id��������
	float cur_time;                          //�뿪cur_node_id��ʱ��
	int node_num;
	int lap;
	int node[33];							//tour�Ͻڵ�i���
	int gc_need[33];						//����ÿ�η������ø���
	int r[33];							    //�ڼ��η���node
	int q[33];							    //Oir������
	float a_time[33];
	float U_time[33];
	float S_time[33];
	float d_time[33];
	float d_cost[33];
	float ser_time[33];                         //��ʼ����ʱ��
	int useable;                            //������״̬
	int gc_left;							//ʣ����ø���
	

}tour;


													//�û���Ϣ��
extern short int phe_table[N_NODE_COUNT][N_NODE_COUNT][R_MAX][R_MAX][Q_CANBE_COUNT][Q_CANBE_COUNT];										//��Ϣ�ر�
extern tank c_info_ini[N_NODE_COUNT];
extern tour tour_info_ini[CAR_COUNT];
//extern int num_of_station;
//extern int num_of_tank;
extern float Car[4];
extern int q_canbe[27];
//��ʼ�������ڵ����Ϣ
extern int get_info();
extern double RealDistance(double lat1, double lng1, double lat2, double lng2);
extern double rad(double d);











 