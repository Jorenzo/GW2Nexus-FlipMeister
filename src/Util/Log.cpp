#include "pch.h"

void Log(EntryData* Data, ELogLevel LogType, const std::string Log, ...)
{
  constexpr size_t buffer_size = 1024;  // Adjust this as needed
  char buffer[buffer_size];

  // Start processing the variadic arguments
  va_list args;
  va_start(args, Log);

  // Format the message into the buffer
  std::vsnprintf(buffer, buffer_size, Log.c_str(), args);

  // End processing the variadic arguments
  va_end(args);

  // Convert the buffer to a std::string
  std::string logMessage(buffer);

  // Pass the formatted string to the logging system
  Data->APIDefs->Log(LogType, ADDON_LOG_NAME, logMessage.c_str());
}