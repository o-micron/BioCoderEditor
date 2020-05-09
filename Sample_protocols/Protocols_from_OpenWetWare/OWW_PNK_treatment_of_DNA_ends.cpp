#include "biocoder.h"

int
main()
{
  start_protocol("PNK Treatment of DNA ends");

  Fluid pnk_stock = new_fluid("PNK stock", "10, 000 U/ml");
  Fluid ligase_buffer = new_fluid("T4 ligase buffer");
  Fluid substrate = new_fluid("substrate");

  Container rxn_tube1 = new_container(RXN_TUBE);

  // Reaction Mix (10μl)
  //
  //    * 1 μL PNK stock (10,000 U/ml)
  //    * 1 μL T4 Ligase Buffer
  //    * 8 μL Substrate
  //
  first_step("Reaction Mix (10 µl)");
  Fluid fluid_array[3] = { pnk_stock, ligase_buffer, substrate };
  Volume* volumes[3] = { vol(1, UL), vol(1, UL), vol(8, UL) };
  char* tube[1] = { "Reaction Mix" };
  mixing_table(2, 4, fluid_array, tube, volumes, vol(10, UL), rxn_tube1);

  // Reaction Conditions
  //
  //   1. 37°C for 30mins
  //   2. 65°C for 20mins
  //   3. Store at 4°C
  next_step("Reaction Conditions");
  incubate(rxn_tube1, 37, time(30, MINS));
  incubate(rxn_tube1, 65, time(20, MINS));
  store(rxn_tube1, 4);

  end_protocol();
}