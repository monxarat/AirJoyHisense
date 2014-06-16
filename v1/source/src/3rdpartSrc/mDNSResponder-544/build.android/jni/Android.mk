# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)

############################################################
#
# mdnscore -- mdns core, by ouyang, 2011-3-8
#
############################################################
#include $(CLEAR_VARS)

#LOCAL_MODULE    := mdnscore 

#LOCAL_SRC_FILES := ../../mDNSCore/mDNS.c 	    \
		   ../../mDNSCore/DNSDigest.c 	        \
		   ../../mDNSCore/uDNS.c 		        \
		   ../../mDNSCore/DNSCommon.c           \
		   ../../mDNSPosix/mDNSPosix.c   	    \
		   ../../mDNSPosix/mDNSUNP.c 	        \
		   ../../mDNSShared/mDNSDebug.c         \
		   ../../mDNSShared/PlatformCommon.c    \
		   ../../mDNSShared/uds_daemon.c 	    \
		   ../../mDNSShared/GenLinkedList.c     \
		   ../../mDNSShared/dnssd_ipc.c

#LOCAL_CFLAGS := -I../mDNSCore 	        \
                -I../mDNSShared	        \
		        -fwrapv	                \
		        -w	                    \
		        -Wall                   \
		        -DPID_FILE=\"/data/data/cn.airjoy.android.service/mdnsd.pid\" 	        \
		        -DMDNS_UDS_SERVERPATH=\"/data/data/cn.airjoy.android.service/mdnsd\"   \
		        -DNOT_HAVE_SA_LEN 	\
		        -DUSES_NETLINK 		\
		        -DHAVE_LINUX 		\
		        -DTARGET_OS_LINUX 	\
		        -Os 			    \
		        -DMDNS_DEBUGMSGS=0  \
                -D__ANDROID__       \
                -DANDROID 

		        #-DUSE_TCP_LOOPBACK                          
		        #-DMDNS_TCP_SERVERPORT=56789                 

#include $(BUILD_SHARED_LIBRARY)


############################################################
#
# mdnsd -- mdns daemon, by ouyang, 2010-2-23
#
############################################################
include $(CLEAR_VARS)

LOCAL_MODULE    := mdnsd_aj 

LOCAL_SRC_FILES := ../../mDNSPosix/PosixDaemon.c 	    \
                   ../../mDNSPosix/mDNSPosix.c   	    \
                   ../../mDNSPosix/mDNSUNP.c 	        \
                   ../../mDNSCore/mDNS.c 		        \
                   ../../mDNSCore/DNSDigest.c 	        \
                   ../../mDNSCore/uDNS.c 		        \
                   ../../mDNSCore/DNSCommon.c 	        \
                   ../../mDNSShared/uds_daemon.c 	    \
                   ../../mDNSShared/mDNSDebug.c 	    \
                   ../../mDNSShared/dnssd_ipc.c 	    \
                   ../../mDNSShared/GenLinkedList.c     \
                   ../../mDNSShared/PlatformCommon.c    \
                   ../../mDNSCore/CryptoAlg.c 		    \
                   ../../mDNSCore/anonymous.c

LOCAL_CFLAGS := -I../mDNSCore 	\
                -I../mDNSShared	\
		        -fwrapv	\
		        -w	    \
		        -Wall   \
		        -DPID_FILE=\"/data/data/cn.airjoy.android.service/mdnsd.pid\"	         \
		        -DMDNS_UDS_SERVERPATH=\"/data/data/cn.airjoy.android.service/mdnsd\"    \
		        -DNOT_HAVE_SA_LEN 	\
		        -DUSES_NETLINK 		\
		        -DHAVE_LINUX 		\
		        -DTARGET_OS_LINUX 	\
		        -Os 			    \
		        -DMDNS_DEBUGMSGS=0  \
                -D__ANDROID__       \
                -DANDROID 

include $(BUILD_EXECUTABLE)

############################################################
#
# libbonjour.so -- mdns daemon, by ouyang, 2010-3-7
#
############################################################
include $(CLEAR_VARS)

LOCAL_MODULE    := libbonjour_aj

LOCAL_SRC_FILES := ../../mDNSPosix/Bonjour.c 	        \
                   ../../mDNSPosix/mDNSPosix.c   	    \
                   ../../mDNSPosix/mDNSUNP.c    	    \
                   ../../mDNSCore/mDNS.c 		        \
                   ../../mDNSCore/DNSDigest.c 	        \
                   ../../mDNSCore/uDNS.c 		        \
                   ../../mDNSCore/DNSCommon.c 	        \
                   ../../mDNSShared/uds_daemon.c 	    \
                   ../../mDNSShared/mDNSDebug.c 	    \
                   ../../mDNSShared/dnssd_ipc.c 	    \
                   ../../mDNSShared/GenLinkedList.c 	\
                   ../../mDNSShared/PlatformCommon.c    \
                   ../../mDNSCore/CryptoAlg.c 		    \
                   ../../mDNSCore/anonymous.c

LOCAL_CFLAGS := -I../mDNSCore 	\
                -I../mDNSShared	\
		        -fwrapv	\
		        -w	    \
		        -Wall   \
		        -DPID_FILE=\"/data/data/cn.airjoy.android.service/mdnsd.pid\"          \
		        -DMDNS_UDS_SERVERPATH=\"/data/data/cn.airjoy.android.service/mdnsd\"   \
		        -DNOT_HAVE_SA_LEN 	\
		        -DUSES_NETLINK 		\
		        -DHAVE_LINUX 		\
		        -DTARGET_OS_LINUX 	\
		        -Os 			    \
		        -DMDNS_DEBUGMSGS=0  \
                -D__ANDROID__       \
                -DANDROID 

