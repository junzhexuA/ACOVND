#pragma once
#include<iostream>
#include"set.h"
#include"ant.h"
#include<time.h>

#define MAX_ITE	  500			//最大迭代次数500（全局）500
#define MAX_ITE_NOIPV 20			//最大无改进次数（全局）20
#define MAX_LOC_ITE	   60 		    //最大迭代次数（局部）40
#define VNS_MAXITER 10             //VND迭代次数 10
#define PMAX	100					//定义信息素最大值
#define PMIN	10					//定义信息素最小值
#define ITE_PHE 50000				//一次迭代中进行信息素更新的总量50000 is better


using namespace std;


class vnd
{
public:
	vnd() {};
	~vnd() {};
public:
	//蚂蚁数组
	ant antarray[ANT_COUNT];
	//全局最优蚂蚁
	ant Global_Best_Ant;
	//本次迭代最优蚂蚁
	ant Ite_Best_Ant;
	//临时蚂蚁
	ant Temp_Ant;

	float ITER_COST[MAX_ITE];
	float tour_out[CAR_COUNT][33][7];
	float gante[CAR_COUNT][5][25][3];

	//进行本次最优蚂蚁的路径寻找
	void search_best_ant();

	int iter_all=0;
	int iter = 0;
	int best_ite;
	clock_t best_time;//最优解时间

	//更新信息素的分布情况,发生在一次迭代之后
	void update_phe();
	void cal_someone();
	float keep2(float a);

	//自适应框架
	ant VND_search();
	int better_EX1_num = 0; //局部1优于全局最优解次数
	int better_EX2_num = 0; //局部2优于全局最优解次数
	int better_EX3_num = 0; //局部3优于全局最优解次数	
	int better_S1_num = 0;  //局部4优于全局最优解次数
	int better_S2_num = 0;  //局部5优于全局最优解次数
	int better_Station_num = 0;  //局部6优于全局最优解次数
	int better_UN1_num = 0; //局部1优于全局最优解次数
	int better_UN2_num = 0; //局部2优于全局最优解次数
	int better_UN3_num = 0; //局部3优于全局最优解次数	

	clock_t EX1_time; //局部1优于全局最优解次数
	clock_t EX2_time; //局部2优于全局最优解次数
	clock_t EX3_time; //局部3优于全局最优解次数	
	clock_t S1_time;  //局部4优于全局最优解次数
	clock_t S2_time;  //局部5优于全局最优解次数
	clock_t Station_time;  //局部6优于全局最优解次数
	clock_t UN1_time; //局部7优于全局最优解次数
	clock_t UN2_time; //局部8优于全局最优解次数
	clock_t UN3_time; //局部9优于全局最优解次数	

	

	//int optimize_num_ = 0;//每只蚂蚁采用局部优化策略成功次数
	/*
	//算子性能分析指标

	int EX1_num = 0;//局部1使用次数
	int EX2_num = 0;//局部2使用次数
	int EX3_num = 0;//局部3使用次数
	int TW_num = 0; //局部4使用次数
	int WV_num = 0; //局部5使用次数
	int WR_num = 0; //局部6使用次数

	int best_EX1_num = 0; //局部1优于全局最优解次数
	int best_EX2_num = 0; //局部2优于全局最优解次数
	int best_EX3_num = 0; //局部3优于全局最优解次数	
	int best_TW_num = 0;  //局部4优于全局最优解次数
	int best_WV_num = 0;  //局部5优于全局最优解次数
	int best_WR_num = 0;  //局部6优于全局最优解次数
	int best_num = 0;

	clock_t EX1_time; //局部1优于全局最优解次数
	clock_t EX2_time; //局部2优于全局最优解次数
	clock_t EX3_time; //局部3优于全局最优解次数	
	clock_t TW_time;  //局部4优于全局最优解次数
	clock_t WV_time;  //局部5优于全局最优解次数
	clock_t WR_time;  //局部6优于全局最优解次数

	



	//更新信息素的分布情况,发生在一次迭代之后
	void update_phe();

	//自适应框架
	ant Adapt_search();

	//根据固定节点的交换进行局部优化
	ant ExchangeNode1();         //局部优化策略1_随机选择同一回路两个客户进行交换
	ant ExchangeNode2();         //局部优化策略2_随机选择同一回路两个客户，两客户间所有客户顺序倒置(包含两客户）
	ant ExchangeNode3();         //局部优化策略3_随机选择不同回路两个客户进行交换

	//根据时间窗进行局部优化
	ant TimeWindow();            //局部优化策略4_选择时间窗违背量最大的客户插入到成本最低的位置
	ant TimeEarly();             //局部优化策略5_选择时间窗早到量最大的客户，调整该客户之前的某个客户的前一段弧的速度以减小当前客户的早到量
	ant TimeDelay();             //局部优化策略6_选择时间窗延误量最大的客户，调整该客户之前的某个客户的前一段弧的速度以减小当前客户的延误量

	//根据速度进行局部优化
	ant WorstV();                //局部优化策略7_选择与最优速度间隙最大的一段弧（第二个节点为客户），将该客户插入到成本最低的位置

	//根据充电站进行局部优化
	ant WorstRecharge();         //局部优化策略8_选择充电成本最大的充电站移除，在该位置插入使成本最小的充电站，若不可行，则以相同方式尝试再之前的弧
*/
};
