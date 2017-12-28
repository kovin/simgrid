#include "s4u-p2p-random-data.hpp"

XBT_LOG_NEW_DEFAULT_CATEGORY(p2p_random_data, "Messages specific for this msg example");

int main(int argc, char *argv[])
{
  srand(1);
  xbt_assert(argc > 2, "Usage: %s platform_file deployment_file\n", argv[0]);

  simgrid::s4u::Engine e(&argc, argv);
  e.registerFunction<Node>("node");

  e.loadPlatform(argv[1]);
  e.loadDeployment(argv[2]);
  e.run();

  return 0;
}
