
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

int createMsgQ(int key, bool attachIfExists);
void destroyMsgQ(int msgqid);
void getMessage(int msgid, struct paziente *msg, long msgtype);
void sendMessage(int msgid, char* msgp, size_t length);

int getRand(int min, int max);

