#pragma once

#include <tesla.hpp>
#include <ui/views/MainLayout.hpp>

#ifdef LIBRED
#define VERSION "v1.0.0 - PRIVATE"
#else
#define VERSION "v1.0.0"
#endif

class MainApplication : public tsl::Overlay
{
public:
  virtual void initServices() override;
  virtual void exitServices() override;
  virtual std::unique_ptr<tsl::Gui> loadInitialGui() override;
};