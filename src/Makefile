CXX=g++
CXXFLAGS=-g
SRCS=main.cpp physics_equipped_entity.cpp static_component.cpp render_target.cpp keyboard_io.cpp physics_engine.cpp tilemap_component.cpp animated_component.cpp
OBJS=$(subst .cpp,.o,$(SRCS))
LDLIBS=-lsfml-system -lsfml-window -lsfml-graphics -llua

all: $(OBJS)
	$(CXX) $(CXXFLAGS) -o m $(OBJS) $(LDLIBS)
	mv m ../
clean:
	rm $(OBJS)
