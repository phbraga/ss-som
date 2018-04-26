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
CND_CONF=Release_x64
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/a341d9b2/DisplayMap.o \
	${OBJECTDIR}/_ext/a845ff68/ClusteringMetrics.o \
	${OBJECTDIR}/_ext/a845ff68/ClusteringSOM.o \
	${OBJECTDIR}/_ext/a342a8fc/ArffData.o \
	${OBJECTDIR}/_ext/c5140741/DebugOut.o \
	${OBJECTDIR}/_ext/d0624b86/Defines.o \
	${OBJECTDIR}/_ext/8bffeb2f/MatUtils.o \
	${OBJECTDIR}/_ext/15c88dff/DSNode.o \
	${OBJECTDIR}/_ext/15c88dff/NodeW.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=-m64

# CC Compiler Flags
CCFLAGS=-m64 -m64 -g -Wno-comment
CXXFLAGS=-m64 -m64 -g -Wno-comment

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/larfdssom

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/larfdssom: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/larfdssom ${OBJECTFILES} ${LDLIBSOPTIONS} -m64 -L/usr/X11/lib -static-libstdc++

${OBJECTDIR}/_ext/a341d9b2/DisplayMap.o: ../Libs/CImg/DisplayMap.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/a341d9b2
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I../SOM -I../Cluster -I../../../Libs/Data -I../../../Libs/Debug -I../../../Libs/Defines -I../../../Libs/MatMatrix -I../../../Libs/Parameters -I../../../Libs/CImg -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/a341d9b2/DisplayMap.o ../Libs/CImg/DisplayMap.cpp

${OBJECTDIR}/_ext/a845ff68/ClusteringMetrics.o: ../Libs/Cluster/ClusteringMetrics.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/a845ff68
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I../SOM -I../Cluster -I../../../Libs/Data -I../../../Libs/Debug -I../../../Libs/Defines -I../../../Libs/MatMatrix -I../../../Libs/Parameters -I../../../Libs/CImg -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/a845ff68/ClusteringMetrics.o ../Libs/Cluster/ClusteringMetrics.cpp

${OBJECTDIR}/_ext/a845ff68/ClusteringSOM.o: ../Libs/Cluster/ClusteringSOM.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/a845ff68
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I../SOM -I../Cluster -I../../../Libs/Data -I../../../Libs/Debug -I../../../Libs/Defines -I../../../Libs/MatMatrix -I../../../Libs/Parameters -I../../../Libs/CImg -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/a845ff68/ClusteringSOM.o ../Libs/Cluster/ClusteringSOM.cpp

${OBJECTDIR}/_ext/a342a8fc/ArffData.o: ../Libs/Data/ArffData.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/a342a8fc
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I../SOM -I../Cluster -I../../../Libs/Data -I../../../Libs/Debug -I../../../Libs/Defines -I../../../Libs/MatMatrix -I../../../Libs/Parameters -I../../../Libs/CImg -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/a342a8fc/ArffData.o ../Libs/Data/ArffData.cpp

${OBJECTDIR}/_ext/c5140741/DebugOut.o: ../Libs/Debug/DebugOut.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/c5140741
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I../SOM -I../Cluster -I../../../Libs/Data -I../../../Libs/Debug -I../../../Libs/Defines -I../../../Libs/MatMatrix -I../../../Libs/Parameters -I../../../Libs/CImg -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/c5140741/DebugOut.o ../Libs/Debug/DebugOut.cpp

${OBJECTDIR}/_ext/d0624b86/Defines.o: ../Libs/Defines/Defines.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/d0624b86
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I../SOM -I../Cluster -I../../../Libs/Data -I../../../Libs/Debug -I../../../Libs/Defines -I../../../Libs/MatMatrix -I../../../Libs/Parameters -I../../../Libs/CImg -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/d0624b86/Defines.o ../Libs/Defines/Defines.cpp

${OBJECTDIR}/_ext/8bffeb2f/MatUtils.o: ../Libs/MatMatrix/MatUtils.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/8bffeb2f
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I../SOM -I../Cluster -I../../../Libs/Data -I../../../Libs/Debug -I../../../Libs/Defines -I../../../Libs/MatMatrix -I../../../Libs/Parameters -I../../../Libs/CImg -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/8bffeb2f/MatUtils.o ../Libs/MatMatrix/MatUtils.cpp

${OBJECTDIR}/_ext/15c88dff/DSNode.o: ../Libs/SOM/DSNode.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/15c88dff
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I../SOM -I../Cluster -I../../../Libs/Data -I../../../Libs/Debug -I../../../Libs/Defines -I../../../Libs/MatMatrix -I../../../Libs/Parameters -I../../../Libs/CImg -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/15c88dff/DSNode.o ../Libs/SOM/DSNode.cpp

${OBJECTDIR}/_ext/15c88dff/NodeW.o: ../Libs/SOM/NodeW.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/15c88dff
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I../SOM -I../Cluster -I../../../Libs/Data -I../../../Libs/Debug -I../../../Libs/Defines -I../../../Libs/MatMatrix -I../../../Libs/Parameters -I../../../Libs/CImg -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/15c88dff/NodeW.o ../Libs/SOM/NodeW.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I../SOM -I../Cluster -I../../../Libs/Data -I../../../Libs/Debug -I../../../Libs/Defines -I../../../Libs/MatMatrix -I../../../Libs/Parameters -I../../../Libs/CImg -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

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
