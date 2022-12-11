#pragma once

#include <tesla.hpp>
#include <ui/views/MainLayout.hpp>

class MainApplication : public tsl::Overlay {
public:
  virtual void initServices() override;
  virtual void exitServices() override;
  virtual std::unique_ptr<tsl::Gui> loadInitialGui() override;

  static void toggleEatEverythings();
  static bool isEatEvertingsEnabled();

  static u16 getItemToEat();
  static void setItemToEat(u16 val);
};