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
//��ʼ�����ϵļ�¼
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

	//c_info��ʼ��
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
			c_info[y].next_dtime = 0;					//����ĩ��ȱ����ʱ��
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
			c_info[y].next_dtime = 0;					//����ĩ��ȱ����ʱ��
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

	//��ʼ��tour

	for (int j = 0; j < CAR_COUNT; j++)
	{
		tour_info[j].car_id = 0;
		tour_info[j].cur_node_id = 0;
		tour_info[j].cur_q = 0;                               //��cur_node_id��������
		tour_info[j].cur_time = 0.5;
		tour_info[j].lap = 0;
		tour_info[j].useable = 1;                            //������״̬
		tour_info[j].gc_left = 3;							//ʣ����ø���
		tour_info[j].node_num = 1;
		for (int l = 0; l < 18; l++)
		{
			tour_info[j].node[l] = -1;
			if (l == 0)
			{
				tour_info[j].node[l] = 0;
			}
			tour_info[j].gc_need[l] = 0;						//����ÿ�η������ø���
			//tour_info[i][j].unnece[l] = -1;							//��¼ÿ�������Ƿ��Ǳ�Ҫ���ͣ���Ҫ��0.�Ǳ�Ҫ��1
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
		int rand_step = (rand() % (2)) + 1;//����1-2֮��������
		int steped = 0;
		int choose_or_not = -1;
		while (steped < rand_step)
		{
			if (Qall > HMIN)
			{
				//���ɽ�
				break;
			}
			choose_or_not = -1;
			while (choose_or_not < 0) {
				choose_or_not = choose_next_city();												//�ҳ���һ��Ҫ�ƶ��ĳ���
				//������ص���-1,˵��û�ҵ��ڵ㣬���������У���Ҫ����

				if (choose_or_not == -1) {//��Ҫ����
					tour_info[cur_car_NO].useable = 0;
					car_useable[cur_car_NO] = 0;
					//����Ƿ����г���������
					int useable_num = 0;
					for (int u = 0; u < CAR_COUNT; u++)
					{//
						useable_num = useable_num + car_useable[u];
					}
					if (useable_num == 0)
					{//�������ˣ��������ò�����Ӧ�����ӳ�������
						int warning = 111;

						//cout << "Rebuild" << endl;
						return 0;
					}
					cur_car_NO = (cur_car_NO + 1 > CAR_COUNT - 1) ? 0 : cur_car_NO + 1;
					cur_tank_NO = tour_info[cur_car_NO].cur_node_id;
					cur_car_r = tour_info[cur_car_NO].lap;    //����
					cur_time = tour_info[cur_car_NO].cur_time;							//��ǰʱ��
					capacity = tour_info[cur_car_NO].gc_left;
				}
				else//�ҵ��ڵ�
				{
					//int a = 1;
					steped = steped + 1;
				}
			}
		}
		//�����Ĳ����������Ҫ����
		if (Qall < HMIN)
		{
			cur_car_NO = (cur_car_NO + 1 > CAR_COUNT - 1) ? 0 : cur_car_NO + 1;
			cur_tank_NO = tour_info[cur_car_NO].cur_node_id;
			cur_car_r = tour_info[cur_car_NO].lap;                              //����
			cur_time = tour_info[cur_car_NO].cur_time;							//��ǰʱ��
			capacity = tour_info[cur_car_NO].gc_left;
		}
	}
	for (int c = 0; c < CAR_COUNT; c++)
	{
		cur_car_NO = c;
		cur_tank_NO = tour_info[c].cur_node_id;
		cur_car_r = tour_info[c].lap;                              //����
		cur_time = tour_info[c].cur_time;							//��ǰʱ��
		capacity = tour_info[c].gc_left;
		if (cur_tank_NO != 0)
		{
			change_stasus(0, 0);
		}
	}
	//�س���������������

	success = 1;

	//�����ĳɱ���
	cal_plan_cost();
	return success;
}

