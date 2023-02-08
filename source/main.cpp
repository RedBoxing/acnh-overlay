#define TESLA_INIT_IMPL

#include <tesla.hpp>
#include <ui/MainApplication.hpp>

int main(int argc, char **argv)
{
  return tsl::loop<MainApplication, tsl::impl::LaunchFlags::None>(argc, argv);
}