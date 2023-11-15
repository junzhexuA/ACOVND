#pragma once
#include<iostream>
#include"set.h"
#include"ant.h"
#include"vnd.h"
#include<cmath>
#include<time.h>
#include<stdlib.h>
#include<math.h>
#include<algorithm>


using namespace std;
//初始化蚂蚁的记录
void ant::init(int i)
{
	//m_npath.clear();
	//m_npath1.clear();
	//rest_energy = FULL_ENERGY;
	ant_NO = i;
	Qall = 0;
	cur_tank_NO = 0;
	cur_car_r = 0;
	cur_time = 0.5;
	cur_car_NO = 0;
	cur_car_r = 0;
	cost = 0;
	tran_cost = 0;
	unload_cost = 0;
	delay_cost = 0;
	next_delay_cost = 0;
	//sati_num = 0;
	capacity = COMP_COUNT;
	//tankallsati = 0;
	for (int g = 0; g < CAR_COUNT; g++)
	{
		car_useable[g] = 1;
		for (int j = 0; j < 35; j++)
		{
			tour_sub[g][j][0] = -1;
			tour_sub[g][j][1] = -1;
			tour_sub[g][j][2] = -1;
		}
		tour_sub[g][0][0] = 0;
		tour_sub[g][0][1] = 0;
		tour_sub[g][0][2] = 0;
	}

	//c_info初始化
	for (int y = 0; y < N_NODE_COUNT; y++)
	{
		if (y > 0)
		{
			c_info[y].id = y;
			c_info[y].t = c_info_ini[y].t;
			c_info[y].oil = c_info_ini[y].oil;
			c_info[y].station = c_info_ini[y].station;
			c_info[y].x = c_info_ini[y].x;
			c_info[y].y = c_info_ini[y].y;
			c_info[y].ini_stock = c_info_ini[y].ini_stock;
			c_info[y].tank_cap = c_info_ini[y].tank_cap;
			c_info[y].sell_v = c_info_ini[y].sell_v;
			c_info[y].recent_left_time = 0;
			c_info[y].send_all = 0;
			c_info[y].dtime_all = 0;
			c_info[y].dtime_cost = 0;
			c_info[y].next_dtime = 0;					//下期末初缺货总时长
			c_info[y].next_d_time_cost = 0;
			c_info[y].rmax = floor((c_info[y].tank_cap - c_info[y].ini_stock + B_END * c_info[y].sell_v) / (0.8 * Car[1]));
			c_info[y].S_time = c_info[y].ini_stock / c_info[y].sell_v;
			c_info[y].r_now = 0;
			for (int j = 0; j < 10; j++)
			{
				for (int u = 0; u < 8; u++)
				{
					c_info[y].order[j][u] = 0;
				}
			}
		}
		if (y == 0)
		{
			c_info[y].id = y;
			c_info[y].t = c_info_ini[y].t;
			c_info[y].oil = c_info_ini[y].oil;
			c_info[y].station = c_info_ini[y].station;
			c_info[y].x = c_info_ini[y].x;
			c_info[y].y = c_info_ini[y].y;
			c_info[y].ini_stock = c_info_ini[y].ini_stock;
			c_info[y].tank_cap = c_info_ini[y].tank_cap;
			c_info[y].sell_v = c_info_ini[y].sell_v;
			c_info[y].recent_left_time = 0;
			c_info[y].send_all = 0;
			c_info[y].dtime_all = 0;
			c_info[y].dtime_cost = 0;
			c_info[y].next_dtime = 0;					//下期末初缺货总时长
			c_info[y].next_d_time_cost = 0;
			c_info[y].rmax = 1000;
			c_info[y].S_time = 0;
			c_info[y].r_now = 0;
			//c_info[i][y].sati = 0;
			for (int j = 0; j < 10; j++)
			{
				for (int u = 0; u < 8; u++)
				{
					c_info[y].order[j][u] = 0;
				}
			}
		}
	}

	//初始化tour

	for (int j = 0; j < CAR_COUNT; j++)
	{
		tour_info[j].car_id = 0;
		tour_info[j].cur_node_id = 0;
		tour_info[j].cur_q = 0;                               //在cur_node_id的配送量
		tour_info[j].cur_time = 0.5;
		tour_info[j].lap = 0;
		tour_info[j].useable = 1;                            //车可用状态
		tour_info[j].gc_left = 3;							//剩余可用隔舱
		tour_info[j].node_num = 1;
		for (int l = 0; l < 18; l++)
		{
			tour_info[j].node[l] = -1;
			if (l == 0)
			{
				tour_info[j].node[l] = 0;
			}
			tour_info[j].gc_need[l] = 0;						//保存每次访问所用隔舱
			//tour_info[i][j].unnece[l] = -1;							//记录每个配送是否是必要配送，必要是0.非必要是1
			tour_info[j].r[l] = -1;
			tour_info[j].q[l] = 0;
			tour_info[j].a_time[l] = 0;
			tour_info[j].U_time[l] = 0;
			tour_info[j].S_time[l] = 0;
			tour_info[j].d_time[l] = 0;
			tour_info[j].d_cost[l] = 0;
			tour_info[j].ser_time[l] = 0;
		}
	}

}

int ant::move()
{
	int success = -1;
	while (Qall < HMIN)
	{
		int rand_step = (rand() % (2)) + 1;//生成1-2之间的随机数
		int steped = 0;
		int choose_or_not = -1;
		while (steped < rand_step)
		{
			if (Qall > HMIN)
			{
				//生成解
				break;
			}
			choose_or_not = -1;
			while (choose_or_not < 0) {
				choose_or_not = choose_next_city();												//找出下一次要移动的城市
				//如果返回的是-1,说明没找到节点，这辆车不行，需要换车

				if (choose_or_not == -1) {//需要换车
					tour_info[cur_car_NO].useable = 0;
					car_useable[cur_car_NO] = 0;
					//检查是否所有车都不可用
					int useable_num = 0;
					for (int u = 0; u < CAR_COUNT; u++)
					{//
						useable_num = useable_num + car_useable[u];
					}
					if (useable_num == 0)
					{//出问题了，算例设置不合理，应该增加车辆数量
						int warning = 111;

						//cout << "Rebuild" << endl;
						return 0;
					}
					cur_car_NO = (cur_car_NO + 1 > CAR_COUNT - 1) ? 0 : cur_car_NO + 1;
					cur_tank_NO = tour_info[cur_car_NO].cur_node_id;
					cur_car_r = tour_info[cur_car_NO].lap;    //趟数
					cur_time = tour_info[cur_car_NO].cur_time;							//当前时间
					capacity = tour_info[cur_car_NO].gc_left;
				}
				else//找到节点
				{
					//int a = 1;
					steped = steped + 1;
				}
			}
		}
		//本车的步数走完后，需要换车
		if (Qall < HMIN)
		{
			cur_car_NO = (cur_car_NO + 1 > CAR_COUNT - 1) ? 0 : cur_car_NO + 1;
			cur_tank_NO = tour_info[cur_car_NO].cur_node_id;
			cur_car_r = tour_info[cur_car_NO].lap;                              //趟数
			cur_time = tour_info[cur_car_NO].cur_time;							//当前时间
			capacity = tour_info[cur_car_NO].gc_left;
		}
	}
	for (int c = 0; c < CAR_COUNT; c++)
	{
		cur_car_NO = c;
		cur_tank_NO = tour_info[c].cur_node_id;
		cur_car_r = tour_info[c].lap;                              //趟数
		cur_time = tour_info[c].cur_time;							//当前时间
		capacity = tour_info[c].gc_left;
		if (cur_tank_NO != 0)
		{
			change_stasus(0, 0);
		}
	}
	//回车场，生成完整解

	success = 1;

	//计算解的成本！
	cal_plan_cost();
	return success;
}

