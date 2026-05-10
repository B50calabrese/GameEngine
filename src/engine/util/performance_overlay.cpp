/**
 * @file performance_overlay.cpp
 * @brief Implementation of the performance overlay.
 */

#include <engine/util/performance_overlay.h>

#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

#include <engine/core/engine.h>
#include <engine/graphics/renderer.h>
#include <engine/scene/scene_manager.h>

#ifdef __linux__
#include <sys/resource.h>
#include <unistd.h>
#endif

namespace engine::util {

PerformanceOverlay& PerformanceOverlay::Get() {
  static PerformanceOverlay instance;
  return instance;
}

void PerformanceOverlay::Update(double delta_time) {
  frame_time_accum_ += delta_time;
  frame_count_++;

  if (frame_time_accum_ >= 1.0) {
    current_fps_ = static_cast<float>(frame_count_ / frame_time_accum_);
    current_frame_time_ms_ = static_cast<float>((frame_time_accum_ / frame_count_) * 1000.0);

    UpdateSystemMetrics();

    frame_time_accum_ = 0.0;
    frame_count_ = 0;
  }
}

void PerformanceOverlay::UpdateSystemMetrics() {
#ifdef __linux__
  // RAM Usage
  std::ifstream status_file("/proc/self/status");
  std::string line;
  while (std::getline(status_file, line)) {
    if (line.substr(0, 6) == "VmRSS:") {
      std::stringstream ss(line.substr(7));
      long long rss_kb;
      ss >> rss_kb;
      ram_usage_mb_ = rss_kb / 1024.0;
      break;
    }
  }

  // CPU Usage (Process-specific)
  // Read total system time
  std::ifstream sys_stat_file("/proc/stat");
  std::string cpu_label;
  long long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  sys_stat_file >> cpu_label >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
  long long total_sys_time = user + nice + system + idle + iowait + irq + softirq + steal + guest + guest_nice;

  // Read process time
  std::ifstream proc_stat_file("/proc/self/stat");
  std::string tmp;
  for (int i = 0; i < 13; ++i) proc_stat_file >> tmp; // Skip first 13 fields
  long long utime, stime, cutime, cstime;
  proc_stat_file >> utime >> stime >> cutime >> cstime;
  long long total_proc_time = utime + stime;

  if (last_sys_time_ != 0) {
    long long sys_diff = total_sys_time - last_sys_time_;
    long long proc_diff = total_proc_time - last_proc_time_;
    if (sys_diff > 0) {
      cpu_usage_percent_ = 100.0f * (static_cast<float>(proc_diff) / sys_diff);
    }
  }

  last_sys_time_ = total_sys_time;
  last_proc_time_ = total_proc_time;
#else
  // Placeholder for non-Linux systems
  cpu_usage_percent_ = 0.0f;
  ram_usage_mb_ = 0.0;
#endif
}

void PerformanceOverlay::Render() {
  if (!visible_) return;

  auto& renderer = graphics::Renderer::Get();

  float x = 10.0f;
  float y = static_cast<float>(Engine::window().height()) - 30.0f;
  float line_height = 20.0f;
  float width = 220.0f;
  float height = 110.0f;

  // Background box
  renderer.DrawQuad({5.0f, y - height + 25.0f}, {width, height}, {0.1f, 0.1f, 0.1f, 0.7f});

  std::stringstream ss;

  // FPS
  ss << "FPS: " << std::fixed << std::setprecision(1) << current_fps_ << " (" << current_frame_time_ms_ << " ms)";
  renderer.DrawText("default", ss.str(), {x, y}, 0.0f, 0.7f, {1.0f, 1.0f, 1.0f, 1.0f});
  y -= line_height;
  ss.str("");

  // CPU
  ss << "CPU: " << std::fixed << std::setprecision(1) << cpu_usage_percent_ << "%";
  renderer.DrawText("default", ss.str(), {x, y}, 0.0f, 0.7f, {1.0f, 1.0f, 1.0f, 1.0f});
  y -= line_height;
  ss.str("");

  // RAM
  ss << "RAM: " << std::fixed << std::setprecision(1) << ram_usage_mb_ << " MB";
  renderer.DrawText("default", ss.str(), {x, y}, 0.0f, 0.7f, {1.0f, 1.0f, 1.0f, 1.0f});
  y -= line_height;
  ss.str("");

  // Entities
  size_t entity_count = 0;
  std::string scene_name = "None";
  auto* active_scene = SceneManager::Get().GetActiveScene();
  if (active_scene) {
    entity_count = active_scene->registry().GetEntityCount();
    scene_name = active_scene->name();
  }

  ss << "Entities: " << entity_count;
  renderer.DrawText("default", ss.str(), {x, y}, 0.0f, 0.7f, {1.0f, 1.0f, 1.0f, 1.0f});
  y -= line_height;
  ss.str("");

  // Scene
  ss << "Scene: " << scene_name;
  renderer.DrawText("default", ss.str(), {x, y}, 0.0f, 0.7f, {1.0f, 1.0f, 1.0f, 1.0f});
}

}  // namespace engine::util
