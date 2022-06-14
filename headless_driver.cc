//
// Copyright (c) Tencent Corporation. All rights reserved.
//
#include "headless_driver.h"
#include <thread>
using namespace tdfcore;

HeadlessDriver::HeadlessDriver(const Callback &callback, Duration interval)
    : VSyncTaskDriver(callback), interval_(interval) {}

HeadlessDriver::~HeadlessDriver() { Stop(); }

void HeadlessDriver::Start() {
  VSyncTaskDriver::Start();
  active_ = true;
  std::thread t([=]() {
    while (active_.load()) {
      std::this_thread::sleep_for(interval_);
      if (!active_.load()) {
        return;
      }
      if (callback_) {
        callback_(waiter_);
      }
    }
  });
  t.detach();
}

void HeadlessDriver::Stop() {
  TaskDriver::Stop();
  active_ = false;
}