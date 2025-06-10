#include <LogWriter.hpp>

LogWriter::LogWriter() : data_file_(kfile_name_, std::ios::app) {}

void LogWriter::Write(std::string ip, std::string data) {
  // guarantees that there will be no simulteanous register on the file
  std::lock_guard<std::mutex> lock(file_mutex_);
  if (data_file_.is_open()) {
    // assuming that there is no guarantee that will be a line breaker on the
    // message, add at the end of the message on the file
    data_file_ << ip << ":" << data << std::endl;
  }
}