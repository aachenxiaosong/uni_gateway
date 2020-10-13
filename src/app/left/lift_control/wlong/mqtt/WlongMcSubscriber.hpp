#ifndef LEFT_LIFT_CONTROL_WLONG_MQTT_WLONG_MC_SUBSCRIBER_H_
#define LEFT_LIFT_CONTROL_WLONG_MQTT_WLONG_MC_SUBSCRIBER_H_

#include "IMcSubscriber.hpp"
#include "WlongBookLiftMsgHandler.hpp"
#include "WlongInitMsgHandler.hpp"

class WlongMcSubscriber : public IMcSubscriber {
    private:
    WlongBookLiftMsgHandler mBookLiftHandler;
    WlongInitMsgHandler mInitHandler;
    public:
    WlongMcSubscriber();
    int registerMsgHandler();
};

#endif  //  LEFT_LIFT_CONTROL_WLONG_MQTT_WLONG_MC_SUBSCRIBER_H_