inline int ant::choose_next_city()//����ת�ƹ���ѡ����һ����~
{
	float pro = ((float)rand()) / RAND_MAX;
	if (c_info[cur_tank_NO].t == 'd')//��ǰ�ڵ����Ϳ�
	{
		if (tour_info[cur_car_NO].lap < Max_tour_per_car)//��ǰ�����п�������
		{
			float probability[N_NODE_COUNT][Q_CANBE_COUNT] = { 0 };
			bool allow_list[N_NODE_COUNT][Q_CANBE_COUNT] = { false };
			float pro_total = 0;
			float sum = 0;
			//��¼��ѡ�ڵ�ĸ���
			int num_candidate = 0;
			for (int t = 0; t < N_NODE_COUNT; t++)
			{
				if ((c_info[t].t == 't') && (c_info[t].r_now < c_info[t].rmax) && (cur_time >= c_info[t].recent_left_time)) {
					for (int q = 0; q < Q_CANBE_COUNT; q++)
					{//�ڵ�cur_tank_NO,���͹�t����������q_canbe[q]�Ŀ�����

						float U_time_q = (c_info[t].ini_stock - c_info[t].tank_cap + c_info[t].send_all + q_canbe[q]) / c_info[t].sell_v + c_info[t].dtime_all;//���ɵ�
						float S_time_q = c_info[t].S_time;//ȱ����
						float a_time_q = cur_time + dis[cur_tank_NO][t] / CAR_V;//����ʱ��
						float l_time_q = q_canbe[q] / SERVICE_RATE;//ж��ʱ��
						float w_time_q = (0 > U_time_q - a_time_q) ? 0 : U_time_q - a_time_q;//�ȴ�ʱ��
						float d_time_q = (0 > a_time_q - S_time_q) ? 0 : a_time_q - S_time_q;//�ٵ�ʱ��
						int gc_need = ceil(q_canbe[q] / COMP_Q);
						if (a_time_q + l_time_q + w_time_q + dis[t][0] / CAR_V <= B_END)//���ϵ�����Լ��
						{
							if ((tour_info[cur_car_NO].gc_left >= gc_need) && q_canbe[q] <= c_info[t].tank_cap)//�˴�gc.leftӦ��=3������Լ��
							{
								if (q_canbe[q] + Qall <= HMAX)//��������Լ��&&���뵱tank�ϴ����ͽ����󣨿����ɱ�ĳ��ͣ�������������һ��
								{
									//ͨ��Լ������ʼ��ת�Ƹ���
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
									float yz_xxs = (2 * phe_table[cur_tank_NO][t][c_info[cur_tank_NO].r_now][c_info[t].r_now + 1][cur_q][q]) / (PMAX + PMIN);//��Ϣ������
									float yz_dis = 1 / (dis[cur_tank_NO][t] + 1);//��������
									float yz_kxb = 1 / (abs(S_time_q - a_time_q) + 1);//����������
									float yz_twmatch = 0;//ʱ�䴰ƥ������
									if (a_time_q < U_time_q)
									{//�絽
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

									if (load_rate >= 0.8)//����
									{
										load_rate = 1;
									}
									float yz_load = exp(load_rate) / exp(1);//װ��������
									probability[t][q] = yz_xxs + yz_dis + yz_kxb + yz_twmatch + yz_load;
									pro_total = pro_total + probability[t][q];
								}
							}
						}
					}
				}
			}
			//���ʼ�����ϣ���ʼѡ�ڵ㣡
			if (num_candidate > 0)//�к�ѡ�ڵ�
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
					return -2;//Ҫ��
				}
				else
				{//ѡ������һ�ڵ㣬��Ҫ�ı�״̬
					change_stasus(next_node, next_q);
					return 1;//�ɹ��ҵ���һ�ڵ�
				}
			}
			else {//û�п�ѡ�ڵ㣬��k���ͽ��������Ϳ�û�п�ѡ�ڵ㣬˵��������߲�����

				return -1;//���ػ�ȥ��������������Ϣ
			}
		}
		else//�˶�������
		{
			return -1;//���ػ�ȥ��������������Ϣ
		}
	}
	if (c_info[cur_tank_NO].t == 't')//��ǰ�ڵ����͹�
	{
		if (tour_info[cur_car_NO].gc_left > 0)//��ǰ�����п��ø���
		{
			float probability[N_NODE_COUNT][Q_CANBE_COUNT] = { 0 };
			bool allow_list[N_NODE_COUNT][Q_CANBE_COUNT] = { false };
			float pro_total = 0;
			float sum = 0;
			//��¼��ѡ�ڵ�ĸ���
			int num_candidate = 0;

			for (int t = 0; t < N_NODE_COUNT; t++)
			{
				if (t == 10)
				{
					int www = 1;
				}
				if ((c_info[t].t == 't') && (c_info[t].r_now < c_info[t].rmax) && (cur_time >= c_info[t].recent_left_time)) {
					for (int q = 0; q < Q_CANBE_COUNT; q++)
					{//�ڵ�cur_tank_NO,���͹�t����������q_canbe[q]�Ŀ�����

						float U_time_q = (c_info[t].ini_stock - c_info[t].tank_cap + c_info[t].send_all + q_canbe[q]) / c_info[t].sell_v + c_info[t].dtime_all;//���ɵ�
						float S_time_q = c_info[t].S_time;//ȱ����
						float a_time_q = cur_time + dis[cur_tank_NO][t] / CAR_V;//����ʱ��
						float l_time_q = q_canbe[q] / SERVICE_RATE;//ж��ʱ��
						float w_time_q = (0 > U_time_q - a_time_q) ? 0 : U_time_q - a_time_q;//�ȴ�ʱ��
						float d_time_q = (0 > a_time_q - S_time_q) ? 0 : a_time_q - S_time_q;//�ٵ�ʱ��
						int gc_need = ceil(q_canbe[q] / COMP_Q);
						if (a_time_q + l_time_q + w_time_q + dis[t][0] / CAR_V <= B_END)//���ϵ�����Լ��
						{
							if (tour_info[cur_car_NO].gc_left >= gc_need && q_canbe[q] <= c_info[t].tank_cap)//�˴�gc.leftӦ��=3������Լ��
							{
								if (q_canbe[q] + Qall <= HMAX)//��������Լ��
								{
									//ͨ��Լ������ʼ��ת�Ƹ���
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
									float yz_xxs = (2 * phe_table[cur_tank_NO][t][c_info[cur_tank_NO].r_now][c_info[t].r_now + 1][cur_q][q]) / (PMAX + PMIN);//��Ϣ������
									float yz_dis = 1 / (dis[cur_tank_NO][t] + 1);//��������
									yz_dis = pow(yz_dis, 0.25);
									float yz_kxb = 1 / (abs(S_time_q - a_time_q) + 1);//����������
									float yz_twmatch = 0;//ʱ�䴰ƥ������
									if (a_time_q < U_time_q)
									{//�絽
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
									if (load_rate < 0.01)//����
									{
										load_rate = 1;
									}
									float yz_load = exp(load_rate) / exp(1);//װ��������
									probability[t][q] = yz_xxs + yz_dis + yz_kxb + yz_twmatch + yz_load;
									pro_total = pro_total + probability[t][q];
								}
							}
						}
					}
				}
			}
			//���ʼ�����ϣ���ʼѡ�ڵ㣡
			if (num_candidate > 0)//�к�ѡ�ڵ�
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
					return -2;//Ҫ��
				}
				else
				{//ѡ������һ�ڵ㣬��Ҫ�ı�״̬
					change_stasus(next_node, next_q);
					return 1;//�ɹ��ҵ���һ�ڵ�
				}

			}
			else {//û�п�ѡ�ڵ㣬����

				return -1;//���ػ�ȥ����
			}
		}
		else//û�п��ø����ˣ����ͳ�
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
	if (c_info[next_tank].t == 't')//��һ�ڵ����͹޽ڵ�
	{
		float U_time_next = ((c_info[next_tank].ini_stock - c_info[next_tank].tank_cap + c_info[next_tank].send_all + q_canbe[next_q]) / c_info[next_tank].sell_v) + c_info[next_tank].dtime_all;//���ɵ�
		float S_time_next = ((c_info[next_tank].ini_stock + c_info[next_tank].send_all) / c_info[next_tank].sell_v) + c_info[next_tank].dtime_all;//Oir��ȱ����,�������ǰ��ȱ����
		float a_time_next = cur_time + dis[cur_tank_NO][next_tank] / CAR_V;//����ʱ��
		float l_time_next = q_canbe[next_q] / SERVICE_RATE;//ж��ʱ��
		float w_time_next = (0 > U_time_next - a_time_next) ? 0 : U_time_next - a_time_next;//�ȴ�ʱ��
		float d_time_next = (0 > a_time_next - S_time_next) ? 0 : a_time_next - S_time_next;//�ٵ�ʱ��
		float left_time = a_time_next + l_time_next + w_time_next;
		//�����͹�c_info��Ϣ
		c_info[next_tank].send_all = c_info[next_tank].send_all + q_canbe[next_q];
		c_info[next_tank].S_time = ((c_info[next_tank].ini_stock + c_info[next_tank].send_all) / c_info[next_tank].sell_v) + c_info[next_tank].dtime_all;//��������һ�κ��ȱ����
		c_info[next_tank].r_now = c_info[next_tank].r_now + 1;
		c_info[next_tank].dtime_all = c_info[next_tank].dtime_all + d_time_next;
		c_info[next_tank].order[c_info[next_tank].r_now - 1][0] = c_info[next_tank].r_now;
		c_info[next_tank].recent_left_time = left_time;
		int r_next = c_info[next_tank].r_now;
		c_info[next_tank].order[r_next - 1][0] = r_next;//r
		c_info[next_tank].order[r_next - 1][1] = q_canbe[next_q];//q
		c_info[next_tank].order[r_next - 1][2] = (ceil(q_canbe[next_q] / COMP_Q));//���ø���
		c_info[next_tank].order[r_next - 1][3] = a_time_next;//����
		c_info[next_tank].order[r_next - 1][4] = U_time_next;
		c_info[next_tank].order[r_next - 1][5] = S_time_next;
		c_info[next_tank].order[r_next - 1][6] = d_time_next;
		c_info[next_tank].order[r_next - 1][7] = d_time_next * c_info[next_tank].sell_v * short_c;//�ٵ��ɱ�
		//�ж��Ƿ�sati

		//���³���tour��Ϣ
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
		tour_info[cur_car_NO].ser_time[tour_info[cur_car_NO].node_num] = (U_time_next > a_time_next) ? U_time_next : a_time_next;//��ʼ����ʱ��
		//float check = a_time_next + w_time_next;
		tour_info[cur_car_NO].node_num = tour_info[cur_car_NO].node_num + 1;
		//����������Ϣ
		Qall = Qall + q_canbe[next_q];
		cur_tank_NO = next_tank;
		cur_time = a_time_next + l_time_next + w_time_next;
		capacity = capacity - (floor(next_q / 5) + 1);
		tour_sub[cur_car_NO][tour_info[cur_car_NO].node_num - 1][0] = next_tank;
		tour_sub[cur_car_NO][tour_info[cur_car_NO].node_num - 1][1] = q_canbe[next_q];
		tour_sub[cur_car_NO][tour_info[cur_car_NO].node_num - 1][2] = r_next;
	}
	if (c_info[next_tank].t == 'd')//��һ�ڵ����ͳ��ڵ�
	{

		float U_time_next = 0;//���ɵ�
		float S_time_next = 0;//Oir��ȱ����,������κ��²�����ȱ����
		float a_time_next = cur_time + dis[cur_tank_NO][next_tank] / CAR_V;//����ʱ��
		float l_time_next = 0;//ж��ʱ��
		float w_time_next = 0;//�ȴ�ʱ��
		float d_time_next = 0;//�ٵ�ʱ��
		float left_time = a_time_next + LOAD_T;


		//���³���tour��Ϣ
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
		tour_info[cur_car_NO].ser_time[tour_info[cur_car_NO].node_num] = 0;//��ʼ����ʱ��
		//float check = a_time_next + w_time_next;
		tour_info[cur_car_NO].node_num = tour_info[cur_car_NO].node_num + 1;
		//����������Ϣ
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
	//��ʻ�ɱ�����
	float dis_road = 0;
	int node_i = 0;
	int node_j = 0;
	for (int i = 0; i < CAR_COUNT; i++)
	{//i�ǳ��ı��
		int pos = 0;
		while (tour_sub[i][pos + 1][0] >= 0)
		{//�ۼӵ�pos���ڵ㵽��һ�ڵ�֮��ľ���
			node_i = tour_sub[i][pos][0];
			node_j = tour_sub[i][pos + 1][0];
			dis_road = dis_road + dis[node_i][node_j];
			pos = pos + 1;
		}
	}
	tran_cost = dis_road * drive_c;
	//ж���ɱ�����
	float unload_cost_all = 0;
	for (int t = 1; t < N_NODE_COUNT; t++)
	{
		unload_cost_all = unload_cost_all + ((c_info[t].send_all / SERVICE_RATE) * c_info[t].sell_v * unload_c);
	}
	unload_cost = unload_cost_all;
	//ȱ���ɱ�����
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
	//�ܳɱ�
	cost = tran_cost + delay_cost + next_delay_cost;
	//cost = tran_cost;
	//cost = delay_cost + next_delay_cost;
	delay_time = delay_time_all;
	next_delay_time = next_delay_time_all;
}

