#pragma once
#include<vector>
#include<iostream>
#define ANT_COUNT 50														//蚂蚁个数，50better
#define N_NODE_COUNT 83                                                    //所有节点数量（油库+油罐），实际案例176
#define N_STATION_COUNT 50                                                 //所有加油站数量，实际案例72
#define CAR_COUNT 16                                                     //油罐车数量
#define SERVICE_RATE 60000.0                                               //卸油速率 L/h
#define INIT_PHE 20.0													   //初始信息素
#define CAR_V	50.0													   //油罐车行驶速度
#define B_START 0                                                          //调度期开始
#define B_END 16                                                           //调度期结束，实例取24
#define LOAD_T 0.5                                                         //装油固定时间
#define R_MAX 6															   //油罐配送趟数最大值
#define Q_CANBE_COUNT 27	
#define pi 3.1415926535897932384626433832795
#define EARTH_RADIUS 6378.137 //地球半径 KM
#define Max_tour_per_car 4												// 车辆的趟数上限

//一共15个可能的q
//和花费相关的宏定义
#define drive_c		15.0														//行驶成本，单位为元/km
#define unload_c    1.8													//卸油成本，单位为元/L														//92号惩罚成本,单位为元/L
#define short_c		0.9													//惩罚成本,单位为元/L


typedef struct Node
{
	int id;
	char t;									//节点种类，d是油厂，t是油罐
	char oil;								//油罐油品类型a/b/c
	int station;						    //所属加油站编号
	double x;
	double y;
	double ini_stock;                      //升
	//double fin_stock;                   //升
	double tank_cap;                   //升
	double sell_v;                     //升/小时
	double recent_left_time;			//最近一次配送的离开时间
	float S_time;						//缺货点
	int send_all;						//一共被送了多少油
	int r_now;                         //一共被送了多少次
	float dtime_all;					//缺货总时长
	float dtime_cost;
	float next_dtime;					//下期末初缺货总时长
	float next_d_time_cost;
	//int sati;							//满足期末库存。满足1.不满足0
	int rmax;							//被配送次数上限=（tank_cap-ini_stock+B_END*sell_v）/0.8*Car[2],floor()
	float order[10][8];					//订单相关信息
	//order[0]:[r][qir][air][a_time][Uir][Sir][d_time][d_cost];
}tank;

typedef struct Tour
{
	int car_id;
	int cur_node_id;
	int cur_q;                               //在cur_node_id的配送量
	float cur_time;                          //离开cur_node_id的时间
	int node_num;
	int lap;
	int node[33];							//tour上节点i编号
	int gc_need[33];						//保存每次访问所用隔舱
	int r[33];							    //第几次访问node
	int q[33];							    //Oir配送量
	float a_time[33];
	float U_time[33];
	float S_time[33];
	float d_time[33];
	float d_cost[33];
	float ser_time[33];                         //开始服务时刻
	int useable;                            //车可用状态
	int gc_left;							//剩余可用隔舱
	

}tour;


													//用户信息表
extern short int phe_table[N_NODE_COUNT][N_NODE_COUNT][R_MAX][R_MAX][Q_CANBE_COUNT][Q_CANBE_COUNT];										//信息素表
extern tank c_info_ini[N_NODE_COUNT];
extern tour tour_info_ini[CAR_COUNT];
//extern int num_of_station;
//extern int num_of_tank;
extern float Car[4];
extern int q_canbe[27];
//初始化各个节点的信息
extern int get_info();
extern double RealDistance(double lat1, double lng1, double lat2, double lng2);
extern double rad(double d);











 