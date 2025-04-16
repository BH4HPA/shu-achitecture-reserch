#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// 获取当前时间（纳秒）
long get_nanotime() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts); // 更稳定，适合性能测量
    return ts.tv_sec * 1000000000L + ts.tv_nsec;
}

void usage(const char* progname) {
    printf("用法: %s --array-size <MB> --num-accesses <次数> [--verbose]\n", progname);
}

int main(int argc, char* argv[]) {
    long array_size_mb = 0;
    long num_accesses = 0;
    int verbose = 0;

    // 参数解析
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--array-size") == 0 && i + 1 < argc) {
            array_size_mb = atol(argv[++i]);
        } else if (strcmp(argv[i], "--num-accesses") == 0 && i + 1 < argc) {
            num_accesses = atol(argv[++i]);
        } else if (strcmp(argv[i], "--verbose") == 0) {
            verbose = 1;
        } else {
            usage(argv[0]);
            return 1;
        }
    }

    if (array_size_mb <= 0 || num_accesses <= 0) {
        usage(argv[0]);
        return 1;
    }

    long array_size = array_size_mb * 1024 * 1024 / sizeof(int);
    volatile int* array = malloc(array_size * sizeof(int));
    if (!array) {
        perror("内存申请失败");
        return 1;
    }

    if (verbose) {
        printf("已申请 %ld MB (%ld 个 int 元素)\n", array_size_mb, array_size);
        printf("即将执行 %ld 次随机访问...\n", num_accesses);
    }

    srand(time(NULL));  // 初始化随机数

    long start_time = get_nanotime();

    for (long i = 0; i < num_accesses; i++) {
        int index = rand() % array_size;
        array[index] += 1;
    }

    long end_time = get_nanotime();
    double elapsed_sec = (end_time - start_time) / 1e9;

    printf("实际运行时间: %.4f 秒\n", elapsed_sec);

    // 清理
    free(array);
    return 0;
}