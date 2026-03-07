#include "util/logger.h"

#include <cstdio>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace engine::util {

Logger& Logger::Get() {
  static Logger instance;
  return instance;
}

Logger::~Logger() {}

void Logger::Init(const std::string& filename) {
  if (filename.empty()) {
    to_file_ = false;
  } else {
    log_file_path_ = "logs/" + filename;
    try {
      std::filesystem::create_directories("logs");
      to_file_ = true;
    } catch (const std::exception& e) {
      std::cerr << "Failed to create logs directory: " << e.what() << std::endl;
      to_file_ = false;
    }
  }
  initialized_ = true;
}

void Logger::Log(LogLevel level, const char* file, int line, const char* format,
                 ...) {
  va_list args;
  va_start(args, format);
  LogInternal(level, file, line, format, args);
  va_end(args);
}

void Logger::LogInternal(LogLevel level, const char* file, int line,
                         const char* format, va_list args) {
  const char* level_str = "INFO";
  switch (level) {
    case LogLevel::INFO:
      level_str = "INFO";
      break;
    case LogLevel::WARNING:
      level_str = "WARNING";
      break;
    case LogLevel::ERROR:
      level_str = "ERROR";
      break;
  }

  // Format the message
  char buffer[1024];
  vsnprintf(buffer, sizeof(buffer), format, args);

  // Get current time
  std::time_t now = std::time(nullptr);
  char time_buffer[20];
  std::strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S",
                std::localtime(&now));

  // Construct final message
  // [YYYY-MM-DD HH:MM:SS] [LEVEL] [file:line] message
  char final_buffer[2048];
  snprintf(final_buffer, sizeof(final_buffer), "[%s] [%s] [%s:%d] %s",
           time_buffer, level_str, file, line, buffer);

  // Output to console
  if (level == LogLevel::ERROR) {
    std::cerr << final_buffer << std::endl;
  } else {
    std::cout << final_buffer << std::endl;
  }

  // Output to file
  if (to_file_) {
    std::ofstream log_file(log_file_path_, std::ios_base::app);
    if (log_file.is_open()) {
      log_file << final_buffer << std::endl;
    }
  }
}

}  // namespace engine::util
