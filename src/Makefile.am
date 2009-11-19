AM_CXXFLAGS = -W -Wall -Wextra -ansi $(GLUT_CFLAGS)

bin_PROGRAMS = scene

scene_SOURCES = base.hpp util.hpp util.cpp models.hpp models.cpp \
	lights.hpp lights.cpp app.hpp app.cpp main.cpp 

LDADD = $(GLUT_LIBS)