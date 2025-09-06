HEADERS_DIR := include
RAYLIB_HEADERS_DIR := C:/dev/vcpkg/installed/x64-windows/include
RAYLIB_LIB_DIR := C:/dev/vcpkg/installed/x64-windows/lib
SRC_DIR := src
BIN_DIR := bin
OUT_DIR := out

MAIN_OBJ := ${BIN_DIR}/main.o
LAUNCHOPTIONS_OBJ := ${BIN_DIR}/launchOptions.o
ORBITALSIM_OBJ := ${BIN_DIR}/orbitalSim.o
VIEW_OBJ := ${BIN_DIR}/view.o
EPHEMERIDES_OBJ := ${BIN_DIR}/ephemerides.o
NMATH_OBJ := ${BIN_DIR}/nmath.o
KEYBINDS_OBJ := ${BIN_DIR}/keyBinds.o
ORBITALSIM_EXE := ${OUT_DIR}/orbitalSim.exe

MAIN_DEPENDENCIES := ${SRC_DIR}/main.cpp ${HEADERS_DIR}/launchOptions.h \
	${HEADERS_DIR}/orbitalSim.h ${HEADERS_DIR}/view.h \
	${HEADERS_DIR}/ephemerides.h ${HEADERS_DIR}/nmath.h

LAUNCHOPTIONS_DEPENDENCIES := ${SRC_DIR}/launchOptions.c ${HEADERS_DIR}/launchOptions.h

ORBITALSIM_DEPENDENCIES := ${SRC_DIR}/orbitalSim.cpp ${HEADERS_DIR}/orbitalSim.h \
	${HEADERS_DIR}/ephemerides.h ${HEADERS_DIR}/nmath.h \
	${HEADERS_DIR}/keyBinds.h

VIEW_DEPENDENCIES := ${SRC_DIR}/view.cpp ${HEADERS_DIR}/view.h \
	${HEADERS_DIR}/orbitalSim.h ${HEADERS_DIR}/ephemerides.h \
	${HEADERS_DIR}/nmath.h ${HEADERS_DIR}/keyBinds.h

EPHEMERIDES_DEPENDENCIES := ${SRC_DIR}/ephemerides.c ${HEADERS_DIR}/ephemerides.h ${HEADERS_DIR}/nmath.h

NMATH_DEPENDENCIES := ${SRC_DIR}/nmath.c ${HEADERS_DIR}/nmath.h

KEYBINDS_DEPENDENCIES := ${SRC_DIR}/keyBinds.c ${HEADERS_DIR}/keyBinds.h

CC := g++
CFLAGS := -Wall -O3 -I${HEADERS_DIR} -I${RAYLIB_HEADERS_DIR}
LDFLAGS := -L${RAYLIB_LIB_DIR} -lraylib -lopengl32 -lgdi32 -lwinmm

${ORBITALSIM_EXE}: ${MAIN_OBJ} ${LAUNCHOPTIONS_OBJ} ${ORBITALSIM_OBJ} ${VIEW_OBJ} ${EPHEMERIDES_OBJ} ${NMATH_OBJ} ${KEYBINDS_OBJ}
	${CC} ${CFLAGS} -o ${ORBITALSIM_EXE} ${MAIN_OBJ} ${LAUNCHOPTIONS_OBJ} ${ORBITALSIM_OBJ} \
	${VIEW_OBJ} ${EPHEMERIDES_OBJ} ${NMATH_OBJ} ${KEYBINDS_OBJ} ${LDFLAGS}

${MAIN_OBJ}: ${MAIN_DEPENDENCIES}
	${CC} ${CFLAGS} -c ${SRC_DIR}/main.cpp -o ${MAIN_OBJ}

${LAUNCHOPTIONS_OBJ}: ${LAUNCHOPTIONS_DEPENDENCIES}
	${CC} ${CFLAGS} -c ${SRC_DIR}/launchOptions.c -o ${LAUNCHOPTIONS_OBJ}

${ORBITALSIM_OBJ}: ${ORBITALSIM_DEPENDENCIES}
	${CC} ${CFLAGS} -c ${SRC_DIR}/orbitalSim.cpp -o ${ORBITALSIM_OBJ}

${VIEW_OBJ}: ${VIEW_DEPENDENCIES}
	${CC} ${CFLAGS} -c ${SRC_DIR}/view.cpp -o ${VIEW_OBJ}

${EPHEMERIDES_OBJ}: ${EPHEMERIDES_DEPENDENCIES}
	${CC} ${CFLAGS} -c ${SRC_DIR}/ephemerides.c -o ${EPHEMERIDES_OBJ}

${NMATH_OBJ}: ${NMATH_DEPENDENCIES}
	${CC} ${CFLAGS} -c ${SRC_DIR}/nmath.c -o ${NMATH_OBJ}

${KEYBINDS_OBJ}: ${KEYBINDS_DEPENDENCIES}
	${CC} ${CFLAGS} -c ${SRC_DIR}/keyBinds.c -o ${KEYBINDS_OBJ}

clean:
	del ${BIN_DIR}\*.o
	del ${OUT_DIR}\*.exe