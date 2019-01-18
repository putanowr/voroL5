// Copyright (c) 2018 Roman Putanowicz
//! @brief Define Manager class for managaing application.

#pragma once

#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/property_tree/ptree.hpp>

#include <iostream>
#include <string>
#include <vector>

namespace pt=boost::property_tree;
namespace bf=boost::filesystem;

namespace vor {

template <typename T>
std::vector<T> get_vector(pt::ptree const& pt, pt::ptree::key_type const& path)
{
    std::vector<T> r;
    for (auto& item : pt.get_child(path))
       r.push_back(item.second.get_value<T>());
    return r;
}

struct FieldDef {
  std::string name = std::string("data");
  std::string expression = std::string("x");
  FieldDef(const std::string &fieldName, const std::string &fieldExpression) : name(fieldName), expression(fieldExpression){}
  FieldDef() = default;
  // operator= will be used to assign to the FieldDef
  FieldDef& operator=(const std::string &str);
  std::ostream& print(std::ostream &out) const;
};

std::ostream& operator<<(std::ostream &os, const FieldDef &f);

enum class USourceType {
  EXPRESSION,
  SIMPLE_SHEAR
};

class Manager {
public:
  Manager();
  bool parseCmdArgs(int argc, char *argv[]);

  bool read(const bf::path &path);
  bool write(const bf::path &path, bool overwrite=false) const;

  void echo(std::ostream &out = std::cout) const;

  //! @brief Return true if program should be run in selftesting mode.
  bool do_testing() const;

  std::string inFile;
  bool noLog = false;
  bool doEcho = false;
  int exitStatus = EXIT_SUCCESS;

private:
  using ConfigData = pt::ptree;
  using ConfigDataIter = pt::ptree::const_iterator;
  ConfigData props_;
  bool doTesting_ = false;
  void setup_props_();
  void check_prop_exists_(const std::string &key) const;
  void verify_() const;
};

} // namespace vor
