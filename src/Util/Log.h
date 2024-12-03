#pragma once

#define OFF ELogLevel_OFF
#define CRITICAL ELogLevel_CRITICAL
#define WARNING ELogLevel_WARNING
#define INFO ELogLevel_INFO
#define DEBUG ELogLevel_DEBUG
#define TRACE ELogLevel_TRACE

void Log(struct EntryData* Data, ELogLevel LogType, const std::string Log, ...);