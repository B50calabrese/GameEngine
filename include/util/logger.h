#ifndef INCLUDE_UTIL_LOGGER_H_
#define INCLUDE_UTIL_LOGGER_H_

#include <cstdarg>
#include <string>

namespace engine::util {

/**
 * @brief Severity levels for logging.
 */
enum class LogLevel { INFO, WARNING, ERROR };

/**
 * @brief A simple logger utility for the engine and client.
 *
 * Supports console output and optional file output.
 * Formats messages with file, line, and severity information.
 */
class Logger {
 public:
  /**
   * @brief Gets the singleton instance of the Logger.
   * @return Reference to the Logger instance.
   */
  static Logger& Get();

  /**
   * @brief Initializes the logger.
   *
   * If a filename is provided, logs will be written to "logs/<filename>".
   * Otherwise, it logs only to the console.
   *
   * @param filename Optional filename for log output.
   */
  void Init(const std::string& filename = "");

  /**
   * @brief Logs a message with the specified severity.
   *
   * @param level Severity level.
   * @param file Source file where the log was called.
   * @param line Source line where the log was called.
   * @param format Printf-style format string.
   * @param ... Variadic arguments for the format string.
   */
  void Log(LogLevel level, const char* file, int line, const char* format, ...);

  /**
   * @brief Returns whether the logger has been initialized.
   */
  bool IsInitialized() const { return initialized_; }

 private:
  Logger() = default;
  ~Logger();

  // Prevent copying
  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;

  void LogInternal(LogLevel level, const char* file, int line, const char* format,
                   va_list args);

  bool initialized_ = false;
  bool to_file_ = false;
  std::string log_file_path_;
};

}  // namespace engine::util

// Macros for easier logging with file and line information
#define LOG_INFO(...) \
  engine::util::Logger::Get().Log(engine::util::LogLevel::INFO, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_WARN(...) \
  engine::util::Logger::Get().Log(engine::util::LogLevel::WARNING, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_ERR(...) \
  engine::util::Logger::Get().Log(engine::util::LogLevel::ERROR, __FILE__, __LINE__, __VA_ARGS__)

#endif  // INCLUDE_UTIL_LOGGER_H_
