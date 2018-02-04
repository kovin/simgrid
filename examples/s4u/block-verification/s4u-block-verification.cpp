#include "node.hpp"
#include "miner.hpp"
#include "monitor.hpp"

XBT_LOG_NEW_DEFAULT_CATEGORY(block_verification, "Messages specific for this msg example");

int main(int argc, char *argv[])
{
  srand(1);
  xbt_assert(argc > 2, "Usage: %s platform_file deployment_file", argv[0]);

  simgrid::s4u::Engine e(&argc, argv);
  e.registerFunction<Node>("node");
  e.registerFunction<Miner>("miner");
  e.registerFunction<Monitor>("monitor");

  e.loadPlatform(argv[1]);
  e.loadDeployment(argv[2]);
  e.run();

  return 0;
}
