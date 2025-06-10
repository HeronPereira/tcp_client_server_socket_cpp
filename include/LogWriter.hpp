#ifndef LOG_WRITER_HPP_
#define LOG_WRITER_HPP_

#include <netinet/in.h>

#include <fstream>
#include <mutex>
#include <string>

class LogWriter {
 public:
  explicit LogWriter();
  ~LogWriter() = default;

  void Write(std::string ip, std::string data);

 private:
  const std::string kfile_name_ = "SensorData.log";
  std::mutex file_mutex_;
  std::ofstream data_file_;
};

#endif
