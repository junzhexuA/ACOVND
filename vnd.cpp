#pragma once
#include<windows.h>
#include<iostream>
#include"set.h"
#include"ant.h"
#include"vnd.h"
#include<time.h>
#include<stdlib.h>
#include<cmath>



#define ROU 0.9															//信息素残留参数
#define BEST_NUM  2														//精英蚂蚁数
using namespace std;
int temp_random1 = 0;
int best5ant[BEST_NUM];
float best5thistime[BEST_NUM];
int best5thistime_num[BEST_NUM];
//int best5route[BEST_NUM][200];
double best5solution[BEST_NUM];
int best5route_num[BEST_NUM];
void vnd::search_best_ant()
{
	const float max_cost = 10000000000;
	Global_Best_Ant.cost = max_cost;//最优解
	//int iter = 0;
	int no_ipv = 0;
	while(no_ipv==0){
	
		Ite_Best_Ant.cost = max_cost;//本轮循环最优解
		for (int j = 0; j < ANT_COUNT; j++)
		{//每个蚂蚁生成路径
			int success = 0;
			while (success==0)
			{
				antarray[j].init(j);
				success = antarray[j].move();
			}
		}
		double best_num_cost[BEST_NUM];          //精英蚂蚁成本
		int best_num_ant[BEST_NUM];              //精英蚂蚁
		double best_cost = antarray[0].cost;    //本次最好解值
		int best_ant = 0;                        //本次最好蚂蚁ant1
		double sum_tmp[ANT_COUNT][2];

		for (int k = 0; k < ANT_COUNT; k++)
		{
			sum_tmp[k][0] = antarray[k].cost;
			sum_tmp[k][1] = k;
		}
		for (int k = 0; k < ANT_COUNT - 1; k++)
		{
			for (int j = k + 1; j < ANT_COUNT; j++)
			{
				double tmp = 0;
				double tmpdx = 0;
				if (sum_tmp[j][0] < sum_tmp[k][0])
				{
					tmp = sum_tmp[j][0];
					tmpdx = sum_tmp[j][1];

					sum_tmp[j][0] = sum_tmp[k][0];
					sum_tmp[j][1] = sum_tmp[k][1];

					sum_tmp[k][0] = tmp;
					sum_tmp[k][1] = tmpdx;
				}
			}
		}
		//get BEST_NUM best route this iterate
		for (int i = 0; i < BEST_NUM; i++)
		{
			//int iter3 = 0;
			best_num_cost[i] = sum_tmp[i][0];
			best_num_ant[i] = sum_tmp[i][1];


			//VND策略!!!!!!!!!!!!!!!
			Temp_Ant = antarray[best_num_ant[i]];
			if (iter==62)//ant_No==48
			{


 				int www = 1;
			}
			antarray[best_num_ant[i]] = VND_search();//返回vnd结束后的改进解
		}
		for (int j = 0; j < ANT_COUNT; j++)
		{
			if (antarray[j].cost < Ite_Best_Ant.cost)
				Ite_Best_Ant = antarray[j];
		}
		//cout << iter + 1 << "本次最优" << Ite_Best_Ant.cost << " ";
		if (Ite_Best_Ant.cost < Global_Best_Ant.cost)
		{
			Global_Best_Ant = Ite_Best_Ant;
			best_time = clock();

			best_ite = iter;
		}
		//cout << "迄今最优" << Global_Best_Ant.cost << endl;
		ITER_COST[iter] = Global_Best_Ant.cost;
		update_phe();
		//判断20次内是否有改进
		if (iter>=MAX_ITE_NOIPV)
		{
			if (ITER_COST[iter]>= ITER_COST[iter- MAX_ITE_NOIPV])
			{//无改进
				no_ipv = 1;
				iter_all = iter;
			}
		}
		iter = iter + 1;
	}
	//计算最优蚂蚁装载率
	//cal_someone();

	int order_all = 0;
	int gc_need_all = 0;
	int lap_all = 0;
	for (int c = 0; c < CAR_COUNT; c++)
	{
		for (int n = 0; n < 18; n++)
		{
			if (Global_Best_Ant.tour_sub[c][n][0] == 0)
			{
				lap_all = lap_all + 1;
			}
			if (Global_Best_Ant.tour_sub[c][n][0]>0)
			{	
				order_all = order_all + 1;
				gc_need_all = gc_need_all + ceil(Global_Best_Ant.tour_sub[c][n][1] / COMP_Q);
			}
		}
		lap_all = lap_all - 1;
	}
	Global_Best_Ant.zzl = Global_Best_Ant.Qall / (gc_need_all*COMP_Q);
	Global_Best_Ant.order_all = order_all;
	Global_Best_Ant.lap_all = lap_all;
	if (Global_Best_Ant.zzl>1)
	{
		int www = 1;
	}
	//tour_out,gante初始化
	for (int c = 0; c < CAR_COUNT; c++)
	{
		for (int x = 0; x < 18; x++)
		{
			for (int y = 0; y < 7; y++)
			{
				tour_out[c][x][y] = -1;
			}
		}
	}
	for (int c = 0; c < CAR_COUNT; c++)
	{
		for (int x = 0; x < 5; x++)
		{
			for (int y = 0; y < 17; y++)
			{
				for (int z = 0; z < 3; z++)
				{
					gante[c][x][y][z] = -1;
				}
			}
		}
	}
	//写入tour_out
	for (int car = 0; car < CAR_COUNT; car++)
	{
		for (int n = 0; n < 18; n++)
		{
			tour_out[car][n][0] = Global_Best_Ant.tour_sub[car][n][0];
			tour_out[car][n][1] = Global_Best_Ant.tour_sub[car][n][1];
			tour_out[car][n][2] = Global_Best_Ant.tour_sub[car][n][2];
			tour_out[car][n][3] = Global_Best_Ant.tour_info[car].a_time[n];
			tour_out[car][n][4] = Global_Best_Ant.tour_info[car].ser_time[n];
			tour_out[car][n][5] = Global_Best_Ant.tour_info[car].d_time[n];
			tour_out[car][n][6] = Global_Best_Ant.tour_info[car].d_cost[n];
		}
	}

	//初始化
	//tour_out{节点0、配送量1、配送次数2、到达时间3、服务时间4、迟到时长5、迟到成本6}
	//gante{[CAR_COUNT][type][17][3]}type:{行驶1、等待2、装油3、卸油4、休息5}

	for (int car = 0; car < CAR_COUNT; car++)
	{//tour0:(0,0,0,0,0,0,0)->(6,10000,1,0.848528147,0.848528147,0,0)->(6,10000,2,1.01519477,1.01519477,0,0)->(1,10000,1,1.49808919,1.49808919,0,0)->(0,0,0,2.24785089,2.24785089,0,0)->(10,20000,1,3.06407857,3.06407857,0,0)->(8,10000,1,3.39741182,3.39741182,0,0)->
	//(0,0,0,3.88030624,3.88030624,0,0)->(15,10000,1,4.98030615,4.98030615,0,0)->(1,10000,3,5.46320057,7.6730175,0,0)->(2,10000,1,7.83968401,7.83968401,0,0)->(0,0,0,8.58944607,8.58944607,0,0)->
		//行驶：：：
		int xs = 0;
		for (int i = 0; i < 18; i++)
		{
			int this_tank = tour_out[car][i][0];
			int that_tank = tour_out[car][i + 1][0];
			if (that_tank < 0)
			{
				break;
			}
			if (Global_Best_Ant.c_info[this_tank].station != Global_Best_Ant.c_info[that_tank].station) {
			//有行驶距离
				gante[car][0][xs][1] = keep2(tour_out[car][i + 1][3]);
				gante[car][0][xs][0] = keep2(tour_out[car][i + 1][3]-((dis[this_tank][that_tank])/50));
				xs = xs + 1;
			}
		}
		//等待
		int dd = 0;
		for (int i = 0; i < 18; i++)
		{
			int this_tank = tour_out[car][i][0];
			if (this_tank < 0)
			{
				break;
			}
			float arr = keep2(tour_out[car][i][3]);
			float ser = keep2(tour_out[car][i][4]);
			if (arr<ser) {
				//发生等待
				gante[car][1][dd][0] =arr;
				gante[car][1][dd][1] = ser;
				gante[car][1][dd][2] = this_tank;
				dd = dd + 1;
			}
		}
		//装油
		int zy = 0;
		for (int i = 0; i < 18; i++)
		{
			int this_tank = tour_out[car][i][0];
			int that_tank = tour_out[car][i+1][0];
			if (that_tank < 0)
			{
				break;
			}
			float arr = keep2(tour_out[car][i][3]);
			if (this_tank==0) {
				//发生装油
				gante[car][2][zy][0] = arr;
				gante[car][2][zy][1] = arr+0.5;
				zy = zy + 1;
			}
		}
		//卸油
		int xy = 0;
		for (int i = 0; i < 18; i++)
		{
			int this_tank = tour_out[car][i][0];
			if (this_tank < 0)
			{
				break;
			}
			float ser = keep2(tour_out[car][i][4]);
			if (this_tank > 0) {
				//发生卸油
				gante[car][3][xy][0] = ser;
				gante[car][3][xy][1] = ser + keep2(tour_out[car][i][1]/60000) ;
				gante[car][3][xy][2] = this_tank;
				xy = xy + 1;
			}
		}
		//休息
		int xx = 0;
		for (int i = 0; i < 18; i++)
		{
			int this_tank = tour_out[car][i][0];
			int that_tank = tour_out[car][i+1][0];
			if (this_tank < 0)
			{
				break;
			}
			float arr = keep2(tour_out[car][i][3]);
			if (that_tank<0&&this_tank==0) {
				//发生卸油
				gante[car][4][xx][0] = arr;
				gante[car][4][xx][1] = 16;
				xx = xx + 1;
			}
		}


	}
	return;
}

