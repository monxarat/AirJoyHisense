
@REM #-----------------------------------------------------------------
@REM #
@REM # common include header
@REM #
@REM #-----------------------------------------------------------------
copy ..\src\AirCommon\include\AirJoyApi.hpp ..\public.airjoy.all\Win32\include\
copy ..\src\AirCommon\include\AirJoyTypeDef.hpp ..\public.airjoy.all\Win32\include\

@REM #-----------------------------------------------------------------
@REM #
@REM # for airplay.dll
@REM #
@REM #-----------------------------------------------------------------
copy output\Debug\airplay.dll ..\public.airjoy.all\Win32\lib\Debug\
copy output\Debug\airplay.lib ..\public.airjoy.all\Win32\lib\Debug\

copy output\Release\airplay.dll ..\public.airjoy.all\Win32\lib\Release\
copy output\Release\airplay.lib ..\public.airjoy.all\Win32\lib\Release\

copy ..\src\AirJoyAll\AirPlayX\Common\include\AirPlayServiceInfo.hpp ..\public.airjoy.all\Win32\include\
copy ..\src\AirJoyAll\AirPlayX\Common\include\AirTunesServiceInfo.hpp ..\public.airjoy.all\Win32\include\

copy ..\src\AirJoyAll\AirPlayX\AirPlayNsd\include\AirPlayNsd.hpp ..\public.airjoy.all\Win32\include\
copy ..\src\AirJoyAll\AirPlayX\AirPlayNsd\include\AirPlayNsdDiscoveryListener.hpp ..\public.airjoy.all\Win32\include\
copy ..\src\AirJoyAll\AirPlayX\AirPlayNsd\include\AirPlayNsdPublishListener.hpp ..\public.airjoy.all\Win32\include\

copy ..\src\AirJoyAll\AirPlayX\AirPlay\server\include\AirPlayServer.hpp ..\public.airjoy.all\Win32\include\
copy ..\src\AirJoyAll\AirPlayX\AirPlay\server\include\AirPlayServerDefault.hpp ..\public.airjoy.all\Win32\include\
copy ..\src\AirJoyAll\AirPlayX\AirPlay\server\include\AirPlayServerListener.hpp ..\public.airjoy.all\Win32\include\

@REM #-----------------------------------------------------------------
@REM #
@REM # demo for airplay
@REM #
@REM #-----------------------------------------------------------------
copy ..\src\AirJoyAll\AirPlayX\testAirPlayServer\testAirPlayServer.cpp ..\public.airjoy.all\Win32\testAirPlayServer\

@REM #-----------------------------------------------------------------
@REM #
@REM # for airtunes.dll
@REM #
@REM #-----------------------------------------------------------------
copy output\Debug\airtunes.dll ..\public.airjoy.all\Win32\lib\Debug\
copy output\Debug\airtunes.lib ..\public.airjoy.all\Win32\lib\Debug\

copy output\Release\airtunes.dll ..\public.airjoy.all\Win32\lib\Release\
copy output\Release\airtunes.lib ..\public.airjoy.all\Win32\lib\Release\

copy ..\src\AirJoyAll\AirPlayX\AirTunes\server\include\AirTunesServer.hpp ..\public.airjoy.all\Win32\include\
copy ..\src\AirJoyAll\AirPlayX\AirTunes\server\include\AirTunesServerListener.hpp ..\public.airjoy.all\Win32\include\

@REM #-----------------------------------------------------------------
@REM #
@REM # demo for airtunes
@REM #
@REM #-----------------------------------------------------------------
copy ..\src\AirJoyAll\AirPlayX\testAirTunesServer\testAirTunesServer.cpp ..\public.airjoy.all\Win32\testAirTunesServer\

@REM #-----------------------------------------------------------------
@REM #
@REM # for airjoy.dll
@REM #
@REM #-----------------------------------------------------------------
copy output\Debug\airjoy.dll ..\public.airjoy.all\Win32\lib\Debug\
copy output\Debug\airjoy.lib ..\public.airjoy.all\Win32\lib\Debug\

copy output\Release\airjoy.dll ..\public.airjoy.all\Win32\lib\Release\
copy output\Release\airjoy.lib ..\public.airjoy.all\Win32\lib\Release\

copy ..\src\AirJoyAll\AirJoyX\AirJoyNsd\include\AirJoyNsd.hpp ..\public.airjoy.all\Win32\include\
copy ..\src\AirJoyAll\AirJoyX\AirJoyNsd\include\AirJoyNsdDiscoveryListener.hpp ..\public.airjoy.all\Win32\include\

copy ..\src\AirJoyAll\AirJoyX\AirJoy\server\include\AirJoyServer.hpp ..\public.airjoy.all\Win32\include\
copy ..\src\AirJoyAll\AirJoyX\AirJoy\server\include\AirJoyServiceInfo.hpp ..\public.airjoy.all\Win32\include\
copy ..\src\AirJoyAll\AirJoyX\AirJoy\server\include\AirJoyDefault.hpp ..\public.airjoy.all\Win32\include\
copy ..\src\AirJoyAll\AirJoyX\AirJoy\server\include\AirMediaListener.hpp ..\public.airjoy.all\Win32\include\
copy ..\src\AirJoyAll\AirJoyX\AirJoy\server\include\AirFileListener.hpp ..\public.airjoy.all\Win32\include\

copy ..\src\AirJoyAll\AirJoyX\AirJoy\client\include\AirJoyClient.hpp ..\public.airjoy.all\Win32\include\
copy ..\src\AirJoyAll\AirJoyX\AirJoy\client\include\AirMediaClient.hpp ..\public.airjoy.all\Win32\include\
copy ..\src\AirJoyAll\AirJoyX\AirJoy\client\include\AirMediaClientQueryListener.hpp ..\public.airjoy.all\Win32\include\
copy ..\src\AirJoyAll\AirJoyX\AirJoy\client\include\AirMediaClientAudio.hpp ..\public.airjoy.all\Win32\include\
copy ..\src\AirJoyAll\AirJoyX\AirJoy\client\include\AirMediaClientPhoto.hpp ..\public.airjoy.all\Win32\include\
copy ..\src\AirJoyAll\AirJoyX\AirJoy\client\include\AirMediaClientVideo.hpp ..\public.airjoy.all\Win32\include\
copy ..\src\AirJoyAll\AirJoyX\AirJoy\client\include\AirMediaClientVolume.hpp ..\public.airjoy.all\Win32\include\

@REM #-----------------------------------------------------------------
@REM #
@REM # for airstream.dll
@REM #
@REM #-----------------------------------------------------------------
copy output\Debug\airstream.dll ..\public.airjoy.all\Win32\lib\Debug\
copy output\Debug\airstream.lib ..\public.airjoy.all\Win32\lib\Debug\

copy output\Release\airstream.dll ..\public.airjoy.all\Win32\lib\Release\
copy output\Release\airstream.lib ..\public.airjoy.all\Win32\lib\Release\

copy ..\src\AirJoyAll\AirStreamX\AirFileServer\include\AirFileServer.hpp ..\public.airjoy.all\Win32\include\

@REM #-----------------------------------------------------------------
@REM #
@REM # demo for airjoy
@REM #
@REM #-----------------------------------------------------------------
copy ..\src\AirJoyAll\AirJoyX\test\testAirJoyClient.cpp ..\public.airjoy.all\Win32\testAirJoyClient\
copy ..\src\AirJoyAll\AirJoyX\test\testAirJoyServer.cpp ..\public.airjoy.all\Win32\testAirJoyServer\

pause
