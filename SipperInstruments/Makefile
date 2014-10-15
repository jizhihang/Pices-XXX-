EXEC = libSipperInstruments
LIBS = -lfftw3 -lrt
VPATH = ../BaseLibrary
LIBDIR = ../BaseLibrary
#FFTWAPI = ../OutsidePackages/fftw-3.1.2/api
CFLAGS = -O3 -I$(LIBDIR)
#CFLAGS = -g   -I$(LIBDIR)

CC = g++

CTD_PLUS_DATA              = CTD_Plus_Data.h
INSTRUMENT                 = Instrument.h
INSTRUMENTDATA             = InstrumentData.h              $(CTD_PLUS_DATA)

INSTRUMENTDATAMANAGER      = InstrumentDataManager.h       $(CTD_PLUS_DATA)

INSTRUMENTDATAFILEMANAGER  = InstrumentDataFileManager.h
INSTRUMENTDATALIST         = InstrumentDataList.h

INSTRUMENTDATAREPORT       = InstrumentDataReport.h  
INSTRUMENTDATABATTERYMETER = InstrumentDataBatteryMeter.h  $(INSTRUMENTDATAREPORT)
INSTRUMENTDATACTDPLUS      = InstrumentDataCTDplus.h       $(INSTRUMENTDATAREPORT)
INSTRUMENTDATAFLOWMETER1   = InstrumentDataFlowMeter1.h    $(INSTRUMENTDATAREPORT)
INSTRUMENTDATAGPS          = InstrumentDataGPS.h           $(INSTRUMENTDATAREPORT)
INSTRUMENTDATAPITCHANDROLL = InstrumentDataPitchAndRoll.h  $(INSTRUMENTDATAREPORT)
INSTRUMENTDATAUSERMSGS     = InstrumentDataUserMsgs.h      $(INSTRUMENTDATAREPORT)

RASTERSIPPER               = RasterSipper.h

SIPPERBUFF                 = SipperBuff.h            
SIPPERBUFF3BIT             = SipperBuff3Bit.h        $(SIPPERBUFF)
SIPPER3BUFF                = Sipper3Buff.h           $(SIPPERBUFF)
SIPPER3REVBUFF             = Sipper3RevBuff.h        $(SIPPERBUFF)  $(SIPPER3BUFF)
SIPPERBUFF4BIT             = SipperBuff4Bit.h        $(SIPPERBUFF)
SIPPERBUFFBINARY           = SipperBuffBinary.h      $(SIPPERBUFF)
SIPPERBLOB                 = SipperBlob.h
SIPPERFILE                 = SipperFile.h 
SIPPERHEADERREC            = SipperHeaderRec.h       $(INSTRUMENT)
SIPPERVARIABLES            = SipperVariables.h

SIPPERCRUISE               = SipperCruise.h          $(INSTRUMENT)   $(INSTRUMENTDATAFILEMANAGER)
SIPPERDEPLOYMENT           = SipperDeployment.h
SIPPERSTATION              = SipperStation.h

OBJECTS	= CTD_Plus_Data.o               Instrument.o                  InstrumentData.o              \
          InstrumentDataBatteryMeter.o  InstrumentDataCTDplus.o       InstrumentDataFileManager.o   \
          InstrumentDataFlowMeter1.o    InstrumentDataGPS.o           InstrumentDataList.o          \
          InstrumentDataManager.o       InstrumentDataPitchAndRoll.o  InstrumentDataReport.o        \
          InstrumentDataUserMsgs.o      RasterSipper.o                Sipper3Buff.o                 \
          Sipper3RevBuff.o              SipperBuff.o                  SipperBuff3Bit.o              \
          SipperBuff4Bit.o              SipperBuffBinary.o            SipperCruise.o                \
          SipperDeployment.o            SipperFile.o                  SipperHeaderRec.o             \
          SipperStation.o               SipperVariables.o 
          

all:	$(EXEC)

clean:
	rm *.o; rm *.a

$(EXEC): $(OBJECTS)
	ar cq libSipperInstruments.a  $(OBJECTS)


