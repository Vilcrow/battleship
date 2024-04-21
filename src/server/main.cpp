#include "bs_server.h"
#include "const.h"
#include "tools/write_log.h"
#include "message.h"

#define SERVER_LOG_FILE "server.log"

int main()
{
    //WRITE_LOG->set_log_file(SERVER_LOG_FILE);
    //WRITE_LOG->print_log_header();
    BattleshipServer server(MAIN_SERVER_PORT);
    server.run();
    return 0;
}
