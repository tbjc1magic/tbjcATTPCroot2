
#ifndef FAIRRUNTIMEDB_H
#define FAIRRUNTIMEDB_H

#include "TObject.h"

#include "Rtypes.h"
#include "TList.h"
#include "TString.h"
#include <map>
#include "FairParSet.h"

class FairRuntimeDb : public TObject
{
  private:
    static FairRuntimeDb* gRtdb; //!

  public:
    FairRuntimeDb();
    ~FairRuntimeDb();
    static FairRuntimeDb* instance(void);

    Bool_t addContainer(char*,FairParSet*);
    FairParSet* getContainer(const char*);

    std::map<TString, FairParSet*> fMap;

};

#endif
