BLD_DIR_REL =../../..
include $(BLD_DIR_REL)/config.mk
include $(BLD_DIR)/tools/config.mk
include $(BLD_DIR)/inter/config.mk
include $(BLD_DIR)/util/config.mk
include $(SRC_DIR)/tools/patterns.mk
include $(SRC_DIR)/tools/sources.mk
include $(SRC_DIR)/inter/sources.mk
include $(SRC_DIR)/util/sources.mk
include $(SRC_DIR)/tools/tests/storage/sources.mk

all: $(tools_tests_storage_OBJS)

clean:
	rm -f $(tools_tests_storage_OBJS) 
	rm -f $(tools_tests_storage_OBJS:.o=.d)
	rm -f $(tools_tests_storage_OBJS:.o=)

-include $(tools_tests_storage_OBJS:.o=.d)
-include $(tools_OBJS:.o=.d)
-include $(inter_OBJS:.o=.d)
-include $(util_OBJS:.o=.d)

