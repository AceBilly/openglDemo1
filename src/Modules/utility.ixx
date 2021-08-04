module;
#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>

export module utility;
export import utility.type;



namespace Ace {
    namespace fs = std::filesystem;
using namespace std::literals::string_literals;
// readfile
// \param filePath 文件路径
// \returns {std::string} 返回包含文件内容的string
export std::string readFile(const fs::path& filePath) {
  try {
    if (!Ace::fs::exists(filePath)) {
      throw std::runtime_error("file path not exists!");
    }
    std::string res;
    std::ifstream file;
    file.open(filePath);
    std::stringstream fileStream;
    fileStream << file.rdbuf();
    res = fileStream.str();
    return res;
  
  } catch (std::exception& e) {
    std::cout << "failed read file: " << e.what() << std::endl;
  }
  return ""s;
}
}  // namespace ace;
