#include "logger.h"
#include "data_structures/options.h"

#include <filesystem>

void Logger::OpenFile() {
  time_t now = time(0);
  tm *ltm = localtime(&now);

  std::string filename = "HyPA_";
  filename += std::to_string(ltm->tm_hour);
  filename += "-";
  filename += std::to_string(ltm->tm_min);
  filename += "-";
  filename += std::to_string(ltm->tm_sec);

  if (!std::filesystem::exists(filename + ".txt")) {
    std::filesystem::remove(filename + ".txt");
  }
  output_file_.open(filename + ".txt");

  if (!std::filesystem::exists(filename + "_err.txt")) {
    std::filesystem::remove(filename + "_err.txt");
  }
  error_file_.open(filename + "_err.txt");

  if (Options::verbose) {
    if (!std::filesystem::exists(filename + "_log.txt")) {
      std::filesystem::remove(filename + "_log.txt");
    }
    log_file_.open(filename + "_log.txt");
  }
}

void Logger::CloseFile() {
  output_file_.close();
  error_file_.close();

  if (Options::verbose) {
    log_file_.close();
  }
}

void Logger::Log(std::string msg, std::string level) {
  time_t now = time(0);
  tm *ltm = localtime(&now);

  if (level == "ERROR") {
    std::lock_guard<std::mutex> guard(error_file_mutex_);
    error_file_ << ltm->tm_hour;
    error_file_ << ":";
    error_file_ << ltm->tm_min;
    error_file_ << ":";
    error_file_ << ltm->tm_sec;
    error_file_ << " ";
    error_file_ << level;
    error_file_ << " | ";
    error_file_ << msg;
  } else {
    std::lock_guard<std::mutex> guard(log_file_mutex_);
    log_file_ << ltm->tm_hour;
    log_file_ << ":";
    log_file_ << ltm->tm_min;
    log_file_ << ":";
    log_file_ << ltm->tm_sec;
    log_file_ << " ";
    log_file_ << level;
    log_file_ << " | ";
    log_file_ << msg;
  }
}

void Logger::Print(std::string msg) {
  std::lock_guard<std::mutex> guard(output_file_mutex_);
  output_file_ << msg;
}

