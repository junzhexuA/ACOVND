#pragma once
#include<iostream>
#include<cstdio>
#include<vector>
#include"set.h"
#include<time.h>


//和油罐车相关的宏定义
#define COMP_COUNT	 Car[0]												//隔舱数
#define COMP_Q		Car[1]												//隔舱容量（L）
#define HMIN  Car[2]													//总配送下限
#define HMAX  Car[3]													//总配送上限


using namespace std;

const double DB_MAX = 10e9;


class ant {
public:
	ant(void) {};
	~ant(void) {};
public:
	//vector<int> m_npath;//创建一个向量储存器
	//需要初始化
	int ant_NO;									//蚂蚁编号
	float cost;			//蚂蚁当前花费
	int Qall;//总配送量
	float tran_cost;							//运输成本
	float unload_cost;							//卸油成本
	float delay_cost;							//延误成本

	float next_delay_cost;						//下一期初缺货成本
	//int tankallsati;						    //所有油罐配送量满足时=1
	int car_useable[CAR_COUNT];					//记录车是否可行
	int cur_tank_NO;							//当前油罐编号
	//int cur_tank_r;						    //当前油罐配送次数编号
	int cur_car_NO;
	int cur_car_r;							    //当前车配送趟数编号
	double cur_time;							//车走的当前时间
	float capacity;								//当前车的可用隔舱
	tank c_info[N_NODE_COUNT];
	tour tour_info[CAR_COUNT];
	
	int tour_sub[CAR_COUNT][33][3];				//[node][q][r]第i只蚂蚁的第k台车tourk的第n个节点的序号,（蚂蚁从0开始，tour数从1开始，节点数从1开始）
	float zzl;
	float delay_time;
	float next_delay_time;
	int order_all;
	int lap_all;
	float iter_cost[500];

	//int sati_tank[N_NODE_COUNT];				//满足的tank对应位置为1
	//int sati_num;								//记录满足的油罐个数
	

	//vector<once_move> m_npath1;					//记录路径

	//once_move last_move;								//记录上一次移动的信息
	//once_move next_move;								//记录这一次移动的信息
	//int ant_num_;

public:
	void init(int i);
	//once_move cal_cost(int cityNO);				//计算到下一城市的花费，参数为城市编号
	int choose_next_city();						//根据转移规则选择下一城市
	int move();								//完成一次移动
	void cal_plan_cost();
	bool cal_next_move(int cityNO);				//计算下一次移动的信息

	void return_last_node();					//返回上一节点，并且进行返回仓库或充电站操作
	bool change_stasus(int next_city,int q);						//在移动过程中进行状态的变化，在此会判断电量和容量约束是否满足

	//计算以速度v到达下一城市所需要的能量花费以及
	bool CalculateCostOFV(int city_num, float v);
	void init_befor_calcul();
	void make_gc_reasonable(int tour);
	int insert(int ins, int tour_ins, int out, int tour_out);
	int scheduling_con();
	int fin_test();
	float Exchange_order1();         //局部优化策略1_随机选择同一回路两个客户进行交换
	float Exchange_order2();         //局部优化策略2_随机选择同一tour两个客户交换
	float Exchange_order3();         //局部优化策略3_随机选择不同tour两个客户进行交换
	float Short1();					 //局部优化策略4_随机选择一个tour上缺货成本最大的订单，插入tour内其他位置，遍历位置求出成本最小解决方案
	float Short2();					 //局部优化策略5_随机选择一个tour上缺货成本最大的订单，再随机选择一个tour，与其中订单交换，遍历位置求出成本最小的交换方案
	float Station();				 //局部优化策略6_找到服务时刻相差最小的属于同加油站的订单对，相差时间由小到大排列，依次插入优化
	float Unneed_forshort();         //局部优化策略7_随机选择一个tour，随机选择一个其中的非必要配送订单，将此订单的配送量转移到其他下期初缺货成本大油罐上
	float Unneed_exchange();         //局部优化策略8_随机选择两个tour，将其中的非必要配送订单找出来，遍历交换。找到成本最小交换方式
	float Unneed_forH();			 //局部优化策略9_随机选择一个/多个非必要配送订单，取消后，按照装载率由低到高分配给其他订单
};


												//用户信息表
extern short int phe_table[N_NODE_COUNT][N_NODE_COUNT][R_MAX][R_MAX][Q_CANBE_COUNT][Q_CANBE_COUNT];										//信息素表
extern tour tour_info_ini[CAR_COUNT];
//extern int num_of_station;
//extern int num_of_tank;
extern float Car[4];
extern double dis[N_NODE_COUNT][N_NODE_COUNT];
extern int q_canbe[27];