inline float vnd::keep2(float a) {
	int aa = (int)(a * 100 + 0.5);
	float aaa = aa / 100.0;
	return aaa;
}

inline void vnd::update_phe()//信息素更新，精英蚂蚁策略
{
	for (int a = 0; a < N_NODE_COUNT; a++)//MIN-MAX策略
	{//i
		for (int b = 0; b < N_NODE_COUNT; b++)
		{//j
			for (int c = 0; c < R_MAX; c++)
			{//r
				for (int d = 0; d < R_MAX; d++)
				{//s
					for (int e = 0; e < Q_CANBE_COUNT; e++)
					{//q1
						for (int f = 0; f < Q_CANBE_COUNT; f++)
						{//q2
							if (a != b)
							{
								phe_table[a][b][c][d][e][f] = (short int)ROU * phe_table[a][b][c][d][e][f];
								if (phe_table[a][b][c][d][e][f] > PMAX)
									phe_table[a][b][c][d][e][f] /= 2;
								if (phe_table[a][b][c][d][e][f] < PMIN)
									phe_table[a][b][c][d][e][f] = PMIN + 5;

							}
						}
					}
				}
			}
		}
	}
	double bestthistime = antarray[0].cost;;//本次最好解值
	int bestant = 0;//本次最好蚂蚁ant1
	//int bestthistime_num = antarray[0].m_npath.size();
	for (int i = 0; i < ANT_COUNT; i++)
		if (antarray[i].cost < bestthistime)
		{
			bestthistime = antarray[i].cost;
			bestant = i;
			//bestthistime_num = antarray[i].m_npath.size();
		}
	double sum_tmp[ANT_COUNT][2];
	for (int k = 0; k < ANT_COUNT; k++)
	{
		sum_tmp[k][0] = antarray[k].cost;
		sum_tmp[k][1] = k;
		if (antarray[k].cost < bestthistime)
		{
			bestthistime = antarray[k].cost;
			bestant = k;
			//bestthistime_num = antarray[k].m_npath.size();
		}

	}
	//重排序，小的在前面，大的在后面
	for (int k = 0; k < ANT_COUNT - 1; k++)
	{
		for (int j = k + 1; j < ANT_COUNT; j++)
		{
			double tmp = 0;
			double tmpdx = 0;
			if (sum_tmp[j][0] < sum_tmp[k][0])
			{
				tmp = sum_tmp[j][0];
				tmpdx = sum_tmp[j][1];

				sum_tmp[j][0] = sum_tmp[k][0];
				sum_tmp[j][1] = sum_tmp[k][1];

				sum_tmp[k][0] = tmp;
				sum_tmp[k][1] = tmpdx;
			}
		}
	}
	//get 5 best route this iterate 				
	for (int i = 0; i < BEST_NUM; i++)
	{
		best5thistime[i] = sum_tmp[i][0];
		best5ant[i] = sum_tmp[i][1];
		//best5thistime_num[i] = antarray[best5ant[i]].m_npath.size();
	}
	for (int m = 0; m < BEST_NUM; m++)
	{//m是蚂蚁
		for (int k = 0; k < CAR_COUNT; k++)
		{//k是车
			int ant_no=best5ant[m];
			int o_node = antarray[ant_no].tour_sub[k][0][0];
			int d_node = antarray[ant_no].tour_sub[k][1][0];
			int o_q = 0;
			int d_q = 0;
			int step = 0;
			int o_r = 0;
			int d_r = 0;
			while (d_node>-1)
			{
				int o_q_l = antarray[ant_no].tour_sub[k][step][1];
				int d_q_l = antarray[ant_no].tour_sub[k][step + 1][1];
				for (int w = 0; w < Q_CANBE_COUNT; w++)
				{
					if (o_q_l == q_canbe[w]) 
						o_q = w;
					if (d_q_l == q_canbe[w]) 
						d_q = w;
				}
				if (o_node==0)
					o_q = 0;
				if (d_node == 0)
					d_q = 0;
				o_r = antarray[ant_no].tour_sub[k][step][2];
				d_r = antarray[ant_no].tour_sub[k][step+1][2];
				phe_table[o_node][d_node][o_r][d_r][o_q][d_q]= (short int)phe_table[o_node][d_node][o_r][d_r][o_q][d_q] + ITE_PHE / antarray[ant_no].cost;
				if (phe_table[o_node][d_node][o_r][d_r][o_q][d_q] >= PMAX)
					phe_table[o_node][d_node][o_r][d_r][o_q][d_q] /= 2;
				phe_table[d_node][o_node][d_r][o_r][d_q][o_q] = phe_table[o_node][d_node][o_r][d_r][o_q][d_q];
				step = step + 1;
				o_node = antarray[ant_no].tour_sub[k][step][0];
				d_node = antarray[ant_no].tour_sub[k][step + 1][0];
			}
		}
	}


}

