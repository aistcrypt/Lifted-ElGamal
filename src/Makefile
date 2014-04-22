include ../common.mk

SRC=vote_tool.cpp
TARGET=$(addprefix $(TOPDIR)bin/, $(SRC:.cpp=$(OBJSUF).exe))
LIBS=

all: $(TARGET)

$(OBJDIR):
	-$(MKDIR) $(OBJDIR)

clean:
	$(CLEAN)

-include $(DEPEND_FILE)

