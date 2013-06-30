#ifdef fft_lib
#define fft_lib
void init(int* tfstoreA,int* tfstoreB,int* brLookup);
void fft(int* dataA, int* dataB,int* wA,int* wB,int* bitRLocations);
#endif