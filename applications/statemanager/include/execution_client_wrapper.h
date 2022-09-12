#ifndef EXECUTION_CLIENT_WRAPPER
#define EXECUTION_CLIENT_WRAPPER

#include "ara/exec/execution_client.h"
#include "ara/log/logging.h"

class ExecutionClientWrapper
{
public:
  ExecutionClientWrapper();
  ExecutionClientWrapper(const ExecutionClientWrapper&) = delete;
  ExecutionClientWrapper& operator=(const ExecutionClientWrapper&) = delete;
  ExecutionClientWrapper(ExecutionClientWrapper&&) = delete;
  ExecutionClientWrapper& operator=(ExecutionClientWrapper&&) = delete;
  ~ExecutionClientWrapper();
private:
  ara::exec::ExecutionClient exec_client_;
  ara::log::Logger& logger_{ara::log::CreateLogger("EXCL", "Execution client wrapper context", ara::log::LogLevel::kVerbose)};
};

#endif
