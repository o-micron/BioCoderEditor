#include "biocoder.h"

int
main()
{
  start_protocol("Richard's Lab - Restriction Digestion");

  Fluid dna = new_fluid(
    "prepared DNA", "from Miniprep, PCR or Gel Extraction", vol(20, UL));
  Fluid re = new_fluid("restriction endonucleases");
  Fluid re_buffer = new_fluid("10X restriction endonuclease buffer");
  Fluid bsa = new_fluid("BSA", "optional");
  Fluid phosphatase = new_fluid("phosphatase");
  Fluid phosphatase_buff = new_fluid("phosphatase buffer");
  Fluid water = new_fluid("distilled water");

  Container rxn_tube1 = new_container(STERILE_MICROFUGE_TUBE);
  Container rxn_tube2 = new_container(STERILE_MICROFUGE_TUBE);

  // Procedure
  //
  // 1. Quickly vortex all ingredients (Buffer, BSA, DNA, Enzymes) before
  // beginning
  first_step();
  to_do("Quickly vortex all ingredients (Buffer, BSA, DNA, Enzymes) before "
        "beginning.");

  // 2. Add the following in a micro-centrifuge tube
  //         1. 5μl of Buffer
  //         2. 1μl of BSA
  //         3. 40μl of DNA solution (Dilute PCR products 1:4)
  // 3. Vortex Enzymes and add 20 units (1μl) of each to the tube
  next_step();
  measure_fluid(dna, rxn_tube1);
  measure_prop(rxn_tube1, water, 4);
  name_sample(rxn_tube1, "DNA solution");
  Fluid fluid_array[4] = { re_buffer, bsa, rxn_tube1.contents, re };
  Volume* volumes[4] = { vol(5, UL), vol(1, UL), vol(40, UL), vol(4, UL) };
  char* tubes[1] = { "Restriction Digestion" };
  mixing_table(2, 5, fluid_array, tubes, volumes, vol(50, UL), rxn_tube2);

  // 4. Incubate reaction in a 37°C water bath for at least one hour (I like
  // 1:45).
  next_step();
  incubate(rxn_tube2, 37, min_time(1, HRS));
  comment("Use a water bath for incubation.");

  // 5. Heat kill the digest for 15 minutes at 75°C.
  next_step();
  store_for(rxn_tube2, 75, time(15, MINS), ENZYME_INAC);

  // 6. If digesting a vector add 10 units (1µl) phosphatase) and 5µl
  // Phosphatase Buffer and incubate an additional 45 minutes at then heat kill
  // againat 75°C for 15 min.
  next_step("If digesting vector:");
  measure_fluid(phosphatase, vol(1, UL), rxn_tube2);
  measure_fluid(phosphatase_buff, vol(5, UL), rxn_tube2);
  incubate(rxn_tube2, 37, min_time(45, MINS));
  store_for(rxn_tube2, 75, time(15, MINS), ENZYME_INAC);

  // 7. Store digested DNA in the freezer (-20°C).
  next_step();
  store(rxn_tube2, -20);

  end_protocol();
}