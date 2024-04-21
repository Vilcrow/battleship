#ifndef WRITE_LOG_H
#define WRITE_LOG_H

#include <stdio.h>

#define WRITE_LOG WriteLog::instance()

class WriteLog {
public:
    static WriteLog* instance();

    void print(const char *fmt, ...) const;

    bool set_log_file(const char *path);
    void print_log_header();

    void start();
    void stop();
protected:
    static WriteLog *unique;
private:
    FILE *out_stream;

    bool need_print;

    WriteLog();
    ~WriteLog();
};

#endif