include $(BUILD_SHARED_LIBRARY)

############################################################
#
# test for libbonjour.so -- mdns daemon, by ouyang, 2010-3-7
#
############################################################
include $(CLEAR_VARS)

LOCAL_MODULE    := testBonjour_aj

LOCAL_SRC_FILES := ../../mDNSPosix/testBonjour.c

LOCAL_CFLAGS := -I../mDNSCore       \
                -I../mDNSPosix      \
                -I../mDNSShared	    \
		        -fwrapv	\
		        -w	    \
		        -Wall   \
		        -DPID_FILE=\"/data/data/cn.airjoy.android.service/mdnsd.pid\" 	        \
		        -DMDNS_UDS_SERVERPATH=\"/data/data/cn.airjoy.android.service/mdnsd\"   \
		        -DNOT_HAVE_SA_LEN 	\
		        -DUSES_NETLINK 		\
		        -DHAVE_LINUX 		\
		        -DTARGET_OS_LINUX 	\
		        -Os 			    \
		        -DMDNS_DEBUGMSGS=0  \
                -D__ANDROID__       \
                -DANDROID 

LOCAL_SHARED_LIBRARIES := bonjour_aj

include $(BUILD_EXECUTABLE)


############################################################
#
# libdns_sd.so -- by ouyang, 2010-2-23
#
############################################################
include $(CLEAR_VARS)

LOCAL_MODULE := libdns_sd_aj

LOCAL_SRC_FILES := ../../mDNSShared/dnssd_clientlib.c    \
                   ../../mDNSShared/dnssd_clientstub.c   \
                   ../../mDNSShared/dnssd_ipc.c

LOCAL_CFLAGS := -I../mDNSCore 	\
                -I../mDNSShared	\
		        -fwrapv	\
		        -w	    \
		        -Wall   \
		        -DPID_FILE=\"/data/data/cn.airjoy.android.service/mdnsd.pid\"           \
		        -DMDNS_UDS_SERVERPATH=\"/data/data/cn.airjoy.android.service/mdnsd\"    \
		        -DNOT_HAVE_SA_LEN 	\
		        -DUSES_NETLINK 		\
		        -DHAVE_LINUX 		\
		        -DTARGET_OS_LINUX 	\
		        -Os 			    \
		        -DMDNS_DEBUGMSGS=0  \
                -D__ANDROID__       \
                -DANDROID 

include $(BUILD_SHARED_LIBRARY)

############################################################
#
# mDNSNetMonitor -- by ouyang, 2010-2-25
#
############################################################
include $(CLEAR_VARS)

LOCAL_MODULE := mDNSNetMonitor_aj

LOCAL_SRC_FILES := ../../mDNSPosix/mDNSPosix.c         \
                   ../../mDNSPosix/mDNSUNP.c           \
                   ../../mDNSShared/mDNSDebug.c        \
                   ../../mDNSShared/GenLinkedList.c    \
                   ../../mDNSCore/DNSDigest.c          \
                   ../../mDNSCore/uDNS.c               \
                   ../../mDNSCore/DNSCommon.c          \
                   ../../mDNSCore/CryptoAlg.c 		    \
                   ../../mDNSCore/anonymous.c 		    \
                   ../../mDNSShared/PlatformCommon.c   \
                   ../../mDNSPosix/NetMonitor.c        

LOCAL_CFLAGS := -I../mDNSCore 	\
                -I../mDNSShared	\
		        -fwrapv	\
		        -w	    \
		        -Wall   \
		        -DPID_FILE=\"/data/data/cn.airjoy.android.service/mdnsd.pid\"         \
		        -DMDNS_UDS_SERVERPATH=\"/data/data/cn.airjoy.android.service/mdnsd\"  \
		        -DNOT_HAVE_SA_LEN 	\
		        -DUSES_NETLINK 		\
		        -DHAVE_LINUX 		\
		        -DTARGET_OS_LINUX 	\
		        -Os 			    \
		        -DMDNS_DEBUGMSGS=0  \
                -D__ANDROID__       \
                -DANDROID 

include $(BUILD_EXECUTABLE)

############################################################
#
# dns-sd -- by ouyang, 2010-2-25
#
############################################################
#include $(CLEAR_VARS)

#LOCAL_MODULE := dns-sd_aj

#LOCAL_SRC_FILES := ../../Clients/dns-sd.c         \
                   ../../Clients/ClientCommon.c   \
                   ../../Clients/dnsctl.c           

#LOCAL_CFLAGS := -I../mDNSCore 	\
                -I../mDNSShared	\
		        -fwrapv	\
		        -w	    \
		        -Wall   \
		        -DPID_FILE=\"/data/data/cn.airjoy.android.service/mdnsd.pid\"	         \
		        -DMDNS_UDS_SERVERPATH=\"/data/data/cn.airjoy.android.service/mdnsd\"    \
		        -DNOT_HAVE_SA_LEN 	\
		        -DUSES_NETLINK 		\
		        -DHAVE_LINUX 		\
		        -DTARGET_OS_LINUX 	\
		        -Os 			    \
		        -DMDNS_DEBUGMSGS=0  \
                -D__ANDROID__       \
                -DANDROID 

#LOCAL_SHARED_LIBRARIES := dns_sd_aj

#include $(BUILD_EXECUTABLE)
