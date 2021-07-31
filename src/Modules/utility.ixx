//
// Created by yhtse on 7/22/2021.
//


export module utility;
export import utility.type;

import<string>;
import<filesystem>;
import <iostream>;
import <fstream>;
import <sstream>;

export namespace Ace {
namespace fs = std::filesystem;
using namespace std::literals::string_literals;
// readfile
// \param filePath 文件路径
// \returns {std::string} 返回包含文件内容的string
std::string readFile(const Ace::fs::path& filePath) {
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