inline int ant::choose_next_city()//根据转移规则选择下一城市~
{
	float pro = ((float)rand()) / RAND_MAX;
	if (c_info[cur_tank_NO].t == 'd')//当前节点在油库
	{
		if (tour_info[cur_car_NO].lap < Max_tour_per_car)//当前车还有可用趟数
		{
			float probability[N_NODE_COUNT][Q_CANBE_COUNT] = { 0 };
			bool allow_list[N_NODE_COUNT][Q_CANBE_COUNT] = { false };
			float pro_total = 0;
			float sum = 0;
			//记录候选节点的个数
			int num_candidate = 0;
			for (int t = 0; t < N_NODE_COUNT; t++)
			{
				if ((c_info[t].t == 't') && (c_info[t].r_now < c_info[t].rmax) && (cur_time >= c_info[t].recent_left_time)) {
					for (int q = 0; q < Q_CANBE_COUNT; q++)
					{//节点cur_tank_NO,到油罐t，配送量是q_canbe[q]的可行性

						float U_time_q = (c_info[t].ini_stock - c_info[t].tank_cap + c_info[t].send_all + q_canbe[q]) / c_info[t].sell_v + c_info[t].dtime_all;//容纳点
						float S_time_q = c_info[t].S_time;//缺货点
						float a_time_q = cur_time + dis[cur_tank_NO][t] / CAR_V;//到达时刻
						float l_time_q = q_canbe[q] / SERVICE_RATE;//卸油时长
						float w_time_q = (0 > U_time_q - a_time_q) ? 0 : U_time_q - a_time_q;//等待时长
						float d_time_q = (0 > a_time_q - S_time_q) ? 0 : a_time_q - S_time_q;//迟到时长
						int gc_need = ceil(q_canbe[q] / COMP_Q);
						if (a_time_q + l_time_q + w_time_q + dis[t][0] / CAR_V <= B_END)//符合调度期约束
						{
							if ((tour_info[cur_car_NO].gc_left >= gc_need) && q_canbe[q] <= c_info[t].tank_cap)//此处gc.left应该=3，隔舱约束
							{
								if (q_canbe[q] + Qall <= HMAX)//总配送量约束&&必须当tank上次配送结束后（可能由别的车送），才能配送下一次
								{
									//通过约束，开始算转移概率
									allow_list[t][q] = true;
									num_candidate = num_candidate + 1;
									int cur_q = -1;
									for (int w = 0; w < Q_CANBE_COUNT; w++)
									{
										if (tour_info[cur_car_NO].cur_q == q_canbe[w]) {
											cur_q = w;
											break;
										}
									}
									float yz_xxs = (2 * phe_table[cur_tank_NO][t][c_info[cur_tank_NO].r_now][c_info[t].r_now + 1][cur_q][q]) / (PMAX + PMIN);//信息素因子
									float yz_dis = 1 / (dis[cur_tank_NO][t] + 1);//距离因子
									float yz_kxb = 1 / (abs(S_time_q - a_time_q) + 1);//库销比因子
									float yz_twmatch = 0;//时间窗匹配因子
									if (a_time_q < U_time_q)
									{//早到
										yz_twmatch = 1 / (U_time_q - a_time_q + 1);
									}
									else if (a_time_q > S_time_q)
									{
										yz_twmatch = 1 / (3 * (U_time_q - a_time_q + 1));
									}
									else
									{
										yz_twmatch = 1;
									}
									double load_rate = q_canbe[q] / (ceil(q_canbe[q] / COMP_Q) * COMP_Q);

									if (load_rate >= 0.8)//满载
									{
										load_rate = 1;
									}
									float yz_load = exp(load_rate) / exp(1);//装载率因子
									probability[t][q] = yz_xxs + yz_dis + yz_kxb + yz_twmatch + yz_load;
									pro_total = pro_total + probability[t][q];
								}
							}
						}
					}
				}
			}
			//概率计算完毕，开始选节点！
			if (num_candidate > 0)//有候选节点
			{
				float pro = ((float)rand()) / RAND_MAX * pro_total;
				pro = (pro > 0.0000001) ? pro : 0.0000001;
				int next_node = -6;
				int next_q = -6;
				for (int x = 0; x < N_NODE_COUNT; x++)
				{
					for (int y = 0; y < Q_CANBE_COUNT; y++)
					{
						sum += probability[x][y];
						if (pro <= sum)
						{
							next_node = x;
							next_q = y;
							goto here;
						}
					}
				}
			here:
				if (next_node == -6)
				{
					cout << "wrong!!!" << endl;
					cout << "allownum is " << num_candidate;
					return -2;//要改
				}
				else
				{//选好了下一节点，需要改变状态
					change_stasus(next_node, next_q);
					return 1;//成功找到下一节点
				}
			}
			else {//没有可选节点，车k配送结束，从油库没有可选节点，说明这个车走不了了

				return -1;//返回回去换车，改蚂蚁信息
			}
		}
		else//趟都跑满了
		{
			return -1;//返回回去换车，改蚂蚁信息
		}
	}
	if (c_info[cur_tank_NO].t == 't')//当前节点在油罐
	{
		if (tour_info[cur_car_NO].gc_left > 0)//当前车还有可用隔舱
		{
			float probability[N_NODE_COUNT][Q_CANBE_COUNT] = { 0 };
			bool allow_list[N_NODE_COUNT][Q_CANBE_COUNT] = { false };
			float pro_total = 0;
			float sum = 0;
			//记录候选节点的个数
			int num_candidate = 0;

			for (int t = 0; t < N_NODE_COUNT; t++)
			{
				if (t == 10)
				{
					int www = 1;
				}
				if ((c_info[t].t == 't') && (c_info[t].r_now < c_info[t].rmax) && (cur_time >= c_info[t].recent_left_time)) {
					for (int q = 0; q < Q_CANBE_COUNT; q++)
					{//节点cur_tank_NO,到油罐t，配送量是q_canbe[q]的可行性

						float U_time_q = (c_info[t].ini_stock - c_info[t].tank_cap + c_info[t].send_all + q_canbe[q]) / c_info[t].sell_v + c_info[t].dtime_all;//容纳点
						float S_time_q = c_info[t].S_time;//缺货点
						float a_time_q = cur_time + dis[cur_tank_NO][t] / CAR_V;//到达时刻
						float l_time_q = q_canbe[q] / SERVICE_RATE;//卸油时长
						float w_time_q = (0 > U_time_q - a_time_q) ? 0 : U_time_q - a_time_q;//等待时长
						float d_time_q = (0 > a_time_q - S_time_q) ? 0 : a_time_q - S_time_q;//迟到时长
						int gc_need = ceil(q_canbe[q] / COMP_Q);
						if (a_time_q + l_time_q + w_time_q + dis[t][0] / CAR_V <= B_END)//符合调度期约束
						{
							if (tour_info[cur_car_NO].gc_left >= gc_need && q_canbe[q] <= c_info[t].tank_cap)//此处gc.left应该=3，隔舱约束
							{
								if (q_canbe[q] + Qall <= HMAX)//总配送量约束
								{
									//通过约束，开始算转移概率
									allow_list[t][q] = true;
									num_candidate = num_candidate + 1;
									int cur_q = -1;
									for (int w = 0; w < Q_CANBE_COUNT; w++)
									{
										if (tour_info[cur_car_NO].cur_q == q_canbe[w]) {
											cur_q = w;
											break;
										}
									}
									float yz_xxs = (2 * phe_table[cur_tank_NO][t][c_info[cur_tank_NO].r_now][c_info[t].r_now + 1][cur_q][q]) / (PMAX + PMIN);//信息素因子
									float yz_dis = 1 / (dis[cur_tank_NO][t] + 1);//距离因子
									yz_dis = pow(yz_dis, 0.25);
									float yz_kxb = 1 / (abs(S_time_q - a_time_q) + 1);//库销比因子
									float yz_twmatch = 0;//时间窗匹配因子
									if (a_time_q < U_time_q)
									{//早到
										yz_twmatch = 1 / (U_time_q - a_time_q + 1);
									}
									else if (a_time_q > S_time_q)
									{
										yz_twmatch = 1 / (3 * (U_time_q - a_time_q + 1));
									}
									else
									{
										yz_twmatch = 1;
									}
									double load_rate = (q_canbe[q] / COMP_Q) - floor(q_canbe[q] / COMP_Q);
									if (load_rate < 0.01)//满载
									{
										load_rate = 1;
									}
									float yz_load = exp(load_rate) / exp(1);//装载率因子
									probability[t][q] = yz_xxs + yz_dis + yz_kxb + yz_twmatch + yz_load;
									pro_total = pro_total + probability[t][q];
								}
							}
						}
					}
				}
			}
			//概率计算完毕，开始选节点！
			if (num_candidate > 0)//有候选节点
			{
				float pro = ((float)rand()) / RAND_MAX * pro_total;
				int next_node = -6;
				int next_q = -6;
				int find = 0;
				for (int x = 0; x < N_NODE_COUNT; x++)
				{
					for (int y = 0; y < Q_CANBE_COUNT; y++)
					{
						sum += probability[x][y];
						if (pro <= sum)
						{
							next_node = x;
							next_q = y;
							goto here2;
						}
					}
				}
			here2:
				if (next_node == -6)
				{
					cout << "wrong!!!" << endl;
					cout << "allownum is " << num_candidate;
					return -2;//要改
				}
				else
				{//选好了下一节点，需要改变状态
					change_stasus(next_node, next_q);
					return 1;//成功找到下一节点
				}

			}
			else {//没有可选节点，换车

				return -1;//返回回去换车
			}
		}
		else//没有可用隔舱了，回油厂
		{
			int next_node = 0;
			int next_q = 0;
			change_stasus(next_node, next_q);
			return 1;
		}
	}

}


inline bool ant::change_stasus(int next_tank, int next_q)
{
	if (c_info[next_tank].t == 't')//下一节点是油罐节点
	{
		float U_time_next = ((c_info[next_tank].ini_stock - c_info[next_tank].tank_cap + c_info[next_tank].send_all + q_canbe[next_q]) / c_info[next_tank].sell_v) + c_info[next_tank].dtime_all;//容纳点
		float S_time_next = ((c_info[next_tank].ini_stock + c_info[next_tank].send_all) / c_info[next_tank].sell_v) + c_info[next_tank].dtime_all;//Oir的缺货点,送完这次前的缺货点
		float a_time_next = cur_time + dis[cur_tank_NO][next_tank] / CAR_V;//到达时刻
		float l_time_next = q_canbe[next_q] / SERVICE_RATE;//卸油时长
		float w_time_next = (0 > U_time_next - a_time_next) ? 0 : U_time_next - a_time_next;//等待时长
		float d_time_next = (0 > a_time_next - S_time_next) ? 0 : a_time_next - S_time_next;//迟到时长
		float left_time = a_time_next + l_time_next + w_time_next;
		//更新油罐c_info信息
		c_info[next_tank].send_all = c_info[next_tank].send_all + q_canbe[next_q];
		c_info[next_tank].S_time = ((c_info[next_tank].ini_stock + c_info[next_tank].send_all) / c_info[next_tank].sell_v) + c_info[next_tank].dtime_all;//配送完这一次后的缺货点
		c_info[next_tank].r_now = c_info[next_tank].r_now + 1;
		c_info[next_tank].dtime_all = c_info[next_tank].dtime_all + d_time_next;
		c_info[next_tank].order[c_info[next_tank].r_now - 1][0] = c_info[next_tank].r_now;
		c_info[next_tank].recent_left_time = left_time;
		int r_next = c_info[next_tank].r_now;
		c_info[next_tank].order[r_next - 1][0] = r_next;//r
		c_info[next_tank].order[r_next - 1][1] = q_canbe[next_q];//q
		c_info[next_tank].order[r_next - 1][2] = (ceil(q_canbe[next_q] / COMP_Q));//所用隔舱
		c_info[next_tank].order[r_next - 1][3] = a_time_next;//到达
		c_info[next_tank].order[r_next - 1][4] = U_time_next;
		c_info[next_tank].order[r_next - 1][5] = S_time_next;
		c_info[next_tank].order[r_next - 1][6] = d_time_next;
		c_info[next_tank].order[r_next - 1][7] = d_time_next * c_info[next_tank].sell_v * short_c;//迟到成本
		//判断是否sati

		//更新车辆tour信息
		tour_info[cur_car_NO].cur_node_id = next_tank;
		tour_info[cur_car_NO].cur_q = q_canbe[next_q];
		tour_info[cur_car_NO].cur_time = a_time_next + l_time_next + w_time_next;
		tour_info[cur_car_NO].gc_left = tour_info[cur_car_NO].gc_left - (ceil(q_canbe[next_q] / COMP_Q));
		tour_info[cur_car_NO].node[tour_info[cur_car_NO].node_num] = next_tank;
		//tour_info[ant_NO][cur_car_NO].unnece[tour_info[ant_NO][cur_car_NO].node_num] = sati_before;
		tour_info[cur_car_NO].gc_need[tour_info[cur_car_NO].node_num] = ceil(q_canbe[next_q] / COMP_Q);
		tour_info[cur_car_NO].r[tour_info[cur_car_NO].node_num] = r_next;
		tour_info[cur_car_NO].q[tour_info[cur_car_NO].node_num] = q_canbe[next_q];
		tour_info[cur_car_NO].a_time[tour_info[cur_car_NO].node_num] = a_time_next;
		tour_info[cur_car_NO].U_time[tour_info[cur_car_NO].node_num] = U_time_next;
		tour_info[cur_car_NO].S_time[tour_info[cur_car_NO].node_num] = S_time_next;
		tour_info[cur_car_NO].d_time[tour_info[cur_car_NO].node_num] = d_time_next;
		tour_info[cur_car_NO].d_cost[tour_info[cur_car_NO].node_num] = d_time_next * c_info[next_tank].sell_v * short_c;
		tour_info[cur_car_NO].ser_time[tour_info[cur_car_NO].node_num] = (U_time_next > a_time_next) ? U_time_next : a_time_next;//开始服务时刻
		//float check = a_time_next + w_time_next;
		tour_info[cur_car_NO].node_num = tour_info[cur_car_NO].node_num + 1;
		//更新蚂蚁信息
		Qall = Qall + q_canbe[next_q];
		cur_tank_NO = next_tank;
		cur_time = a_time_next + l_time_next + w_time_next;
		capacity = capacity - (floor(next_q / 5) + 1);
		tour_sub[cur_car_NO][tour_info[cur_car_NO].node_num - 1][0] = next_tank;
		tour_sub[cur_car_NO][tour_info[cur_car_NO].node_num - 1][1] = q_canbe[next_q];
		tour_sub[cur_car_NO][tour_info[cur_car_NO].node_num - 1][2] = r_next;
	}
	if (c_info[next_tank].t == 'd')//下一节点是油厂节点
	{

		float U_time_next = 0;//容纳点
		float S_time_next = 0;//Oir的缺货点,送完这次后新产生的缺货点
		float a_time_next = cur_time + dis[cur_tank_NO][next_tank] / CAR_V;//到达时刻
		float l_time_next = 0;//卸油时长
		float w_time_next = 0;//等待时长
		float d_time_next = 0;//迟到时长
		float left_time = a_time_next + LOAD_T;


		//更新车辆tour信息
		tour_info[cur_car_NO].lap = tour_info[cur_car_NO].lap + 1;
		tour_info[cur_car_NO].cur_node_id = next_tank;
		tour_info[cur_car_NO].cur_q = 0;
		tour_info[cur_car_NO].cur_time = left_time;
		tour_info[cur_car_NO].gc_left = COMP_COUNT;
		tour_info[cur_car_NO].node[tour_info[cur_car_NO].node_num] = next_tank;
		tour_info[cur_car_NO].gc_need[tour_info[cur_car_NO].node_num] = 0;
		//tour_info[ant_NO][cur_car_NO].unnece[tour_info[ant_NO][cur_car_NO].node_num] = 0;
		tour_info[cur_car_NO].r[tour_info[cur_car_NO].node_num] = 0;
		tour_info[cur_car_NO].q[tour_info[cur_car_NO].node_num] = 0;
		tour_info[cur_car_NO].a_time[tour_info[cur_car_NO].node_num] = a_time_next;
		tour_info[cur_car_NO].U_time[tour_info[cur_car_NO].node_num] = U_time_next;
		tour_info[cur_car_NO].S_time[tour_info[cur_car_NO].node_num] = S_time_next;
		tour_info[cur_car_NO].d_time[tour_info[cur_car_NO].node_num] = d_time_next;
		tour_info[cur_car_NO].d_cost[tour_info[cur_car_NO].node_num] = 0;
		tour_info[cur_car_NO].ser_time[tour_info[cur_car_NO].node_num] = 0;//开始服务时刻
		//float check = a_time_next + w_time_next;
		tour_info[cur_car_NO].node_num = tour_info[cur_car_NO].node_num + 1;
		//更新蚂蚁信息
		//Qall = Qall + 0;
		cur_tank_NO = next_tank;
		cur_time = a_time_next + LOAD_T;
		capacity = COMP_COUNT;
		tour_sub[cur_car_NO][tour_info[cur_car_NO].node_num - 1][0] = next_tank;
		tour_sub[cur_car_NO][tour_info[cur_car_NO].node_num - 1][1] = 0;
		tour_sub[cur_car_NO][tour_info[cur_car_NO].node_num - 1][2] = 0;
	}

	return true;
}

