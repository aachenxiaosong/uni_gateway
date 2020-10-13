#ifndef  SDK_TCP_TCP_SERVER_TCP_SERVER_HPP_
#define  SDK_TCP_TCP_SERVER_TCP_SERVER_HPP_

#include "TcpConnMgr.hpp"
#include "TcpHandle.hpp"
#include <string>
#include "event2/event.h"
#include "event2/bufferevent.h"
#include "event2/listener.h"
#include <thread>

#define MAX_TCP_RECV_LEN 1024

using namespace std;

class TcpServer {
private:
    string mName;
    string mIp;
    int mPort;
    TcpConnMgr mConnMgr;
    TcpHandle mHandle;

    struct event_base *mEventBase;
    struct evconnlistener *mListener;
    thread *mThread;

public:
    TcpServer(const char *name, int port);
    TcpServer(const char *name, string ip, int port);
    ~TcpServer();
    int listen();
    TcpHandle *getHandle();
    TcpConnMgr *getConnMgr();

private:
    static void readCb(struct bufferevent *bev, void *arg);
    static void eventCb(struct bufferevent *bev, short events, void *arg);
    static void listenerCb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *addr, int len, void *ptr);
    static void dispathTask(void *arg);
};
#endif  //  SDK_TCP_TCP_SERVER_TCP_SERVER_HPP_