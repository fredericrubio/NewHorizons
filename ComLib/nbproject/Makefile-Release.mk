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
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/Src/HEM_Client.o \
	${OBJECTDIR}/Src/HEM_Message.o \
	${OBJECTDIR}/Src/HEM_Server.o \
	${OBJECTDIR}/Src/IMP_Client.o \
	${OBJECTDIR}/Src/IMP_ImageSizeMessageBody.o \
	${OBJECTDIR}/Src/IMP_Message.o \
	${OBJECTDIR}/Src/IMP_MessageBody.o \
	${OBJECTDIR}/Src/IMP_Server.o \
	${OBJECTDIR}/Src/IMP_ackMessageBody.o


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
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libComLib.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libComLib.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libComLib.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -shared -fPIC

${OBJECTDIR}/Src/HEM_Client.o: Src/HEM_Client.cpp
	${MKDIR} -p ${OBJECTDIR}/Src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Src/HEM_Client.o Src/HEM_Client.cpp

${OBJECTDIR}/Src/HEM_Message.o: Src/HEM_Message.cpp
	${MKDIR} -p ${OBJECTDIR}/Src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Src/HEM_Message.o Src/HEM_Message.cpp

${OBJECTDIR}/Src/HEM_Server.o: Src/HEM_Server.cpp
	${MKDIR} -p ${OBJECTDIR}/Src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Src/HEM_Server.o Src/HEM_Server.cpp

${OBJECTDIR}/Src/IMP_Client.o: Src/IMP_Client.cpp
	${MKDIR} -p ${OBJECTDIR}/Src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Src/IMP_Client.o Src/IMP_Client.cpp

${OBJECTDIR}/Src/IMP_ImageSizeMessageBody.o: Src/IMP_ImageSizeMessageBody.cpp
	${MKDIR} -p ${OBJECTDIR}/Src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Src/IMP_ImageSizeMessageBody.o Src/IMP_ImageSizeMessageBody.cpp

${OBJECTDIR}/Src/IMP_Message.o: Src/IMP_Message.cpp
	${MKDIR} -p ${OBJECTDIR}/Src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Src/IMP_Message.o Src/IMP_Message.cpp

${OBJECTDIR}/Src/IMP_MessageBody.o: Src/IMP_MessageBody.cpp
	${MKDIR} -p ${OBJECTDIR}/Src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Src/IMP_MessageBody.o Src/IMP_MessageBody.cpp

${OBJECTDIR}/Src/IMP_Server.o: Src/IMP_Server.cpp
	${MKDIR} -p ${OBJECTDIR}/Src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Src/IMP_Server.o Src/IMP_Server.cpp

${OBJECTDIR}/Src/IMP_ackMessageBody.o: Src/IMP_ackMessageBody.cpp
	${MKDIR} -p ${OBJECTDIR}/Src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Src/IMP_ackMessageBody.o Src/IMP_ackMessageBody.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