void ant::cal_plan_cost() {
	//行驶成本计算
	float dis_road = 0;
	int node_i = 0;
	int node_j = 0;
	for (int i = 0; i < CAR_COUNT; i++)
	{//i是车的编号
		int pos = 0;
		while (tour_sub[i][pos + 1][0] >= 0)
		{//累加第pos个节点到下一节点之间的距离
			node_i = tour_sub[i][pos][0];
			node_j = tour_sub[i][pos + 1][0];
			dis_road = dis_road + dis[node_i][node_j];
			pos = pos + 1;
		}
	}
	tran_cost = dis_road * drive_c;
	//卸货成本计算
	float unload_cost_all = 0;
	for (int t = 1; t < N_NODE_COUNT; t++)
	{
		unload_cost_all = unload_cost_all + ((c_info[t].send_all / SERVICE_RATE) * c_info[t].sell_v * unload_c);
	}
	unload_cost = unload_cost_all;
	//缺货成本计算
	float delay_cost_all = 0;
	float next_delay_cost_all = 0;
	float delay_time_all = 0;
	float next_delay_time_all = 0;
	for (int t = 1; t < N_NODE_COUNT; t++)
	{
		delay_cost_all = delay_cost_all + c_info[t].dtime_all * c_info[t].sell_v * short_c;
		c_info[t].dtime_cost = c_info[t].dtime_all * c_info[t].sell_v * short_c;
		delay_time_all = delay_time_all + c_info[t].dtime_all;
		float next_delay_time = (0 > B_END - c_info[t].S_time) ? 0 : B_END - c_info[t].S_time;
		next_delay_time_all = next_delay_time_all + next_delay_time;
		c_info[t].next_dtime = next_delay_time;
		c_info[t].next_d_time_cost = next_delay_time * c_info[t].sell_v * short_c;
		next_delay_cost_all = next_delay_cost_all + next_delay_time * c_info[t].sell_v * short_c;
	}
	next_delay_cost = next_delay_cost_all;
	delay_cost = delay_cost_all;
	//总成本
	cost = tran_cost + delay_cost + next_delay_cost;
	//cost = tran_cost;
	//cost = delay_cost + next_delay_cost;
	delay_time = delay_time_all;
	next_delay_time = next_delay_time_all;
}

float ant::Exchange_order1()//局部策略1_随机选择同一回路两个订单进行交换
{//不涉及油库节点顺序调整F
	int tour_num = (rand() % (CAR_COUNT));//生成0-车数-1之间的随机数，代表选择哪个回路
	int trip_num = (rand() % (tour_info[tour_num].lap));//选择哪个趟数，从0开始
	//找到指定trip的头尾节点编号
	int lap = -1;
	int startn = 0, endn = 0;
	int order_a = 0, order_b = 0;
	int ar_should = 0, br_should = 0;
	for (int n = 0; n < 18; n++)
	{
		if (tour_sub[tour_num][n][0] == 0)
		{//过了一个lap
			lap = lap + 1;
			if (lap == trip_num)
			{
				startn = n + 1;
			}
			else if (lap == trip_num + 1)
			{
				endn = n - 1;
			}
		}
	}
	if (startn > endn)
	{//这一趟只有一个节点，换不了

		return cost;
	}
	if (startn == endn)
	{//这一趟只有一个节点，换不了
		return cost;
	}
	else
	{//选两个订单交换
		int change_times = 0;
		order_a = (rand() % (endn - startn + 1)) + startn;//生成startn-endn之间的随机数
		order_b = order_a;
		while (tour_sub[tour_num][order_a][0] == tour_sub[tour_num][order_b][0])
		{

			order_b = (rand() % (endn - startn + 1)) + startn;//生成startn-endn之间的随机数
			change_times = change_times + 1;
			if (change_times > 30)
			{
				break;
			}
		}
		if (change_times > 30)
		{//所选中的trip中只有两个同油罐节点，不可交换
			return cost;
		}
	}
	//调整tour_sub
	int temp[3] = { 0 };
	temp[0] = tour_sub[tour_num][order_a][0];
	temp[1] = tour_sub[tour_num][order_a][1];
	temp[2] = tour_sub[tour_num][order_a][2];
	tour_sub[tour_num][order_a][0] = tour_sub[tour_num][order_b][0];
	tour_sub[tour_num][order_a][1] = tour_sub[tour_num][order_b][1];
	tour_sub[tour_num][order_a][2] = tour_sub[tour_num][order_b][2];
	tour_sub[tour_num][order_b][0] = temp[0];
	tour_sub[tour_num][order_b][1] = temp[1];
	tour_sub[tour_num][order_b][2] = temp[2];
	int gowrong = 0;
	for (int n = 0; n < 18; n++)
	{//检验有没有一个tour内不符合访问顺序约束的
		if (tour_sub[tour_num][n][0] < 0)
		{
			break;
		}
		if (tour_sub[tour_num][n][0] == tour_sub[tour_num][order_a][0])
		{
			if (tour_sub[tour_num][n][2] < ar_should)
			{
				gowrong = 1;
				break;
			}
			ar_should = tour_sub[tour_num][n][2];
		}
		if (tour_sub[tour_num][n][0] == tour_sub[tour_num][order_b][0])
		{
			if (tour_sub[tour_num][n][2] < br_should)
			{
				gowrong = 1;
				break;
			}
			br_should = tour_sub[tour_num][n][2];
		}
	}
	if (gowrong == 1)
	{
		return cost;
	}

	//初始化一些参数
	init_befor_calcul();
	//检查是否超出调度期约束.返回1，说明不违反，返回0说明违反。
	int planisok = scheduling_con();
	//计算成本
	if (planisok == 1)
	{
		cal_plan_cost();
	}
	return cost;
}

float ant::Exchange_order2()//局部策略2_随机选择同一tour两个订单进行交换
{//不涉及油库节点顺序调整,选择同隔舱订单

	int tour_num = (rand() % (CAR_COUNT));//生成0-车数-1之间的随机数，代表选择哪个回路
	int node_num = tour_info[tour_num].node_num;
	int order_a = 0, order_b = -1;
	int ar_should = 0, br_should = 0;
	while (tour_sub[tour_num][order_a][0] == 0)
	{//随机选择一个油罐节点（不能是油库）
		order_a = (rand() % (tour_info[tour_num].node_num));
	}
	int order_air = ceil(tour_sub[tour_num][order_a][1] / COMP_Q);//交换订单需满足隔舱数
	int ok_order[33] = { 0 };//存放符合隔仓约束的节点位置
	int ok_order_count = 0;
	for (int b = 0; b < node_num; b++)
	{
		int this_air = ceil(tour_sub[tour_num][b][1] / COMP_Q);
		if (this_air == order_air && b != order_a)
		{//放入ok_order
			ok_order[ok_order_count] = b;
			ok_order_count = ok_order_count + 1;
		}
		if (tour_sub[tour_num][b][0] == -1)
		{
			int l = 0;
		}
	}
	if (ok_order_count == 0)
	{//整个tour里没有和order_a一样隔舱的订单
		return cost;
	}
	else
	{
		order_b = ok_order[(rand() % (ok_order_count))];
	}

	//调整tour_sub
	int temp[3] = { 0 };
	temp[0] = tour_sub[tour_num][order_a][0];
	temp[1] = tour_sub[tour_num][order_a][1];
	temp[2] = tour_sub[tour_num][order_a][2];
	tour_sub[tour_num][order_a][0] = tour_sub[tour_num][order_b][0];
	tour_sub[tour_num][order_a][1] = tour_sub[tour_num][order_b][1];
	tour_sub[tour_num][order_a][2] = tour_sub[tour_num][order_b][2];
	tour_sub[tour_num][order_b][0] = temp[0];
	tour_sub[tour_num][order_b][1] = temp[1];
	tour_sub[tour_num][order_b][2] = temp[2];
	int gowrong = 0;
	for (int n = 0; n < 18; n++)
	{//检验有没有一个tour内不符合访问顺序约束的
		if (tour_sub[tour_num][n][0] < 0)
		{
			break;
		}
		if (tour_sub[tour_num][n][0] == tour_sub[tour_num][order_a][0])
		{
			if (tour_sub[tour_num][n][2] < ar_should)
			{
				gowrong = 1;
				break;
			}
			ar_should = tour_sub[tour_num][n][2];
		}
		if (tour_sub[tour_num][n][0] == tour_sub[tour_num][order_b][0])
		{
			if (tour_sub[tour_num][n][2] < br_should)
			{
				gowrong = 1;
				break;
			}
			br_should = tour_sub[tour_num][n][2];
		}
	}
	if (gowrong == 1)
	{
		return cost;
	}
	//初始化一些参数
	init_befor_calcul();
	//检查是否超出调度期约束.返回1，说明不违反，返回0说明违反。
	int planisok = scheduling_con();
	//计算成本
	if (planisok == 1)
	{
		cal_plan_cost();
	}
	return cost;
}

