cd /d %~sdp0

wsdl2h.exe MESService.wsdl -s -o MESService.h 

soapcpp2.exe MESService.h -i -x -L -I./import