#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-Wl,-rpath,'../ComLib/dist/Debug/GNU-Linux' -L../ComLib/dist/Debug/GNU-Linux -lComLib ../DataLib/dist/Debug/GNU-Linux/libdatalib.a ../CameraLib/dist/Debug/GNU-Linux/libcameralib.a -Wl,-rpath,'../UtilsLib/dist/Debug/GNU-Linux' -L../UtilsLib/dist/Debug/GNU-Linux -lUtilsLib ../RaspiCam/Lib/libraspicam.so

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/hem_server

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/hem_server: ../ComLib/dist/Debug/GNU-Linux/libComLib.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/hem_server: ../DataLib/dist/Debug/GNU-Linux/libdatalib.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/hem_server: ../CameraLib/dist/Debug/GNU-Linux/libcameralib.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/hem_server: ../UtilsLib/dist/Debug/GNU-Linux/libUtilsLib.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/hem_server: ../RaspiCam/Lib/libraspicam.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/hem_server: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/hem_server ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../ComLib/Inc -I../UtilsLib/Inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:
	cd ../ComLib && ${MAKE}  -f Makefile CONF=Debug
	cd ../DataLib && ${MAKE}  -f Makefile CONF=Debug
	cd ../CameraLib && ${MAKE}  -f Makefile CONF=Debug
	cd ../UtilsLib && ${MAKE}  -f Makefile CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} -r ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libComLib.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libUtilsLib.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libraspicam.so
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/hem_server

# Subprojects
.clean-subprojects:
	cd ../ComLib && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../DataLib && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../CameraLib && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../UtilsLib && ${MAKE}  -f Makefile CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
