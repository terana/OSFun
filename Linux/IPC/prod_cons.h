#define BSIZE 1000

char* GetShMem(size_t bsize);
int GetSemID();
int ConsumerInitSems (int semid);
int ProducerInitSems (int semid);
int ProducerEnter(int semid);
int ProducerExit(int semid);
int ConsumerEnter(int semid);
int ConsumerExit(int semid);