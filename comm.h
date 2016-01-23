
//typedef enum {false, true} bool; 

// non compila su mac
#if defined(__linux__)
union semun {
        int val;
        struct semid_ds* buf;
        unsigned short* array;
        #if defined(__linux__)
        struct seminfo* __buf;
        #endif
};
#endif


int createSem(int key, int num);
void destroySem(int semid);
void initSem(int semid, int semnum, int val);
void semReserve(int semid, int semnum);
void semRelease(int semid, int semnum);
int semGetVal(int semid, int semnum);

int createMsgQ(int key, bool attachIfExists);
void destroyMsgQ(int msgqid);
void sendMessage(int msgid, void *msg, int msgSize);
bool recvMessage(int msgid, void *msg, int msgSize, long msgtype);

void waitAllChild();
int getRand(int min_num, int max_num);

