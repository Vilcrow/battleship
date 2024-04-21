#include "write_log.h"
#include <stdarg.h>
#include <time.h>

WriteLog* WriteLog::unique = 0;

WriteLog* WriteLog::instance()
{
    if(!unique) {
        unique = new WriteLog();
    }
    return unique;
}

void WriteLog::print(const char *fmt, ...) const
{
    if(!need_print) {
        return;
    }

    va_list vl;
    va_start(vl, fmt);
    va_end(vl);
    vfprintf(out_stream, fmt, vl);
    fprintf(out_stream, "%s", "\n");
}

bool WriteLog::set_log_file(const char *path)
{
    if(out_stream && out_stream != stdout) {
        fclose(out_stream);
    }

    FILE *log_file = fopen(path, "a");
    if(!log_file) {
        out_stream = stdout;
        return false;
    }
    out_stream = log_file;
    return true;
}

void WriteLog::print_log_header()
{
    time_t t = time(NULL);
    struct tm *cur_time = localtime(&t);
    char str[64];
    strftime(str, 64, "%d.%m.%y %H:%M:%S", cur_time);
    print("--------------------------------------------------");
    print("START WRITE LOG [%s]", str);
    print("--------------------------------------------------");
}

void WriteLog::start()
{
    need_print = true;
}

void WriteLog::stop()
{
    need_print = false;
}

WriteLog::WriteLog()
    : out_stream(stdout)
    , need_print(true)
{

}

WriteLog::~WriteLog()
{
    if(out_stream) {
        fflush(out_stream);
        if(out_stream != stdout) {
            fclose(out_stream);
        }
    }
}
