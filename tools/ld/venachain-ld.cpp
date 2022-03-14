#include <fstream>
#include <iostream>
#include <string>

#include "llvm/Support/CommandLine.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Path.h"

#include "venachain/utils.hpp"
#include "venachain/Log.h"

#define ONLY_LD 1
#include "options.hpp"

const std::string kLdName = "venachain-ld";

using namespace venachain;
using namespace std;

int main(int argc, const char** argv) {
  if (verbose) {
      if (logPath.empty()) {
        logPath = venachain::cdt::utils::pwd();
      }
      std::string logName = "venachainld.log";
      initLog(logPath, logLevel,logName);
  }
  for (auto i = 0; i < argc; i++) {
    if (argv[i] == std::string("-v")) {
      venachain::cdt::runtime::exec_subprogram("wasm-ld", {"-v"});
      return 0;
    }
  }

  LOGDEBUG << "ld begin...";
  llvm::cl::SetVersionPrinter([](llvm::raw_ostream& os) {
    os << kLdName << " version "
       << "${VERSION_FULL}"
       << "\n";
  });
  llvm::cl::ParseCommandLineOptions(argc, argv,
                                    kLdName + " (WebAssembly linker)");
  auto opts = CreateOptions();

  if (!opts.export_file.empty()) {
    std::fstream fs(opts.export_file);
    std::string line;
    while (std::getline(fs, line)) {
      opts.ld_opts.emplace_back("--export " + line);
    }
  }
  std::string op = "";
  for (auto s : opts.ld_opts){
    op = op + s + " ";
  }
  LOGDEBUG << "wasm-ld_opts are: " + op;
  if (!venachain::cdt::runtime::exec_subprogram("wasm-ld", opts.ld_opts)) {
    return -1;
  }

  if (!llvm::sys::fs::exists(opts.output_filename)) {
    return -1;
  }
  LOGDEBUG << "ld done";
  return 0;
}