float ant::Exchange_order3()//局部策略3_随机选择两个tour两个订单进行交换
{//不涉及油库节点顺序调整,选择同隔舱订单,tour_a不等于tour_b,两个tour都需要交换检查
	int tour_a = (rand() % (CAR_COUNT));//生成0-车数-1之间的随机数，代表选择哪个回路
	int tour_b = tour_a;
	while (tour_a == tour_b)
	{//随机选择一个油罐节点（不能是油库）
		tour_b = (rand() % (CAR_COUNT));
	}
	int node_num_a = tour_info[tour_a].node_num;
	int node_num_b = tour_info[tour_b].node_num;
	int order_a = 0, order_b = -1;
	int ar_should = 0, br_should = 0;
	while (tour_sub[tour_a][order_a][0] == 0)
	{//随机选择一个油罐节点（不能是油库）
		order_a = (rand() % (tour_info[tour_a].node_num));
	}
	int order_air = ceil(tour_sub[tour_a][order_a][1] / COMP_Q);//交换订单需满足隔舱数
	int ok_order[33] = { 0 };//存放符合隔仓约束的节点位置
	int ok_order_count = 0;
	for (int b = 0; b < node_num_b; b++)
	{
		int this_air = ceil(tour_sub[tour_b][b][1] / COMP_Q);
		if (this_air == order_air)
		{//放入ok_order
			ok_order[ok_order_count] = b;
			ok_order_count = ok_order_count + 1;
		}
	}
	if (ok_order_count == 0)
	{//整个tour里没有和order_a一样隔舱的订单
		return cost;
	}
	else
	{
		order_b = ok_order[(rand() % (ok_order_count))];
	}
	//调整tour_sub
	int temp[3] = { 0 };
	temp[0] = tour_sub[tour_a][order_a][0];
	temp[1] = tour_sub[tour_a][order_a][1];
	temp[2] = tour_sub[tour_a][order_a][2];
	tour_sub[tour_a][order_a][0] = tour_sub[tour_b][order_b][0];
	tour_sub[tour_a][order_a][1] = tour_sub[tour_b][order_b][1];
	tour_sub[tour_a][order_a][2] = tour_sub[tour_b][order_b][2];
	tour_sub[tour_b][order_b][0] = temp[0];
	tour_sub[tour_b][order_b][1] = temp[1];
	tour_sub[tour_b][order_b][2] = temp[2];
	int gowrong = 0;
	for (int n = 0; n < 18; n++)
	{//检验tourb内有无不符合访问顺序约束的
		if (tour_sub[tour_b][n][0] < 0)
		{
			break;
		}
		if (tour_sub[tour_b][n][0] == tour_sub[tour_b][order_b][0])
		{
			if (tour_sub[tour_b][n][2] < ar_should)
			{
				gowrong = 1;
				break;
			}
			ar_should = tour_sub[tour_b][n][2];
		}
	}
	for (int n = 0; n < 18; n++)
	{//检验toura内有无不符合访问顺序约束的
		if (tour_sub[tour_a][n][0] < 0)
		{
			break;
		}
		if (tour_sub[tour_a][n][0] == tour_sub[tour_a][order_a][0])
		{
			if (tour_sub[tour_a][n][2] < br_should)
			{
				gowrong = 1;
				break;
			}
			br_should = tour_sub[tour_a][n][2];
		}
	}
	if (gowrong == 1)
	{
		return cost;
	}
	//初始化一些参数
	init_befor_calcul();
	//检查是否超出调度期约束.返回1，说明不违反，返回0说明违反。
	int planisok = scheduling_con();
	//计算成本
	if (planisok == 1)
	{
		cal_plan_cost();
	}
	return cost;
}

float ant::Short1() {//局部优化策略4_随机选择一个tour上缺货成本最大的订单，插入tour内其他位置，遍历位置求出成本最小解决方案
	//约束，插入订单不能等待，难点2，遍历位置,难点3，调整油库位置
	if (delay_cost > 0)
	{
		int w = 0;
	}

	int tour = (rand() % (CAR_COUNT));//生成0-车数-1之间的随机数，代表选择哪个回路
	int node_num = tour_info[tour].node_num;
	float delay_max = 0;
	int delay_max_node = -1;
	int tour_sub_copy[CAR_COUNT][33][3];
	int tour_sub_best[CAR_COUNT][33][3];
	float cost_best = cost;//改float
	//找到该tour上缺货成本最大的订单
	for (int y = 0; y < node_num; y++)
	{
		int thistank = tour_sub[tour][y][0];
		if (c_info[thistank].dtime_cost > delay_max)
		{
			delay_max_node = y;
			delay_max = c_info[thistank].dtime_cost;
		}
	}
	if (delay_max == 0)
	{//没有缺货的节点，随机选一个节点
		delay_max_node = 0;
		while (tour_sub[tour][delay_max_node][0] == 0)
		{//随机选择一个油罐节点（不能是油库）
			delay_max_node = (rand() % (tour_info[tour].node_num));
		}
	}

	//遍历寻找符合约束的最优插入点
	//先把tour_sub保留下来至tour_sub_copy
	::memcpy(tour_sub_copy, tour_sub, sizeof(tour_sub));//试一下行不；
	::memcpy(tour_sub_best, tour_sub, sizeof(tour_sub));//试一下行不；

	int order_ins[3];
	::memcpy(order_ins, tour_sub[tour][delay_max_node], sizeof(tour_sub[tour][delay_max_node]));//要插入的订单copy出来
	for (int ins = 1; ins < node_num; ins++)
	{//遍历可插入位置，每生成一种可能，就拿去验证
		//插入ins的位置，其余往后顺延
		//把这个订单从tour里取出来
		if (tour_sub[tour][ins][0] == 0 || ins == delay_max_node)
		{
			continue;
		}
		for (int rr = delay_max_node; rr < node_num + 1; rr++)
		{
			tour_sub[tour][rr][0] = tour_sub[tour][rr + 1][0];
			tour_sub[tour][rr][1] = tour_sub[tour][rr + 1][1];
			tour_sub[tour][rr][2] = tour_sub[tour][rr + 1][2];
		}

		int ar_should = 0;
		int gowrong = 0;
		for (int w = node_num; w > ins; w--)
		{//ins后的节点都向后串一位
			tour_sub[tour][w][0] = tour_sub[tour][w - 1][0];
			tour_sub[tour][w][1] = tour_sub[tour][w - 1][1];
			tour_sub[tour][w][2] = tour_sub[tour][w - 1][2];
		}
		//插入delay订单
		tour_sub[tour][ins][0] = order_ins[0];
		tour_sub[tour][ins][1] = order_ins[1];
		tour_sub[tour][ins][2] = order_ins[2];
		//检验tour内有无不符合访问顺序约束的
		for (int n = 0; n < 18; n++)
		{
			if (tour_sub[tour][n][0] < 0)
			{
				break;
			}
			if (tour_sub[tour][n][0] == tour_sub[tour][ins][0])
			{
				if (tour_sub[tour][n][2] < ar_should)
				{
					gowrong = 1;
					break;
				}
				ar_should = tour_sub[tour][n][2];
			}
		}
		//串位置！！！！
		make_gc_reasonable(tour);
		if (gowrong == 0)
		{//检验是否符合调度期约束
			init_befor_calcul();
			//检查是否超出调度期约束.返回1，说明不违反，返回0说明违反。
			int planisok = scheduling_con();
			int dont_wait = 0;
			if (planisok == 1)
			{//检查是否满足不等待约束
				float a_time = c_info[order_ins[0]].order[order_ins[2] - 1][3];
				float U_time = c_info[order_ins[0]].order[order_ins[2] - 1][4];
				if (a_time >= U_time)
				{//没有等待,通过重重考验，最后一步算成本
					dont_wait = 1;
					cal_plan_cost();
					if (cost < cost_best)
					{
						cost_best = cost;
						::memcpy(tour_sub_best, tour_sub, sizeof(tour_sub));//试一下行不；
					}
				}



				//cal_plan_cost();
			}
		}
		//一种插入方法尝试过了，回溯原始方案，再尝试
		::memcpy(tour_sub, tour_sub_copy, sizeof(tour_sub_copy));
		init_befor_calcul();
		int ww = scheduling_con();
	}
	//最后回溯最佳状态，返回
	::memcpy(tour_sub, tour_sub_best, sizeof(tour_sub_best));//
	init_befor_calcul();
	int planisok = scheduling_con();
	//计算成本
	if (planisok == 1)
	{
		cal_plan_cost();
	}
	else {
		int wrong = 1;

	}
	return cost;
}

float ant::Short2() {//局部优化策略5_随机选择一个tour上缺货成本最大的订单，再随机选择一个tour，与其中订单交换，遍历位置求出成本最小的交换方案
	//难点1，遍历插入位置,难点2，调整油库位置
	if (delay_cost > 0)
	{
		int w = 0;
	}
	//int tour = (rand() % (CAR_COUNT));//生成0-车数-1之间的随机数，代表选择哪个回路
	int tour_a = (rand() % (CAR_COUNT));//生成0-车数-1之间的随机数，代表选择哪个回路
	int tour_b = tour_a;
	while (tour_a == tour_b)
	{//随机选择一个tour
		tour_b = (rand() % (CAR_COUNT));
	}
	int node_num = tour_info[tour_a].node_num;
	float delay_max = 0;
	int delay_max_node = -1;
	int tour_sub_copy[CAR_COUNT][33][3];
	int tour_sub_best[CAR_COUNT][33][3];
	float cost_best = cost;//改float
	//找到该tour_a上缺货成本最大的订单
	for (int y = 0; y < node_num; y++)
	{
		int thistank = tour_sub[tour_a][y][0];
		if (c_info[thistank].dtime_cost > delay_max)
		{
			delay_max_node = y;
			delay_max = c_info[thistank].dtime_cost;
		}
	}
	if (delay_max == 0)
	{//没有缺货的节点，此算子结束
		return cost;
	}
	else
	{//遍历寻找符合约束的最优交换订单
		//先把tour_sub保留下来至tour_sub_copy
		::memcpy(tour_sub_copy, tour_sub, sizeof(tour_sub));//试一下行不；
		::memcpy(tour_sub_best, tour_sub, sizeof(tour_sub));//试一下行不；
		int order_ins[3];
		::memcpy(order_ins, tour_sub[tour_a][delay_max_node], sizeof(tour_sub[tour_a][delay_max_node]));//要交换的订单copy出来
		int temp[3] = { 0 };
		int order_air = ceil(order_ins[1] / COMP_Q);
		for (int ins = 1; ins < node_num; ins++)
		{//遍历可插入位置，每生成一种可能，就拿去验证
			//和tour_b中 ins位置的订单交换
			if (tour_sub[tour_b][ins][0] == 0 || ceil(tour_sub[tour_b][ins][1]) != order_air)
			{
				continue;
			}
			int ar_should = 0, br_should = 0;
			int gowrong = 0;
			temp[0] = tour_sub[tour_a][delay_max_node][0];
			temp[1] = tour_sub[tour_a][delay_max_node][1];
			temp[2] = tour_sub[tour_a][delay_max_node][2];
			tour_sub[tour_a][delay_max_node][0] = tour_sub[tour_b][ins][0];
			tour_sub[tour_a][delay_max_node][1] = tour_sub[tour_b][ins][1];
			tour_sub[tour_a][delay_max_node][2] = tour_sub[tour_b][ins][2];
			tour_sub[tour_b][ins][0] = temp[0];
			tour_sub[tour_b][ins][1] = temp[1];
			tour_sub[tour_b][ins][2] = temp[2];
			for (int n = 0; n < 18; n++)
			{//检验tourb内有无不符合访问顺序约束的
				if (tour_sub[tour_b][n][0] < 0)
				{
					break;
				}
				if (tour_sub[tour_b][n][0] == tour_sub[tour_b][ins][0])
				{
					if (tour_sub[tour_b][n][2] < ar_should)
					{
						gowrong = 1;
						break;
					}
					ar_should = tour_sub[tour_b][n][2];
				}
			}
			for (int n = 0; n < 18; n++)
			{//检验toura内有无不符合访问顺序约束的
				if (tour_sub[tour_a][n][0] < 0)
				{
					break;
				}
				if (tour_sub[tour_a][n][0] == tour_sub[tour_a][delay_max_node][0])
				{
					if (tour_sub[tour_a][n][2] < br_should)
					{
						gowrong = 1;
						break;
					}
					br_should = tour_sub[tour_a][n][2];
				}
			}
			if (gowrong == 0)
			{//检验是否符合调度期约束
				init_befor_calcul();
				//检查是否超出调度期约束.返回1，说明不违反，返回0说明违反。

				int planisok = scheduling_con();
				int dont_wait = 0;
				if (planisok == 1)
				{//检查是否满足不等待约束
					float a_time = c_info[order_ins[0]].order[order_ins[2] - 1][3];
					float U_time = c_info[order_ins[0]].order[order_ins[2] - 1][4];
					if (a_time >= U_time)
					{//没有等待,通过重重考验，最后一步算成本
						dont_wait = 1;
						cal_plan_cost();
						if (cost < cost_best)
						{
							cost_best = cost;
							::memcpy(tour_sub_best, tour_sub, sizeof(tour_sub));//试一下行不；
						}
					}
					//cal_plan_cost();
				}
			}
			//一种插入方法尝试过了，回溯原始方案，再尝试
			::memcpy(tour_sub, tour_sub_copy, sizeof(tour_sub_copy));
			init_befor_calcul();
			int ww = scheduling_con();
		}
	}
	//最后回溯最方案，返回
	::memcpy(tour_sub, tour_sub_best, sizeof(tour_sub_best));
	init_befor_calcul();
	int planisok = scheduling_con();
	//计算成本
	if (planisok == 1)
	{
		cal_plan_cost();
	}
	else {
		int wrong = 1;

	}
	return cost;
}

