#include <stdio.h>
#include <math.h>
#include "que.h"
#define point_max 36
#define inf 10000000
#define lpmax 5
#define checked 1

int st_point[point_max] = { 0 };

// 座標
typedef struct coordinate {
	double x, y;
}coordinate;

typedef struct location {
	int this_point_num;//交差点番号
	coordinate p;
	char name[30];//交差点名

	int lpnum;//つながっている交差点の数
	int link_point[lpmax]; //つながっている交差点番号
	double route_dist;	//スタートからこの点に至る最短の距離
	int back_point; //この地点に至る交差点番号
}Location;
Location point[point_max];

void read_data(char *filename) {
	int i, j;
	FILE *fp;
	fp = fopen(filename, "r");
	if (fp == NULL)
		printf("can't open\n");

	for (i = 1; i < point_max; i++) {
		fscanf(fp, "%d ,%lf ,%lf , %[^,], %d", &point[i].this_point_num, \
			&point[i].p.x, &point[i].p.y, \
			point[i].name, &point[i].lpnum);

		for (j = 0; j < point[i].lpnum; j++) {
			fscanf(fp, ", %d", &point[i].link_point[j]);
		}

		// 距離の初期化
		point[i].route_dist = inf;
		point[i].back_point = 0;

	}
	fclose(fp);
}

double dist(coordinate p1, coordinate p2) {
	double x, y;
	x = p1.x - p2.x;
	y = p1.y - p2.y;
	return sqrt(pow(x, 2) + pow(y, 2));
}

bool ischeck(int point) {
	int i;
    if(st_point[point] == checked)
        return true;
    return false;
}

void set_dist(int current_point) {
	int i,j,sp;
	int q_search[qmax] = { 0 };

	// つながっている交差点をすべて調べるのでpushする
	for (i = 0; i < point[current_point].lpnum; i++) {
		int lp = point[current_point].link_point[i];
		if(ischeck(lp) == false)
			q_push(q_search, lp);
	}
	
	for (;;) {
		sp = q_pop(q_search);								// 現在の交差点と接続している交差点
		coordinate xy_p = point[sp].p;						// 接続している交差点の座標
		coordinate xy_p_cur = point[current_point].p;		// 現在参照している交差点の座標
		double p_dist = dist(xy_p_cur, xy_p);				// 接続した交差点と現在の交差点の距離

		// より小さい経路p_distであればそっちを代入する
		if (point[sp].route_dist > p_dist) {
			/*
			* 現在参照している交差点がスタート地点なら
			* 距離はp_distだがそれ以外はそこまでの経路の長さ
			* point[search_point].route_distを加算した距離になる
			*/
			point[sp].route_dist = p_dist;
			point[sp].back_point = point[current_point].this_point_num;
			if (point[sp].route_dist != 0) {
				point[sp].route_dist += point[current_point].route_dist;
			}
		}
		if (q_search[0] == 0){
		    st_point[current_point] = checked;
			break;
		}
	}
}


int main(void) {
	read_data("./map.dat");
	
	int st_p, go_p,i,j;
	int q_point[qmax] = { 0 };		//探索待ちの交差点番号

	printf("start & goal >>");
	scanf("%d %d",&st_p,&go_p);
	
	// 検索点初期化
	int search_point = st_p;	//探索中の交差点番号
	point[search_point].route_dist = 0;

	do {
		//最短経路を検索
		if (search_point != go_p) {

			set_dist(search_point);

			//重みをつけおわったら次に参照する点をqueueにpush
			//同じ点が含まれていなければpush
			for (i = 0; i < point[search_point].lpnum; i++) {
				int lp = point[search_point].link_point[i];
				if(isContents(q_point,lp) == false)
					q_push(q_point, lp);
			}
			search_point = q_pop(q_point);
			if (search_point == go_p)
				continue;
		}
		else {
			// 最短経路を出力
			while (search_point != st_p) {
				printf("%d <-", search_point);
				search_point = point[search_point].back_point;
			}
			printf(" %d\n", search_point);
			break;
		}
	} while (1);
}
