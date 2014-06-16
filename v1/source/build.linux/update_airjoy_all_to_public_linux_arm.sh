#!/bin/sh

#-----------------------------------------------------------------
#
# for common library
#
#-----------------------------------------------------------------
cp ../src/3rdpartLib/Linux.arm/* ../public.airjoy.all/linux.arm/lib/

#-----------------------------------------------------------------
#
# common include header
#
#-----------------------------------------------------------------
cp ../src/AirCommon/include/AirJoyApi.hpp ../public.airjoy.all/linux.arm/include/
cp ../src/AirCommon/include/AirJoyTypeDef.hpp ../public.airjoy.all/linux.arm/include/

#-----------------------------------------------------------------
#
# for libairplay.so
#
#-----------------------------------------------------------------
cp output/libairplay.so ../public.airjoy.all/linux.arm/lib/

cp ../src/AirJoyAll/AirPlayX/Common/include/AirPlayServiceInfo.hpp ../public.airjoy.all/linux.arm/include/
cp ../src/AirJoyAll/AirPlayX/Common/include/AirTunesServiceInfo.hpp ../public.airjoy.all/linux.arm/include/

cp ../src/AirJoyAll/AirPlayX/AirPlayNsd/include/AirPlayNsd.hpp ../public.airjoy.all/linux.arm/include/
cp ../src/AirJoyAll/AirPlayX/AirPlayNsd/include/AirPlayNsdDiscoveryListener.hpp ../public.airjoy.all/linux.arm/include/
cp ../src/AirJoyAll/AirPlayX/AirPlayNsd/include/AirPlayNsdPublishListener.hpp ../public.airjoy.all/linux.arm/include/


cp ../src/AirJoyAll/AirPlayX/AirPlay/server/include/AirPlayServer.hpp ../public.airjoy.all/linux.arm/include/
cp ../src/AirJoyAll/AirPlayX/AirPlay/server/include/AirPlayServerDefault.hpp ../public.airjoy.all/linux.arm/include/
cp ../src/AirJoyAll/AirPlayX/AirPlay/server/include/AirPlayServerListener.hpp ../public.airjoy.all/linux.arm/include/

#-----------------------------------------------------------------
#
# demo for airplay
#
#-----------------------------------------------------------------
cp ../src/AirJoyAll/AirPlayX/testAirPlayServer/testAirPlayServer.cpp ../public.airjoy.all/linux.arm/testAirPlayServer/

#-----------------------------------------------------------------
#
# for libairtunes.so
#
#-----------------------------------------------------------------
cp output/libairtunes.so ../public.airjoy.all/linux.arm/lib/

cp ../src/AirJoyAll/AirPlayX/AirTunes/server/include/AirTunesServer.hpp ../public.airjoy.all/linux.arm/include/
cp ../src/AirJoyAll/AirPlayX/AirTunes/server/include/AirTunesServerListener.hpp ../public.airjoy.all/linux.arm/include/

#-----------------------------------------------------------------
#
# demo for airtunes
#
#-----------------------------------------------------------------
cp ../src/AirJoyAll/AirPlayX/testAirTunesServer/testAirTunesServer.cpp ../public.airjoy.all/linux.arm/testAirTunesServer/

#-----------------------------------------------------------------
#
# for libairjoy.so
#
#-----------------------------------------------------------------
cp output/libairjoy.so ../public.airjoy.all/linux.arm/lib/

cp ../src/AirJoyAll/AirJoyX/AirJoyNsd/include/AirJoyNsd.hpp ../public.airjoy.all/linux.arm/include/
cp ../src/AirJoyAll/AirJoyX/AirJoyNsd/include/AirJoyNsdDiscoveryListener.hpp ../public.airjoy.all/linux.arm/include/

cp ../src/AirJoyAll/AirJoyX/AirJoy/server/include/AirJoyServer.hpp ../public.airjoy.all/linux.arm/include/
cp ../src/AirJoyAll/AirJoyX/AirJoy/server/include/AirJoyServiceInfo.hpp ../public.airjoy.all/linux.arm/include/
cp ../src/AirJoyAll/AirJoyX/AirJoy/server/include/AirJoyDefault.hpp ../public.airjoy.all/linux.arm/include/
cp ../src/AirJoyAll/AirJoyX/AirJoy/server/include/AirMediaListener.hpp ../public.airjoy.all/linux.arm/include/
cp ../src/AirJoyAll/AirJoyX/AirJoy/server/include/AirFileListener.hpp ../public.airjoy.all/linux.arm/include/

cp ../src/AirJoyAll/AirJoyX/AirJoy/client/include/AirJoyClient.hpp ../public.airjoy.all/linux.arm/include/
cp ../src/AirJoyAll/AirJoyX/AirJoy/client/include/AirMediaClient.hpp ../public.airjoy.all/linux.arm/include/
cp ../src/AirJoyAll/AirJoyX/AirJoy/client/include/AirMediaClientQueryListener.hpp ../public.airjoy.all/linux.arm/include/
cp ../src/AirJoyAll/AirJoyX/AirJoy/client/include/AirMediaClientAudio.hpp ../public.airjoy.all/linux.arm/include/
cp ../src/AirJoyAll/AirJoyX/AirJoy/client/include/AirMediaClientPhoto.hpp ../public.airjoy.all/linux.arm/include/
cp ../src/AirJoyAll/AirJoyX/AirJoy/client/include/AirMediaClientVideo.hpp ../public.airjoy.all/linux.arm/include/
cp ../src/AirJoyAll/AirJoyX/AirJoy/client/include/AirMediaClientVolume.hpp ../public.airjoy.all/linux.arm/include/

#-----------------------------------------------------------------
#
# for airstream.so
#
#-----------------------------------------------------------------
cp output/libairstream.so ../public.airjoy.all/linux.arm/lib/

cp ../src/AirJoyAll/AirStreamX/AirFileServer/include/AirFileServer.hpp ../public.airjoy.all/linux.arm/include/

#-----------------------------------------------------------------
#
# demo for airjoy
#
#-----------------------------------------------------------------
cp ../src/AirJoyAll/AirJoyX/test/testAirJoyClient.cpp ../public.airjoy.all/linux.arm/testAirJoyClient/
cp ../src/AirJoyAll/AirJoyX/test/testAirJoyServer.cpp ../public.airjoy.all/linux.arm/testAirJoyServer/