float ant::Station() {//局部优化策略6_找到服务时刻相差最小的属于同加油站的订单对，相差时间由小到大排列，依次插入优化
	//难点1，对所有订单对按服务时间排序；难点2，遍历4种插入方式,难点3，调整油库位置
	//--------------找到按照相差时间升序排列的数组---------------
	//统计各个加油站一共有多少订单

	float cost_best = cost;
	int s_order_num[N_STATION_COUNT] = { 0 };//加油站的订单数量
	for (int c = 1; c < N_NODE_COUNT; c++)
	{
		s_order_num[c_info[c].station - 1] = s_order_num[c_info[c].station - 1] + c_info[c].r_now;
	}
	//依次把属于各加油站的订单放入数组,一个加油站一个加油站的处理
	float*** sta_order = new float** [N_STATION_COUNT];//[tank][q][r][ser_time]////order:[r][qir][air][a_time][Uir][Sir][d_time][d_cost];
	for (int s = 0; s < N_STATION_COUNT; s++)
	{
		sta_order[s] = new float* [s_order_num[s]];
		int pos = 0;
		for (int c = 1; c < N_NODE_COUNT; c++)
		{
			if (c_info[c].station == s + 1)
			{//此油罐属于s加油站
				for (int r = 0; r < c_info[c].r_now; r++)
				{
					if (s == 3 && pos == 1)
					{
						int bbbb = 1;
					}
					sta_order[s][pos] = new float[4];
					sta_order[s][pos][0] = c_info[c].id;//tank
					sta_order[s][pos][1] = c_info[c].order[r][1];//q
					sta_order[s][pos][2] = c_info[c].order[r][0];//r
					sta_order[s][pos][3] = (c_info[c].order[r][4] > c_info[c].order[r][3]) ? c_info[c].order[r][4] : c_info[c].order[r][3];//(U_time_next > a_time_next) ? U_time_next : a_time_next
					pos = pos + 1;
				}
			}
		}//对此加油站的所有订单冒泡按照ser_time排序
		float exchange[4];
		for (int i = 1; i <= s_order_num[s] - 1; i++)
		{
			for (int j = 1; j <= s_order_num[s] - i; j++)
			{
				if (sta_order[s][j - 1][3] > sta_order[s][j][3])
				{
					exchange[0] = sta_order[s][j - 1][0];
					exchange[1] = sta_order[s][j - 1][1];
					exchange[2] = sta_order[s][j - 1][2];
					exchange[3] = sta_order[s][j - 1][3];
					sta_order[s][j - 1][0] = sta_order[s][j][0];
					sta_order[s][j - 1][1] = sta_order[s][j][1];
					sta_order[s][j - 1][2] = sta_order[s][j][2];
					sta_order[s][j - 1][3] = sta_order[s][j][3];
					sta_order[s][j][0] = exchange[0];
					sta_order[s][j][1] = exchange[1];
					sta_order[s][j][2] = exchange[2];
					sta_order[s][j][3] = exchange[3];
				}
			}
		}
	}
	//将订单与同加油站其后的订单比较，拿出ser_time之差小于0.5的,存入o_pair
	//获得一共有多少这样的订单
	float** o_pair = new float* [500];//[station][在sta_order里order1的位置][order2的位置][ser_time相差值]//可能会有问题
	int o_pair_num_sum = 0;
	//int o_pair_num[N_STATION_COUNT] = { 0 };
	for (int s = 0; s < N_STATION_COUNT; s++)
	{
		for (int order = 0; order < s_order_num[s]; order++)
		{
			for (int y = order + 1; y < s_order_num[s]; y++)
			{
				if ((sta_order[s][order][3] + 0.5 >= sta_order[s][y][3]) && (sta_order[s][order][0] != sta_order[s][y][0]))//相差小于0.5，且不属于同油罐
				{
					o_pair[o_pair_num_sum] = new float[4];
					o_pair[o_pair_num_sum][0] = s + 1;
					o_pair[o_pair_num_sum][1] = order;
					o_pair[o_pair_num_sum][2] = y;
					o_pair[o_pair_num_sum][3] = sta_order[s][y][3] - sta_order[s][order][3];
					o_pair_num_sum = o_pair_num_sum + 1;
				}
			}
		}
	}
	//再次冒泡排序，以ser_time相差值升序排序
	float exchange[4];
	for (int i = 1; i <= o_pair_num_sum - 1; i++)
	{
		for (int j = 1; j <= o_pair_num_sum - i; j++)
		{
			if (o_pair[j - 1][3] > o_pair[j][3])
			{
				exchange[0] = o_pair[j - 1][0];
				exchange[1] = o_pair[j - 1][1];
				exchange[2] = o_pair[j - 1][2];
				exchange[3] = o_pair[j - 1][3];
				o_pair[j - 1][0] = o_pair[j][0];
				o_pair[j - 1][1] = o_pair[j][1];
				o_pair[j - 1][2] = o_pair[j][2];
				o_pair[j - 1][3] = o_pair[j][3];
				o_pair[j][0] = exchange[0];
				o_pair[j][1] = exchange[1];
				o_pair[j][2] = exchange[2];
				o_pair[j][3] = exchange[3];
			}
		}
	}
	//拿出一对订单对，找到他们在tour_sub中的位置
	if (o_pair_num_sum > 1) {
		int ooo = 1;
	}
	int gettingbetter = 0;
	int tour_sub_copy[CAR_COUNT][33][3];
	int tour_sub_best[CAR_COUNT][33][3];
	::memcpy(tour_sub_copy, tour_sub, sizeof(tour_sub));
	::memcpy(tour_sub_best, tour_sub, sizeof(tour_sub));
	for (int pair = 0; pair < o_pair_num_sum; pair++)
	{
		int tour_a = -1, tour_b = -1, order_a = -1, order_b = -1;
		int o_a_station = int(o_pair[pair][0]);
		int o_a_pos = int(o_pair[pair][1]);
		int o_a_tank = int(sta_order[o_a_station - 1][o_a_pos][0]);
		int o_a_r = int(sta_order[o_a_station - 1][o_a_pos][2]);
		int o_b_pos = int(o_pair[pair][2]);
		int o_b_tank = int(sta_order[o_a_station - 1][o_b_pos][0]);
		int o_b_r = int(sta_order[o_a_station - 1][o_b_pos][2]);
		for (int tour_x = 0; tour_x < CAR_COUNT; tour_x++)
		{
			for (int n = 0; n < tour_info[tour_x].node_num; n++)
			{

				if ((tour_sub[tour_x][n][0] == o_a_tank) && (tour_sub[tour_x][n][2] == o_a_r))
				{
					tour_a = tour_x;
					order_a = n;
				}
				if ((tour_sub[tour_x][n][0] == o_b_tank) && (tour_sub[tour_x][n][2] == o_b_r))
				{
					tour_b = tour_x;
					order_b = n;
				}
			}
		}
		if (order_a < 0 || order_b < 0)
		{
			int wwww = 0;
		}
		//四种操作，开始
		//操作1：a插入b前
		int order_ex = order_b - 1;
		int tour_ex = tour_b;
		if ((tour_a == tour_ex && order_a == order_ex) || tour_info[tour_a].node_num <= 3) {//b前面的订单就是a,所以没必要换或者a的tour只有它一个订单了
			int next_oper = 1;//直接下一个操作
		}
		else
		{//ins=order_b
			int succ = insert(order_b, tour_b, order_a, tour_a);//ins是插入位置，tour_ins是被插入tour，out是订单原有位置，tour_out是订单原在tour
			//检验是否符合调度期约束

			if (succ == 1)
			{
				init_befor_calcul();
				//检查是否超出调度期约束.返回1，说明不违反，返回0说明违反。
				int planisok = scheduling_con();
				if (planisok == 1)
				{//检查是否满足不等待约束
					cal_plan_cost();
					if (cost < cost_best)
					{
						gettingbetter = 1;
						cost_best = cost;
						::memcpy(tour_sub_best, tour_sub, sizeof(tour_sub));
					}
				}
			}
		}
		::memcpy(tour_sub, tour_sub_copy, sizeof(tour_sub_copy));////回溯
		init_befor_calcul();
		int planisok = scheduling_con();
		//操作2：a插入b后
		order_ex = order_b + 1;
		tour_ex = tour_b;
		if ((tour_a == tour_ex && order_a == order_ex) || tour_info[tour_a].node_num <= 3)
		{//b前面的订单就是a,所以没必要换
			int next_oper = 1;//直接下一个操作
		}
		else
		{//ins=order_b
			int succ = insert(order_b + 1, tour_b, order_a, tour_a);//ins是插入位置，tour_ins是被插入tour，out是订单原有位置，tour_out是订单原在tour
			//检验是否符合调度期约束


			if (succ == 1)
			{
				init_befor_calcul();
				//检查是否超出调度期约束.返回1，说明不违反，返回0说明违反。
				int planisok = scheduling_con();
				if (planisok == 1)
				{//检查是否满足不等待约束
					cal_plan_cost();
					if (cost < cost_best)
					{
						gettingbetter = 1;
						cost_best = cost;
						::memcpy(tour_sub_best, tour_sub, sizeof(tour_sub));//试一下行不；
					}
				}
			}
		}
		::memcpy(tour_sub, tour_sub_copy, sizeof(tour_sub_copy));////回溯
		init_befor_calcul();
		int ww = scheduling_con();
		//操作3：b插入a前
		order_ex = order_a - 1;
		tour_ex = tour_a;
		if ((tour_b == tour_ex && order_b == order_ex) || tour_info[tour_b].node_num <= 3)
		{//b前面的订单就是a,所以没必要换
			int next_oper = 1;//直接下一个操作
		}
		else
		{//ins=order_b
			int succ = insert(order_a, tour_a, order_b, tour_b);//ins是插入位置，tour_ins是被插入tour，out是订单原有位置，tour_out是订单原在tour
			//检验是否符合调度期约束

			if (succ == 1)
			{
				init_befor_calcul();
				//检查是否超出调度期约束.返回1，说明不违反，返回0说明违反。
				int planisok = scheduling_con();
				if (planisok == 1)
				{//检查是否满足不等待约束
					cal_plan_cost();
					if (cost < cost_best)
					{
						gettingbetter = 1;
						cost_best = cost;
						::memcpy(tour_sub_best, tour_sub, sizeof(tour_sub));//试一下行不；
					}
				}
			}
		}
		::memcpy(tour_sub, tour_sub_copy, sizeof(tour_sub_copy));////回溯
		init_befor_calcul();
		ww = scheduling_con();
		//操作4：b插入a后
		order_ex = order_a + 1;
		tour_ex = tour_a;
		if ((tour_b == tour_ex && order_b == order_ex) || tour_info[tour_b].node_num <= 3)
		{//b前面的订单就是a,所以没必要换
			int next_oper = 1;//直接下一个操作
		}
		else
		{//ins=order_b
			int succ = insert(order_a + 1, tour_a, order_b, tour_b);//ins是插入位置，tour_ins是被插入tour，out是订单原有位置，tour_out是订单原在tour
			//检验是否符合调度期约束

			if (succ == 1)
			{
				init_befor_calcul();
				//检查是否超出调度期约束.返回1，说明不违反，返回0说明违反。
				int planisok = scheduling_con();
				if (planisok == 1)
				{//检查是否满足不等待约束
					cal_plan_cost();
					if (cost < cost_best)
					{
						gettingbetter = 1;
						cost_best = cost;
						::memcpy(tour_sub_best, tour_sub, sizeof(tour_sub));//试一下行不；
					}
				}
			}
		}
		::memcpy(tour_sub, tour_sub_copy, sizeof(tour_sub_copy));////回溯
		init_befor_calcul();
		ww = scheduling_con();
		if (gettingbetter > 0)
		{//已经有所改进
			break;
		}
	}
	::memcpy(tour_sub, tour_sub_best, sizeof(tour_sub_best));
	init_befor_calcul();
	int planisok = scheduling_con();
	//计算成本
	if (planisok == 1)
	{
		cal_plan_cost();
	}
	else {
		int wrong = 1;
	}
	return cost;
}

