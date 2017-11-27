#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <pthread.h>

struct thread_arg {
    long long toss, cnt;
};

void *func (void *args) {
    unsigned seed = rand();
    long long toss = ((thread_arg *)args)->toss;
    long long cnt = 0;
    long long limit = RAND_MAX * 1L * RAND_MAX;
    for (long long i = 0; i < toss; ++i) {
        long long x = rand_r(&seed);
        long long y = rand_r(&seed);
        if (x * x + y * y <= limit) {
            cnt++;
        }
    }
    ((thread_arg *)args)->cnt = cnt;
    return NULL;
}

int main(int argc, char const *argv[]) {
    if (argc != 3) {
        puts("Usage: ./pi <#thread> <#toss>");
        return 1;
    }
    srand(time(NULL));
    int thread_num;
    int toss_count;
    sscanf(argv[1], "%d", &thread_num);
    sscanf(argv[2], "%d", &toss_count);

    pthread_t *threads = new pthread_t [thread_num];
    thread_arg *args   = new thread_arg [thread_num];

    for (int i = 0; i < thread_num; ++i) {
        args[i].toss = toss_count / thread_num;
        pthread_create(&threads[i], NULL, func, &args[i]);
    }
    for (int i = 0; i < thread_num; ++i) {
        pthread_join(threads[i], NULL);
    }
    long long total_in_circle = 0;
    for (int i = 0; i < thread_num; ++i) {
        total_in_circle += args[i].cnt;
    }
    printf("%4f\n", total_in_circle * 4.0f / toss_count);

    delete [] threads;
    delete [] args;

    return 0;
}
