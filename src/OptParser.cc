#include <iostream>

#include "TString.h"

#include "OptParser.h"

using namespace std;
namespace po = boost::program_options;
using namespace po;

OptParser::OptParser():
  desc("Allowed options")
{

  desc.add_options()
    ("help,h",                                                                          "Produce help message and exit")
    ("cfgFile,c",     value<string>(&cfgFile)->default_value("dat/config.dat"),         "Name of datfile with run config")
    ("outFile,o",     value<string>(&outFile)->default_value("root/AnalysisOut.root"),  "Name of output file")
    ("firstEntry,f",  value<Long64_t>(&firstEntry)->default_value(0),                   "Starting entry")
    ("lastEntry,l",   value<Long64_t>(&lastEntry)->default_value(0),                    "Last entry")
    ("usage,u",       bool_switch(&usage)->default_value(false),                        "Print config and exit")
    ("verbose,v",     bool_switch(&verbose)->default_value(false),                      "Run in verbose mode")
    ("debug,d",       bool_switch(&debug)->default_value(false),                        "Run in debug mode")
    ("batch,b",       bool_switch(&batch)->default_value(false),                        "Run in batch mode")
    ("batchdir,d",    value<string>(&batchdir)->default_value("batch"),                 "Batch jobs directory")
    ("queue,q",       value<string>(&queue)->default_value(""),                         "Batch queue to submit to")
    ;
}

OptParser::~OptParser(){}

void OptParser::parseOptions(int argc, char **argv) {

  prog_name = string(argv[0]);

  store( parse_command_line( argc, argv, desc) , vm);
  notify(vm);

  if (vm.count("help")) {
    cout << desc << endl;
    exit(0);
  }
}