float ant::Unneed_forshort() {//局部优化策略7_随机选择一个tour，随机选择一个其中的非必要配送订单，将此订单的配送量转移到其他下期初缺货成本大油罐上
	//找到非必要配送订单，此类订单的S_time要大于28,28=24+4(下期初)
	int tour = (rand() % (CAR_COUNT));//生成0-车数-1之间的随机数，代表选择哪个回路
	int node_num = tour_info[tour].node_num;
	int unneed[12][4];//放本tour中的非必要配送订单[tank][q][r][pos in tour_sub[tour]]
	int unneed_num = 0;
	int nd_num = 0;
	float next_delay[N_NODE_COUNT][2] = { -1 };//放有下期初缺货的油罐
	//get unneed[]
	for (int n = 0; n < tour_info[tour].node_num; n++)
	{
		if (tour_info[tour].S_time[n] > 28)
		{
			unneed[unneed_num][0] = tour_sub[tour][n][0];
			unneed[unneed_num][1] = tour_sub[tour][n][1];
			unneed[unneed_num][2] = tour_sub[tour][n][2];
			unneed[unneed_num][3] = n;
			unneed_num = unneed_num + 1;
		}
	}
	for (int c = 1; c < N_NODE_COUNT; c++)
	{//get next_delay[]
		if (c_info[c].next_d_time_cost > 0)
		{
			next_delay[nd_num][0] = c;
			next_delay[nd_num][1] = c_info[c].next_d_time_cost;
			nd_num = nd_num + 1;
		}
	}
	if (unneed_num == 0 || nd_num == 0)
	{//没有下期缺货，或者没有非必要配送
		return cost;
	}
	else
	{//随机选择一个非必要配送订单
		int un = (rand() % (unneed_num));//选择哪个非必要订单
		int node_a = unneed[un][3];
		int tour_a = tour;
		//冒泡排序，下期初缺货成本最高的订单
		float exchange[2];
		for (int i = 1; i <= nd_num - 1; i++)
		{
			for (int j = 1; j <= nd_num - i; j++)
			{
				if (next_delay[j - 1][1] < next_delay[j][1])
				{
					exchange[0] = next_delay[j - 1][0];
					exchange[1] = next_delay[j - 1][1];
					next_delay[j - 1][0] = next_delay[j][0];
					next_delay[j - 1][1] = next_delay[j][1];
					next_delay[j][0] = exchange[0];
					next_delay[j][1] = exchange[1];
				}
			}
		}
		//-------订单转移----------
		int gettingbetter = 0;
		float cost_best = cost;
		int tour_sub_copy[CAR_COUNT][33][3];
		int tour_sub_best[CAR_COUNT][33][3];
		::memcpy(tour_sub_copy, tour_sub, sizeof(tour_sub));
		::memcpy(tour_sub_best, tour_sub, sizeof(tour_sub));
		for (int g = 0; g < nd_num; g++)
		{//
			int tran = next_delay[g][0];
			int r_should = 0;
			int gowrong = 0;
			tour_sub[tour][node_a][0] = tran;
			tour_sub[tour][node_a][2] = c_info[tran].r_now + 1;
			for (int n = 0; n < 34; n++)
			{
				if (tour_sub[tour][n][0] < 0)
				{
					break;
				}
				if (tour_sub[tour][n][0] == tour_sub[tour][tran][0])
				{
					if (tour_sub[tour][n][2] < r_should)
					{
						gowrong = 1;
						break;
					}
					r_should = tour_sub[tour][n][2];
				}
			}
			if (gowrong == 0)
			{
				//make_gc_reasonable(tour);
				if (g == 23) {
					int www = 1;
				}
				init_befor_calcul();
				//检查是否超出调度期约束.返回1，说明不违反，返回0说明违反。
				int planisok = scheduling_con();
				if (planisok == 1)
				{//检查是否满足不等待约束
					cal_plan_cost();
					if (cost < cost_best)
					{
						gettingbetter = 1;
						cost_best = cost;
						::memcpy(tour_sub_best, tour_sub, sizeof(tour_sub));
					}
				}
			}
			//回溯
			::memcpy(tour_sub, tour_sub_copy, sizeof(tour_sub_copy));
			init_befor_calcul();
			int ww = scheduling_con();
		}
		::memcpy(tour_sub, tour_sub_best, sizeof(tour_sub_best));
		init_befor_calcul();
		int planisok = scheduling_con();
		//计算成本
		if (planisok == 1)
		{
			cal_plan_cost();
		}
		else {
			int wrong = 1;
		}
		return cost;
	}
}

float ant::Unneed_exchange() {//局部优化策略8_随机选择两个tour，将其中的非必要配送订单找出来，遍历交换。找到成本最小交换方式
	int tour_a = (rand() % (CAR_COUNT));//生成0-车数-1之间的随机数，代表选择哪个回路
	int tour_b = tour_a;
	while (tour_a == tour_b)
	{
		tour_b = (rand() % (CAR_COUNT));
	}
	int unneed_a[12][4];//放tour_a中的非必要配送订单[tank][q][r][pos in tour_sub[tour]]
	int unneed_num_a = 0;
	int unneed_b[12][4];//放tour_b中的非必要配送订单[tank][q][r][pos in tour_sub[tour]]
	int unneed_num_b = 0;
	//get unneed_a[]
	for (int n = 0; n < tour_info[tour_a].node_num; n++)
	{
		if (tour_info[tour_a].S_time[n] > 28)
		{
			unneed_a[unneed_num_a][0] = tour_sub[tour_a][n][0];
			unneed_a[unneed_num_a][1] = tour_sub[tour_a][n][1];
			unneed_a[unneed_num_a][2] = tour_sub[tour_a][n][2];
			unneed_a[unneed_num_a][3] = n;
			unneed_num_a = unneed_num_a + 1;
		}

	}
	for (int n = 0; n < tour_info[tour_b].node_num; n++)
	{
		if (tour_info[tour_b].S_time[n] > 28)
		{
			unneed_b[unneed_num_b][0] = tour_sub[tour_b][n][0];
			unneed_b[unneed_num_b][1] = tour_sub[tour_b][n][1];
			unneed_b[unneed_num_b][2] = tour_sub[tour_b][n][2];
			unneed_b[unneed_num_b][3] = n;
			unneed_num_b = unneed_num_b + 1;
		}
	}
	int gettingbetter = 0;
	float cost_best = cost;
	int tour_sub_copy[CAR_COUNT][33][3];
	int tour_sub_best[CAR_COUNT][33][3];
	::memcpy(tour_sub_copy, tour_sub, sizeof(tour_sub));
	::memcpy(tour_sub_best, tour_sub, sizeof(tour_sub));
	int order_a = -1, order_b = -1;
	for (int a = 0; a < unneed_num_a; a++)
	{//
		order_a = unneed_a[a][3];
		for (int b = 0; b < unneed_num_b; b++)
		{
			order_b = unneed_b[b][3];
			int temp[3] = { 0 };
			temp[0] = tour_sub[tour_a][order_a][0];
			temp[1] = tour_sub[tour_a][order_a][1];
			temp[2] = tour_sub[tour_a][order_a][2];
			tour_sub[tour_a][order_a][0] = tour_sub[tour_b][order_b][0];
			tour_sub[tour_a][order_a][1] = tour_sub[tour_b][order_b][1];
			tour_sub[tour_a][order_a][2] = tour_sub[tour_b][order_b][2];
			tour_sub[tour_b][order_b][0] = temp[0];
			tour_sub[tour_b][order_b][1] = temp[1];
			tour_sub[tour_b][order_b][2] = temp[2];
			int gowrong = 0;
			int ar_should = 0, br_should = 0;
			for (int n = 0; n < 34; n++)
			{//检验tour_a&tour_b内有无不符合访问顺序约束的
				if (tour_sub[tour_b][n][0] < 0)
				{
					break;
				}
				if (tour_sub[tour_b][n][0] == tour_sub[tour_b][order_b][0])
				{
					if (tour_sub[tour_b][n][2] < ar_should)
					{
						gowrong = 1;
						break;
					}
					ar_should = tour_sub[tour_b][n][2];
				}
			}
			for (int n = 0; n < 34; n++)
			{
				if (tour_sub[tour_a][n][0] < 0)
				{
					break;
				}
				if (tour_sub[tour_a][n][0] == tour_sub[tour_a][order_a][0])
				{
					if (tour_sub[tour_a][n][2] < br_should)
					{
						gowrong = 1;
						break;
					}
					br_should = tour_sub[tour_a][n][2];
				}
			}
			if (gowrong == 0)
			{
				make_gc_reasonable(tour_a);
				make_gc_reasonable(tour_b);
				init_befor_calcul();
				//检查是否超出调度期约束.返回1，说明不违反，返回0说明违反。
				int planisok = scheduling_con();
				//计算成本
				if (planisok == 1)
				{
					cal_plan_cost();
					if (cost < cost_best)
					{
						gettingbetter = 1;
						cost_best = cost;
						::memcpy(tour_sub_best, tour_sub, sizeof(tour_sub));
					}
				}
			}
			//回溯
			::memcpy(tour_sub, tour_sub_copy, sizeof(tour_sub_copy));
			init_befor_calcul();
			int ww = scheduling_con();
		}
	}
	::memcpy(tour_sub, tour_sub_best, sizeof(tour_sub_best));//
	init_befor_calcul();
	int planisok = scheduling_con();
	//计算成本
	if (planisok == 1)
	{
		cal_plan_cost();
	}
	else {
		int wrong = 1;
	}
	return cost;
}

