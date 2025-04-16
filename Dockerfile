# 使用较小的 Debian 镜像作为基础
FROM debian:bullseye

# 清空所有已有源配置，再写入清华源
RUN rm -f /etc/apt/sources.list.d/* && \
    echo "deb http://mirrors.tuna.tsinghua.edu.cn/debian bookworm main contrib non-free\n\
deb http://mirrors.tuna.tsinghua.edu.cn/debian bookworm-updates main contrib non-free\n\
deb http://mirrors.tuna.tsinghua.edu.cn/debian-security bookworm-security main contrib non-free" \
> /etc/apt/sources.list

# 安装 gcc 和必要工具
RUN apt update && \
    apt install -y build-essential && \
    apt clean

# 拷贝测试程序源码到容器中
WORKDIR /app
COPY memory_test.c .
COPY run.sh .

# 编译测试程序
RUN gcc memory_test.c -o memory_test -O0

# 设置默认启动命令
ENTRYPOINT ["./run.sh"]