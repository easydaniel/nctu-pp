#include <cstdio>
#include <vector>
#include <pthread.h>
#include <ctime>
#include <cstdlib>

using namespace std;

struct thread_arg {
  int id;
  long long toss;
};

vector<thread_arg> args;
vector<long long> in_circle;

void *func (void *args) {
  srand(time(NULL));
  unsigned seed = rand();
  for (long long i = 0; i < ((thread_arg *)args)->toss; ++i) {
    double x = double(rand_r(&seed)) / RAND_MAX;
    double y = double(rand_r(&seed)) / RAND_MAX;
    if (x * x + y * y <= 1) {
      in_circle[((thread_arg *)args)->id]++;
    }
  }
  return NULL;
}

int main(int argc, char const *argv[]) {
  if (argc != 3) {
    puts("Usage: ./pi <#thread> <#toss>");
    return 1;
  }
  int thread_num;
  int toss_count;
  sscanf(argv[1], "%d", &thread_num);
  sscanf(argv[2], "%d", &toss_count);

  vector<pthread_t> threads(thread_num);
  in_circle = vector<long long>(thread_num, 0);
  for (int i = 0; i < thread_num; ++i) {
    args.push_back({i, toss_count / thread_num});
    pthread_create(&threads[i], NULL, func, &args[i]);
  }
  for (int i = 0; i < thread_num; ++i) {
    pthread_join(threads[i], NULL);
  }
  long long total_in_circle = 0;
  for (int i = 0; i < thread_num; ++i) {
      total_in_circle += in_circle[i];
  }
  printf("%4f\n", total_in_circle * 4.0f / toss_count);
  pthread_exit(NULL);
  return 0;
}
