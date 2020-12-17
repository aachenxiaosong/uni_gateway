#ifndef  SDK_MQTT_MQTT_HANDLE_IMQTT_MESSAGE_HPP_
#define  SDK_MQTT_MQTT_HANDLE_IMQTT_MESSAGE_HPP_

#include <string>
#include <vector>
#include "Message.hpp"

using namespace std;


class IMqttMessage : public Message {
    MEMBER(string, topic)
    SERI_BEGIN
    SERI(topic)
    SERI_END
public:
    IMqttMessage(MessageType type) : Message(type) {}
};

#endif  //  SDK_MQTT_MQTT_HANDLE_IMQTT_MESSAGE_HPP_