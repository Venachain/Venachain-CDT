#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <fstream>

#include "llvm/Support/CommandLine.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Path.h"

#include "venachain/utils.hpp"

#include "options.hpp"
#include "venachain/Log.h"
#if defined(__WIN32) || defined(__WIN32__) || defined(WIN32)
#define CLANG "clang"
#else
#define CLANG "clang-7"
#endif

const std::string kCompilerName = "venachain-cpp";

using namespace llvm;
using namespace venachain;
using namespace std;

int main(int argc, const char** argv) {
  if (verbose) {
      if (logPath.empty()) {
        logPath = venachain::cdt::utils::pwd();
      }
      std::string logName = "venachaincpp.log";
      initLog(logPath, logLevel,logName);
  }
  LOGDEBUG << "compile begin...";
  for (auto i = 0; i < argc; i++) {
    if (argv[i] == std::string("-v")) {
      venachain::cdt::runtime::exec_subprogram(CLANG, {"-v"});
      return 0;
    }
  }

  cl::SetVersionPrinter([](llvm::raw_ostream& os) {
    os << kCompilerName << " version "
       << "${VERSION_FULL}"
       << "\n";
  });
  cl::ParseCommandLineOptions(
      argc, argv, kCompilerName + " (Venachain C++ -> WebAssembly compiler)");
  auto opts = CreateOptions();
  LOGDEBUG << "opts get successfully";
  if (opts.abigen) {
    LOGDEBUG << "goto abi gen";
    std::string op = "";
      for (auto s : opts.abigen_opts){
        op = op + s + " ";
      }
    LOGDEBUG << "venachain-abigen are: " + op;
    venachain::cdt::runtime::exec_subprogram("venachain-abigen", opts.abigen_opts);
    if (!llvm::sys::fs::exists(opts.abi_filename)) {
      LOGDEBUG << "abi gen failed";
      return -1;
    }
    LOGDEBUG << "abi gen successfully";
  }

  std::vector<std::string> outputs;
  try {
    for (auto input : opts.inputs) {
      std::vector<std::string> new_opts = opts.compiler_opts;
      llvm::SmallString<64> res;
      llvm::sys::path::system_temp_directory(true, res);
      std::string tmp_file = std::string(res.c_str()) + "/" +
                             llvm::sys::path::filename(input).str();
      std::string output;
      LOGDEBUG << "input_tmp_file is " << tmp_file;
      if (llvm::sys::fs::exists(tmp_file)) {
        input = tmp_file;
      }
      output = tmp_file + ".o";

      new_opts.insert(new_opts.begin(), input);

      if (!opts.link) {
        output = opts.output_filename.empty() ? "a.out" : opts.output_filename;
      }

      new_opts.insert(new_opts.begin(), "-o " + output);
      outputs.push_back(output);
      std::string op = "";
      for (auto s : new_opts){
        op = op + s + " ";
      }
      LOGDEBUG << "clang opts are: " + op;
      if (!venachain::cdt::runtime::exec_subprogram(CLANG, new_opts)) {
        LOGDEBUG << "clang complie failed";
        llvm::sys::fs::remove(tmp_file);
        return -1;
      }
      LOGDEBUG << "clang compile successfully";
      //llvm::sys::fs::remove(tmp_file);
    }
  } catch (std::runtime_error& err) {
    LOGDEBUG << "error happend while clang compile";
    llvm::errs() << err.what() << '\n';
    return -1;
  }

  if (opts.link) {
    LOGDEBUG << "start link...";
    std::vector<std::string> new_opts = opts.ld_opts;
    for (const auto& output : outputs) {
      new_opts.insert(new_opts.begin(), output);
    }

    if (opts.abigen) {
      std::fstream fs(opts.exports_filename);
      std::string line;
      while (std::getline(fs, line)) {
        new_opts.emplace_back(" --export " + line + " ");
      }
    } else {
      new_opts.emplace_back(" --export _Z4mainiPPc ");
    }
    if (!venachain::cdt::runtime::exec_subprogram("venachain-ld", new_opts)) {
      LOGDEBUG << "venachain-ld link failed";
      for (const auto& output : outputs) {
        llvm::sys::fs::remove(output);
      }
      return -1;
    }

    LOGDEBUG << "venachain-ld link successfully";
    LOGDEBUG << "opts.output_filename: " << opts.output_filename;
    if (!llvm::sys::fs::exists(opts.output_filename)) {
      LOGDEBUG << "venachain-ld error happend,output file not exixts";
      return -1;
    }
  }
  LOGDEBUG << "done successfully";
  return 0;
}
