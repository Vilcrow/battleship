#include "CppUTest/CommandLineTestRunner.h"
#include <stdlib.h>
#include <time.h>
#include "tools/write_log.h"

int main(int argc, char **argv)
{
    srand(time(0));
    WRITE_LOG->set_log_file("/dev/null");
    return CommandLineTestRunner::RunAllTests(argc, argv);
}
