INCLUDE_FILE = include
THIRDPARTY_FILE = thrid_party
CXX = g++
LDFLAGS += `pkg-config --libs ncm-aico` 
# LDFLAGS += -Llib -litemsaico
CPPFLAGS += -I ${INCLUDE_FILE}
CPPFLAGS += -I ${THIRDPARTY_FILE}
CXXFLAGS += -std=c++11
NAME=netease_cloud_music_aico

OBJFILE = ncurses.o main_menu.o http.o file.o json.o color.o resource.o songs_controller.o utils.o windows.o sub_win_print.o items.o

all: 
	@sh -c './script/configure.sh'
	@make ${NAME}
	@sh -c './script/end.sh'

${NAME}: ${OBJFILE}
	${CXX} $^ -o $@ ${CXXFLAGS} ${CPPFLAGS} ${LDFLAGS}

vpath %.cpp cmd src/menu_windows src/sub_win_print src/windows \
	src/utils src/json src/file src/http src/song src/controller\
	src/item


.PRECIOUS:%.o
%.o:%.cpp 
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -f *.o ${NAME}



