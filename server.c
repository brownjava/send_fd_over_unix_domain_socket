#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

void send_fd(int socket, int fd) {
    struct msghdr msg = { 0 };
    char buf[CMSG_SPACE(sizeof(fd))];
    memset(buf, '\0', sizeof(buf));
    
    struct iovec io = { .iov_base = "send_socket", .iov_len = 12 };

    msg.msg_iov = &io;
    msg.msg_iovlen = 1;
    msg.msg_control = buf;
    msg.msg_controllen = sizeof(buf);

    struct cmsghdr * cmsg = CMSG_FIRSTHDR(&msg);
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    cmsg->cmsg_len = CMSG_LEN(sizeof(fd));

    *((int *) CMSG_DATA(cmsg)) = fd;

    msg.msg_controllen = CMSG_SPACE(sizeof(fd));

    if (sendmsg(socket, &msg, 0) < 0) {
        fprintf(stderr, "Failed to send message\n");
    }
}

int main(int argc, char **argv) {
    struct sockaddr_un addr;

    int sfd = socket(AF_UNIX, SOCK_STREAM, 0);

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, "/tmp/sock", sizeof(addr.sun_path) - 1);

    if (remove("/tmp/sock") == -1 && errno != ENOENT) {
        perror("remove");
        return -1;
    }

    if (bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1) {
        perror("bind");
        return -1;
    }

    if (listen(sfd, 10) == -1) {
        perror("listen");
        return -1;
    }

    while(true) {
        int cfd = accept(sfd, NULL, NULL);

        printf("New client connected!\n");

        send_fd(cfd, 1);

        close(cfd);
    }

    return 0;
}
