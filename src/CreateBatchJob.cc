#include <iostream>
#include <fstream>

#include "boost/filesystem.hpp"

#include "Utils.h"
#include "CreateBatchJob.h"

using namespace std;
using namespace Utils;

CreateBatchJob::CreateBatchJob( const OptParser &_rOpt, const InputFileOptions &_fOpt ):
  rOpt(_rOpt),
  fOpt(_fOpt)
{}

void CreateBatchJob::writeDatFile() 
{
  system(Form("mkdir -p %s",rOpt.batchdir.c_str()));

  string datfname = Form("%s/%s.dat",rOpt.batchdir.c_str(),fOpt.name.Data());
  
  ofstream of(datfname.c_str());

  of << Form("# %s",fOpt.name.Data()) << endl;
  of << endl;

  for ( unsigned int f=0; f<fOpt.filenames.size(); f++) {
    
    of << Form("itype=%-5d  sqrts=%-2d  year=%-5s  name=%-25s   fname=%-100s   tname=%-100s", fOpt.itype, fOpt.sqrts, fOpt.year.Data(), fOpt.name.Data(), fOpt.filenames[f].Data(), fOpt.treenames[f].Data()) << endl;
  }

  of.close();

  //system(Form("cat %s",datfname.c_str()));

}

void CreateBatchJob::writeSubFile() {
 
  system(Form("mkdir -p %s",rOpt.batchdir.c_str()));

  string subfname = Form("%s/%s.sh",rOpt.batchdir.c_str(),fOpt.name.Data());

  ofstream of(subfname.c_str());
  of << "#!/bin/bash" << endl;
  of << Form("# %s",fOpt.name.Data()) << endl;

  string cwd  = boost::filesystem::current_path().string();
  boost::filesystem::path prog( rOpt.prog_name );

  string prog_name = prog.stem().string();

  of << Form("touch %s/%s/%s.run",cwd.c_str(),rOpt.batchdir.c_str(),fOpt.name.Data()) << endl;
  of << Form("rm -f %s/%s/%s.done",cwd.c_str(),rOpt.batchdir.c_str(),fOpt.name.Data()) << endl;
  of << Form("rm -f %s/%s/%s.fail",cwd.c_str(),rOpt.batchdir.c_str(),fOpt.name.Data()) << endl;
  of << Form("rm -f %s/%s/%s.log",cwd.c_str(),rOpt.batchdir.c_str(),fOpt.name.Data()) << endl;

  of << "mkdir -p scratch" << endl;
  of << "cd scratch" << endl;
  of << Form("source %s/setup_lxplus.sh",cwd.c_str()) << endl; 
  of << Form("cp %s/bin/%s .",cwd.c_str(),prog_name.c_str()) << endl;
  of << Form("cp %s/%s/%s.dat .",cwd.c_str(),rOpt.batchdir.c_str(),fOpt.name.Data()) << endl;

  string exec_line;
  exec_line += Form("./%s -c %s.dat -o %s.root -b", prog_name.c_str(), fOpt.name.Data(), fOpt.name.Data());

  of << Form ( "if ( %s ) then ",exec_line.c_str() ) << endl;
  of << Form( "\t cp %s.root %s/%s/", fOpt.name.Data(), cwd.c_str(), rOpt.batchdir.c_str() ) << endl;
  of << Form("\t touch %s/%s/%s.done",cwd.c_str(),rOpt.batchdir.c_str(),fOpt.name.Data()) << endl;
  of << "else" << endl;
  of << Form("\t touch %s/%s/%s.fail",cwd.c_str(),rOpt.batchdir.c_str(),fOpt.name.Data()) << endl;
  of << "fi" << endl;
  of << Form("rm -f %s/%s/%s.run",cwd.c_str(),rOpt.batchdir.c_str(),fOpt.name.Data()) << endl;

  of.close();

  system(Form("chmod +x %s",subfname.c_str()));

}

void CreateBatchJob::writeJobFiles() {
  writeDatFile();
  writeSubFile();
}

void CreateBatchJob::submitJob() {

  string cwd  = boost::filesystem::current_path().string();
  string path = Form("%s/%s/%s",cwd.c_str(),rOpt.batchdir.c_str(),fOpt.name.Data());

  string submit_line;

  if ( rOpt.queue != "" && !rOpt.runLocal ) {
    submit_line = Form( "bsub -q %s -o %s.log %s.sh", rOpt.queue.c_str(),path.c_str(), path.c_str());
  }
  else if ( rOpt.runLocal ) {
    submit_line = Form( "%s.sh", path.c_str() ); 
  }
  else {
    print("",Form("\t Written: %s/%s.sh",rOpt.batchdir.c_str(),fOpt.name.Data()));
  }
  system( submit_line.c_str() );
}

void CreateBatchJob::WriteAndSubmit() {
  writeJobFiles();
  submitJob();
}
