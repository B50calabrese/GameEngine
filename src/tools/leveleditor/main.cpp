#include <leveleditor/level_editor.h>

#include <engine/scene/scene_manager.h>
#include <engine/util/console.h>

#include "../common/demo_utils.h"

class LevelEditorDemo : public demos::common::BaseDemoApp {
 public:
  void OnDemoInit() override {
    // Create a default scene
    auto scene = std::make_unique<engine::Scene>("EditorScene");
    editor_ = std::make_unique<leveleditor::LevelEditor>(scene->GetRegistry());
    engine::SceneManager::Get().PushScene(std::move(scene));

    // Pause the engine by default so we can use the editor
    engine::util::Console::Get().SetPaused(true);
  }

  void OnDemoUpdate(double delta_time) override {
    // The editor renders itself via ImGui calls
    if (editor_) {
      editor_->Render();
    }
  }

  void OnDemoShutdown() override { editor_.reset(); }

 private:
  std::unique_ptr<leveleditor::LevelEditor> editor_;
};

int main() {
  engine::EngineConfig config;
  config.window_title = "Level Editor Demo";
  return demos::common::DemoRunner::Run<LevelEditorDemo>(config);
}