inline void vnd::cal_someone() {
	int tour0[33][3] = { {0,0,0},{6,20000,1},{1,10000,1},{0,0,0},{10,20000,1},{8,10000,1},{0,0,0},{15,10000,1},{1,10000,3},{2,10000,1},{0,0,0},{-1,-1,-1},{-1,-1,-1} ,{-1,-1,-1} ,{-1,-1,-1} ,{-1,-1,-1} ,{-1,-1,-1} ,{-1,-1,-1} };
	int tour1[33][3]={ {0,0,0},{12,10000,1},{11,20000,1},{0,0,0},{12,30000,2},{0,0,0},{-1,-1,-1},{-1,-1,-1},{-1,-1,-1},{-1,-1,-1},{-1,-1,-1},{-1,-1,-1},{-1,-1,-1} ,{-1,-1,-1} ,{-1,-1,-1} ,{-1,-1,-1} ,{-1,-1,-1} ,{-1,-1,-1} };
	int tour2[33][3] = { {0,0,0},{5,10000,1},{4,20000,1},{0,0,0},{1,20000,2},{3,10000,1},{0,0,0},{8,10000,2},{9,20000,1},{0,0,0},{-1,-1,-1},{-1,-1,-1},{-1,-1,-1} ,{-1,-1,-1} ,{-1,-1,-1} ,{-1,-1,-1} ,{-1,-1,-1} ,{-1,-1,-1} };
	::memcpy(Global_Best_Ant.tour_sub[0], tour0, sizeof(tour0));
	::memcpy(Global_Best_Ant.tour_sub[1], tour1, sizeof(tour1));
	::memcpy(Global_Best_Ant.tour_sub[2], tour2, sizeof(tour2));
	Global_Best_Ant.Qall = 240000;
	Global_Best_Ant.init_befor_calcul();
	//检查是否超出调度期约束.返回1，说明不违反，返回0说明违反。
	int planisok = Global_Best_Ant.scheduling_con();
	if (planisok == 1)
	{
		Global_Best_Ant.cal_plan_cost();
	}
}

