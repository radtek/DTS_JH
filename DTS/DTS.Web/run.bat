cd /d %~sdp0

wsdl2h.exe MESService.wsdl -U  -c++11 -o MESService.h 

soapcpp2.exe MESService.h -i -x -L -I./import -Ec -c++11