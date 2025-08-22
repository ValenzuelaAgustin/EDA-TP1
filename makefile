CC := g++
CFLAGS := -Wall -O3 -IC:/dev/vcpkg/installed/x64-windows/include
LDFLAGS := -LC:/dev/vcpkg/installed/x64-windows/lib -lraylib -lopengl32 -lgdi32 -lwinmm
OBJDIR := obj
OUTDIR := out

MAIN_OBJ := ${OBJDIR}/main.o
ORBITALSIM_OBJ := ${OBJDIR}/orbitalSim.o
VIEW_OBJ := ${OBJDIR}/view.o
ORBITAL_SIM := ${OUTDIR}/orbitalSim.exe

${ORBITAL_SIM}: ${MAIN_OBJ} ${ORBITALSIM_OBJ} ${VIEW_OBJ}
	${CC} ${CFLAGS} -o ${ORBITAL_SIM} ${MAIN_OBJ} ${ORBITALSIM_OBJ} ${VIEW_OBJ} ${LDFLAGS}

${MAIN_OBJ}: main.cpp orbitalSim.h view.h
	${CC} ${CFLAGS} -c main.cpp -o ${MAIN_OBJ}

${ORBITALSIM_OBJ}: orbitalSim.cpp orbitalSim.h ephemerides.h
	${CC} ${CFLAGS} -c orbitalSim.cpp -o ${ORBITALSIM_OBJ}

${VIEW_OBJ}: view.cpp view.h orbitalSim.h
	${CC} ${CFLAGS} -c view.cpp -o ${VIEW_OBJ}

clean:
	del ${OBJDIR}\*.o
	del ${OUTDIR}\*.exe