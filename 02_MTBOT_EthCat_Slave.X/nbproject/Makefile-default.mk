#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/02_MTBOT_EthCat_Slave.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/02_MTBOT_EthCat_Slave.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=startup_samd51j19a.c system_samd51j19a.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/startup_samd51j19a.o ${OBJECTDIR}/system_samd51j19a.o
POSSIBLE_DEPFILES=${OBJECTDIR}/startup_samd51j19a.o.d ${OBJECTDIR}/system_samd51j19a.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/startup_samd51j19a.o ${OBJECTDIR}/system_samd51j19a.o

# Source Files
SOURCEFILES=startup_samd51j19a.c system_samd51j19a.c

# Pack Options 
PACK_COMMON_OPTIONS=-I "${DFP_DIR}/samd51a/include"  -I "${CMSIS_DIR}/CMSIS/Core/Include"



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/02_MTBOT_EthCat_Slave.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=SAMD51J19A
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/startup_samd51j19a.o: startup_samd51j19a.c  .generated_files/flags/default/af10f31692afff213a82365f4b3cae8595d480dd .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/startup_samd51j19a.o.d 
	@${RM} ${OBJECTDIR}/startup_samd51j19a.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/startup_samd51j19a.o.d" -o ${OBJECTDIR}/startup_samd51j19a.o startup_samd51j19a.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/system_samd51j19a.o: system_samd51j19a.c  .generated_files/flags/default/9e0e3823a03fc3147e995ee198f084078826cd7c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/system_samd51j19a.o.d 
	@${RM} ${OBJECTDIR}/system_samd51j19a.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4 -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/system_samd51j19a.o.d" -o ${OBJECTDIR}/system_samd51j19a.o system_samd51j19a.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
else
${OBJECTDIR}/startup_samd51j19a.o: startup_samd51j19a.c  .generated_files/flags/default/efef311a0acd5ff19a739733eac37fb06eaf4420 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/startup_samd51j19a.o.d 
	@${RM} ${OBJECTDIR}/startup_samd51j19a.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/startup_samd51j19a.o.d" -o ${OBJECTDIR}/startup_samd51j19a.o startup_samd51j19a.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/system_samd51j19a.o: system_samd51j19a.c  .generated_files/flags/default/9a018fdd165fbcaeead1693d126067fa0c491009 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/system_samd51j19a.o.d 
	@${RM} ${OBJECTDIR}/system_samd51j19a.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m4  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/system_samd51j19a.o.d" -o ${OBJECTDIR}/system_samd51j19a.o system_samd51j19a.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/02_MTBOT_EthCat_Slave.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    samd51j19a_flash.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=cortex-m4  -D__MPLAB_DEBUGGER_SIMULATOR=1 -gdwarf-2  -D__$(MP_PROCESSOR_OPTION)__   -T"samd51j19a_flash.ld"  -mthumb --specs=nosys.specs -Wl,-Map="${DISTDIR}\02_MTBOT_EthCat_Slave.X.${IMAGE_TYPE}.map"  -o ${DISTDIR}/02_MTBOT_EthCat_Slave.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}      -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_SIMULATOR=1 -Wl,--gc-sections  
	
	
	
	
	
	
else
${DISTDIR}/02_MTBOT_EthCat_Slave.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   samd51j19a_flash.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=cortex-m4  -D__$(MP_PROCESSOR_OPTION)__   -T"samd51j19a_flash.ld"  -mthumb --specs=nosys.specs -Wl,-Map="${DISTDIR}\02_MTBOT_EthCat_Slave.X.${IMAGE_TYPE}.map"  -o ${DISTDIR}/02_MTBOT_EthCat_Slave.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}      -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION) -Wl,--gc-sections  
	
	${MP_CC_DIR}\\arm-none-eabi-objcopy -O ihex -R .eeprom -R .fuse -R .lock -R .signature "${DISTDIR}/02_MTBOT_EthCat_Slave.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "${DISTDIR}/02_MTBOT_EthCat_Slave.X.${IMAGE_TYPE}.hex"
	
	
	
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
