#include "WlongTakeLiftMessageHandler.hpp"
#include "SulinkLiftInitData.hpp"
#include "WlongLiftCtrl.hpp"
#include "UniLog.hpp"

#define WLONG_TAKE_LIFT_MSG_HANDLER_TAG getName().c_str()

WlongTakeLiftMessageHandler :: WlongTakeLiftMessageHandler() : ILiftCtrlHttpMessageHandler("wlong_take_lift_msg_handler")
{
}

WlongTakeLiftMessageHandler :: ~WlongTakeLiftMessageHandler()
{
}

LiftCtrlMessageRsp* WlongTakeLiftMessageHandler :: handle(const LiftCtrlMessageReq &request)
{
    if (request.type() != MSG_LIFT_CTRL_TAKE_LIFT_REQ) {
        return NULL;
    }
    LOGT(WLONG_TAKE_LIFT_MSG_HANDLER_TAG, "request message is handling...");
    LiftCtrlMessageTakeLiftReq& req = (LiftCtrlMessageTakeLiftReq&)request;
    LiftCtrlMessageTakeLiftRsp rsp;
    //step1: 获取基本参数:appId,appSecret,license
    string app_id = SulinkLiftInitData :: getAppId();
    string app_secret = SulinkLiftInitData :: getAppSecret();
    string license = SulinkLiftInitData :: getLicense();
    //step2: 根据默认空间获取群控器的ip和端口;并找到到达楼层
    string cluster_url = SulinkLiftInitData :: getClusterUrlBySpaceId(req.defaultHomeId());
    string open_floor = SulinkLiftInitData :: getFloorNoBySpaceId(req.defaultHomeId());
    //step3: 根据其他权限空间找到解锁层,根据设备编码找到公共楼层,解锁楼层要加上公共楼层
    string unlock_floors = "";
    string pub_floors = SulinkLiftInitData :: getDevicePubFloors(req.deviceCode());
    if (pub_floors.empty() != true) {
        unlock_floors += pub_floors;
    }
    for (int i = 0; i < req.authorizedHomeIds().size(); i++) {
        string unlock_floor = SulinkLiftInitData :: getFloorNoBySpaceId(req.authorizedHomeIds()[i]);
        if (unlock_floor.empty() != true) {
            if (unlock_floors.empty() != true) {
                unlock_floors += ",";
            }
            unlock_floors += unlock_floor;
        }
    }
    string not_found_msg = "";
    if (cluster_url.empty()) {
        not_found_msg = "cluster url not found for home id " + req.defaultHomeId();
    } else if (open_floor.empty()) {
        not_found_msg = "default floor not found for home id " + req.defaultHomeId();
    }
    if (!not_found_msg.empty()) {
        rsp.retcode(RETCODE_ERROR);
        rsp.msg(not_found_msg);
        rsp.ackCode(0);
        LOGE(WLONG_TAKE_LIFT_MSG_HANDLER_TAG, not_found_msg);
        return new LiftCtrlMessageTakeLiftRsp(rsp);
    }
    //step4: 根据设备编码找到电梯id
    int lift_id = atoi(SulinkLiftInitData :: getDeviceLiftId(req.deviceCode()).c_str());
    //step5: 调用wlong乘梯接口
    WlongResponse wl_response;
    WlongLiftCtrl wlong_lift_ctrl(cluster_url, app_id, app_secret, license);
    int ret = wlong_lift_ctrl.callElevatorByFoor(lift_id, open_floor, unlock_floors, wl_response);
    if (ret == 0) {
        LOGT(WLONG_TAKE_LIFT_MSG_HANDLER_TAG, "handle request of wlong take lift OK");
        rsp.retcode(0);
        rsp.msg(wl_response.msg);
        if (wl_response.code == 0) {
            rsp.ackCode(1);
        } else {
            rsp.ackCode(0);
        }
    } else {
        LOGT(WLONG_TAKE_LIFT_MSG_HANDLER_TAG, "handle request of wlong take lift failed");
        rsp.retcode(RETCODE_ERROR);
        rsp.msg("calling WangLong interface error");
        rsp.ackCode(0);
    }
    return new LiftCtrlMessageTakeLiftRsp(rsp);
}