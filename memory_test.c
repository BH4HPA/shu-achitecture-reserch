#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define CHUNK_SIZE_MB 64

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

    const long ints_per_mb = (1024L * 1024L) / sizeof(int);
    long total_ints = array_size_mb * ints_per_mb;
    long chunk_size_ints = CHUNK_SIZE_MB * ints_per_mb;
    long num_chunks = array_size_mb / CHUNK_SIZE_MB;

    if (array_size_mb % CHUNK_SIZE_MB != 0) {
        fprintf(stderr, "错误：array-size 必须是 %dMB 的整数倍。\n", CHUNK_SIZE_MB);
        return 1;
    }

    int **chunks = malloc(num_chunks * sizeof(int*));
    if (!chunks) {
        perror("分配 chunks 指针数组失败");
        return 1;
    }

    for (long i = 0; i < num_chunks; ++i) {
        chunks[i] = malloc(chunk_size_ints * sizeof(int));
        if (!chunks[i]) {
            perror("分配 chunk 失败");
            return 1;
        }
    }

    if (verbose) {
        printf("已申请 %ld MB（%ld 个 int 元素，分成 %ld 个 chunk，每个 %dMB）\n",
               array_size_mb, total_ints, num_chunks, CHUNK_SIZE_MB);
        printf("即将执行 %ld 次随机访问...\n", num_accesses);
    }

    srand(time(NULL));

    long start_time = get_nanotime();

    for (long i = 0; i < num_accesses; i++) {
        long index = rand() % total_ints;
        long chunk_idx = index / chunk_size_ints;
        long inner_idx = index % chunk_size_ints;
        chunks[chunk_idx][inner_idx]++;
    }

    long end_time = get_nanotime();
    double elapsed_sec = (end_time - start_time) / 1e9;
    printf("实际运行时间: %.4f 秒\n", elapsed_sec);

    for (long i = 0; i < num_chunks; ++i) {
        free(chunks[i]);
    }
    free(chunks);

    return 0;
}