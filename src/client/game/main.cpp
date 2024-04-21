#include "const.h"
#include "tools/write_log.h"
#include "bs_game.h"
#include <unistd.h>

#define GAME_LOG_FILE "game.log"
#define CLIENT_LOG_FILE "client.log"

int main(int argc, char **argv)
{
    if(argc != 2) {
        WRITE_LOG->print("Too few arguments");
        return 1;
    }

    int fd[2];
    int res = socketpair(AF_UNIX, SOCK_STREAM, 0, fd);
    if(res != 0) {
        WRITE_LOG->print("Can't create socket pair");
        return 2;
    }
    int pid = fork();
#ifndef DEBUG_CLIENT
    if(pid != 0) {
#else
    if(pid == 0) {
#endif
        // The client process.
        WRITE_LOG->set_log_file(CLIENT_LOG_FILE);
        WRITE_LOG->print_log_header();
        ClientServer client(argv[1], MAIN_SERVER_PORT);
        close(fd[1]);
        client.set_interface_fd(fd[0]);
        client.run();
        return 0;
    }

    // The interface process.
    WRITE_LOG->set_log_file(GAME_LOG_FILE);
    WRITE_LOG->print_log_header();
    BattleshipGame game;
    close(fd[0]);
    game.set_client_fd(fd[1]);
    game.run();
    return 0;
}