float ant::Exchange_order1()//�ֲ�����1_���ѡ��ͬһ��·�����������н���
{//���漰�Ϳ�ڵ�˳�����F
	int tour_num = (rand() % (CAR_COUNT));//����0-����-1֮��������������ѡ���ĸ���·
	int trip_num = (rand() % (tour_info[tour_num].lap));//ѡ���ĸ���������0��ʼ
	//�ҵ�ָ��trip��ͷβ�ڵ���
	int lap = -1;
	int startn = 0, endn = 0;
	int order_a = 0, order_b = 0;
	int ar_should = 0, br_should = 0;
	for (int n = 0; n < 18; n++)
	{
		if (tour_sub[tour_num][n][0] == 0)
		{//����һ��lap
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
	{//��һ��ֻ��һ���ڵ㣬������

		return cost;
	}
	if (startn == endn)
	{//��һ��ֻ��һ���ڵ㣬������
		return cost;
	}
	else
	{//ѡ������������
		int change_times = 0;
		order_a = (rand() % (endn - startn + 1)) + startn;//����startn-endn֮��������
		order_b = order_a;
		while (tour_sub[tour_num][order_a][0] == tour_sub[tour_num][order_b][0])
		{

			order_b = (rand() % (endn - startn + 1)) + startn;//����startn-endn֮��������
			change_times = change_times + 1;
			if (change_times > 30)
			{
				break;
			}
		}
		if (change_times > 30)
		{//��ѡ�е�trip��ֻ������ͬ�͹޽ڵ㣬���ɽ���
			return cost;
		}
	}
	//����tour_sub
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
	{//������û��һ��tour�ڲ����Ϸ���˳��Լ����
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

	//��ʼ��һЩ����
	init_befor_calcul();
	//����Ƿ񳬳�������Լ��.����1��˵����Υ��������0˵��Υ����
	int planisok = scheduling_con();
	//����ɱ�
	if (planisok == 1)
	{
		cal_plan_cost();
	}
	return cost;
}

float ant::Exchange_order2()//�ֲ�����2_���ѡ��ͬһtour�����������н���
{//���漰�Ϳ�ڵ�˳�����,ѡ��ͬ���ն���

	int tour_num = (rand() % (CAR_COUNT));//����0-����-1֮��������������ѡ���ĸ���·
	int node_num = tour_info[tour_num].node_num;
	int order_a = 0, order_b = -1;
	int ar_should = 0, br_should = 0;
	while (tour_sub[tour_num][order_a][0] == 0)
	{//���ѡ��һ���͹޽ڵ㣨�������Ϳ⣩
		order_a = (rand() % (tour_info[tour_num].node_num));
	}
	int order_air = ceil(tour_sub[tour_num][order_a][1] / COMP_Q);//�������������������
	int ok_order[33] = { 0 };//��ŷ��ϸ���Լ���Ľڵ�λ��
	int ok_order_count = 0;
	for (int b = 0; b < node_num; b++)
	{
		int this_air = ceil(tour_sub[tour_num][b][1] / COMP_Q);
		if (this_air == order_air && b != order_a)
		{//����ok_order
			ok_order[ok_order_count] = b;
			ok_order_count = ok_order_count + 1;
		}
		if (tour_sub[tour_num][b][0] == -1)
		{
			int l = 0;
		}
	}
	if (ok_order_count == 0)
	{//����tour��û�к�order_aһ�����յĶ���
		return cost;
	}
	else
	{
		order_b = ok_order[(rand() % (ok_order_count))];
	}

	//����tour_sub
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
	{//������û��һ��tour�ڲ����Ϸ���˳��Լ����
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
	//��ʼ��һЩ����
	init_befor_calcul();
	//����Ƿ񳬳�������Լ��.����1��˵����Υ��������0˵��Υ����
	int planisok = scheduling_con();
	//����ɱ�
	if (planisok == 1)
	{
		cal_plan_cost();
	}
	return cost;
}

float ant::Exchange_order3()//�ֲ�����3_���ѡ������tour�����������н���
{//���漰�Ϳ�ڵ�˳�����,ѡ��ͬ���ն���,tour_a������tour_b,����tour����Ҫ�������
	int tour_a = (rand() % (CAR_COUNT));//����0-����-1֮��������������ѡ���ĸ���·
	int tour_b = tour_a;
	while (tour_a == tour_b)
	{//���ѡ��һ���͹޽ڵ㣨�������Ϳ⣩
		tour_b = (rand() % (CAR_COUNT));
	}
	int node_num_a = tour_info[tour_a].node_num;
	int node_num_b = tour_info[tour_b].node_num;
	int order_a = 0, order_b = -1;
	int ar_should = 0, br_should = 0;
	while (tour_sub[tour_a][order_a][0] == 0)
	{//���ѡ��һ���͹޽ڵ㣨�������Ϳ⣩
		order_a = (rand() % (tour_info[tour_a].node_num));
	}
	int order_air = ceil(tour_sub[tour_a][order_a][1] / COMP_Q);//�������������������
	int ok_order[33] = { 0 };//��ŷ��ϸ���Լ���Ľڵ�λ��
	int ok_order_count = 0;
	for (int b = 0; b < node_num_b; b++)
	{
		int this_air = ceil(tour_sub[tour_b][b][1] / COMP_Q);
		if (this_air == order_air)
		{//����ok_order
			ok_order[ok_order_count] = b;
			ok_order_count = ok_order_count + 1;
		}
	}
	if (ok_order_count == 0)
	{//����tour��û�к�order_aһ�����յĶ���
		return cost;
	}
	else
	{
		order_b = ok_order[(rand() % (ok_order_count))];
	}
	//����tour_sub
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
	{//����tourb�����޲����Ϸ���˳��Լ����
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
	{//����toura�����޲����Ϸ���˳��Լ����
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
	//��ʼ��һЩ����
	init_befor_calcul();
	//����Ƿ񳬳�������Լ��.����1��˵����Υ��������0˵��Υ����
	int planisok = scheduling_con();
	//����ɱ�
	if (planisok == 1)
	{
		cal_plan_cost();
	}
	return cost;
}

float ant::Short1() {//�ֲ��Ż�����4_���ѡ��һ��tour��ȱ���ɱ����Ķ���������tour������λ�ã�����λ������ɱ���С�������
	//Լ�������붩�����ܵȴ����ѵ�2������λ��,�ѵ�3�������Ϳ�λ��
	if (delay_cost > 0)
	{
		int w = 0;
	}

	int tour = (rand() % (CAR_COUNT));//����0-����-1֮��������������ѡ���ĸ���·
	int node_num = tour_info[tour].node_num;
	float delay_max = 0;
	int delay_max_node = -1;
	int tour_sub_copy[CAR_COUNT][33][3];
	int tour_sub_best[CAR_COUNT][33][3];
	float cost_best = cost;//��float
	//�ҵ���tour��ȱ���ɱ����Ķ���
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
	{//û��ȱ���Ľڵ㣬���ѡһ���ڵ�
		delay_max_node = 0;
		while (tour_sub[tour][delay_max_node][0] == 0)
		{//���ѡ��һ���͹޽ڵ㣨�������Ϳ⣩
			delay_max_node = (rand() % (tour_info[tour].node_num));
		}
	}

	//����Ѱ�ҷ���Լ�������Ų����
	//�Ȱ�tour_sub����������tour_sub_copy
	::memcpy(tour_sub_copy, tour_sub, sizeof(tour_sub));//��һ���в���
	::memcpy(tour_sub_best, tour_sub, sizeof(tour_sub));//��һ���в���

	int order_ins[3];
	::memcpy(order_ins, tour_sub[tour][delay_max_node], sizeof(tour_sub[tour][delay_max_node]));//Ҫ����Ķ���copy����
	for (int ins = 1; ins < node_num; ins++)
	{//�����ɲ���λ�ã�ÿ����һ�ֿ��ܣ�����ȥ��֤
		//����ins��λ�ã���������˳��
		//�����������tour��ȡ����
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
		{//ins��Ľڵ㶼���һλ
			tour_sub[tour][w][0] = tour_sub[tour][w - 1][0];
			tour_sub[tour][w][1] = tour_sub[tour][w - 1][1];
			tour_sub[tour][w][2] = tour_sub[tour][w - 1][2];
		}
		//����delay����
		tour_sub[tour][ins][0] = order_ins[0];
		tour_sub[tour][ins][1] = order_ins[1];
		tour_sub[tour][ins][2] = order_ins[2];
		//����tour�����޲����Ϸ���˳��Լ����
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
		//��λ�ã�������
		make_gc_reasonable(tour);
		if (gowrong == 0)
		{//�����Ƿ���ϵ�����Լ��
			init_befor_calcul();
			//����Ƿ񳬳�������Լ��.����1��˵����Υ��������0˵��Υ����
			int planisok = scheduling_con();
			int dont_wait = 0;
			if (planisok == 1)
			{//����Ƿ����㲻�ȴ�Լ��
				float a_time = c_info[order_ins[0]].order[order_ins[2] - 1][3];
				float U_time = c_info[order_ins[0]].order[order_ins[2] - 1][4];
				if (a_time >= U_time)
				{//û�еȴ�,ͨ�����ؿ��飬���һ����ɱ�
					dont_wait = 1;
					cal_plan_cost();
					if (cost < cost_best)
					{
						cost_best = cost;
						::memcpy(tour_sub_best, tour_sub, sizeof(tour_sub));//��һ���в���
					}
				}



				//cal_plan_cost();
			}
		}
		//һ�ֲ��뷽�����Թ��ˣ�����ԭʼ�������ٳ���
		::memcpy(tour_sub, tour_sub_copy, sizeof(tour_sub_copy));
		init_befor_calcul();
		int ww = scheduling_con();
	}
	//���������״̬������
	::memcpy(tour_sub, tour_sub_best, sizeof(tour_sub_best));//
	init_befor_calcul();
	int planisok = scheduling_con();
	//����ɱ�
	if (planisok == 1)
	{
		cal_plan_cost();
	}
	else {
		int wrong = 1;

	}
	return cost;
}

float ant::Short2() {//�ֲ��Ż�����5_���ѡ��һ��tour��ȱ���ɱ����Ķ����������ѡ��һ��tour�������ж�������������λ������ɱ���С�Ľ�������
	//�ѵ�1����������λ��,�ѵ�2�������Ϳ�λ��
	if (delay_cost > 0)
	{
		int w = 0;
	}
	//int tour = (rand() % (CAR_COUNT));//����0-����-1֮��������������ѡ���ĸ���·
	int tour_a = (rand() % (CAR_COUNT));//����0-����-1֮��������������ѡ���ĸ���·
	int tour_b = tour_a;
	while (tour_a == tour_b)
	{//���ѡ��һ��tour
		tour_b = (rand() % (CAR_COUNT));
	}
	int node_num = tour_info[tour_a].node_num;
	float delay_max = 0;
	int delay_max_node = -1;
	int tour_sub_copy[CAR_COUNT][33][3];
	int tour_sub_best[CAR_COUNT][33][3];
	float cost_best = cost;//��float
	//�ҵ���tour_a��ȱ���ɱ����Ķ���
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
	{//û��ȱ���Ľڵ㣬�����ӽ���
		return cost;
	}
	else
	{//����Ѱ�ҷ���Լ�������Ž�������
		//�Ȱ�tour_sub����������tour_sub_copy
		::memcpy(tour_sub_copy, tour_sub, sizeof(tour_sub));//��һ���в���
		::memcpy(tour_sub_best, tour_sub, sizeof(tour_sub));//��һ���в���
		int order_ins[3];
		::memcpy(order_ins, tour_sub[tour_a][delay_max_node], sizeof(tour_sub[tour_a][delay_max_node]));//Ҫ�����Ķ���copy����
		int temp[3] = { 0 };
		int order_air = ceil(order_ins[1] / COMP_Q);
		for (int ins = 1; ins < node_num; ins++)
		{//�����ɲ���λ�ã�ÿ����һ�ֿ��ܣ�����ȥ��֤
			//��tour_b�� insλ�õĶ�������
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
			{//����tourb�����޲����Ϸ���˳��Լ����
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
			{//����toura�����޲����Ϸ���˳��Լ����
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
			{//�����Ƿ���ϵ�����Լ��
				init_befor_calcul();
				//����Ƿ񳬳�������Լ��.����1��˵����Υ��������0˵��Υ����

				int planisok = scheduling_con();
				int dont_wait = 0;
				if (planisok == 1)
				{//����Ƿ����㲻�ȴ�Լ��
					float a_time = c_info[order_ins[0]].order[order_ins[2] - 1][3];
					float U_time = c_info[order_ins[0]].order[order_ins[2] - 1][4];
					if (a_time >= U_time)
					{//û�еȴ�,ͨ�����ؿ��飬���һ����ɱ�
						dont_wait = 1;
						cal_plan_cost();
						if (cost < cost_best)
						{
							cost_best = cost;
							::memcpy(tour_sub_best, tour_sub, sizeof(tour_sub));//��һ���в���
						}
					}
					//cal_plan_cost();
				}
			}
			//һ�ֲ��뷽�����Թ��ˣ�����ԭʼ�������ٳ���
			::memcpy(tour_sub, tour_sub_copy, sizeof(tour_sub_copy));
			init_befor_calcul();
			int ww = scheduling_con();
		}
	}
	//���������������
	::memcpy(tour_sub, tour_sub_best, sizeof(tour_sub_best));
	init_befor_calcul();
	int planisok = scheduling_con();
	//����ɱ�
	if (planisok == 1)
	{
		cal_plan_cost();
	}
	else {
		int wrong = 1;

	}
	return cost;
}

float ant::Station() {//�ֲ��Ż�����6_�ҵ�����ʱ�������С������ͬ����վ�Ķ����ԣ����ʱ����С�������У����β����Ż�
	//�ѵ�1�������ж����԰�����ʱ�������ѵ�2������4�ֲ��뷽ʽ,�ѵ�3�������Ϳ�λ��
	//--------------�ҵ��������ʱ���������е�����---------------
	//ͳ�Ƹ�������վһ���ж��ٶ���

	float cost_best = cost;
	int s_order_num[N_STATION_COUNT] = { 0 };//����վ�Ķ�������
	for (int c = 1; c < N_NODE_COUNT; c++)
	{
		s_order_num[c_info[c].station - 1] = s_order_num[c_info[c].station - 1] + c_info[c].r_now;
	}
	//���ΰ����ڸ�����վ�Ķ�����������,һ������վһ������վ�Ĵ���
	float*** sta_order = new float** [N_STATION_COUNT];//[tank][q][r][ser_time]////order:[r][qir][air][a_time][Uir][Sir][d_time][d_cost];
	for (int s = 0; s < N_STATION_COUNT; s++)
	{
		sta_order[s] = new float* [s_order_num[s]];
		int pos = 0;
		for (int c = 1; c < N_NODE_COUNT; c++)
		{
			if (c_info[c].station == s + 1)
			{//���͹�����s����վ
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
		}//�Դ˼���վ�����ж���ð�ݰ���ser_time����
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
	//��������ͬ����վ���Ķ����Ƚϣ��ó�ser_time֮��С��0.5��,����o_pair
	//���һ���ж��������Ķ���
	float** o_pair = new float* [500];//[station][��sta_order��order1��λ��][order2��λ��][ser_time���ֵ]//���ܻ�������
	int o_pair_num_sum = 0;
	//int o_pair_num[N_STATION_COUNT] = { 0 };
	for (int s = 0; s < N_STATION_COUNT; s++)
	{
		for (int order = 0; order < s_order_num[s]; order++)
		{
			for (int y = order + 1; y < s_order_num[s]; y++)
			{
				if ((sta_order[s][order][3] + 0.5 >= sta_order[s][y][3]) && (sta_order[s][order][0] != sta_order[s][y][0]))//���С��0.5���Ҳ�����ͬ�͹�
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
	//�ٴ�ð��������ser_time���ֵ��������
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
	//�ó�һ�Զ����ԣ��ҵ�������tour_sub�е�λ��
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
		//���ֲ�������ʼ
		//����1��a����bǰ
		int order_ex = order_b - 1;
		int tour_ex = tour_b;
		if ((tour_a == tour_ex && order_a == order_ex) || tour_info[tour_a].node_num <= 3) {//bǰ��Ķ�������a,����û��Ҫ������a��tourֻ����һ��������
			int next_oper = 1;//ֱ����һ������
		}
		else
		{//ins=order_b
			int succ = insert(order_b, tour_b, order_a, tour_a);//ins�ǲ���λ�ã�tour_ins�Ǳ�����tour��out�Ƕ���ԭ��λ�ã�tour_out�Ƕ���ԭ��tour
			//�����Ƿ���ϵ�����Լ��

			if (succ == 1)
			{
				init_befor_calcul();
				//����Ƿ񳬳�������Լ��.����1��˵����Υ��������0˵��Υ����
				int planisok = scheduling_con();
				if (planisok == 1)
				{//����Ƿ����㲻�ȴ�Լ��
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
		::memcpy(tour_sub, tour_sub_copy, sizeof(tour_sub_copy));////����
		init_befor_calcul();
		int planisok = scheduling_con();
		//����2��a����b��
		order_ex = order_b + 1;
		tour_ex = tour_b;
		if ((tour_a == tour_ex && order_a == order_ex) || tour_info[tour_a].node_num <= 3)
		{//bǰ��Ķ�������a,����û��Ҫ��
			int next_oper = 1;//ֱ����һ������
		}
		else
		{//ins=order_b
			int succ = insert(order_b + 1, tour_b, order_a, tour_a);//ins�ǲ���λ�ã�tour_ins�Ǳ�����tour��out�Ƕ���ԭ��λ�ã�tour_out�Ƕ���ԭ��tour
			//�����Ƿ���ϵ�����Լ��


			if (succ == 1)
			{
				init_befor_calcul();
				//����Ƿ񳬳�������Լ��.����1��˵����Υ��������0˵��Υ����
				int planisok = scheduling_con();
				if (planisok == 1)
				{//����Ƿ����㲻�ȴ�Լ��
					cal_plan_cost();
					if (cost < cost_best)
					{
						gettingbetter = 1;
						cost_best = cost;
						::memcpy(tour_sub_best, tour_sub, sizeof(tour_sub));//��һ���в���
					}
				}
			}
		}
		::memcpy(tour_sub, tour_sub_copy, sizeof(tour_sub_copy));////����
		init_befor_calcul();
		int ww = scheduling_con();
		//����3��b����aǰ
		order_ex = order_a - 1;
		tour_ex = tour_a;
		if ((tour_b == tour_ex && order_b == order_ex) || tour_info[tour_b].node_num <= 3)
		{//bǰ��Ķ�������a,����û��Ҫ��
			int next_oper = 1;//ֱ����һ������
		}
		else
		{//ins=order_b
			int succ = insert(order_a, tour_a, order_b, tour_b);//ins�ǲ���λ�ã�tour_ins�Ǳ�����tour��out�Ƕ���ԭ��λ�ã�tour_out�Ƕ���ԭ��tour
			//�����Ƿ���ϵ�����Լ��

			if (succ == 1)
			{
				init_befor_calcul();
				//����Ƿ񳬳�������Լ��.����1��˵����Υ��������0˵��Υ����
				int planisok = scheduling_con();
				if (planisok == 1)
				{//����Ƿ����㲻�ȴ�Լ��
					cal_plan_cost();
					if (cost < cost_best)
					{
						gettingbetter = 1;
						cost_best = cost;
						::memcpy(tour_sub_best, tour_sub, sizeof(tour_sub));//��һ���в���
					}
				}
			}
		}
		::memcpy(tour_sub, tour_sub_copy, sizeof(tour_sub_copy));////����
		init_befor_calcul();
		ww = scheduling_con();
		//����4��b����a��
		order_ex = order_a + 1;
		tour_ex = tour_a;
		if ((tour_b == tour_ex && order_b == order_ex) || tour_info[tour_b].node_num <= 3)
		{//bǰ��Ķ�������a,����û��Ҫ��
			int next_oper = 1;//ֱ����һ������
		}
		else
		{//ins=order_b
			int succ = insert(order_a + 1, tour_a, order_b, tour_b);//ins�ǲ���λ�ã�tour_ins�Ǳ�����tour��out�Ƕ���ԭ��λ�ã�tour_out�Ƕ���ԭ��tour
			//�����Ƿ���ϵ�����Լ��

			if (succ == 1)
			{
				init_befor_calcul();
				//����Ƿ񳬳�������Լ��.����1��˵����Υ��������0˵��Υ����
				int planisok = scheduling_con();
				if (planisok == 1)
				{//����Ƿ����㲻�ȴ�Լ��
					cal_plan_cost();
					if (cost < cost_best)
					{
						gettingbetter = 1;
						cost_best = cost;
						::memcpy(tour_sub_best, tour_sub, sizeof(tour_sub));//��һ���в���
					}
				}
			}
		}
		::memcpy(tour_sub, tour_sub_copy, sizeof(tour_sub_copy));////����
		init_befor_calcul();
		ww = scheduling_con();
		if (gettingbetter > 0)
		{//�Ѿ������Ľ�
			break;
		}
	}
	::memcpy(tour_sub, tour_sub_best, sizeof(tour_sub_best));
	init_befor_calcul();
	int planisok = scheduling_con();
	//����ɱ�
	if (planisok == 1)
	{
		cal_plan_cost();
	}
	else {
		int wrong = 1;
	}
	return cost;
}

float ant::Unneed_forshort() {//�ֲ��Ż�����7_���ѡ��һ��tour�����ѡ��һ�����еķǱ�Ҫ���Ͷ��������˶�����������ת�Ƶ��������ڳ�ȱ���ɱ����͹���
	//�ҵ��Ǳ�Ҫ���Ͷ��������ඩ����S_timeҪ����28,28=24+4(���ڳ�)
	int tour = (rand() % (CAR_COUNT));//����0-����-1֮��������������ѡ���ĸ���·
	int node_num = tour_info[tour].node_num;
	int unneed[12][4];//�ű�tour�еķǱ�Ҫ���Ͷ���[tank][q][r][pos in tour_sub[tour]]
	int unneed_num = 0;
	int nd_num = 0;
	float next_delay[N_NODE_COUNT][2] = { -1 };//�������ڳ�ȱ�����͹�
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
	{//û������ȱ��������û�зǱ�Ҫ����
		return cost;
	}
	else
	{//���ѡ��һ���Ǳ�Ҫ���Ͷ���
		int un = (rand() % (unneed_num));//ѡ���ĸ��Ǳ�Ҫ����
		int node_a = unneed[un][3];
		int tour_a = tour;
		//ð���������ڳ�ȱ���ɱ���ߵĶ���
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
		//-------����ת��----------
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
				//����Ƿ񳬳�������Լ��.����1��˵����Υ��������0˵��Υ����
				int planisok = scheduling_con();
				if (planisok == 1)
				{//����Ƿ����㲻�ȴ�Լ��
					cal_plan_cost();
					if (cost < cost_best)
					{
						gettingbetter = 1;
						cost_best = cost;
						::memcpy(tour_sub_best, tour_sub, sizeof(tour_sub));
					}
				}
			}
			//����
			::memcpy(tour_sub, tour_sub_copy, sizeof(tour_sub_copy));
			init_befor_calcul();
			int ww = scheduling_con();
		}
		::memcpy(tour_sub, tour_sub_best, sizeof(tour_sub_best));
		init_befor_calcul();
		int planisok = scheduling_con();
		//����ɱ�
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

float ant::Unneed_exchange() {//�ֲ��Ż�����8_���ѡ������tour�������еķǱ�Ҫ���Ͷ����ҳ����������������ҵ��ɱ���С������ʽ
	int tour_a = (rand() % (CAR_COUNT));//����0-����-1֮��������������ѡ���ĸ���·
	int tour_b = tour_a;
	while (tour_a == tour_b)
	{
		tour_b = (rand() % (CAR_COUNT));
	}
	int unneed_a[12][4];//��tour_a�еķǱ�Ҫ���Ͷ���[tank][q][r][pos in tour_sub[tour]]
	int unneed_num_a = 0;
	int unneed_b[12][4];//��tour_b�еķǱ�Ҫ���Ͷ���[tank][q][r][pos in tour_sub[tour]]
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
			{//����tour_a&tour_b�����޲����Ϸ���˳��Լ����
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
				//����Ƿ񳬳�������Լ��.����1��˵����Υ��������0˵��Υ����
				int planisok = scheduling_con();
				//����ɱ�
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
			//����
			::memcpy(tour_sub, tour_sub_copy, sizeof(tour_sub_copy));
			init_befor_calcul();
			int ww = scheduling_con();
		}
	}
	::memcpy(tour_sub, tour_sub_best, sizeof(tour_sub_best));//
	init_befor_calcul();
	int planisok = scheduling_con();
	//����ɱ�
	if (planisok == 1)
	{
		cal_plan_cost();
	}
	else {
		int wrong = 1;
	}
	return cost;
}

float ant::Unneed_forH() {//�ֲ��Ż�����9_���ѡ��һ��/����Ǳ�Ҫ���Ͷ�����ȡ���󣬰���װ�����ɵ͵��߷������������
	int tour = (rand() % (CAR_COUNT));//����0-����-1֮��������������ѡ���ĸ���·
	int node_num = tour_info[tour].node_num;
	int unneed[33][4];//�ű�tour�еķǱ�Ҫ���Ͷ���[tank][q][r][pos in tour_sub[tour]]
	int unneed_num = 0;
	int zz_left_all = 0;
	float zzl[CAR_COUNT][33][3] = { 0 };//�Ÿ�������װ������װ����[tour][pos]|||[tank][װ����][����װ��]
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
				float zzl_rate = tour_sub[t][n][1] / (ceil(tour_sub[t][n][1] / COMP_Q) * COMP_Q);//װ����
				int zz_left = (ceil(tour_sub[t][n][1] / COMP_Q) * COMP_Q) - tour_sub[t][n][1];//����
				zzl[t][n][0] = tour_sub[t][n][0];
				if (zzl_rate == 0)
				{
					zzl[t][n][1] = 1;
				}
				if (zzl_rate < 0.95)    //װ����
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
	{//û�зǱ�Ҫ����
		return cost;
	}
	else
	{//���ѡ��һ���Ǳ�Ҫ���Ͷ���
		int q_out = 0;

		while (unneed_num > 0 && tour_info[tour].node_num > 3)
		{
			int un = (rand() % (unneed_num));//ѡ���ĸ��Ǳ�Ҫ����
			int node_out = unneed[un][3];
			int tour_out = tour;
			int q_in_all = 0;
			q_out = tour_sub[tour_out][node_out][1];
			if (Qall - q_out + zz_left_all < HMIN)
			{//����֮�󲹲��������ǾͲ�����
				//Qall = Qall - q_out;
				break;
			}
			if (Qall - q_out < HMIN)
			{//����֮����Ҫ��������Ҫ���Ļ���ֱ��ɾ��
				int q_in_min = HMIN - Qall + q_out;//������Ҫ����Щ
				//�����ҵ�װ������С����
				int node_in = -1, tour_in = -1;
				while (q_in_all == 0 || Qall < HMIN)
				{//��û����
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
					//�ı�װ����
					int q_in_this = (q_in_min < q_out) ? q_in_min : q_out;//�������Ҫ�ӵ���
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
			//��tour��ɾ���������
			for (int rr = node_out; rr < tour_info[tour_out].node_num + 1; rr++)
			{
				tour_sub[tour][rr][0] = tour_sub[tour][rr + 1][0];
				tour_sub[tour][rr][1] = tour_sub[tour][rr + 1][1];
				tour_sub[tour][rr][2] = tour_sub[tour][rr + 1][2];
			}
			//��unneed��ɾ���������
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
			//����Ƿ񳬳�������Լ��.����1��˵����Υ��������0˵��Υ����
			int planisok = scheduling_con();
			if (planisok == 1)
			{//����Ƿ����㲻�ȴ�Լ��
				cal_plan_cost();
				if (cost < cost_best)
				{
					gettingbetter = 1;
					cost_best = cost;
					Qall_best = Qall;
					::memcpy(tour_sub_best, tour_sub, sizeof(tour_sub));
				}
			}

			//����
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
		//����ɱ�
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
	//ant ��ʼ��
	//cost = 0;
	tran_cost = 0;
	unload_cost = 0;
	delay_cost = 0;
	next_delay_cost = 0;
	//cur_tank_NO = 0;
	//cur_car_NO = 0;
	//cur_car_r = 0;
	//cur_time = 0;
	//c_info��ʼ��

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
	//tour_info��ʼ��
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
			tour_info[y].gc_need[t] = 0;						//����ÿ�η������ø���
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
	//��tour_sub�ĵ��ȷ���Ϊ�������������ɵĵ��ȷ����Ƿ񳬳�������Լ��������Լ��
	//����1��˵����Υ��������0˵��Υ����
	int planisok = 1;
	int visit_num[N_NODE_COUNT] = { 0 };//��¼ÿ��tank�������Ĵ���
	int tour_fin[CAR_COUNT] = { 0 };//��¼�����tour��0��û���ˣ�1������
	int tour_step[CAR_COUNT];
	int tour_fin_num = 0;//��¼���������tour����
	int stuck = 0;
	for (int c = 0; c < CAR_COUNT; c++)
	{
		tour_step[c] = 1;//��¼ÿ��tour���˼���
	}
	while (tour_fin_num < CAR_COUNT)
	{
		for (int car = 0; car < CAR_COUNT; car++)
		{
			if (tour_fin[car] == 0)
			{//�˳���û����
				int tank_to_go = tour_sub[car][tour_step[car]][0];
				int r_to_go = tour_sub[car][tour_step[car]][2];
				int q_to_go = tour_sub[car][tour_step[car]][1];
				int tank_now = tour_sub[car][tour_step[car] - 1][0];
 				if ((r_to_go == visit_num[tank_to_go] + 1) || (c_info[tank_to_go].t == 'd'))
				{//������������ͣ���һ�������ߣ��ı�״̬
					if (tank_to_go == 10)
					{
						int www = 0;
					}
					tour_step[car] = tour_step[car] + 1;
					if (tour_step[car] > 33)
					{//����4��
						return 0;
					}
					visit_num[tank_to_go] = visit_num[tank_to_go] + 1;
					stuck = 0;
					if (c_info[tank_to_go].t == 't')//��һ�ڵ����͹޽ڵ�
					{
						if (tank_to_go == 124)
						{
							int bbbb = 0;
						}
						//���ɵ��Ǳ���������ɵ���ϴ��뿪�ڵ�ʱ��ȡ�ϴ���Ǹ�
						float U_time_next = ((((c_info[tank_to_go].ini_stock - c_info[tank_to_go].tank_cap + c_info[tank_to_go].send_all + q_to_go) / c_info[tank_to_go].sell_v) + c_info[tank_to_go].dtime_all) > c_info[tank_to_go].recent_left_time) ? ((c_info[tank_to_go].ini_stock - c_info[tank_to_go].tank_cap + c_info[tank_to_go].send_all + q_to_go) / c_info[tank_to_go].sell_v) + c_info[tank_to_go].dtime_all : c_info[tank_to_go].recent_left_time;//���ɵ�
						float S_time_next = ((c_info[tank_to_go].ini_stock + c_info[tank_to_go].send_all) / c_info[tank_to_go].sell_v) + c_info[tank_to_go].dtime_all;//Oir��ȱ����,�������ǰ��ȱ����
						float a_time_next = tour_info[car].cur_time + dis[tank_now][tank_to_go] / CAR_V;//����ʱ��
						float l_time_next = q_to_go / SERVICE_RATE;//ж��ʱ��
						float w_time_next = (0 > U_time_next - a_time_next) ? 0 : U_time_next - a_time_next;//�ȴ�ʱ��
						float ser_time_next = a_time_next + w_time_next;
						float d_time_next = (0 > ser_time_next - S_time_next) ? 0 : ser_time_next - S_time_next;//�ٵ�ʱ��
						float left_time = a_time_next + l_time_next + w_time_next;
						//�����͹�c_info��Ϣ
						c_info[tank_to_go].send_all = c_info[tank_to_go].send_all + q_to_go;
						c_info[tank_to_go].S_time = ((c_info[tank_to_go].ini_stock + c_info[tank_to_go].send_all) / c_info[tank_to_go].sell_v) + c_info[tank_to_go].dtime_all;//��������һ�κ��ȱ����
						c_info[tank_to_go].r_now = c_info[tank_to_go].r_now + 1;
						c_info[tank_to_go].dtime_all = c_info[tank_to_go].dtime_all + d_time_next;
						//c_info[tank_to_go].order[c_info[tank_to_go].r_now - 1][0] = c_info[tank_to_go].r_now;
						c_info[tank_to_go].recent_left_time = left_time;
						int r_next = c_info[tank_to_go].r_now;
						c_info[tank_to_go].order[r_next - 1][0] = r_next;//r
						c_info[tank_to_go].order[r_next - 1][1] = q_to_go;//q
						c_info[tank_to_go].order[r_next - 1][2] = (ceil(q_to_go / COMP_Q));//���ø���
						c_info[tank_to_go].order[r_next - 1][3] = a_time_next;//����
						c_info[tank_to_go].order[r_next - 1][4] = U_time_next;
						c_info[tank_to_go].order[r_next - 1][5] = S_time_next;
						c_info[tank_to_go].order[r_next - 1][6] = d_time_next;
						c_info[tank_to_go].order[r_next - 1][7] = d_time_next * c_info[tank_to_go].sell_v * short_c;//�ٵ��ɱ�

						//�ж��Ƿ�sati

						//���³���tour��Ϣ
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
						tour_info[car].ser_time[tour_info[car].node_num] = (U_time_next > a_time_next) ? U_time_next : a_time_next;//��ʼ����ʱ��
						//float check = a_time_next + w_time_next;
						tour_info[car].node_num = tour_info[car].node_num + 1;
						//����������Ϣ
						//Qall = Qall + q_to_go;
						cur_tank_NO = tank_to_go;
						cur_time = left_time;
						cur_car_NO = car;
						//capacity = capacity - (floor(next_q / 5) + 1);
						//tour_sub[cur_car_NO][tour_info[cur_car_NO].node_num - 1][0] = next_tank;
						//tour_sub[cur_car_NO][tour_info[cur_car_NO].node_num - 1][1] = q_canbe[next_q];
						//tour_sub[cur_car_NO][tour_info[cur_car_NO].node_num - 1][2] = r_next;

					}
					if (c_info[tank_to_go].t == 'd')//��һ�ڵ����ͳ��ڵ�
					{
						if (tank_to_go == 9)
						{
							int www = 1;
						}
						float U_time_next = 0;//���ɵ�
						float S_time_next = 0;//Oir��ȱ����,������κ��²�����ȱ����
						float a_time_next = tour_info[car].cur_time + dis[tank_now][tank_to_go] / CAR_V;//����ʱ��
						float l_time_next = 0;//ж��ʱ��
						float w_time_next = 0;//�ȴ�ʱ��
						float d_time_next = 0;//�ٵ�ʱ��
						float left_time = a_time_next + LOAD_T;


						//���³���tour��Ϣ
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
						tour_info[car].ser_time[tour_info[car].node_num] = a_time_next;//��ʼ����ʱ��
						//float check = a_time_next + w_time_next;
						tour_info[car].node_num = tour_info[car].node_num + 1;
						//����������Ϣ
						//Qall = Qall + 0;
						cur_tank_NO = tank_to_go;
						cur_time = a_time_next + LOAD_T;
						//capacity = COMP_COUNT;
						//tour_sub[cur_car_NO][tour_info[cur_car_NO].node_num - 1][0] = next_tank;
						//tour_sub[cur_car_NO][tour_info[cur_car_NO].node_num - 1][1] = 0;
						//tour_sub[cur_car_NO][tour_info[cur_car_NO].node_num - 1][2] = 0;
					}
				}
				if (tour_sub[car][tour_step[car]][0] < 0)//�ж��������û����
				{
					tour_fin[car] = 1;
					tour_fin_num = tour_fin_num + 1;
				}
				stuck = stuck + 1;
				if (stuck > CAR_COUNT + 3)
				{//����
					planisok = 0;
					break;
				}
			}
		}
		if (stuck > CAR_COUNT + 3)
		{//����
			planisok = 0;
			break;
		}
	}
	//�ж��Ƿ�Υ��������Լ��
	for (int c = 0; c < CAR_COUNT; c++)
	{
		if (tour_info[c].cur_time - LOAD_T > B_END)
		{
			planisok = 0;
		}
		if (tour_info[c].lap > Max_tour_per_car)
		{
			planisok = 0;//����������Լ��
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
	//�����޸Ĺ���tour�ķ����Ϳ��ʱ������
	int tour_without0[33][3] = { -1 };
	int tour_without0_num = 0;
	for (int n = 0; n < 18; n++)
	{//����û���Ϳ�Ĵ��͹޶�������˳��
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
	int putin_pos = 1;//����tour��λ��
	int putin_num = 0;//�����˵ڼ���������
	while (putin_pos < 18)
	{
		int gc_need = ceil(tour_without0[putin_num][1] / COMP_Q);
		if (gc_num >= gc_need && putin_num < tour_without0_num)
		{//װ����
			gc_num = gc_num - gc_need;
			::memcpy(tour_sub[tour][putin_pos], tour_without0[putin_num], sizeof(tour_without0[putin_num]));
			putin_pos = putin_pos + 1;
			putin_num = putin_num + 1;
		}
		else if (putin_num < tour_without0_num)
		{
			//װ����
			tour_sub[tour][putin_pos][0] = 0;
			tour_sub[tour][putin_pos][1] = 0;
			tour_sub[tour][putin_pos][2] = 0;
			gc_num = 3;
			putin_pos = putin_pos + 1;
		}
		if (putin_num > tour_without0_num)
		{//ȫ�������ˣ�������-1
			tour_sub[tour][putin_pos][0] = -1;
			tour_sub[tour][putin_pos][1] = -1;
			tour_sub[tour][putin_pos][2] = -1;
			putin_pos = putin_pos + 1;
		}
		if (putin_num == tour_without0_num)
		{//ȫ�������ˣ���󷵻ؽڵ�
			tour_sub[tour][putin_pos][0] = 0;
			tour_sub[tour][putin_pos][1] = 0;
			tour_sub[tour][putin_pos][2] = 0;
			putin_num = putin_num + 1;
			putin_pos = putin_pos + 1;
		}

	}
	//����nodenum
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

inline int ant::insert(int ins, int tour_ins, int out, int tour_out)//ins�ǲ���λ�ã�tour_ins�Ǳ�����tour��out�Ƕ���ԭ��λ�ã�tour_out�Ƕ���ԭ��tour
{
	int order_ins[3];//�����Ҫ����Ķ���
	::memcpy(order_ins, tour_sub[tour_out][out], sizeof(tour_sub[tour_out][out]));//Ҫ����Ķ���copy����
	for (int rr = out; rr < tour_info[tour_out].node_num + 1; rr++)
	{
		tour_sub[tour_out][rr][0] = tour_sub[tour_out][rr + 1][0];
		tour_sub[tour_out][rr][1] = tour_sub[tour_out][rr + 1][1];
		tour_sub[tour_out][rr][2] = tour_sub[tour_out][rr + 1][2];
	}
	int r_should = 0;
	int gowrong = 0;

	for (int w = tour_info[tour_ins].node_num + 1; w > ins; w--)
	{//ins��Ľڵ㶼���һλ
		tour_sub[tour_ins][w][0] = tour_sub[tour_ins][w - 1][0];
		tour_sub[tour_ins][w][1] = tour_sub[tour_ins][w - 1][1];
		tour_sub[tour_ins][w][2] = tour_sub[tour_ins][w - 1][2];
	}
	//����delay����
	tour_sub[tour_ins][ins][0] = order_ins[0];
	tour_sub[tour_ins][ins][1] = order_ins[1];
	tour_sub[tour_ins][ins][2] = order_ins[2];
	//����tour_ins�����޲����Ϸ���Լ����
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
		//��λ�ã�������

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