#include<iostream>
#include"set.h"
#include<fstream>
#include<string>
#include<sstream>  


using namespace std;
tank c_info_ini [N_NODE_COUNT];
tour tour_info_ini[CAR_COUNT];
short int phe_table[N_NODE_COUNT][N_NODE_COUNT][R_MAX][R_MAX][Q_CANBE_COUNT][Q_CANBE_COUNT];					//信息素表
double dis[N_NODE_COUNT][N_NODE_COUNT];							//距离表
int num_of_station;												//记录加油站的个数
int num_of_tank;												//记录油罐的个数
float Car[4];													//油罐车信息
int q_canbe[Q_CANBE_COUNT];



int get_info()
{
	int i = 0;
	int j = 0;
	num_of_station = 0;
	num_of_tank = 0;
	int temp_arr[27] = { 8000, 8500, 9000, 9500, 10000,16000,16500,17000,17500,18000, 18500, 19000, 19500, 20000,24000,24500,25000,25500,26000,26500,27000,27500, 28000, 28500, 29000, 29500, 30000 };
	::memcpy(q_canbe, temp_arr, sizeof(temp_arr));

	{//输入算例节点信息与车辆信息
		ifstream infile("h_45%.txt", ios::in);
		int i = 0;
		int j = 0;
		if (!infile.is_open())
		{//没有打开文件，报错退出
			cout << "Error: opening file fail" << endl;
			exit(1);
		}
		while (!infile.eof() && i < N_NODE_COUNT)//输入节点信息
		{
			infile >> c_info_ini[i].t >> c_info_ini[i].oil >> c_info_ini[i].station >> c_info_ini[i].x >> c_info_ini[i].y >> c_info_ini[i].ini_stock >>  c_info_ini[i].tank_cap >> c_info_ini[i].sell_v;
			i++;
		}
		while (!infile.eof() && j < 4)//输入油罐车参数，隔舱数，隔舱容量，今日配送量上下限
		{
			infile >> Car[j];
			j++;
		}
		infile.close();//读取完成之后关闭文件
		//根据算例修改，mark
		
		//num_of_station = 4;
		//num_of_tank = 10;

	}
	//初始化油罐节点
	
	for (int i = 0; i < N_NODE_COUNT; i++)
	{
		if (c_info_ini[i].t=='t')
		{
			c_info_ini[i].send_all = 0;
			c_info_ini[i].id = i;
			c_info_ini[i].recent_left_time = 0;
			c_info_ini[i].dtime_all = 0;
			c_info_ini[i].next_dtime =0 ;
			c_info_ini[i].rmax = floor((c_info_ini[i].tank_cap - c_info_ini[i].ini_stock + B_END * c_info_ini[i].sell_v) / (0.8 * Car[1]));
			c_info_ini[i].S_time = c_info_ini[i].ini_stock / c_info_ini[i].sell_v;
			c_info_ini[i].r_now = 0;
			for (int j = 0; j < 10; j++)
			{
				for (int u = 0; u < 8; u++)
				{
					c_info_ini[i].order[j][u] = 0;
				}
			}
		}
		if (c_info_ini[i].t == 'd')
		{
			c_info_ini[i].send_all = 0;
			c_info_ini[i].id = i;
			c_info_ini[i].recent_left_time = 0;
			c_info_ini[i].dtime_all = 0;
			c_info_ini[i].next_dtime = 0;
			c_info_ini[i].rmax = 1000;
			c_info_ini[i].S_time = 0;
			c_info_ini[i].r_now = 0;
			for (int j = 0; j < 10; j++)
			{
				for (int u = 0; u < 8; u++)
				{
					c_info_ini[i].order[j][u] = 0;
				}
			}
		}
	}
	//初始化tour
	for (int j = 0; j < CAR_COUNT; j++)
	{
		tour_info_ini[j].car_id = 0;
		tour_info_ini[j].cur_node_id = 0;
		tour_info_ini[j].cur_q = 0;                               //在cur_node_id的配送量
		tour_info_ini[j].cur_time = 0.5;
		tour_info_ini[j].lap = 0;
		tour_info_ini[j].useable = 1;                            //车可用状态
		tour_info_ini[j].gc_left = 3;							//剩余可用隔舱
		tour_info_ini[j].node_num = 1;
		for (int l = 0; l < 18; l++)
		{
			tour_info_ini[j].node[l] = -1;
			if (l == 0)
			{
				tour_info_ini[j].node[l] = 0;
			}
			tour_info_ini[j].gc_need[l] = 0;						//保存每次访问所用隔舱
			//tour_info[ant][j].unnece[l] = -1;							//记录每个配送是否是必要配送，必要是0.非必要是1
			tour_info_ini[j].r[l] = -1;
			tour_info_ini[j].q[l] = 0;
			tour_info_ini[j].a_time[l] = 0;
			tour_info_ini[j].U_time[l] = 0;
			tour_info_ini[j].S_time[l] = 0;
			tour_info_ini[j].d_time[l] = 0;
			tour_info_ini[j].ser_time[l] = 0;
		}
	}
	
	//初始化信息素表//重新写！Oirq1-Ojsq2
	for (int a = 0; a < N_NODE_COUNT; a++)
	{//i
		for (int b = 0; b < N_NODE_COUNT; b++)
		{//j
			for (int c = 0; c < R_MAX; c++)
			{//r
				for (int d = 0; d < R_MAX; d++)
				{//s
					for (int e = 0; e <Q_CANBE_COUNT; e++)
					{//q1
						for (int f = 0; f < Q_CANBE_COUNT; f++)
						{//q2
							phe_table[a][b][c][d][e][f] = INIT_PHE;
						}
					}
				}
			}
		}
	}
	//坐标的初始化距离表，适用于小中算例
	
	/*for (int i = 0; i < N_NODE_COUNT; i++)
	{
		for (int j = 0; j < N_NODE_COUNT; j++)
		{
			if (i == j)
				dis[i][j] = 0;
			double x, y;
			x = c_info_ini[i].x - c_info_ini[j].x;
			y = c_info_ini[i].y - c_info_ini[j].y;
			dis[i][j] = sqrt(pow(x, 2) + pow(y, 2));
			dis[j][i] = dis[i][j];
			if (dis[i][j] < 0)
				cout << i << " " << j << " " << dis[i][j] << endl;
		}
	}*/
	
	//经纬度的初始化距离表，适用于实际算例
	
	for (int i = 0; i < N_NODE_COUNT; i++)
	{
		for (int j = 0; j < N_NODE_COUNT; j++)
		{
			if (i == j)
				dis[i][j] = 0;
			double ix, iy,jx,jy;
			ix = c_info_ini[i].x;
			jx = c_info_ini[j].x;
			iy = c_info_ini[i].y;
			jy = c_info_ini[j].y;
			dis[i][j] = RealDistance(ix,iy,jx,jy)/1000;
			dis[j][i] = dis[i][j];
			if (dis[i][j] < 0)
				cout << i << " " << j << " " << dis[i][j] << endl;
		}
	}
	



	return 1;
}

double rad(double d)
{
	return d * pi / 180.0;
}
double RealDistance(double lat1, double lng1, double lat2, double lng2)//lat1第一个点纬度,lng1第一个点经度,lat2第二个点纬度,lng2第二个点经度
{

	double a;
	double b;
	double radLat1 = rad(lat1);
	double radLat2 = rad(lat2);
	a = radLat1 - radLat2;
	b = rad(lng1) - rad(lng2);
	double s = 2 * asin(sqrt(pow(sin(a / 2), 2) + cos(radLat1) * cos(radLat2) * pow(sin(b / 2), 2)));
	s = s * EARTH_RADIUS;
	s = s * 1000;
	return s;
}