float ant::Unneed_forH() {//局部优化策略9_随机选择一个/多个非必要配送订单，取消后，按照装载率由低到高分配给其他订单
	int tour = (rand() % (CAR_COUNT));//生成0-车数-1之间的随机数，代表选择哪个回路
	int node_num = tour_info[tour].node_num;
	int unneed[33][4];//放本tour中的非必要配送订单[tank][q][r][pos in tour_sub[tour]]
	int unneed_num = 0;
	int zz_left_all = 0;
	float zzl[CAR_COUNT][33][3] = { 0 };//放各订单的装载率与装载量[tour][pos]|||[tank][装载率][可再装量]
	float zzl_copy[CAR_COUNT][33][3];
	int gettingbetter = 0;
	float cost_best = cost;
	int Qall_best = Qall;
	int tour_sub_copy[CAR_COUNT][33][3];
	int tour_sub_best[CAR_COUNT][33][3];
	int Qall_copy = Qall;
	::memcpy(tour_sub_copy, tour_sub, sizeof(tour_sub));
	::memcpy(tour_sub_best, tour_sub, sizeof(tour_sub));

	//get unneed[]
	for (int n = 0; n < tour_info[tour].node_num; n++)
	{
		if (tour_info[tour].S_time[n] > 28)
		{
			unneed[unneed_num][0] = tour_sub[tour][n][0];
			unneed[unneed_num][1] = tour_sub[tour][n][1];
			unneed[unneed_num][2] = tour_sub[tour][n][2];
			unneed[unneed_num][3] = n;
			unneed_num = unneed_num + 1;
		}
	}
	for (int t = 0; t < CAR_COUNT; t++)
	{//get zzl[]
		for (int n = 0; n < tour_info[t].node_num; n++)
		{
			if (tour_sub[t][n][0] > 0 && tour_info[t].S_time[n] < 28)
			{
				float zzl_rate = tour_sub[t][n][1] / (ceil(tour_sub[t][n][1] / COMP_Q) * COMP_Q);//装载率
				int zz_left = (ceil(tour_sub[t][n][1] / COMP_Q) * COMP_Q) - tour_sub[t][n][1];//余量
				zzl[t][n][0] = tour_sub[t][n][0];
				if (zzl_rate == 0)
				{
					zzl[t][n][1] = 1;
				}
				if (zzl_rate < 0.95)    //装载率
				{
					int www = 1;
				}
				else
				{
					zzl[t][n][1] = zzl_rate;
				}

				zzl[t][n][2] = zz_left;
				zz_left_all = zz_left_all + zz_left;
			}

		}
	}
	int zz_left_all_copy = zz_left_all;
	::memcpy(zzl_copy, zzl, sizeof(zzl));
	if (unneed_num == 0)
	{//没有非必要配送
		return cost;
	}
	else
	{//随机选择一个非必要配送订单
		int q_out = 0;

		while (unneed_num > 0 && tour_info[tour].node_num > 3)
		{
			int un = (rand() % (unneed_num));//选择哪个非必要订单
			int node_out = unneed[un][3];
			int tour_out = tour;
			int q_in_all = 0;
			q_out = tour_sub[tour_out][node_out][1];
			if (Qall - q_out + zz_left_all < HMIN)
			{//减了之后补不回来，那就不减了
				//Qall = Qall - q_out;
				break;
			}
			if (Qall - q_out < HMIN)
			{//减了之后还需要补，不需要补的话，直接删掉
				int q_in_min = HMIN - Qall + q_out;//最少需要补这些
				//遍历找到装载率最小订单
				int node_in = -1, tour_in = -1;
				while (q_in_all == 0 || Qall < HMIN)
				{//还没补齐
					if (q_in_all == 0)
					{
						Qall = Qall - q_out;
					}

					node_in = -1;
					tour_in = -1;
					float min_zzl = 2;
					for (int t = 0; t < CAR_COUNT; t++)
					{
						for (int n = 0; n < tour_info[t].node_num; n++)
						{
							if ((zzl[t][n][1] < 1) && (zzl[t][n][0] > 0) && (zzl[t][n][1] < 1) && (zzl[t][n][1] < min_zzl) && ((t != tour_out) || (n != node_out)))
							{
								node_in = n;
								tour_in = t;
								q_out = zzl[t][n][2];
								min_zzl = zzl[tour_in][node_in][1];
								if (q_out == 0)
								{
									int www = 0;
								}
							}
						}
					}
					//改变装载量
					int q_in_this = (q_in_min < q_out) ? q_in_min : q_out;//这个订单要加的量
					tour_sub[tour_in][node_in][1] = tour_sub[tour_in][node_in][1] + q_in_this;
					if (tour_sub[tour_in][node_in][1] > 30000 || node_in == -1)
					{
						int www = 1;
					}
					Qall = Qall + q_in_this;
					zzl[tour_in][node_in][1] = (ceil(tour_sub[tour_in][node_in][1] / COMP_Q) * COMP_Q) - tour_sub[tour_in][node_in][1];
					if (zzl[tour_in][node_in][1] == 0)
					{
						zzl[tour_in][node_in][1] = 1;
					}
					zzl[tour_in][node_in][2] = zzl[tour_in][node_in][2] - q_in_this;
					q_in_all = q_in_all + q_in_this;
					zz_left_all = zz_left_all - q_in_this;
					q_in_min = q_in_min - q_in_this;
				}
			}
			else
			{
				Qall = Qall - q_out;
			}
			//从tour中删除这个订单
			for (int rr = node_out; rr < tour_info[tour_out].node_num + 1; rr++)
			{
				tour_sub[tour][rr][0] = tour_sub[tour][rr + 1][0];
				tour_sub[tour][rr][1] = tour_sub[tour][rr + 1][1];
				tour_sub[tour][rr][2] = tour_sub[tour][rr + 1][2];
			}
			//从unneed中删除这个订单
			for (int rr = un; rr < unneed_num + 1; rr++)
			{
				unneed[rr][0] = unneed[rr + 1][0];
				unneed[rr][1] = unneed[rr + 1][1];
				unneed[rr][2] = unneed[rr + 1][2];
				unneed[rr][3] = unneed[rr + 1][3];
			}
			unneed_num = unneed_num - 1;
			make_gc_reasonable(tour_out);
			init_befor_calcul();
			//检查是否超出调度期约束.返回1，说明不违反，返回0说明违反。
			int planisok = scheduling_con();
			if (planisok == 1)
			{//检查是否满足不等待约束
				cal_plan_cost();
				if (cost < cost_best)
				{
					gettingbetter = 1;
					cost_best = cost;
					Qall_best = Qall;
					::memcpy(tour_sub_best, tour_sub, sizeof(tour_sub));
				}
			}

			//回溯
			Qall = Qall_copy;
			::memcpy(tour_sub, tour_sub_copy, sizeof(tour_sub_copy));
			init_befor_calcul();
			int ww = scheduling_con();
			::memcpy(zzl, zzl_copy, sizeof(zzl_copy));
			zz_left_all = zz_left_all_copy;
		}
		Qall = Qall_best;
		::memcpy(tour_sub, tour_sub_best, sizeof(tour_sub_best));//
		init_befor_calcul();
		int planisok = scheduling_con();
		//计算成本
		if (planisok == 1)
		{
			cal_plan_cost();
		}
		else {
			int wrong = 1;
		}
		return cost;
	}
}

inline void ant::init_befor_calcul() {
	//ant 初始化
	//cost = 0;
	tran_cost = 0;
	unload_cost = 0;
	delay_cost = 0;
	next_delay_cost = 0;
	//cur_tank_NO = 0;
	//cur_car_NO = 0;
	//cur_car_r = 0;
	//cur_time = 0;
	//c_info初始化

	for (int y = 0; y < N_NODE_COUNT; y++)
	{
		c_info[y].recent_left_time = 0;
		c_info[y].S_time = c_info[y].ini_stock / c_info[y].sell_v;
		int S_time = c_info[y].ini_stock / c_info[y].sell_v;
		c_info[y].dtime_all = 0;
		c_info[y].dtime_cost = 0;
		c_info[y].next_dtime = 0;
		c_info[y].next_d_time_cost = 0;
		c_info[y].send_all = 0;
		c_info[y].r_now = 0;
		for (int z = 0; z < 10; z++)
		{
			if (c_info[y].order[z][0] > 0)
			{
				c_info[y].order[z][3] = 0;
				c_info[y].order[z][4] = 0;
				c_info[y].order[z][5] = 0;
				c_info[y].order[z][6] = 0;
				c_info[y].order[z][7] = 0;
			}
		}
	}
	//tour_info初始化
	for (int y = 0; y < CAR_COUNT; y++)
	{
		tour_info[y].cur_node_id = 0;
		tour_info[y].cur_q = 0;
		tour_info[y].cur_time = 0.5;
		tour_info[y].node_num = 1;
		tour_info[y].lap = 0;
		for (int t = 0; t < 18; t++)
		{
			tour_info[y].node[t] = -1;
			if (t == 0)
			{
				tour_info[y].node[t] = 0;
			}
			tour_info[y].gc_need[t] = 0;						//保存每次访问所用隔舱
			tour_info[y].r[t] = -1;
			tour_info[y].q[t] = 0;
			tour_info[y].a_time[t] = 0;
			tour_info[y].U_time[t] = 0;
			tour_info[y].S_time[t] = 0;
			tour_info[y].d_time[t] = 0;
			tour_info[y].d_cost[t] = 0;
			tour_info[y].ser_time[t] = 0;
		}
	}

}

