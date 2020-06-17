target := http_server
all: $(target) 

hal_src := src/hal/src
hal_inc := src/hal/inc

utils_src := src/utils/bitmap/src \
             src/utils/json/src \
             src/utils/data_buf/src \
             src/utils/device/src \
             src/utils/encrypt/src \
             src/utils/event/src \
             src/utils/event_list/src \
             src/utils/fsm/src \
             src/utils/hash/src \
             src/utils/http/src \
             src/utils/interruptable_sleep/src \
             src/utils/list/src \
             src/utils/log/src \
             src/utils/uuid/src \
             src/utils/uart/src \
             src/utils/md5sum/src \
             src/utils/memcheck/src \
             src/utils/timer/src

utils_inc := src/utils/bitmap/inc \
             src/utils/json/inc \
             src/utils/data_buf/inc \
             src/utils/device/inc \
             src/utils/encrypt/inc \
             src/utils/event/inc \
             src/utils/event_list/inc \
             src/utils/hash/inc \
             src/utils/http/inc \
             src/utils/fsm/inc \
             src/utils/interruptable_sleep/inc \
             src/utils/list/inc \
             src/utils/log/inc \
             src/utils/uuid/inc \
             src/utils/uart/inc \
             src/utils/md5sum/inc \
             src/utils/memcheck/inc \
             src/utils/timer/inc

SRC := $(hal_src) $(utils_src) \
       src \
       src/http_server/src \
       src/http_client/auth/src \
       src/http_client/resource_prefetch/src \
       src/http_client/wlong \
       src/http_service \
       src/http_service/rili \
       src/http_service/wlong \
       src/msg_center/src \
       src/msg_center_subscriber \
       src/msg_center_subscriber/wlong \
       src/msg_center_subscriber/rili
            
INC := $(hal_inc) $(utils_inc) \
       src \
       src/http_server/inc \
       src/http_client/auth/inc \
       src/http_client/resource_prefetch/inc \
       src/http_client/wlong \
       src/http_service \
       src/http_service/rili \
       src/http_service/wlong \
       src/msg_center/inc \
       src/msg_center/inc/mqtt \
       src/msg_center_subscriber \
       src/msg_center_subscriber/wlong \
       src/msg_center_subscriber/rili

# functions -----------------------
change_file_location = $(foreach f,$1,$2/$(notdir $(f)))
c_to_o = $(subst .c,.o,$1)
cpp_to_o = $(subst .cpp,.o,$1)
o_to_d = $(subst .o,.o.d,$1)

# compile parameters -------------
vpath %.c $(SRC)
vpath %.cpp $(SRC)

BUILD_DIR := build
INC_FLAGS := $(foreach inc, $(INC),-I$(inc))
C_FILES := $(foreach dir, $(SRC), $(wildcard $(dir)/*.c))
CPP_FILES := $(foreach dir, $(SRC), $(wildcard $(dir)/*.cpp))
C_OBJ_FILES := $(call c_to_o,$(C_FILES))
C_OBJ_FILES := $(call change_file_location, $(C_OBJ_FILES), $(BUILD_DIR))
CPP_OBJ_FILES := $(call cpp_to_o,$(CPP_FILES))
CPP_OBJ_FILES := $(call change_file_location, $(CPP_OBJ_FILES), $(BUILD_DIR))
DEP_FILES := $(call o_to_d,$(C_OBJ_FILES) $(CPP_OBJ_FILES))

LIB := -Llib -levent-2.2 -lpaho-embed-mqtt3c -lcurl -lm -lpthread

ifneq ($(DEP_FILES),)
-include $(DEP_FILES)
endif

$(C_OBJ_FILES):$(BUILD_DIR)/%.o: %.c
	g++ -g $(INC_FLAGS) $(LIB) -Wp,-MD,$@.d -c -o $@ $<

$(CPP_OBJ_FILES):$(BUILD_DIR)/%.o: %.cpp
	g++ -g $(INC_FLAGS) $(LIB) -Wp,-MD,$@.d -c -o $@ $<
  
$(target): prepare $(C_OBJ_FILES) $(CPP_OBJ_FILES)
	g++ -g $(C_OBJ_FILES) $(CPP_OBJ_FILES) $(INC_FLAGS) $(LIB) -o $@

prepare:
	@[ -d $(BUILD_DIR) ] || mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(target) build/*

.PHONY: all clean
