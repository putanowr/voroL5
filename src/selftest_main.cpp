#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>

int selftest_main(int argc, char *argv[]) {
  Catch::Session session; // There must be exactly one instance
  
  bool doTest = false; // Some user variable you want to be able to set
  // Build a new parser on top of Catch's
  using namespace Catch::clara;
  auto cli 
    = session.cli() // Get Catch's composite command line parser
      | Opt( doTest )
        ["--selftest"]
        ("Run program in self-testing mode" );
  // Now pass the new composite back to Catch so it uses that
  session.cli( cli ); 
  // Let Catch (using Clara) parse the command line
  int returnCode = session.applyCommandLine( argc, argv );
  if( returnCode != 0 ) // Indicates a command line error
      return returnCode;

  return session.run();
}
