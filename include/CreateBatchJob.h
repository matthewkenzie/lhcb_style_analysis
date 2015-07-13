#ifndef CreateBatchJob_h
#define CreateBatchJob_h

#include <iostream>
#include "OptParser.h"
#include "InputFileOptions.h"

class CreateBatchJob {

  public :

    CreateBatchJob(const OptParser &_rOpt, const InputFileOptions &_fOpt);
    ~CreateBatchJob(){}

    void WriteAndSubmit();

    void writeJobFiles();
    void submitJob();
    void writeDatFile();
    void writeSubFile();

    const OptParser& rOpt;
    const InputFileOptions& fOpt;

};

#endif
