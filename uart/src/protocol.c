#include "protocol.h"

#include "serial.h"
#include "call_lift.h"
#include "exter_visit.h"
#include "inter_visit.h"
#include "get_home.h"
#include "lift_status.h"
#include "log.h"

#include <stdio.h>
#include <stdlib.h>

#define WAIT_ACK_TIMEOUT   2000
#define REQUEST_FRAME_LEN  20
#define RESPONSE_FRAME_LEN 10

static unsigned char g_request_id = 0;

static unsigned char _get_request_id() {
  unsigned char id = g_request_id;
  g_request_id = (g_request_id + 1) % 16;
  return id;
}

static unsigned char _calculate_sum(unsigned char *frame, int len) {
  int i;
  unsigned char sum = 0;
  for (i = 1; i < len - 2; i++) {
    sum += frame[i];
  }
  return sum;
}

static void _printf_frame(unsigned char *frame, int len) {
  int i;
  printf("frame: ");
  for (i = 0; i < len; i++) {
    printf("0x%x ", frame[i]);
  }
  printf("\n");
}

int _assemble_frame_for_call_lift(void *request, unsigned char frame[REQUEST_FRAME_LEN],
                                  unsigned char request_id) {
  RequestCallLift *request_call_lift = (RequestCallLift *)request;
  if (request == NULL) {
    return -1;
  }
  frame[0] = 0x02;
  frame[1] = request_call_lift->building_num / 10 + '0';
  frame[2] = request_call_lift->building_num % 10 + '0';
  frame[5] = EVENT_CALL_LIFT | (request_id << 4);
  frame[14] = request_call_lift->room[0];
  frame[15] = request_call_lift->room[1];
  frame[16] = request_call_lift->room[2];
  frame[17] = request_call_lift->room[3];
  frame[18] = _calculate_sum(frame, REQUEST_FRAME_LEN);
  frame[19] = 0x03;
  return 0;
}

int _parse_frame_for_call_lift(void *response, unsigned char frame[RESPONSE_FRAME_LEN],
                               unsigned char request_id) {
  ResponseCallLift *response_call_lift = (ResponseCallLift *)response;
  unsigned char check_sum = _calculate_sum(frame, RESPONSE_FRAME_LEN);
  _printf_frame(frame, RESPONSE_FRAME_LEN);
  if (frame[0] != 0x02 ||
      frame[RESPONSE_FRAME_LEN - 1] != 0x03) {
    LOGE("wrong header 0x%x or tail 0x%x", frame[0], frame[RESPONSE_FRAME_LEN - 1]);
    return -1;
  }
  if (frame[1] != (unsigned char)(EVENT_CALL_LIFT | (request_id << 4))) {
    LOGE("wrong event type or request id, frame %d request id %d", frame[1], request_id);
    return -1;
  }
  if (frame[RESPONSE_FRAME_LEN - 2] != check_sum) {
    LOGE("wrong check sum");
    return -1;
  }
  response_call_lift->ack_code = frame[2];
  return 0;
}

int _assemble_frame_for_exter_visit(void *request, unsigned char frame[REQUEST_FRAME_LEN],
                                    unsigned char request_id) {
  RequestExterVisit *request_exter_visit = (RequestExterVisit *)request;
  if (request == NULL) {
    return -1;
  }
  frame[0] = 0x02;
  frame[1] = request_exter_visit->building_num / 10 + '0';
  frame[2] = request_exter_visit->building_num % 10 + '0';
  frame[3] = request_exter_visit->hall_num / 10 + '0';
  frame[4] = request_exter_visit->hall_num % 10 + '0';
  frame[5] = EVENT_EXTER_VISIT | (request_id << 4);
  frame[14] = request_exter_visit->room[0];
  frame[15] = request_exter_visit->room[1];
  frame[16] = request_exter_visit->room[2];
  frame[17] = request_exter_visit->room[3];
  frame[18] = _calculate_sum(frame, REQUEST_FRAME_LEN);
  frame[19] = 0x03;
  return 0;
}

