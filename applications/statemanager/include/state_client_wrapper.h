#ifndef STATE_CLIENT_WRAPPER
#define STATE_CLIENT_WRAPPER

#include <memory>
#include "ara/log/logging.h"
#include "ara/core/string.h"
#include "ara/exec/state_client.h"

class StateClientWrapper
{
public:
  StateClientWrapper();
  StateClientWrapper(const StateClientWrapper&) = delete;
  StateClientWrapper& operator=(const StateClientWrapper&) = delete;
  StateClientWrapper(StateClientWrapper&&) = delete;
  StateClientWrapper& operator=(StateClientWrapper&&) = delete;
  ~StateClientWrapper() = default;

  bool ChangeState(const ara::core::String& func_group, const ara::core::String& state);
private:
  std::unique_ptr<ara::exec::StateClient> state_client_;
  ara::log::Logger& logger_{ara::log::CreateLogger("STCL", "State client wrapper context", ara::log::LogLevel::kVerbose)};
};

#endif