inline ant vnd::VND_search() {
	ant ant_to_optimize = Temp_Ant, ant_to_best = Temp_Ant, ant_to_output = Temp_Ant;
	int k = 1;
	int itervnd = 0;
	float cost_after=0;
	clock_t loc_start;
	clock_t loc_end;
	int overiter = 0;//vnd无改进次数，达到100次终止
	int use_num[9] = { 0 };//初始使用次数
	while (overiter < VNS_MAXITER)
	{
		while (k <= 9) {
			itervnd = 0;

			while (itervnd < MAX_LOC_ITE)
			{
				if (k == 1)
				{

					loc_start = clock();
					cost_after = ant_to_optimize.Exchange_order1();
					loc_end = clock();
					EX1_time += (loc_end - loc_start);
					use_num[k - 1] = use_num[k - 1] + 1;

				}
				if (k == 2)
				{

					loc_start = clock();
					cost_after = ant_to_optimize.Exchange_order2();
					loc_end = clock();
					EX2_time += (loc_end - loc_start);
					use_num[k - 1] = use_num[k - 1] + 1;
				}
				if (k == 3)
				{

					loc_start = clock();
					if (CAR_COUNT > 1)
					{
						cost_after = ant_to_optimize.Exchange_order3();
					}
					loc_end = clock();
					EX3_time += (loc_end - loc_start);
					use_num[k - 1] = use_num[k - 1] + 1;
				}
				if (k == 4)
				{//执行1.5倍car数即可，尽力遍历到每一个car

					loc_start = clock();
					cost_after = ant_to_optimize.Short1();
					loc_end = clock();
					S1_time += (loc_end - loc_start);
					use_num[k - 1] = use_num[k - 1] + 1;
					if (itervnd == 0)
					{
						//itervnd = ((MAX_LOC_ITE - ceil(1.6 * CAR_COUNT)) > 0) ? MAX_LOC_ITE - ceil(1.6 * CAR_COUNT) : 0;
					}
				}
				if (k == 5)
				{//每个tour执行一次就行

					loc_start = clock();
					if (CAR_COUNT > 1) {
						cost_after = ant_to_optimize.Short2();
					}
					loc_end = clock();
					S2_time += (loc_end - loc_start);
					use_num[k - 1] = use_num[k - 1] + 1;
					if (itervnd == 0)
					{
						//itervnd = ((MAX_LOC_ITE - ceil(1.6 * CAR_COUNT)) > 0) ? MAX_LOC_ITE - ceil(1.6 * CAR_COUNT) : 0;
					}

				}
				if (k == 6)
				{//此算子只执行一次即可
					if (ant_to_optimize.ant_NO == 1)//ant_to_optimize.ant_NO==42&& ant_to_optimize.Qall==130500&& 
					{

						int www = 1;
					}
					loc_start = clock();
					cost_after = ant_to_optimize.Station();
					loc_end = clock();
					Station_time += (loc_end - loc_start);
					use_num[k - 1] = use_num[k - 1] + 1;
					itervnd = MAX_LOC_ITE;
				}
				if (k == 8)
				{//运行多次
					
					loc_start = clock();
					cost_after = ant_to_optimize.Unneed_forshort();
					loc_end = clock();
					UN1_time += (loc_end - loc_start);
					use_num[k - 1] = use_num[k - 1] + 1;
				}
				if (k == 9)
				{//运行多次

					loc_start = clock();
					if (CAR_COUNT > 1) {
						cost_after = ant_to_optimize.Unneed_exchange();
					}
					loc_end = clock();
					UN2_time += (loc_end - loc_start);
					use_num[k - 1] = use_num[k - 1] + 1;
				}
				if (k == 7)
				{//每个tour都顾及到，就ok
					if (ant_to_optimize.ant_NO == 17 && itervnd == 36)//ant_to_optimize.ant_NO==42&& ant_to_optimize.Qall==130500&& 
					{

						int www = 1;
					}
					loc_start = clock();
					cost_after = ant_to_optimize.Unneed_forH();
					loc_end = clock();
					UN3_time += (loc_end - loc_start);
					use_num[k - 1] = use_num[k - 1] + 1;
					if (itervnd == 0)
					{
						//itervnd = ((MAX_LOC_ITE - ceil(1.6 * CAR_COUNT)) > 0) ? MAX_LOC_ITE - ceil(1.6 * CAR_COUNT) : 0;
					}

				}

				if (ant_to_optimize.cost < ant_to_best.cost)
				{ //改进了
					ant_to_best = ant_to_optimize;

					if (k == 1)
						better_EX1_num++;
					else if (k == 2)
						better_EX2_num++;
					else if (k == 3)
						better_EX3_num++;
					else if (k == 4)
						better_S1_num++;
					else if (k == 5)
						better_S2_num++;
					else if (k == 6)
						better_Station_num++;
					else if (k == 7)
						better_UN1_num++;
					else if (k == 8)
						better_UN2_num++;
					else if (k == 9)
						better_UN3_num++;
					k = 0;
					break;
				}
				else
				{
					ant_to_optimize = ant_to_best;//没改良就回溯
					itervnd = itervnd + 1;
				}
			}
			k = k + 1;
		}
		overiter = overiter + 1;
	}
	return ant_to_best;
}









