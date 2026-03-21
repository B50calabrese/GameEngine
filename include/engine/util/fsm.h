/**
 * @file fsm.h
 * @brief Simple Finite State Machine utility.
 */

#ifndef INCLUDE_ENGINE_UTIL_FSM_H_
#define INCLUDE_ENGINE_UTIL_FSM_H_

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

namespace engine::util {

/**
 * @brief Base class for an FSM state.
 */
class State {
 public:
  virtual ~State() = default;

  /**
   * @brief Called when entering the state.
   */
  virtual void OnEnter() {}

  /**
   * @brief Called every frame while the state is active.
   * @param dt Delta time since the last frame.
   */
  virtual void OnUpdate(float dt) {}

  /**
   * @brief Called when exiting the state.
   */
  virtual void OnExit() {}
};

/**
 * @brief A simple state that uses lambdas for its logic.
 */
class LambdaState : public State {
 public:
  using Callback = std::function<void()>;
  using UpdateCallback = std::function<void(float)>;

  explicit LambdaState(UpdateCallback on_update, Callback on_enter = nullptr,
                       Callback on_exit = nullptr)
      : on_update_(std::move(on_update)),
        on_enter_(std::move(on_enter)),
        on_exit_(std::move(on_exit)) {}

  void OnEnter() override {
    if (on_enter_) {
      on_enter_();
    }
  }
  void OnUpdate(float dt) override {
    if (on_update_) {
      on_update_(dt);
    }
  }
  void OnExit() override {
    if (on_exit_) {
      on_exit_();
    }
  }

 private:
  UpdateCallback on_update_;
  Callback on_enter_;
  Callback on_exit_;
};

/**
 * @brief Manages a collection of states and handles transitions between them.
 */
class StateMachine {
 public:
  /**
   * @brief Adds a state to the machine.
   * @param name The unique name of the state.
   * @param state A shared pointer to the state object.
   */
  void AddState(const std::string& name, std::shared_ptr<State> state) {
    states_[name] = std::move(state);
  }

  /**
   * @brief Transitions to a new state.
   * @param name The name of the state to transition to.
   */
  void ChangeState(const std::string& name) {
    if (current_state_) {
      current_state_->OnExit();
    }

    auto it = states_.find(name);
    if (it != states_.end()) {
      current_state_ = it->second;
      current_state_name_ = name;
      current_state_->OnEnter();
    } else {
      current_state_ = nullptr;
      current_state_name_ = "";
    }
  }

  /**
   * @brief Updates the current state.
   * @param dt Delta time since the last frame.
   */
  void Update(float dt) {
    if (current_state_) {
      current_state_->OnUpdate(dt);
    }
  }

  /**
   * @brief Gets the name of the current state.
   * @return The current state name.
   */
  const std::string& GetCurrentStateName() const { return current_state_name_; }

  /**
   * @brief Checks if the machine has a current state.
   * @return True if a state is active, false otherwise.
   */
  bool HasState() const { return current_state_ != nullptr; }

 private:
  std::unordered_map<std::string, std::shared_ptr<State>> states_;
  std::shared_ptr<State> current_state_ = nullptr;
  std::string current_state_name_ = "";
};

}  // namespace engine::util

#endif  // INCLUDE_ENGINE_UTIL_FSM_H_
