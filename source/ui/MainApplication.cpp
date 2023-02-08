#include <MainApplication.hpp>
#include <Memory.hpp>
#include <Game.hpp>
#include <ui/views/ErrorLayout.hpp>
#include <Utils.hpp>

// static bool exitThread = false;
//  static Thread t;

std::unique_ptr<tsl::Gui> MainApplication::loadInitialGui()
{
  if (R_FAILED(Memory::attach()))
    return initially<ErrorLayout>();

  if (Memory::getTitleId() != 0x01006F8002326000)
    return initially<ErrorLayout>();

  Game::Initialize();

  return initially<MainLayout>();
}
/*
static void checkButtons(void *)
{
  PadState pad;
  padInitializeAny(&pad);
  padUpdate(&pad);

  uint64_t kHeld = padGetButtons(&pad);
  while (!exitThread)
  {
    padUpdate(&pad);
    kHeld = padGetButtons(&pad);

    svcSleepThread(100'000'000);
  }
}*/

void MainApplication::initServices()
{
  smInitialize();
  setInitialize();
  setsysInitialize();
  splInitialize();
  fsInitialize();
  fsdevMountSdmc();
  dmntcht::initialize();
  dmntcht::forceOpenCheatProcess();

  // threadCreate(&t, checkButtons, NULL, NULL, 0x400, 0x3F, -2);
  // threadStart(&t);
}

void MainApplication::exitServices()
{
  Game::Exit();

  // exitThread = true;
  // threadWaitForExit(&t);
  // threadClose(&t);
  // exitThread = false;

  dmntcht::forceCloseCheatProcess();
  dmntcht::exit();
  fsdevUnmountDevice("sdmc");
  fsExit();
  splExit();
  setsysExit();
  setExit();
  smExit();
}