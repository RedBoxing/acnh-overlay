#include <MainApplication.hpp>
#include <Memory.hpp>
#include <Game.hpp>
#include <ui/views/ErrorLayout.hpp>
#include <Utils.hpp>

static bool exitThread = false;
static Thread t;
static bool eatEverythings = false;
static u16 itemToEat = 0xfffe;

std::unique_ptr<tsl::Gui> MainApplication::loadInitialGui()
{
  if (R_FAILED(Memory::attach()))
    return initially<ErrorLayout>();

  if (Memory::getTitleId() != 0x01006F8002326000)
    return initially<ErrorLayout>();

  Game::Initialize();

  return initially<MainLayout>();
}

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

    if (MainApplication::isEatEvertingsEnabled() && (kHeld & HidNpadButton_A) && (kHeld & HidNpadButton_L))
    {
      u16 toEat = MainApplication::getItemToEat();
      Memory::writeMemory(Game::EatEverythings, &toEat, sizeof(toEat));
    }

    svcSleepThread(100'000'000);
  }
}

void MainApplication::initServices()
{
  smInitialize();
  setInitialize();
  dmntcht::initialize();
  dmntcht::forceOpenCheatProcess();

  eatEverythings = false;
  itemToEat = 0xfffe;

  threadCreate(&t, checkButtons, NULL, NULL, 0x400, 0x3F, -2);
  threadStart(&t);
}

void MainApplication::exitServices()
{
  exitThread = true;
  threadWaitForExit(&t);
  threadClose(&t);
  exitThread = false;

  dmntcht::exit();
  setExit();
  smExit();
}

void MainApplication::toggleEatEverythings()
{
  eatEverythings = !eatEverythings;
}

bool MainApplication::isEatEvertingsEnabled()
{
  return eatEverythings;
}

u16 MainApplication::getItemToEat()
{
  return itemToEat;
}

void MainApplication::setItemToEat(u16 val)
{
  itemToEat = val;
}