int _parse_frame_for_exter_visit(void *response, unsigned char frame[RESPONSE_FRAME_LEN],
                                 unsigned char request_id) {
  ResponseExterVisit *response_exter_visit = (ResponseExterVisit *)response;
  unsigned char check_sum = _calculate_sum(frame, RESPONSE_FRAME_LEN);
  _printf_frame(frame, RESPONSE_FRAME_LEN);
  if (frame[0] != 0x02 ||
      frame[RESPONSE_FRAME_LEN - 1] != 0x03) {
    LOGE("wrong header 0x%x or tail 0x%x", frame[0], frame[RESPONSE_FRAME_LEN - 1]);
    return -1;
  }
  if (frame[1] != (unsigned char)(EVENT_EXTER_VISIT | (request_id << 4))) {
    LOGE("wrong event type or request id, frame %d request id %d", frame[1], request_id);
    return -1;
  }
  if (frame[RESPONSE_FRAME_LEN - 2] != check_sum) {
    LOGE("wrong check sum");
    return -1;
  }
  response_exter_visit->ack_code = frame[2];
  return 0;
}

int _assemble_frame_for_inter_visit(void *request, unsigned char frame[REQUEST_FRAME_LEN],
                                    unsigned char request_id) {
  RequestInterVisit *request_inter_visit = (RequestInterVisit *)request;
  if (request == NULL) {
    return -1;
  }
  frame[0] = 0x02;
  frame[1] = request_inter_visit->building_num / 10 + '0';
  frame[2] = request_inter_visit->building_num % 10 + '0';
  frame[5] = EVENT_INTER_VISIT | (request_id << 4);
  frame[10] = request_inter_visit->visitor_room[0];
  frame[11] = request_inter_visit->visitor_room[1];
  frame[12] = request_inter_visit->visitor_room[2];
  frame[13] = request_inter_visit->visitor_room[3];
  frame[14] = request_inter_visit->host_room[0];
  frame[15] = request_inter_visit->host_room[1];
  frame[16] = request_inter_visit->host_room[2];
  frame[17] = request_inter_visit->host_room[3];
  frame[18] = _calculate_sum(frame, REQUEST_FRAME_LEN);
  frame[19] = 0x03;
  return 0;
}

int _parse_frame_for_inter_visit(void *response, unsigned char frame[RESPONSE_FRAME_LEN],
                                 unsigned char request_id) {
  ResponseInterVisit *response_inter_visit = (ResponseInterVisit *)response;
  unsigned char check_sum = _calculate_sum(frame, RESPONSE_FRAME_LEN);
  _printf_frame(frame, RESPONSE_FRAME_LEN);
  if (frame[0] != 0x02 ||
      frame[RESPONSE_FRAME_LEN - 1] != 0x03) {
    LOGE("wrong header 0x%x or tail 0x%x", frame[0], frame[RESPONSE_FRAME_LEN - 1]);
    return -1;
  }
  if (frame[1] != (unsigned char)(EVENT_INTER_VISIT | (request_id << 4))) {
    LOGE("wrong event type or request id, frame %d request id %d", frame[1], request_id);
    return -1;
  }
  if (frame[RESPONSE_FRAME_LEN - 2] != check_sum) {
    LOGE("wrong check sum");
    return -1;
  }
  response_inter_visit->ack_code = frame[2];
  return 0;
}

int _assemble_frame_for_get_home(void *request, unsigned char frame[REQUEST_FRAME_LEN],
                                 unsigned char request_id) {
  RequestGetHome *request_get_home = (RequestGetHome *)request;
  if (request == NULL) {
    return -1;
  }
  frame[0] = 0x02;
  frame[1] = request_get_home->building_num / 10 + '0';
  frame[2] = request_get_home->building_num % 10 + '0';
  frame[3] = request_get_home->hall_num / 10 + '0';
  frame[4] = request_get_home->hall_num % 10 + '0';
  frame[5] = EVENT_GET_HOME | (request_id << 4);
  frame[14] = request_get_home->room[0];
  frame[15] = request_get_home->room[1];
  frame[16] = request_get_home->room[2];
  frame[17] = request_get_home->room[3];
  frame[18] = _calculate_sum(frame, REQUEST_FRAME_LEN);
  frame[19] = 0x03;
  return 0;
}

int _parse_frame_for_get_home(void *response, unsigned char frame[RESPONSE_FRAME_LEN],
                              unsigned char request_id) {
  ResponseGetHome *response_get_home = (ResponseGetHome *)response;
  unsigned char check_sum = _calculate_sum(frame, RESPONSE_FRAME_LEN);
  _printf_frame(frame, RESPONSE_FRAME_LEN);
  if (frame[0] != 0x02 ||
      frame[RESPONSE_FRAME_LEN - 1] != 0x03) {
    LOGE("wrong header 0x%x or tail 0x%x", frame[0], frame[RESPONSE_FRAME_LEN - 1]);
    return -1;
  }
  if (frame[1] != (unsigned char)(EVENT_GET_HOME | (request_id << 4))) {
    LOGE("wrong event type or request id, frame %d request id %d", frame[1], request_id);
    return -1;
  }
  if (frame[RESPONSE_FRAME_LEN - 2] != check_sum) {
    LOGE("wrong check sum");
    return -1;
  }
  response_get_home->ack_code = frame[2];
  return 0;
}

