//! @brief Define Manager class for managaing application.

#include "errors.h"
#include "manager.h"

#include <tclap/CmdLine.h>
#include <easylogging++.h>
#include <string>
#include <sstream>
#include <queue>
#include <utility>
#include <boost/property_tree/info_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ini_parser.hpp>

namespace vor {

std::string trim(std::string const& str)
{
    if(str.empty())
        return str;

    std::size_t firstScan = str.find_first_not_of(' ');
    std::size_t first     = firstScan == std::string::npos ? str.length() : firstScan;
    std::size_t last      = str.find_last_not_of(' ');
    return str.substr(first, last-first+1);
}

void split(std::vector<std::string> &elems, const std::string &s, char delim) {
  std::stringstream ss(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    elems.push_back(std::move(trim(item)));
  }
}

template <typename T>
void put_vector(std::vector<T> &vec, pt::ptree &root,
               const std::string &key) {
  pt::ptree vecNode;
  for (auto &&v : vec) {
    pt::ptree cell;
    cell.put_value(v);
    vecNode.push_back(std::make_pair("", cell));
  }
  root.add_child(key, vecNode);
}

boost::property_tree::ptree
mergePropertyTrees( const boost::property_tree::ptree& rptFirst, const boost::property_tree::ptree& rptSecond )
{
  // Take over first property tree
  boost::property_tree::ptree ptMerged = rptFirst;

  // Keep track of keys and values (subtrees) in second property tree
  std::queue<std::string> qKeys;
  std::queue<boost::property_tree::ptree> qValues;
  qValues.push( rptSecond );

  // Iterate over second property tree
  while( !qValues.empty() )
  {
    // Setup keys and corresponding values
    boost::property_tree::ptree ptree = qValues.front();
    qValues.pop();
    std::string keychain = "";
    if( !qKeys.empty() )
    {
      keychain = qKeys.front();
      qKeys.pop();
    }

    // Iterate over keys level-wise
    for ( const boost::property_tree::ptree::value_type& child : ptree )
    {
      // No "." for first level entries
      std::string s;
      if( keychain != "" ) s = keychain + "." + child.first.data();
      else s = child.first.data();
      // Leaf
      if( child.second.size() == 0 )
      {
        // Put into combined property tree
        ptMerged.put( s, child.second.data() );
      } else if ( child.first.data() == std::string("value")
                || std::string(child.first.data()).substr(0,3) == std::string("pt_")) {
        std::vector<double> v = get_vector<double>(child.second, "");
        put_vector(v, ptMerged, s);
      }
      // Subtree
      else
      {
        // Put keys (identifiers of subtrees) and all of its parents (where present)
        // aside for later iteration. Keys on first level have no parents
        if( keychain != "" )
          qKeys.push( keychain + "." + child.first.data() );
        else
          qKeys.push( child.first.data() );

        // Put values (the subtrees) aside, too
        qValues.push( child.second );
      }
    }  // -- End of foreach
  }  // --- End of while

  return ptMerged;
}

} // namespace vor

namespace vor {


bool Manager::parseCmdArgs(int argc, char *argv[]) {
  auto isOK = true;
  try {
    TCLAP::CmdLine cmd("Generate restricted Voronoi diagrams", ' ', "0.1");

    TCLAP::UnlabeledValueArg<std::string> infileArg("infile", "input file name", false, "", "string", cmd);
    TCLAP::SwitchArg doEchoArg("e", "echo", "print manager echo", cmd, false);
    TCLAP::SwitchArg noLogArg("s", "nolog", "switch off log messages",cmd,  false);
    TCLAP::SwitchArg doTestingArg("", "selftest", "run program in self-testing mode", cmd, false);
    cmd.parse(argc, argv);

    doTesting_ = doTestingArg.getValue();
    doEcho = doEchoArg.getValue();
    noLog = noLogArg.getValue();
    if (!doTesting_) {
      if (infileArg.isSet()) {
        inFile = infileArg.getValue();
        read(inFile);
      }
    }
  } catch(TCLAP::ArgException &e) {
    std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
    isOK = false;
    exitStatus = EXIT_FAILURE;
  }
  return isOK;
}

Manager::Manager() {
  setup_props_();
}

bool Manager::do_testing() const {
  return doTesting_;
}

void Manager::setup_props_() {
   props_.put("dummy_property", 100);
}

// This checks for keys not for paths to children.
void Manager::check_prop_exists_(const std::string &key) const {
  if (props_.find(key) == props_.not_found()){
    std::stringstream oss;
    oss << "ERROR: Cannot find property : " << key;
    throw std::logic_error(oss.str());
  }
}

bool Manager::write(const bf::path &path, bool overwrite) const {
  pt::write_json(path.string(), props_);
  return true;
}

void Manager::echo(std::ostream &out) const {
  out << "Manager settings: " << std::endl;
  pt::write_json(out, props_);
  out << "--------------------------" << std::endl;
}

void Manager::verify_() const {
  bool state = true;
  if (state != true) {
    throw std::logic_error("Invalid configuration: FIXME is there is enything to check");
  }
}

bool Manager::read(const bf::path &path) {
  pt::ptree usertree;
  pt::read_json(path.string(), usertree);
  props_ = mergePropertyTrees(props_, usertree);
  verify_();
  return true;
}

} // namespace vor