CTD_Plus_Data.o: CTD_Plus_Data.cpp  $(CTD_PLUS_DATA)  $(LIBDIR)/*.h
	$(CC) $(CFLAGS) -c $*.cpp

Instrument.o: Instrument.cpp    $(INSTRUMENT)            $(INSTRUMENTDATAMANAGER)      $(INSTRUMENTDATAREPORT) \
              $(CTD_PLUS_DATA)  $(INSTRUMENTDATACTDPLUS) $(INSTRUMENTDATABATTERYMETER) $(INSTRUMENTDATAPITCHANDROLL) \
              $(LIBDIR)/*.h
	$(CC) $(CFLAGS) -c $*.cpp



InstrumentData.o: InstrumentData.cpp  $(INSTRUMENTDATA)  $(LIBDIR)/*.h
	$(CC) $(CFLAGS) -c $*.cpp


InstrumentDataBatteryMeter.o: InstrumentDataBatteryMeter.cpp  $(INSTRUMENTDATABATTERYMETER)  $(INSTRUMENTDATAMANAGER)   $(LIBDIR)/*.h
	$(CC) $(CFLAGS) -c $*.cpp



InstrumentDataCTDplus.o: InstrumentDataCTDplus.cpp  $(INSTRUMENTDATACTDPLUS)   $(INSTRUMENTDATAMANAGER)    $(LIBDIR)/*.h
	$(CC) $(CFLAGS) -c $*.cpp


InstrumentDataFileManager.o:  InstrumentDataFileManager.cpp  $(INSTRUMENTDATAFILEMANAGER)  \
                              $(INSTRUMENTDATAMANAGER)       $(INSTRUMENTDATA)             \
                              $(INSTRUMENTDATALIST)          $(SIPPERVARIABLES)            \
                              $(LIBDIR)/*.h
	$(CC) $(CFLAGS) -c $*.cpp


InstrumentDataFlowMeter1.o: InstrumentDataFlowMeter1.cpp   $(INSTRUMENTDATAFLOWMETER1) $(INSTRUMENTDATAMANAGER)  $(LIBDIR)/*.h
	$(CC) $(CFLAGS) -c $*.cpp



InstrumentDataGPS.o: InstrumentDataGPS.cpp  $(INSTRUMENTDATAGPS)  $(INSTRUMENTDATAMANAGER)  $(LIBDIR)/*.h
	$(CC) $(CFLAGS) -c $*.cpp



InstrumentDataList.o: InstrumentDataList.cpp  $(INSTRUMENTDATALIST)  $(INSTRUMENTDATAMANAGER)  $(LIBDIR)/*.h
	$(CC) $(CFLAGS) -c $*.cpp


InstrumentDataManager.o:  InstrumentDataManager.cpp   $(INSTRUMENTDATAMANAGER)  $(CTD_PLUS_DATA)    \
                          $(INSTRUMENTDATA)           $(INSTRUMENTDATAREPORT)   $(SIPPERHEADERREC)  \
                          $(LIBDIR)/*.h
	$(CC) $(CFLAGS) -c $*.cpp


InstrumentDataPitchAndRoll.o:  InstrumentDataPitchAndRoll.cpp  $(INSTRUMENTDATAPITCHANDROLL)  $(INSTRUMENTDATAMANAGER)  $(LIBDIR)/*.h
	$(CC) $(CFLAGS) -c $*.cpp


InstrumentDataReport.o:  InstrumentDataReport.cpp     $(INSTRUMENTDATAREPORT)        $(INSTRUMENTDATABATTERYMETER)  \
                         $(INSTRUMENTDATAFLOWMETER1)  $(INSTRUMENTDATAPITCHANDROLL)  $(INSTRUMENTDATAGPS)           \
                         $(INSTRUMENTDATAUSERMSGS)    $(INSTRUMENTDATACTDPLUS)       $(INSTRUMENT)                  \
                         $(INSTRUMENTDATAMANAGER)     $(SIPPERHEADERREC)                                            \
                         $(LIBDIR)/*.h
	$(CC) $(CFLAGS) -c $*.cpp


InstrumentDataUserMsgs.o: InstrumentDataUserMsgs.cpp $(INSTRUMENTDATAUSERMSGS)   $(INSTRUMENTDATAMANAGER)  $(LIBDIR)/*.h
	$(CC) $(CFLAGS) -c $*.cpp


RasterSipper.o :  RasterSipper.cpp    $(RASTERSIPPER)      $(LIBDIR)/*.h
	$(CC) $(CFLAGS) -c $*.cpp


Sipper3Buff.o :  Sipper3Buff.cpp    $(SIPPER3BUFF)     $(INSTRUMENTDATAMANAGER)  $(SIPPERHEADERREC)  $(LIBDIR)/*.h
	$(CC) $(CFLAGS) -c $*.cpp


Sipper3RevBuff.o :  Sipper3RevBuff.cpp    $(SIPPER3REVBUFF)    $(LIBDIR)/*.h
	$(CC) $(CFLAGS) -c $*.cpp


SipperBuff.o : SipperBuff.cpp  $(SIPPERBUFF)  $(SIPPERBUFF3BIT)  $(SIPPERBUFFBINARY) $(INSTRUMENTDATAMANAGER) $(LIBDIR)/*.h
	$(CC) $(CFLAGS) -c $*.cpp


SipperBuff3Bit.o:  SipperBuff3Bit.cpp  $(SIPPERBUFF3BIT)   $(RUNLOG)  $(LIBDIR)/*.h
	$(CC) $(CFLAGS) -c $*.cpp


SipperBuff4Bit.o:  SipperBuff4Bit.cpp  $(SIPPERBUFF4BIT)     $(LIBDIR)/*.h
	$(CC) $(CFLAGS) -c $*.cpp


SipperBuffBinary.o: SipperBuffBinary.cpp  $(SIPPERBUFFBINARY)    $(RUNLOG)  $(LIBDIR)/*.h
	$(CC) $(CFLAGS) -c $*.cpp


SipperCruise.o :  SipperCruise.cpp    $(SIPPERCRUISE)     $(INSTRUMENTDATAMANAGER)    $(LIBDIR)/*.h
	$(CC) $(CFLAGS) -c $*.cpp


SipperDeployment.o :  SipperDeployment.cpp    $(SIPPERDEPLOYMENT)     $(INSTRUMENTDATAMANAGER)    $(LIBDIR)/*.h
	$(CC) $(CFLAGS) -c $*.cpp


SipperFile.o:  SipperFile.cpp  $(SIPPERFILE)  $(INSTRUMENTDATAFILEMANAGER)    $(LIBDIR)/*.h
	$(CC) $(CFLAGS) -c $*.cpp


SipperHeaderRec.o:  SipperHeaderRec.cpp  $(SIPPERHEADERREC)    $(LIBDIR)/*.h
	$(CC) $(CFLAGS) -c $*.cpp


SipperStation.o :  SipperStation.cpp  $(SIPPERSTATION)     $(LIBDIR)/*.h
	$(CC) $(CFLAGS) -c $*.cpp


SipperVariables.o :  SipperVariables.cpp  $(SIPPERVARIABLES) $(LIBDIR)/*.h
	$(CC) $(CFLAGS) -c $*.cpp




libBaseLibrary.a : $(LIBDIR)/*.h  $(LIBDIR)/*.cpp
	(cd $(LIBDIR); /bin/rm -f libBaseLibrary.a; make)