int _assemble_frame_for_lift_status(void *request, unsigned char frame[REQUEST_FRAME_LEN],
                                    unsigned char request_id) {
  RequestLiftStatus *request_lift_status = (RequestLiftStatus *)request;
  if (request == NULL) {
    return -1;
  }
  frame[0] = 0x02;
  frame[1] = request_lift_status->building_num / 10 + '0';
  frame[2] = request_lift_status->building_num % 10 + '0';
  frame[5] = EVENT_LIFT_STATUS | (request_id << 4);
  frame[6] = request_lift_status->lift_num;
  frame[18] = _calculate_sum(frame, REQUEST_FRAME_LEN);
  frame[19] = 0x03;
  return 0;
}

int _parse_frame_for_lift_status(void *response, unsigned char frame[RESPONSE_FRAME_LEN],
                                 unsigned char request_id) {
  ResponseLiftStatus *response_lift_status = (ResponseLiftStatus *)response;
  unsigned char check_sum = _calculate_sum(frame, RESPONSE_FRAME_LEN);
  _printf_frame(frame, RESPONSE_FRAME_LEN);
  if (frame[0] != 0x02 ||
      frame[RESPONSE_FRAME_LEN - 1] != 0x03) {
    LOGE("wrong header 0x%x or tail 0x%x", frame[0], frame[RESPONSE_FRAME_LEN - 1]);
    return -1;
  }
  if (frame[1] != (unsigned char)(EVENT_LIFT_STATUS | (request_id << 4))) {
    LOGE("wrong event type or request id, frame %d request id %d", frame[1], request_id);
    return -1;
  }
  if (frame[RESPONSE_FRAME_LEN - 2] != check_sum) {
    LOGE("wrong check sum");
    return -1;
  }
  response_lift_status->lift_num = frame[2];
  response_lift_status->lift_status = frame[3];
  response_lift_status->current_level = frame[7];
  return 0;
}

int protocol_send(int event_type, void *request, void *response) {
  int (* request_assemble) (void *, unsigned char [REQUEST_FRAME_LEN], unsigned char);
  int (* response_parse) (void *, unsigned char [RESPONSE_FRAME_LEN], unsigned char);
  unsigned char request_frame[REQUEST_FRAME_LEN] = {0};
  unsigned char response_frame[REQUEST_FRAME_LEN] = {0};
  int try_count = WAIT_ACK_TIMEOUT / 50;
  int recv_len;
  unsigned char request_id;
  switch (event_type) {
    case EVENT_CALL_LIFT:
      request_assemble = _assemble_frame_for_call_lift;
      response_parse = _parse_frame_for_call_lift;
      break;
    case EVENT_EXTER_VISIT:
      request_assemble = _assemble_frame_for_exter_visit;
      response_parse = _parse_frame_for_exter_visit;
      break;
    case EVENT_INTER_VISIT:
      request_assemble = _assemble_frame_for_inter_visit;
      response_parse = _parse_frame_for_inter_visit;
      break;
    case EVENT_GET_HOME:
      request_assemble = _assemble_frame_for_get_home;
      response_parse = _parse_frame_for_get_home;
      break;
    case EVENT_LIFT_STATUS:
      request_assemble = _assemble_frame_for_lift_status;
      response_parse = _parse_frame_for_lift_status;
      break;
    default:
      LOGE("wrong event type");
      return -1;
  }
  request_id = _get_request_id();
  if (0 != request_assemble(request, request_frame, request_id)) {
    LOGE("failed at step1: assemble request");
    return -1;
  }
  if (0 != serial_send(request_frame, REQUEST_FRAME_LEN)) {
    LOGE("failed at step2: serial send request");
    return -1;
  }
  while (try_count > 0 && 0 != serial_recv(response_frame, &recv_len)) {
    try_count--;
    usleep(50 * 1000);
  }
  if (try_count <= 0) {
    LOGE("failed at step3: serial recv response");
    return -1;
  }
  if (0 != response_parse(response, response_frame, request_id)) {
    LOGE("failed at step4: parse response");
    return -1;
  }
  return 0;
}
