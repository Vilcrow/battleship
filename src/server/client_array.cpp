#include "client_array.h"
#include "client_data.h"
#include "tools/write_log.h"
#include <string.h>
#include <stdio.h>

ClientArray::ClientArray(int p_size)
    : count(0)
    , size(p_size)
    , clients(0)
{
    if(size < 0) {
        size = 0;
    }

    if(size) {
        clients = new ClientData*[size];
        memset(clients, 0, sizeof(ClientData*) * size);
    }
}

ClientArray::~ClientArray()
{
    if(clients) {
        for(int i = 0; i < size; i++) {
            delete clients[i];
        }
        delete[] clients;
    }
}

ClientData* ClientArray::operator[](int index) const
{
    if(index < 0 || index >= size) {
        return 0;
    }

    return clients[index];
}

bool ClientArray::add(ClientData *p_client)
{
    NULL_PTR_CHECK_RET_ZERO(p_client)

    int pos = _find_free_pos();
    WRITE_LOG->print("add_client() pos = %d", pos);
    if(pos != -1) {
        clients[pos] = p_client;
        ++count;
        return true;
    }

    return false;
}

void ClientArray::remove(ClientData *p_client)
{
    NULL_PTR_CHECK_RETURN(p_client)

    for(int i = 0; i < size; i++) {
        if(clients[i] == p_client) {
            clients[i] = 0;
            --count;
            return;
        }
    }
}

int ClientArray::get_count() const
{
    return count;
}

int ClientArray::get_size() const
{
    return size;
}

bool ClientArray::isFull() const
{
    for(int i = 0; i < size; i++) {
        if(!clients[i]) {
            return false;
        }
    }
    return true;
}

int ClientArray::_find_free_pos()
{
    if(size <= 0) {
        return -1;
    }

    for(int i = 0; i < size; i++) {
        if(!clients[i]) {
            return i;
        }
    }

    // Array is full.
    return -1;
}

bool ClientArray::contains(const ClientData *p_client) const
{
    NULL_PTR_CHECK_RET_ZERO(p_client)

    for(int i = 0; i < size; i++) {
        if(clients[i] == p_client) {
            return true;
        }
    }
    return false;
}
