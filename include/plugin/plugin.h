// SPDX-License-Identifier: Apache-2.0
//===-- ssvm/plugin/plugin.h - Plugin class definition --------------------===//
//
// Part of the SSVM Project.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file is the definition class of Plugin interface.
///
//===----------------------------------------------------------------------===//
#pragma once
#include "common/defines.h"
#include "common/errcode.h"
#include "common/filesystem.h"
#include "loader/shared_library.h"
#include "po/argument_parser.h"
#include "runtime/importobj.h"

namespace SSVM {
namespace Plugin {

extern "C" {
struct PluginInfo {
  uint32_t APIVersion;
  const char *PluginName;
  const char *PluginVersion;
  void (*RegisterParser)(PO::ArgumentParser &Parser) noexcept;
  Runtime::ImportObject *(*AllocateHostModule)(void) noexcept;
};
}

class SSVM_SYMBOL_EXPORT Plugin {
public:
  static std::vector<std::filesystem::path>
  enumerate(const std::filesystem::path &SearchPath);
  static Expect<Plugin> load(const std::filesystem::path &Path) noexcept;
  uint32_t getAPIVersion() const noexcept { return Info.APIVersion; }
  const char *getName() const noexcept { return Info.PluginName; }
  const char *getVersion() const noexcept { return Info.PluginVersion; }
  void RegisterArgument(PO::ArgumentParser &Parser) const noexcept {
    Info.RegisterParser(Parser);
  }
  std::unique_ptr<Runtime::ImportObject> AllocateHostModule() const noexcept {
    return std::unique_ptr<Runtime::ImportObject>(Info.AllocateHostModule());
  }

private:
  Plugin(const std::filesystem::path &Path,
         std::shared_ptr<Loader::SharedLibrary> Lib)
      : Path(Path), Lib(std::move(Lib)) {}

  const std::filesystem::path &Path;
  std::shared_ptr<Loader::SharedLibrary> Lib;
  PluginInfo Info;
};

} // namespace Plugin
} // namespace SSVM

#define SSVM_PLUGIN_API_VERSION 1

extern "C" SSVM_ATTRIBUTE_WEAK SSVM_SYMBOL_EXPORT ::SSVM::Plugin::PluginInfo
ssvmGetPluginInfo();