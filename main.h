#ifndef MAIN_H
#define MAIN_H

#pragma once

#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/ioctl.h>

struct processInfo {
    int pid;
    double cpu;
    // std::string time;
    double mem;
    std::string comm;
};

void setRawMode(bool enable) {
    static struct termios oldt, newt;
    if (enable) {
        if (tcgetattr(STDIN_FILENO, &oldt) == -1) return;
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    }
    else {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }
}

bool kbhit() {
    timeval tv = {0, 0};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO+1, &fds, NULL, NULL, &tv) > 0;
}

int getTermSize() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;
}

#endif //MAIN_H