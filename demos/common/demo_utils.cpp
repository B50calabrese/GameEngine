/**
 * @file demo_utils.cpp
 * @brief Implementation of common demo utilities.
 */

#include "demo_utils.h"

namespace demos::common {

void BaseDemoApp::OnInit() {
  engine::graphics::TextRenderer::Get().Init();
  // Load default font for all demos.
  engine::graphics::TextRenderer::Get().LoadFont("default", "arial.ttf", 24);

  OnDemoInit();
}

void BaseDemoApp::OnUpdate(double dt) {
  engine::util::TweenManager::Get().Update(static_cast<float>(dt));

  OnDemoUpdate(dt);
}

void BaseDemoApp::OnShutdown() { OnDemoShutdown(); }

}  // namespace demos::common
