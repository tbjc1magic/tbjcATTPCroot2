#include "ATDigiPar.hh"

//ClassImp(ATDigiPar);

ATDigiPar::ATDigiPar()
:fGas(NULL)
{

    std::cout<<"ATDigiPar initialized here?"<<std::endl;
    getParams();
	fInitialized = kFALSE;
}

ATDigiPar::~ATDigiPar()
{
    std::cout<<"ATDigiPar end here"<<std::endl;
}

Int_t  ATDigiPar::GetPadPlaneX()        { return fPadPlaneX; }
Int_t  ATDigiPar::GetPadPlaneZ()        { return fPadPlaneZ; }
Int_t  ATDigiPar::GetPadSizeX()         { return fPadSizeX; }
Int_t  ATDigiPar::GetPadSizeZ()         { return fPadSizeZ; }
Int_t  ATDigiPar::GetPadRows()          { return fPadRows; }
Int_t  ATDigiPar::GetPadLayers()        { return fPadLayers; }
Double_t  ATDigiPar::GetAnodeWirePlaneY()  { return fAnodeWirePlaneY; }
Double_t  ATDigiPar::GetGroundWirePlaneY() { return fGroundWirePlaneY; }
Double_t  ATDigiPar::GetGatingWirePlaneY() { return fGatingWirePlaneY; }
Int_t  ATDigiPar::GetNumTbs()           { return fNumTbs; }
Double_t  ATDigiPar::GetDriftVelocity()    { return fDriftVelocity; }
Double_t  ATDigiPar::GetDriftLength()      { return fDriftLength; }
Int_t  ATDigiPar::GetYDivider()         { return fYDivider; }
Double_t  ATDigiPar::GetBField()           { return fBField;}
Double_t  ATDigiPar::GetEField() 	         { return fEField;}
Double_t  ATDigiPar::GetTiltAngle() 	     { return fTiltAng;}
Int_t     ATDigiPar::GetTB0() 	           { return fTB0;}
Double_t  ATDigiPar::GetThetaLorentz() 	   { return fThetaLorentz;}
Int_t     ATDigiPar::GetTBEntrance() 	     { return fTBEntrance;}

ATGas *ATDigiPar::GetGas()
{
	if(fGas==NULL){
		std::cerr << "Initializing gas file with " << fGasFileName.Data() << std::endl;
		fGas = new ATGas(fGasFileName.Data());
	}
	return fGas;
}

Int_t ATDigiPar::GetTBTime() {
	switch (fSamplingRate) {
		case 12:
			return 80;
		case 25:
			return 40;
		case 50:
			return 20;
		case 100:
			return 10;
		default:
			return -1;
	}
}

Bool_t ATDigiPar::getParams()
{
	fPadPlaneX = 864;
	fPadPlaneZ = 1344;
	fPadSizeX = 8;
	fPadSizeZ = 12;
	fPadRows = 108;
	fPadLayers = 112;
	fAnodeWirePlaneY = -4.05;
	fGroundWirePlaneY = -8.1;
	fGatingWirePlaneY = -14;
	fEField = 493.2;
	fNumTbs = 512;
	fSamplingRate = 12;
	fDriftVelocity = 2.56;
	fDriftLength = 500;
	fYDivider = 500;
	fGasFile = 2;
	fPadPlaneFile = 3;
	fPadShapeFile = 4;
	fBField = 2;
	fTiltAng = 0;
	fTB0 = 100;
	fThetaLorentz = 0;
	fTBEntrance = 390;

	return 1;
}

TString ATDigiPar::GetFile(Int_t fileNum)
{
	std::ifstream fileList;
	TString parFile = "parameters/AT.files.par";
	fileList.open(parFile.Data());

	if(!fileList) throw;

	Char_t buffer[256];
	for(Int_t iFileNum = 0; iFileNum < fileNum + 1; ++iFileNum){
		if(fileList.eof()) throw;

		fileList.getline(buffer, 256);
	}

	fileList.close();

	return TString(buffer);
}
