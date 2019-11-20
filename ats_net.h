#pragma once

#include "../ats/ats_tool.h"
#include <winsock2.h>

#define LOCAL_HOST  ("127.0.0.1")

static WSADATA wsa_data;

static void NetInit()
{
    WSAStartup(MAKEWORD(2, 2), &wsa_data);
}

static void NetDeinit()
{
    WSACleanup();
}

typedef struct Server {
    SOCKET              socket;
    struct sockaddr_in  addr;
} Server;
        
typedef struct Client {
    SOCKET              socket;
    struct sockaddr_in  addr;
} Client;

static void NetServerInit(Server *server, int port)
{
    memset(server, 0, sizeof server);

    server->socket = socket(AF_INET, SOCK_DGRAM, 0);

    server->addr.sin_family         = AF_INET;
    server->addr.sin_addr.s_addr    = INADDR_ANY;
    server->addr.sin_port           = htons(port);

    bind(server->socket, (struct sockaddr *)&server->addr, sizeof server->addr);
}

static int NetServerRecv(Server *server, void *data, size_t data_size)
{
    int len = recvfrom(server->socket, data, data_size, 0, NULL, 0);
    return len;
}

static void NetClientInit(Client *client, const char *ip, int port)
{
    memset(client, 0, sizeof client);

    client->socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    client->addr.sin_family              = AF_INET;
    client->addr.sin_port                = htons(port);
    client->addr.sin_addr.S_un.S_addr    = inet_addr(ip);
}

static int NetClientSend(Client *client, const void *data, size_t data_size)
{
    sendto(client->socket, data, data_size, 0, (struct sockaddr *)&client->addr, sizeof client->addr);
}

