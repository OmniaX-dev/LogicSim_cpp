TARGET_EXEC ?= LogicSim++
TARGET_LIB ?= libomnia.so
OPT = 
CC = gcc 
GXX = g++ 
AS = as 
BUILD_DIR ?= ./bin/linux
BUILD_LIB_DIR ?= /shared
FRONTEND_BUILD_DIR ?= frontend
SRC_DIRS ?= ./src
RES_DIR ?= res
XTRA_DIR = xtra
BNR_FILE = build.nr
SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)
INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
DBG = -D__DEBUG__ -g
APP_NAME = LogicSim++
APP_LIBS = -lsfml-graphics -lsfml-window -lsfml-system
APP_ARGS = -std=c++17
BNR = $(shell ./inc_bnr.sh $(BNR_FILE))
BNR_DEF = -D__BUILD_NUMBER__=$(BNR)
LDFLAGS = $(APP_ARGS) $(APP_LIBS) $(DBG) $(BNR_DEF) -D__APP_NAME__='"$(APP_NAME)"'
FRONTEND_FLAGS = -g --std=c++17 $(INC_FLAGS) -L $(BUILD_DIR)/$(BUILD_LIB_DIR)/ -lomnia $(APP_LIBS) -Wzero-as-null-pointer-constant $(BNR_DEF)
CPPFLAGS ?= -fPIC $(OPT) $(INC_FLAGS) -MMD -MP $(LDFLAGS) -Wzero-as-null-pointer-constant 

$(BUILD_DIR)/$(BUILD_LIB_DIR)/$(TARGET_LIB): $(OBJS)
	$(RM) -r $(BUILD_DIR)/$(BUILD_LIB_DIR)
	$(MKDIR_P) $(BUILD_DIR)/$(BUILD_LIB_DIR)
	$(CXX) -shared -fPIC $(OPT) -o $@ $(OBJS) $(LDFLAGS) -Wzero-as-null-pointer-constant 
	echo $(BNR) > $(BNR_FILE)

# assembly
$(BUILD_DIR)/%.s.o: %.s
	$(MKDIR_P) $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

release:
	$(MAKE) $(MAKEFILE) DBG="-D__RELEASE__"

run: #TODO: Fix bug where i can't cd into the directory
	#cd  $(BUILD_DIR)/$(FRONTEND_BUILD_DIR)/; LD_LIBRARY_PATH='$(shell pwd)/$(BUILD_DIR)/$(BUILD_LIB_DIR)/' ./LogicSim++; cd ../../../
	
	LD_LIBRARY_PATH='$(shell pwd)/$(BUILD_DIR)/$(BUILD_LIB_DIR)/' ./$(BUILD_DIR)/$(FRONTEND_BUILD_DIR)/$(TARGET_EXEC)

debug:
	LD_LIBRARY_PATH='$(shell pwd)/$(BUILD_DIR)/$(BUILD_LIB_DIR)/' gdb $(BUILD_DIR)/$(FRONTEND_BUILD_DIR)/$(TARGET_EXEC)

copy-res:
	$(RM) -r $(BUILD_DIR)/$(FRONTEND_BUILD_DIR)/$(RES_DIR)
	$(MKDIR_P) $(BUILD_DIR)/$(FRONTEND_BUILD_DIR)/$(RES_DIR)
	cp -r $(RES_DIR) $(BUILD_DIR)/$(FRONTEND_BUILD_DIR)
	cp -r $(XTRA_DIR)/. $(BUILD_DIR)/$(FRONTEND_BUILD_DIR)/

frontend: $(BUILD_DIR)/$(BUILD_LIB_DIR)/$(TARGET_LIB)
	$(RM) -r $(BUILD_DIR)/$(FRONTEND_BUILD_DIR)/$(RES_DIR)
	$(MKDIR_P) $(BUILD_DIR)/$(FRONTEND_BUILD_DIR)/$(RES_DIR)
	cp -r $(RES_DIR) $(BUILD_DIR)/$(FRONTEND_BUILD_DIR)
	cp -r $(XTRA_DIR)/. $(BUILD_DIR)/$(FRONTEND_BUILD_DIR)/
	$(CXX) frontend/*.cpp -Ifrontend/headers -o $(BUILD_DIR)/$(FRONTEND_BUILD_DIR)/$(TARGET_EXEC) $(FRONTEND_FLAGS)
	echo $(BNR) > $(BNR_FILE)

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p