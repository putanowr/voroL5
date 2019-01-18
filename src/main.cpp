#include <iostream>
#include <stdlib.h>

#include <easylogging++.h>

#include "manager.h"

INITIALIZE_EASYLOGGINGPP

void main_body(const vor::Manager &myManager);
void setup_logging(const vor::Manager &myManager);
int selftest_main(int argc, char *argv[]);

int main(int argc, char *argv[])
{
 vor::Manager myManager;
 if (myManager.parseCmdArgs(argc, argv))
 {
   if (myManager.do_testing()) {
     myManager.exitStatus = selftest_main(argc, argv);
   } else {
     setup_logging(myManager);
     main_body(myManager);
   }
 }
 return myManager.exitStatus;
}

void setup_logging(const vor::Manager &myManager)
{
  if (myManager.noLog) {
    el::Configurations defaultConf;
    defaultConf.setToDefault();
    defaultConf.set(el::Level::Global, el::ConfigurationType::Enabled, std::string("false"));
    el::Loggers::reconfigureLogger("default", defaultConf);
  }
}

void main_body(const vor::Manager &myManager) {
  LOG(INFO) << "Starting simulations" << std::endl;
  if (myManager.doEcho) {
    myManager.echo();
  }
}
