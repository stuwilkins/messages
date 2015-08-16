TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = messages
# messages.dbd will be created and installed
DBD += messages.dbd

# messages.dbd will be made up from these files:
messages_DBD += base.dbd

# Include dbd files from all support applications:
messages_DBD += mail.dbd
messages_DBD += asSupport.dbd
messages_DBD += system.dbd
messages_DBD += caPutLog.dbd
messages_DBD += devIocStats.dbd

# Add all the support libraries needed by this IOC
messages_LIBS += curl
messages_LIBS += caPutLog
messages_LIBS += autosave
messages_LIBS += devIocStats

# messages_registerRecordDeviceDriver.cpp derives from messages.dbd
messages_SRCS += messages_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
messages_SRCS_DEFAULT += messagesMain.cpp
messages_SRCS_DEFAULT += mail.c
messages_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#messages_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
messages_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

