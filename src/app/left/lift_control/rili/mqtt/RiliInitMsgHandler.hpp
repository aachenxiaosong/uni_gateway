#ifndef LEFT_LIFT_CONTROL_RILI_MQTT_RILI_INIT_MSG_HANDLER_H_
#define LEFT_LIFT_CONTROL_RILI_MQTT_RILI_INIT_MSG_HANDLER_H_

#include "IMcMsgHandler.hpp"
#include "CJsonObject.hpp"
#include "IHttpService.hpp"

using namespace neb;


class RiliInitMsgHandler : public IMcMsgHandler {
    private:
    IHttpService *mHttpService;
    private:
    int _save_init_info(string &info);
    int _import_init_info(string &info);
    int _parse_home(CJsonObject& djson, CJsonObject& sjson) ;
    int _parse_unit(CJsonObject& djson, CJsonObject& sjson);
    int _parse_building(CJsonObject& djson, CJsonObject& sjson);
    int _parse(CJsonObject& djson, CJsonObject& sjson);
    public:
    RiliInitMsgHandler();
    int handle(string& topic, string& msg);
};

#endif  //  LEFT_LIFT_CONTROL_RILI_MQTT_RILI_INIT_MSG_HANDLER_H_