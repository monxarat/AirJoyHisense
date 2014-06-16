#!/bin/sh

cp build/output/libairjoy.a ../public.airjoy.all/IOS/lib/iPhoneOS.armv7s/

cp ../src/AirCommon/include/AirJoyApi.hpp ../public.airjoy.all/IOS/include/
cp ../src/AirCommon/include/AirJoyTypeDef.hpp ../public.airjoy.all/IOS/include/

cp ../src/AirJoyAll/AirJoyX/AirJoyNsd/include/AirJoyNsd.hpp ../public.airjoy.all/IOS/include/
cp ../src/AirJoyAll/AirJoyX/AirJoyNsd/include/AirJoyNsdDiscoveryListener.hpp ../public.airjoy.all/IOS/include/

cp ../src/AirJoyAll/AirJoyX/AirJoy/server/include/AirJoyServer.hpp ../public.airjoy.all/IOS/include/
cp ../src/AirJoyAll/AirJoyX/AirJoy/server/include/AirJoyServiceInfo.hpp ../public.airjoy.all/IOS/include/
cp ../src/AirJoyAll/AirJoyX/AirJoy/server/include/AirJoyDefault.hpp ../public.airjoy.all/IOS/include/
cp ../src/AirJoyAll/AirJoyX/AirJoy/server/include/AirMediaListener.hpp ../public.airjoy.all/IOS/include/
cp ../src/AirJoyAll/AirJoyX/AirJoy/server/include/AirFileListener.hpp ../public.airjoy.all/IOS/include/

cp ../src/AirJoyAll/AirJoyX/AirJoy/client/include/AirJoyClient.hpp ../public.airjoy.all/IOS/include/
cp ../src/AirJoyAll/AirJoyX/AirJoy/client/include/AirMediaClient.hpp ../public.airjoy.all/IOS/include/
cp ../src/AirJoyAll/AirJoyX/AirJoy/client/include/AirMediaClientQueryListener.hpp ../public.airjoy.all/IOS/include/
cp ../src/AirJoyAll/AirJoyX/AirJoy/client/include/AirMediaClientAudio.hpp ../public.airjoy.all/IOS/include/
cp ../src/AirJoyAll/AirJoyX/AirJoy/client/include/AirMediaClientPhoto.hpp ../public.airjoy.all/IOS/include/
cp ../src/AirJoyAll/AirJoyX/AirJoy/client/include/AirMediaClientVideo.hpp ../public.airjoy.all/IOS/include/
cp ../src/AirJoyAll/AirJoyX/AirJoy/client/include/AirMediaClientVolume.hpp ../public.airjoy.all/IOS/include/


