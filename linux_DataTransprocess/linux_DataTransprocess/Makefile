#CXX = arm-none-linux-gnueabi-g++ 
CXX = /opt/freescale/usr/local/gcc-4.6.2-glibc-2.13-linaro-multilib-2011.12/fsl-linaro-toolchain/bin/arm-none-linux-gnueabi-g++ 

MAININC = ../../include
INCPATH  += -I$(MAININC) \
		-I$(MAININC)/RapidXml \
		-I$(MAININC)/OsBase \
		-I$(MAININC)/SysLog
		
SYSLOGO = ../SysLog/LoggingEvent.o ../SysLog/LogLayout.o ../SysLog/Priority.o ../SysLog/RollingFileAppender.o ../SysLog/SysLog.o 

LIBS = 

TARGET   	= ZigbeeProcess

OBJS 	= ../OsBase/OsBase.o $(SYSLOGO) ZigbeeProcess.o

$(TARGET):$(OBJS)
	$(CXX) $^ -o $@ $(LIBS)

ZigbeeProcess.o:ZigbeeProcess.cpp
	$(CXX) $(CXXFLAGS) $(INCPATH) -c $< -o $@
	

install:
	cp ./$(TARGET) ../../bin/$(TARGET)


clean:
	rm -rf *.o $(TARGET)

