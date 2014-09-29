include ../common.mk

SRC=vote_tool.cpp bench.cpp
TARGET=$(addprefix $(TOPDIR)bin/, $(SRC:.cpp=$(OBJSUF).exe))
LIBS=

all: $(TARGET)

$(OBJDIR):
	-$(MKDIR) $(OBJDIR)

clean:
	$(CLEAN)

-include $(DEPEND_FILE)

