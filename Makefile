SOURCE= main.cpp texture.c	
EXEC= Assignment1
CC = gcc

# The rest should be okay!
GLUT_LIB=/usr/lib
GLUT_INCLUDE=/usr/include
GLUI_LIB=lib
GLUI_INCLUDE=.


FLAGS = -I$(GLUI_INCLUDE) -I$(GLUT_INCLUDE) -L$(GLUI_LIB) -lglui -lglut -lXmu  -lXext -lGLEW -lX11 -lGL -lGLU -lm -g3

$(EXEC): $(SOURCE)
	$(CC) -o $(EXEC) $(SOURCE) $(FLAGS)
