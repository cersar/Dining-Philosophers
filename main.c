/*
 * main.c
 *
 *  Created on: Jun 17, 2018
 *      Author: yangsen
 */

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

void pickup_forks(int );
void putdown_forks(int );
void *runner(void *);
void test(int i);

pthread_mutex_t mutex;
pthread_cond_t cond_var[5];
enum {
	THINKING, HUNGRY, EATING
} state[5];

int main() {
	int index[5];
	srand(time(NULL));

	pthread_t tid[5]; /* the thread identifier */
	pthread_attr_t attr; /* set of thread attributes */

	pthread_mutex_init(&mutex, NULL);

	pthread_attr_init(&attr);

	for (int i = 0; i < 5; i++) {
		state[i] = THINKING;
		pthread_cond_init(&cond_var[i], NULL);
	}
	for (int i = 0; i < 5; i++) {
		index[i] = i;
		pthread_create(&tid[i], &attr, runner, &index[i]);
	}
	for (int i = 0; i < 5; i++) {
		pthread_join(tid[i], NULL);
	}
	return 0;
}

void *runner(void *args) {
	int ind = *((int *) args), time;
	int cnt = 0;
	while (cnt < 2) {
		cnt++;
		printf("哲学家%d进行第%d次思考...\n", ind, cnt);
		time = rand() % 20 + 1;
		sleep(time);
		printf("哲学家%d结束第%d次思考，共%d秒\n", ind, cnt, time);
		printf("哲学家%d进入饥饿状态...\n", ind);
		pickup_forks(ind);
		printf("哲学家%d拿起筷子进行第%d次就餐...\n", ind, cnt);
		time = rand() % 20 + 1;
		sleep(time);
		printf("哲学家%d结束第%d次就餐，共%d秒\n", ind, cnt, time);
		putdown_forks(ind);
		printf("哲学家%d放下筷子\n", ind);

	}
	pthread_exit(0);
}
//拿起筷子，首先判断第i个哲学家是否可以就餐，如果可以，则将其状态改为EATING并进行就餐；否则等待邻座结束就餐，放下筷子。
void pickup_forks(int i) {
	pthread_mutex_lock(&mutex);
	state[i] = HUNGRY;
	test(i);
	while (state[i] != EATING){
		printf("哲学家%d等待筷子可用...\n",i);
		pthread_cond_wait(&cond_var[i], &mutex);
	}
	pthread_mutex_unlock(&mutex);

}

//判断第i个哲学家是否可以就餐。如果可以，则将其状态改为EATING。
void test(int i) {
	if ((state[(i + 4) % 5] != EATING) && (state[i] == HUNGRY)
			&& (state[(i + 1) % 5] != EATING)) {
		state[i] = EATING;
		pthread_cond_signal(&cond_var[i]);
	}
}

//放下筷子，即改变第i个哲学家的状态为THINKING，判断邻座的两个哲学家是否可以就餐，如果可以，则将其状态改为EATING。
void putdown_forks(int i) {
	pthread_mutex_lock(&mutex);
	state[i] = THINKING;
	test((i + 4) % 5);
	test((i + 1) % 5);
	pthread_mutex_unlock(&mutex);

}
