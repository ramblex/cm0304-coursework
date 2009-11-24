AM_CXXFLAGS = -W -Wall -Wextra -ansi $(GLUT_CFLAGS)

bin_PROGRAMS = scene

scene_SOURCES = base.hpp util.hpp util.cpp models.hpp models.cpp \
	 main.cpp 

EXTRA_DIST = textures/raw_particle.raw

LDADD = $(GLUT_LIBS)