#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#define PATH "/tmp"
#define ID 2

static const int mut = 0; //the index in semaphore array
static const int empty = 1;
static const int full = 2;

char* GetShMem(size_t bsize) {
	int key = ftok (PATH, ID);
	if (key < 0) {
		return NULL;
	}
	int shmid = shmget(key, bsize, 0666 | IPC_CREAT);
	if (shmid < 0) {
		return NULL;
	}
	errno = 0;
	char *shmem = shmat(shmid, 0, 0);
	return errno == 0 ? shmem : NULL;
}

int GetSemID() {
	int key = ftok (PATH, ID);
	if (key < 0) {
		return key;
	}
	int semid = semget(key, 3, 0666 | IPC_CREAT); // sem0 for mutex, sem1 for empty, sem2 for full
	return semid;
}

int ProducerInitSems (int semid) {
	int err;
	unsigned short values [3] = {1, 1, 0};
	err = semctl(semid, 0, SETALL, values);
	if (err != 0) {
		return err;
	}

	struct sembuf init [2];
	init [0] =  (struct sembuf) {full, 2, 0};
	init [1] = (struct sembuf) {full, -2, SEM_UNDO};
	return semop(semid, init, 2);
	return 0;
}

int ConsumerInitSems (int semid) {
	struct sembuf init [2];
	init[0] = (struct sembuf) {empty, 2, 0};
	init[1] =  (struct sembuf) {empty, -2, SEM_UNDO};
	return semop(semid, init, 2);
	return 0;
}

int ProducerEnter(int semid) {
	struct sembuf enter[2];
	enter[0] = (struct sembuf) {empty, -1, 0};
	enter[1] = (struct sembuf) {mut, -1, 0};

	int res= semop(semid, enter, 2);

	unsigned short values [3];
	int err = semctl(semid, 0, GETALL, values);
	printf ("pr enter mut %d | empty %d | full %d | id %d\n", values[0], values[1], values [2], semid);

	return res;
}

int ProducerExit(int semid) {
	struct sembuf ext[2];
	ext[0] = (struct sembuf) {mut, 1, 0};
	ext[1] = (struct sembuf) {full, 1, 0};

	int res =  semop(semid, ext, 2);
	
	unsigned short values [3];
	int err = semctl(semid, 0, GETALL, values);
	printf ("pr exit mut %d | empty %d | full %d | id %d\n", values[0], values[1], values [2], semid);
	if (values[2] > 1) {
		return 1;
	}

	return res;
}

int ConsumerEnter(int semid) {
	struct sembuf enter[2];
	enter[0] = (struct sembuf) {full, -1, 0};
	enter[1] = (struct sembuf) {mut, -1, 0};

	int res = semop(semid, enter, 2);
	unsigned short values [3];
	int err = semctl(semid, 0, GETALL, values);
	printf ("con enter mut %d | empty %d | full %d | id %d\n", values[0], values[1], values [2], semid);
	return res;
}

int ConsumerExit(int semid) {
	struct sembuf ext[2];
	ext[0] = (struct sembuf) {mut, 1, 0};
	ext[1] = (struct sembuf) {empty, 1, 0};

	int res = semop(semid, ext, 2);
	unsigned short values [3];
	int err = semctl(semid, 0, GETALL, values);
	printf ("con exit mut %d | empty %d | full %d | id %d\n", values[0], values[1], values [2], semid);
	if (values[1] > 1) {
		return 1;
	}
	return res;
}