inline int ant::scheduling_con() {
	//以tour_sub的调度方案为根，计算新生成的调度方案是否超出调度期约束与趟数约束
	//返回1，说明不违反，返回0说明违反。
	int planisok = 1;
	int visit_num[N_NODE_COUNT] = { 0 };//记录每个tank被补货的次数
	int tour_fin[CAR_COUNT] = { 0 };//记录走完的tour，0是没走了，1是走完
	int tour_step[CAR_COUNT];
	int tour_fin_num = 0;//记录结束走完的tour个数
	int stuck = 0;
	for (int c = 0; c < CAR_COUNT; c++)
	{
		tour_step[c] = 1;//记录每个tour走了几步
	}
	while (tour_fin_num < CAR_COUNT)
	{
		for (int car = 0; car < CAR_COUNT; car++)
		{
			if (tour_fin[car] == 0)
			{//此车还没走完
				int tank_to_go = tour_sub[car][tour_step[car]][0];
				int r_to_go = tour_sub[car][tour_step[car]][2];
				int q_to_go = tour_sub[car][tour_step[car]][1];
				int tank_now = tour_sub[car][tour_step[car] - 1][0];
 				if ((r_to_go == visit_num[tank_to_go] + 1) || (c_info[tank_to_go].t == 'd'))
				{//这个订单可以送！这一步可以走，改变状态
					if (tank_to_go == 10)
					{
						int www = 0;
					}
					tour_step[car] = tour_step[car] + 1;
					if (tour_step[car] > 33)
					{//超过4趟
						return 0;
					}
					visit_num[tank_to_go] = visit_num[tank_to_go] + 1;
					stuck = 0;
					if (c_info[tank_to_go].t == 't')//下一节点是油罐节点
					{
						if (tank_to_go == 124)
						{
							int bbbb = 0;
						}
						//容纳点是本身计算容纳点和上次离开节点时间取较大的那个
						float U_time_next = ((((c_info[tank_to_go].ini_stock - c_info[tank_to_go].tank_cap + c_info[tank_to_go].send_all + q_to_go) / c_info[tank_to_go].sell_v) + c_info[tank_to_go].dtime_all) > c_info[tank_to_go].recent_left_time) ? ((c_info[tank_to_go].ini_stock - c_info[tank_to_go].tank_cap + c_info[tank_to_go].send_all + q_to_go) / c_info[tank_to_go].sell_v) + c_info[tank_to_go].dtime_all : c_info[tank_to_go].recent_left_time;//容纳点
						float S_time_next = ((c_info[tank_to_go].ini_stock + c_info[tank_to_go].send_all) / c_info[tank_to_go].sell_v) + c_info[tank_to_go].dtime_all;//Oir的缺货点,送完这次前的缺货点
						float a_time_next = tour_info[car].cur_time + dis[tank_now][tank_to_go] / CAR_V;//到达时刻
						float l_time_next = q_to_go / SERVICE_RATE;//卸油时长
						float w_time_next = (0 > U_time_next - a_time_next) ? 0 : U_time_next - a_time_next;//等待时长
						float ser_time_next = a_time_next + w_time_next;
						float d_time_next = (0 > ser_time_next - S_time_next) ? 0 : ser_time_next - S_time_next;//迟到时长
						float left_time = a_time_next + l_time_next + w_time_next;
						//更新油罐c_info信息
						c_info[tank_to_go].send_all = c_info[tank_to_go].send_all + q_to_go;
						c_info[tank_to_go].S_time = ((c_info[tank_to_go].ini_stock + c_info[tank_to_go].send_all) / c_info[tank_to_go].sell_v) + c_info[tank_to_go].dtime_all;//配送完这一次后的缺货点
						c_info[tank_to_go].r_now = c_info[tank_to_go].r_now + 1;
						c_info[tank_to_go].dtime_all = c_info[tank_to_go].dtime_all + d_time_next;
						//c_info[tank_to_go].order[c_info[tank_to_go].r_now - 1][0] = c_info[tank_to_go].r_now;
						c_info[tank_to_go].recent_left_time = left_time;
						int r_next = c_info[tank_to_go].r_now;
						c_info[tank_to_go].order[r_next - 1][0] = r_next;//r
						c_info[tank_to_go].order[r_next - 1][1] = q_to_go;//q
						c_info[tank_to_go].order[r_next - 1][2] = (ceil(q_to_go / COMP_Q));//所用隔舱
						c_info[tank_to_go].order[r_next - 1][3] = a_time_next;//到达
						c_info[tank_to_go].order[r_next - 1][4] = U_time_next;
						c_info[tank_to_go].order[r_next - 1][5] = S_time_next;
						c_info[tank_to_go].order[r_next - 1][6] = d_time_next;
						c_info[tank_to_go].order[r_next - 1][7] = d_time_next * c_info[tank_to_go].sell_v * short_c;//迟到成本

						//判断是否sati

						//更新车辆tour信息
						tour_info[car].cur_node_id = tank_to_go;
						tour_info[car].cur_q = q_to_go;
						tour_info[car].cur_time = left_time;
						//tour_info[cur_car_NO].gc_left = tour_info[cur_car_NO].gc_left - (ceil(q_canbe[next_q] / COMP_Q));
						tour_info[car].node[tour_info[car].node_num] = tank_to_go;
						tour_info[car].gc_need[tour_info[car].node_num] = ceil(q_to_go / COMP_Q);
						tour_info[car].r[tour_info[car].node_num] = r_next;
						tour_info[car].q[tour_info[car].node_num] = q_to_go;
						tour_info[car].a_time[tour_info[car].node_num] = a_time_next;
						tour_info[car].U_time[tour_info[car].node_num] = U_time_next;
						tour_info[car].S_time[tour_info[car].node_num] = S_time_next;
						tour_info[car].d_time[tour_info[car].node_num] = d_time_next;
						tour_info[car].d_cost[tour_info[car].node_num] = d_time_next * c_info[tank_to_go].sell_v * short_c;
						tour_info[car].ser_time[tour_info[car].node_num] = (U_time_next > a_time_next) ? U_time_next : a_time_next;//开始服务时刻
						//float check = a_time_next + w_time_next;
						tour_info[car].node_num = tour_info[car].node_num + 1;
						//更新蚂蚁信息
						//Qall = Qall + q_to_go;
						cur_tank_NO = tank_to_go;
						cur_time = left_time;
						cur_car_NO = car;
						//capacity = capacity - (floor(next_q / 5) + 1);
						//tour_sub[cur_car_NO][tour_info[cur_car_NO].node_num - 1][0] = next_tank;
						//tour_sub[cur_car_NO][tour_info[cur_car_NO].node_num - 1][1] = q_canbe[next_q];
						//tour_sub[cur_car_NO][tour_info[cur_car_NO].node_num - 1][2] = r_next;

					}
					if (c_info[tank_to_go].t == 'd')//下一节点是油厂节点
					{
						if (tank_to_go == 9)
						{
							int www = 1;
						}
						float U_time_next = 0;//容纳点
						float S_time_next = 0;//Oir的缺货点,送完这次后新产生的缺货点
						float a_time_next = tour_info[car].cur_time + dis[tank_now][tank_to_go] / CAR_V;//到达时刻
						float l_time_next = 0;//卸油时长
						float w_time_next = 0;//等待时长
						float d_time_next = 0;//迟到时长
						float left_time = a_time_next + LOAD_T;


						//更新车辆tour信息
						tour_info[car].lap = tour_info[car].lap + 1;
						tour_info[car].cur_node_id = tank_to_go;
						tour_info[car].cur_q = 0;
						tour_info[car].cur_time = left_time;
						//tour_info[car].gc_left = COMP_COUNT;
						tour_info[car].node[tour_info[car].node_num] = tank_to_go;
						tour_info[car].gc_need[tour_info[car].node_num] = 0;
						//tour_info[ant_NO][cur_car_NO].unnece[tour_info[ant_NO][cur_car_NO].node_num] = 0;
						tour_info[car].r[tour_info[car].node_num] = 0;
						tour_info[car].q[tour_info[car].node_num] = 0;
						tour_info[car].a_time[tour_info[car].node_num] = a_time_next;
						tour_info[car].U_time[tour_info[car].node_num] = U_time_next;
						tour_info[car].S_time[tour_info[car].node_num] = S_time_next;
						tour_info[car].d_time[tour_info[car].node_num] = d_time_next;
						tour_info[car].d_cost[tour_info[car].node_num] = 0;
						tour_info[car].ser_time[tour_info[car].node_num] = a_time_next;//开始服务时刻
						//float check = a_time_next + w_time_next;
						tour_info[car].node_num = tour_info[car].node_num + 1;
						//更新蚂蚁信息
						//Qall = Qall + 0;
						cur_tank_NO = tank_to_go;
						cur_time = a_time_next + LOAD_T;
						//capacity = COMP_COUNT;
						//tour_sub[cur_car_NO][tour_info[cur_car_NO].node_num - 1][0] = next_tank;
						//tour_sub[cur_car_NO][tour_info[cur_car_NO].node_num - 1][1] = 0;
						//tour_sub[cur_car_NO][tour_info[cur_car_NO].node_num - 1][2] = 0;
					}
				}
				if (tour_sub[car][tour_step[car]][0] < 0)//判断这个车走没走完
				{
					tour_fin[car] = 1;
					tour_fin_num = tour_fin_num + 1;
				}
				stuck = stuck + 1;
				if (stuck > CAR_COUNT + 3)
				{//锁死
					planisok = 0;
					break;
				}
			}
		}
		if (stuck > CAR_COUNT + 3)
		{//锁死
			planisok = 0;
			break;
		}
	}
	//判断是否违反调度期约束
	for (int c = 0; c < CAR_COUNT; c++)
	{
		if (tour_info[c].cur_time - LOAD_T > B_END)
		{
			planisok = 0;
		}
		if (tour_info[c].lap > Max_tour_per_car)
		{
			planisok = 0;//不符合趟数约束
		}
	}
	if (tour_sub[0][tour_info[0].node_num][0] > 0 && planisok == 1)
	{
		int qqqqq = 1;
	}
	if (tour_sub[0][0][0] != 0)
	{
		int qqqqq = 0;
	}
	if (Qall<HMIN || Qall>HMAX)
	{
		int qqqqq = 0;
	}
	int Q_check = 0;
	for (int t = 0; t < CAR_COUNT; t++)
	{
		for (int n = 0; n < 18; n++)
		{
			if (tour_sub[t][n][0] > 0)
			{
				Q_check = Q_check + tour_sub[t][n][1];
			}
		}
	}
	if (Q_check != Qall)
	{
		int www = 1;
	}
	return planisok;
}

inline void ant::make_gc_reasonable(int tour) {
	//帮助修改过的tour的访问油库的时机合理
	int tour_without0[33][3] = { -1 };
	int tour_without0_num = 0;
	for (int n = 0; n < 18; n++)
	{//放入没有油库的纯油罐订单，按顺序
		if (tour_sub[tour][n][0] < 0)
		{
			break;
		}
		if (tour_sub[tour][n][0] > 0)
		{
			::memcpy(tour_without0[tour_without0_num], tour_sub[tour][n], sizeof(tour_sub[tour][n]));
			tour_without0_num = tour_without0_num + 1;
		}
	}
	int gc_num = 3;
	int putin_pos = 1;//插入tour的位置
	int putin_num = 0;//处理到了第几个纯订单
	while (putin_pos < 18)
	{
		int gc_need = ceil(tour_without0[putin_num][1] / COMP_Q);
		if (gc_num >= gc_need && putin_num < tour_without0_num)
		{//装得下
			gc_num = gc_num - gc_need;
			::memcpy(tour_sub[tour][putin_pos], tour_without0[putin_num], sizeof(tour_without0[putin_num]));
			putin_pos = putin_pos + 1;
			putin_num = putin_num + 1;
		}
		else if (putin_num < tour_without0_num)
		{
			//装不下
			tour_sub[tour][putin_pos][0] = 0;
			tour_sub[tour][putin_pos][1] = 0;
			tour_sub[tour][putin_pos][2] = 0;
			gc_num = 3;
			putin_pos = putin_pos + 1;
		}
		if (putin_num > tour_without0_num)
		{//全处理完了，后面是-1
			tour_sub[tour][putin_pos][0] = -1;
			tour_sub[tour][putin_pos][1] = -1;
			tour_sub[tour][putin_pos][2] = -1;
			putin_pos = putin_pos + 1;
		}
		if (putin_num == tour_without0_num)
		{//全处理完了，最后返回节点
			tour_sub[tour][putin_pos][0] = 0;
			tour_sub[tour][putin_pos][1] = 0;
			tour_sub[tour][putin_pos][2] = 0;
			putin_num = putin_num + 1;
			putin_pos = putin_pos + 1;
		}

	}
	//更新nodenum
	int tour_num = 0;
	for (int n = 0; n < 18; n++)
	{
		if (tour_sub[tour][n][0] >= 0)
		{
			tour_num = tour_num + 1;
		}
	}
	tour_info[tour].node_num = tour_num;
}

inline int ant::insert(int ins, int tour_ins, int out, int tour_out)//ins是插入位置，tour_ins是被插入tour，out是订单原有位置，tour_out是订单原在tour
{
	int order_ins[3];//存放需要插入的订单
	::memcpy(order_ins, tour_sub[tour_out][out], sizeof(tour_sub[tour_out][out]));//要插入的订单copy出来
	for (int rr = out; rr < tour_info[tour_out].node_num + 1; rr++)
	{
		tour_sub[tour_out][rr][0] = tour_sub[tour_out][rr + 1][0];
		tour_sub[tour_out][rr][1] = tour_sub[tour_out][rr + 1][1];
		tour_sub[tour_out][rr][2] = tour_sub[tour_out][rr + 1][2];
	}
	int r_should = 0;
	int gowrong = 0;

	for (int w = tour_info[tour_ins].node_num + 1; w > ins; w--)
	{//ins后的节点都向后串一位
		tour_sub[tour_ins][w][0] = tour_sub[tour_ins][w - 1][0];
		tour_sub[tour_ins][w][1] = tour_sub[tour_ins][w - 1][1];
		tour_sub[tour_ins][w][2] = tour_sub[tour_ins][w - 1][2];
	}
	//插入delay订单
	tour_sub[tour_ins][ins][0] = order_ins[0];
	tour_sub[tour_ins][ins][1] = order_ins[1];
	tour_sub[tour_ins][ins][2] = order_ins[2];
	//检验tour_ins内有无不符合访序约束的
	for (int n = 0; n < 18; n++)
	{
		if (tour_sub[tour_ins][n][0] < 0)
		{
			break;
		}
		if (tour_sub[tour_ins][n][0] == tour_sub[tour_ins][ins][0])
		{
			if (tour_sub[tour_ins][n][2] < r_should)
			{
				gowrong = 1;
				break;
			}
			r_should = tour_sub[tour_ins][n][2];
		}
	}
	if (gowrong == 0)
	{
		//串位置！！！！

		make_gc_reasonable(tour_ins);
		make_gc_reasonable(tour_out);
		if (tour_sub[tour_ins][1][0] == 0 || tour_sub[tour_out][1][0] == 0)
		{
			int www = 0;
		}
		return 1;
	}
	else
	{
		return 0;
	}
}

int ant::fin_test() {
	init_befor_calcul();
	int ok = scheduling_con();
	return ok;

}