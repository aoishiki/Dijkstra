/** datファイルの形式
	場所番号, x座標, y座標,交差点の位置,繋がっている交差点の数,繋がっている交差点の番号
	という形式で1番から順に改行して記述している
*/


#include <stdio.h>
#include <math.h>
#include "que.h"
#define point_max 36
#define inf 10000000
#define lpmax 5
#define checked 1

int st_point[point_max] = { 0 };

// ���W
typedef struct coordinate {
	double x, y;
}coordinate;

typedef struct location {
	int this_point_num;//��_�ԍ�
	coordinate p;
	char name[30];//��_��

	int lpnum;//�Ȃ����Ă����_�̐�
	int link_point[lpmax]; //�Ȃ����Ă����_�ԍ�
	double route_dist;	//�X�^�[�g���炱�̓_�Ɏ���ŒZ�̋���
	int back_point; //���̒n�_�Ɏ����_�ԍ�
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

		// �����̏�����
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

	// �Ȃ����Ă����_��ׂĒ��ׂ�̂�push����
	for (i = 0; i < point[current_point].lpnum; i++) {
		int lp = point[current_point].link_point[i];
		if(ischeck(lp) == false)
			q_push(q_search, lp);
	}
	
	for (;;) {
		sp = q_pop(q_search);								// ���݂̌�_�Ɛڑ����Ă����_
		coordinate xy_p = point[sp].p;						// �ڑ����Ă����_�̍��W
		coordinate xy_p_cur = point[current_point].p;		// ���ݎQ�Ƃ��Ă����_�̍��W
		double p_dist = dist(xy_p_cur, xy_p);				// �ڑ�������_�ƌ��݂̌�_�̋���

		// ��菬�����o�Hp_dist�ł���΂�������������
		if (point[sp].route_dist > p_dist) {
			/*
			* ���ݎQ�Ƃ��Ă����_���X�^�[�g�n�_�Ȃ�
			* ������p_dist��������ȊO�͂����܂ł̌o�H�̒���
			* point[search_point].route_dist����Z���������ɂȂ�
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
	int q_point[qmax] = { 0 };		//�T���҂��̌�_�ԍ�

	printf("start & goal >>");
	scanf("%d %d",&st_p,&go_p);
	
	// �����_������
	int search_point = st_p;	//�T�����̌�_�ԍ�
	point[search_point].route_dist = 0;

	do {
		//�ŒZ�o�H���
		if (search_point != go_p) {

			set_dist(search_point);

			//�d�݂����������玟�ɎQ�Ƃ���_��queue��push
			//�����_���܂܂�Ă��Ȃ����push
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
			// �ŒZ�o�H��o��
			while (search_point != st_p) {
				printf("%d <-", search_point);
				search_point = point[search_point].back_point;
			}
			printf(" %d\n", search_point);
			break;
		}
	} while (1);
}
