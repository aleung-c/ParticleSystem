NAME = ParticleSystem

SRC = ./src/main.cpp \
./src/Engine/EngineController/EngineController.cpp \
./src/Engine/EngineController/EngineController_freetype_font.cpp \
./src/Engine/EngineController/EngineController_parsing.cpp \
./src/Engine/EngineController/EngineController_shaders.cpp \
./src/Engine/EngineController/EngineController_textures.cpp \
./src/Engine/EngineController/EngineController_matrices.cpp \
./src/Engine/EngineController/EngineController_camera.cpp \
./src/Engine/EngineController/EngineController_rendering.cpp \
./src/Engine/Managers/TextureManager.cpp \
./src/Engine/ObjectsComponents/BoundingBoxComponent.cpp \
./src/Engine/ObjectsComponents/TextureComponent.cpp \
./src/Engine/ObjectsComponents/MorphTargetAnimation.cpp \
./src/Engine/EngineObjects/EngineObject.cpp \
./src/Engine/EngineObjects/GameObject.cpp \
./src/Engine/EngineObjects/GameObject_Parsing.cpp \
./src/Engine/EngineObjects/GameUIObject.cpp \
./src/Engine/EngineObjects/GameTextObject.cpp

OBJ = $(SRC:.cpp=.o)

CC = clang++ -g -Wall -Werror -Wextra $(FREETYPE2_I)

# Linking opengl and lib GLFW.
GL = -framework OpenGL
GLFW = -framework Cocoa -framework CoreVideo -framework IOKit -framework GLUT -L./glfw-3.2.1/src -lglfw3

# For freetype true font loading library
FREETYPE2_L = -L./freetype-2.4.0/objs -lfreetype
FREETYPE2_I = -I./freetype-2.4.0/include

all: freetype $(NAME)

$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(GL) $(GLFW) $(FREETYPE2_L)

%.o: %.cpp
	$(CC) -o $@ -c $<

freetype :
	make -C ./freetype-2.4.0/

clean:
	rm -rf $(OBJ)
	make clean -C ./freetype-2.4.0/

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
