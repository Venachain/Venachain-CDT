#include <exception>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>

#include "llvm/Support/CommandLine.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Path.h"

#include "venachain/utils.hpp"
#include "venachain/Log.h"

using namespace venachain;

class Project final {
 public:
  Project(const std::string& project_name, const std::string& path, bool bare)
      : project_name_(project_name), path_(path), bare_(bare) {
    llvm::sys::fs::create_directory(path_);
    if (!bare) {
      llvm::sys::fs::create_directory(path_ + "/src");
      llvm::sys::fs::create_directory(path_ + "/include");
      llvm::sys::fs::create_directory(path_ + "/build");
    }
  }

  ~Project() = default;

  void WriteCpp() {
    std::ofstream cpp_out;
    if (bare_) {
      cpp_out.open(path_ + "/" + project_name_ + ".cpp");
    } else {
      cpp_out.open(path_ + "/src/" + project_name_ + ".cpp");
    }
    cpp_out << ReplaceIncludeType(ReplaceName(kCpp));
  }

  void WriteCMake() {
    if (!bare_) {
      std::ofstream cmake_out(path_ + "/src/CMakeLists.txt");
      cmake_out << ReplaceName(kCMake);
    }
  }

  void WriteCMakeExtern() {
    if (!bare_) {
      std::ofstream cmake_out(path_ + "/CMakeLists.txt");
      cmake_out << ReplaceName(kCMakeExtern);
    }
  }

  void WriteReadme() {
    std::ofstream readme_out(path_ + "/README.txt");
    if (bare_) {
      readme_out << ReplaceName(kReadme);
    } else {
      readme_out << ReplaceName(kReadmeCMake);
    }
  }

 private:
  std::string ReplaceName(const std::string& in) {
    std::stringstream ss;
    for (char c : in) {
      if (c == '@') {
        ss << project_name_;
      } else {
        ss << c;
      }
    }
    return ss.str();
  }

  std::string ReplaceIncludeType(const std::string& in) {
    std::stringstream ss;
    for (char c : in) {
      if ((c == '<' || c == '>') && bare_) {
        ss << '"';
      } else {
        ss << c;
      }
    }
    return ss.str();
  }

  std::string project_name_;
  std::string path_;
  bool bare_;

  const std::string kCpp =
    "//auto create contract\n"
    "#include <stdlib.h>\n"
    "#include <string.h>\n"
    "#include <string>\n"
    "#include <bcwasm/bcwasm.hpp>\n\n"
    "namespace demo { \n"
    "class @ : public bcwasm::Contract{\n"
    "    public:\n"
    "    @(){}\n\n"
    "    /// 实现父类: bcwasm::Contract 的虚函数\n"
    "    /// 该函数在合约首次发布时执行，仅调用一次\n"
    "    void init()\n"
    "    {\n"
    "        bcwasm::println(\"init success...\");\n"
    "    }\n"
    "    /// 定义Event.\n"
    "    /// BCWASM_EVENT(eventName, arguments...)\n"
    "    BCWASM_EVENT(setName, const char *)\n\n"
    "    public:\n"
    "    void setName(const char *msg)\n"
    "    {\n"
    "        // 定义状态变量\n"
    "        bcwasm::setState(\"NAME_KEY\", std::string(msg));\n"
    "        bcwasm::println(\"setName success...\");\n\n"
    "        // 事件返回\n"
    "      BCWASM_EMIT_EVENT(setName, \"std::string(msg)\");\n"
    "      bcwasm::println(\"emit event success...\");\n"
    "    }\n"
    "    const char* getName() const\n"
    "    {\n"
    "        std::string value;\n"
    "        bcwasm::getState(\"NAME_KEY\", value);\n"
    "        // 读取合约数据并返回\n"
    "        bcwasm::println(value);\n"
    "        return value.c_str();\n"
    "    }\n"
    "};\n"
    "}\n"
    "// 此处定义的函数会生成ABI文件供外部调用\n"
    "BCWASM_ABI(demo::@, setName)\n"
    "BCWASM_ABI(demo::@, getName)\n"
    "BCWASM_ABI(demo::@, init)\n";


