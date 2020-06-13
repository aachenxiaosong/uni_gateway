#ifndef MSG_CENTER_SUBSCRIBER_RILI_RILI_MC_SUBSCRIBER_H_
#define MSG_CENTER_SUBSCRIBER_RILI_RILI_MC_SUBSCRIBER_H_

#include "IMcSubscriber.hpp"
#include "RiliBookLiftMsgHandler.hpp"

class RiliMcSubscriber : public IMcSubscriber {
    private:
    RiliBookLiftMsgHandler mBookLiftHandler;
    public:
    RiliMcSubscriber();
    int registerMsgHandler();
};

#endif  //  MSG_CENTER_SUBSCRIBER_RILI_RILI_MC_SUBSCRIBER_H_