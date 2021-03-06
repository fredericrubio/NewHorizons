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
LDLIBSOPTIONS=-L../RaspiCam/Lib -Wl,-rpath,'../ComLib/dist/Debug/GNU-Linux' -L../ComLib/dist/Debug/GNU-Linux -lComLib ../DataLib/dist/Debug/GNU-Linux/libdatalib.a ../CameraLib/dist/Debug/GNU-Linux/libcameralib.a -lraspicam

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/hem_client

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/hem_client: ../ComLib/dist/Debug/GNU-Linux/libComLib.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/hem_client: ../DataLib/dist/Debug/GNU-Linux/libdatalib.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/hem_client: ../CameraLib/dist/Debug/GNU-Linux/libcameralib.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/hem_client: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/hem_client ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../ComLib/Inc -I../DataLib/Inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:
	cd ../ComLib && ${MAKE}  -f Makefile CONF=Debug
	cd ../DataLib && ${MAKE}  -f Makefile CONF=Debug
	cd ../CameraLib && ${MAKE}  -f Makefile CONF=Debug
	cd ../ComLib && ${MAKE}  -f Makefile CONF=Debug
	cd ../DataLib && ${MAKE}  -f Makefile CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} -r ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libComLib.so
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/hem_client

# Subprojects
.clean-subprojects:
	cd ../ComLib && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../DataLib && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../CameraLib && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../ComLib && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../DataLib && ${MAKE}  -f Makefile CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