  const std::string kCMake =
      "project(@)\n\n"
      "if (VENACHAIN_CDT_ROOT STREQUAL \"\" OR NOT VENACHAIN_CDT_ROOT)\n"
      "  find_package(venachain.cdt)\n"
      "else()\n"
      "  list(APPEND CMAKE_MODULE_PATH "
      "${VENACHAIN_CDT_ROOT}/lib/cmake/venachain.cdt)\n"
      "  include(VenachainWasmToolchain)\n"
      "  include(VenachainCDTMacros)\n"
      "endif()\n\n"
      "add_contract(@ @ @.cpp)\n"
      "target_include_directories(@ PUBLIC ${CMAKE_SOURCE_DIR}/../include)\n";

  const std::string kCMakeExtern =
      "include(ExternalProject)\n"
      "# if no cdt root is given use default path\n"
      "if (VENACHAIN_CDT_ROOT STREQUAL \"\" OR NOT VENACHAIN_CDT_ROOT)\n"
      "  find_package(venachain.cdt)\n"
      "endif()\n\n"
      "ExternalProject_Add(\n"
      "  @_project\n"
      "  SOURCE_DIR ${CMAKE_SOURCE_DIR}/src\n"
      "  BINARY_DIR ${CMAKE_BINARY_DIR}/@\n"
      "  CMAKE_ARGS "
      "-DCMAKE_TOOLCHAIN_FILE=${VENACHAIN_CDT_ROOT}/lib/cmake/venachain.cdt/"
      "VenachainWasmToolchain.cmake -DVENACHAIN_CDT_ROOT=${VENACHAIN_CDT_ROOT}\n"
      "  UPDATE_COMMAND \"\"\n"
      "  PATCH_COMMAND \"\"\n"
      "  TEST_COMMAND \"\"\n"
      "  INSTALL_COMMAND \"\"\n"
      "  BUILD_ALWAYS 1\n"
      ")";

  const std::string kReadmeCMake =
      "--- @ Porject ---\n\n"
      " - How to Build\n"
      "   - cd to 'build' directory\n"
      "   - run the command export VENACHAIN_CDT_ROOT=<where you place cdt>\n"
      "   - run the command 'cmake ..'\n"
      "   - run the command 'make' \n\n"
      " - After build - \n"
      "   - The built smart contract is under the '@' directory in the 'build' "
      "directory\n"
      "   - You can then use 'ctool' to deploy contract\n\n"
      " - Additions to CMake should be done to CMakeLists.tx in the './src' "
      "directory and not in the top level CMakeLists.txt";

  const std::string kReadme =
      "--- @ Project ---\n\n"
      "- How to Build -\n"
      "  - run the command 'venachain-cpp -o @.wasm @.cpp -abigen'\n";
};

int main(int argc, const char** argv) {
  std::string logLevel = "debug";
  std::string logPath = venachain::cdt::utils::pwd();
  std::string logName = "venachaininit.log";
  initLog(logPath, logLevel,logName);
  llvm::cl::SetVersionPrinter([](llvm::raw_ostream& os) {
    os << "venachain-init version "
       << "@VERSION_FULL@" << '\n';
  });
  llvm::cl::OptionCategory cat("venachain-init",
                               "generates an venachain smart contract project");

  llvm::cl::opt<bool> bare_opt(
      "bare",
      llvm::cl::desc(
          "produces only a skeleton smart contract without CMake support"),
      llvm::cl::cat(cat));
  llvm::cl::opt<std::string> project_name(
      "project", llvm::cl::desc("output project name"), llvm::cl::Required,
      llvm::cl::cat(cat));
  llvm::cl::opt<std::string> output_dir(
      "path", llvm::cl::desc("directory to place the project"),
      llvm::cl::cat(cat));

  llvm::cl::ParseCommandLineOptions(argc, argv, std::string("venachain-proj"));
  try {
    if (!std::regex_match(project_name,
                          std::regex("^[_a-zA-Z][_a-zA-Z0-9]*$"))) {
      throw std::runtime_error("ERROR: invalid identifier: " + project_name +
                               " (ensure that it is a valid C++ identifier)");
    }

    llvm::SmallString<128> rp;
    std::string path = output_dir;
    if (path.empty()) {
      path = ".";
    }
    llvm::sys::fs::real_path(path, rp, false);
    path = rp.str();
    if (path.empty()) {
      llvm::outs() << output_dir + " path not found\n";
      return -1;
    }
    path += "/" + project_name;

    Project project(project_name, path, bare_opt);
    project.WriteCpp();
    project.WriteCMake();
    project.WriteCMakeExtern();
    project.WriteReadme();
  } catch (std::exception& e) {
    std::cout << e.what() << "\n";
    return -1;
  }
  return 0;
}
