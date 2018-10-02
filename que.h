
#define qmax 1000
typedef enum { true = 1, false = 0 } bool;

bool isEmpty(int *queue) {
	int i;
	for (i = 0; i < qmax; i++) {
		if (queue[i] != 0)
			return false;
	}
	return true;
}

bool isFull(int *queue) {
	int i;
	for (i = 0; i < qmax; i++) {
		if (queue[i] == 0)
			return false;
	}
	return true;
}

int q_pop(int *queue) {
	if (isEmpty(queue) == true)
		return 0;
	int p_num = queue[0];
	int i;
	for (i = 0; i < qmax; i++) {
		queue[i] = queue[i + 1];
	}
	queue[qmax] = 0;
	return p_num;
}

void q_push(int *queue, int num) {
	if (isFull(queue) != true) {
		int i;
		for (i = 0; i < qmax; i++) {
			if (queue[i] == 0) {
				queue[i] = num;
				break;
			}
		}
	}
}

bool isContents(int *queue, int num) {
	int j;
	for (j = 0; j < qmax; j++) {
		if (queue[j] == num)
			return true;
	}
	return false;
}

