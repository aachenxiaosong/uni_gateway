#ifndef MSG_CENTER_SUBSCRIBER_WLONG_WLONG_INIT_MSG_HANDLER_H_
#define MSG_CENTER_SUBSCRIBER_WLONG_WLONG_INIT_MSG_HANDLER_H_

#include "IMcMsgHandler.hpp"
#include "CJsonObject.hpp"

using namespace neb;


class WlongInitMsgHandler : public IMcMsgHandler {
    private:
    int _parse_home(CJsonObject& djson, CJsonObject& sjson) ;
    int _parse_unit(CJsonObject& djson, CJsonObject& sjson);
    int _parse_building(CJsonObject& djson, CJsonObject& sjson);
    int _parse(CJsonObject& djson, CJsonObject& sjson);
    public:
    WlongInitMsgHandler();
    int handle(string& msg);
};

#endif  //  MSG_CENTER_SUBSCRIBER_WLONG_WLONG_INIT_MSG_HANDLER_H_