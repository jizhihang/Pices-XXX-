LIBS   = -lPICL -lfftw 
LIBDIR = ../../Library
VPATH  = ../../Library
CFLAGS = -O3 -I$(LIBDIR)

CC = g++ 

EXEC = GrayImageExtraction


INSTRUMENTDATAMANAGER      = InstrumentDataManager.h 

INSTRUMENTDATAREPORT       = InstrumentDataReport.h  
INSTRUMENTDATABATTERYMETER = InstrumentDataBatteryMeter.h  $(INSTRUMENTDATAREPORT)
INSTRUMENTDATACTDPLUS      = InstrumentDataCTDplus.h       $(INSTRUMENTDATAREPORT)
INSTRUMENTDATAFLOWMETER1   = InstrumentDataFlowMeter1.h    $(INSTRUMENTDATAREPORT)
INSTRUMENTDATAGPS          = InstrumentDataGPS.h           $(INSTRUMENTDATAREPORT)
INSTRUMENTDATAPITCHANDROLL = InstrumentDataPitchAndRoll.h  $(INSTRUMENTDATAREPORT)
INSTRUMENTDATAUSERMSGS     = InstrumentDataUserMsgs.h      $(INSTRUMENTDATAREPORT)

SIPPERBUFF                 = SipperBuff.h            
SIPPERBUFF3BIT             = SipperBuff3Bit.h        $(SIPPERBUFF)
SIPPER3BUFF                = Sipper3Buff.h           $(SIPPERBUFF)
SIPPERBUFFBINARY           = SipperBuffBinary.h      $(SIPPERBUFF)
SIPPERBLOB                 = SipperBlob.h 

SIPPERIMAGEEXTRACTION = SipperImageExtraction.h  $(SIPPERBUFF) $(SIPPERBLOB) $(SIPPERIMAGEEXTRACTIONPARMS) $(INSTRUMENTDATAMANAGER)

SIPPERIMAGEEXTRACTIONPARMS = SipperImageExtractionParms.h $(SIPPERBUFF)


OBJECTS	= GrayImageExtraction.o         InstrumentDataReport.o        InstrumentDataManager.o    InstrumentDataBatteryMeter.o  \
          InstrumentDataCTDplus.h       InstrumentDataFlowMeter1.o    InstrumentDataGPS.o        InstrumentDataPitchAndRoll.o  \
          InstrumentDataUserMsgs.o      SipperBlob.o                  SipperBuff.o               SipperBuff3Bit.o              \
          Sipper3Buff.o                 SipperBuffBinary.o            SipperImageExtraction.o    SipperImageExtractionParms.o  


$(EXEC): $(OBJECTS) libPICL.a  $(LIBDIR)/*.h  $(LIBDIR)/*.cpp
	$(CC) -o $(EXEC) $(OBJECTS) $(LIBS) -L$(LIBDIR)


InstrumentDataManager.o: InstrumentDataManager.cpp  $(INSTRUMENTDATAMANAGER)  $(LIBDIR)/*.h
	$(CC) $(CFLAGS) -c $*.cpp


InstrumentDataReport.o: InstrumentDataReport.cpp  $(INSTRUMENTDATAREPORT)  $(LIBDIR)/*.h
	$(CC) $(CFLAGS) -c $*.cpp


InstrumentDataBatteryMeter.o: InstrumentDataBatteryMeter.cpp  $(INSTRUMENTDATABATTERYMETER)   $(LIBDIR)/*.h
	$(CC) $(CFLAGS) -c $*.cpp


InstrumentDataCTDplus.o: InstrumentDataCTDplus.cpp  $(INSTRUMENTDATACTDPLUS)   $(LIBDIR)/*.h
	$(CC) $(CFLAGS) -c $*.cpp


InstrumentDataFlowMeter1.o: InstrumentDataFlowMeter1.cpp   $(INSTRUMENTDATAFLOWMETER1)  $(LIBDIR)/*.h
	$(CC) $(CFLAGS) -c $*.cpp


InstrumentDataGPS.o: InstrumentDataGPS.cpp  $(INSTRUMENTDATAGPS)  $(LIBDIR)/*.h
	$(CC) $(CFLAGS) -c $*.cpp


InstrumentDataPitchAndRoll.o:  InstrumentDataPitchAndRoll.cpp  $(INSTRUMENTDATAPITCHANDROLL)  $(LIBDIR)/*.h
	$(CC) $(CFLAGS) -c $*.cpp


InstrumentDataUserMsgs.o: InstrumentDataUserMsgs.cpp $(INSTRUMENTDATAUSERMSGS)  $(LIBDIR)/*.h
	$(CC) $(CFLAGS) -c $*.cpp


SipperBuffBinary.o: SipperBuffBinary.cpp  $(SIPPERBUFFBINARY)    $(RUNLOG)  $(LIBDIR)/*.h
	$(CC) $(CFLAGS) -c $*.cpp


SipperBuff3Bit.o: SipperBuff3Bit.cpp  $(SIPPERBUFF3BIT)   $(RUNLOG)  $(LIBDIR)/*.h
	$(CC) $(CFLAGS) -c $*.cpp


SipperBuff.o : SipperBuff.cpp  $(SIPPERBUFF)  $(SIPPERBUFF3BIT)  $(SIPPERBUFFBINARY) $(INSTRUMENTDATAMANAGER) $(LIBDIR)/*.h
	$(CC) $(CFLAGS) -c $*.cpp


Sipper3Buff.o : $(SIPPER3BUFF) Sipper3Buff.cpp  $(RUNLOG)  $(LIBDIR)/*.h
	$(CC) $(CFLAGS) -c $*.cpp


SipperBlob.o : SipperBlob.cpp  $(SIPPERBLOB) $(LIBDIR)/*.h
	$(CC) $(CFLAGS) -c $*.cpp


SipperImageExtraction.o : SipperImageExtraction.cpp  $(SIPPERIMAGEEXTRACTION)  $(SIPPERBLOB)              $(SIPPERBUFF)  \
                          $(SIPPERBUFF3BIT)          $(SIPPERBIFFBINARY)       $(INSTRUMENTDATAMANAGER)   $(LIBDIR)/*.h
	$(CC) $(CFLAGS) -c $*.cpp


SipperImageExtractionParms.o : SipperImageExtractionParms.cpp  $(SIPPERIMAGEEXTRACTIONPARMS)  $(SIPPERBUFF)  $(LIBDIR)/*.h
	$(CC) $(CFLAGS) -c $*.cpp


GrayImageExtraction.o : GrayImageExtraction.cpp  $(SIPPERIMAGEEXTRACTION)  $(SIPPERIMAGEEXTRACTIONPARMS) $(LIBDIR)/*.h
	$(CC) $(CFLAGS) -c $*.cpp


libPICL.a : $(LIBDIR)/*.h  $(LIBDIR)/*.cpp
	(cd $(LIBDIR); /bin/rm -f libPICL.a; make)
