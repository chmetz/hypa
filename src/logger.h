#ifndef PTX_ANALYSER_SRC_LOGGER_H_
#define PTX_ANALYSER_SRC_LOGGER_H_

#include <fstream>
#include <mutex>

class Logger {
  inline static std::ofstream output_file_;
  inline static std::ofstream log_file_;
  inline static std::ofstream error_file_;
  inline static std::mutex output_file_mutex_;
  inline static std::mutex log_file_mutex_;
  inline static std::mutex error_file_mutex_;
 public:
  static void OpenFile();
  static void CloseFile();
  static void Log(std::string msg, std::string level);
  static void Print(std::string msg);
};

#endif //PTX_ANALYSER_SRC_LOGGER_H_

