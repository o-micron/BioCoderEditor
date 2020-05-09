#include "biocoder.h"
#define X 60

int
main()
{

  start_protocol("Knight - Colony PCR");

  Fluid supermix = new_fluid("PCR supermix");
  Fluid vf2 = new_fluid("VF2", "40 µM");
  Fluid vr = new_fluid("VR", "40 µM");
  Fluid colony = new_fluid("colony template");

  Container rxn_tube = new_container(RXN_TUBE);

  // Reaction mixture
  //
  // 1X Reaction
  //
  //    * 9 μL PCR supermix
  //    * 0.25 μL 40μM VF2
  //    * 0.25 μL 40μM VR
  //    * 0.5 μL colony template
  //
  first_step("Reaction Mixture");
  Fluid fluid_array[4] = { supermix, vf2, vr, colony };
  Volume* volumes[4] = {
    vol(9, UL), vol(0.25, UL), vol(0.25, UL), vol(0.5, UL)
  };
  char* tube[1] = { "Colony PCR" };
  mixing_table(2, 5, fluid_array, tube, volumes, vol(10, UL), rxn_tube);

  // PCR conditions
  //
  //   1. 95°C for 15 mins
  //   2. 94°C for 30 secs
  //   3. 56°C for 30 secs
  //   4. 68°C for 1 min per kb of expected product
  //          * I typically round up for this step. i.e. For a 3.6kb construct,
  //          I used a 4 min elongation time. It seems to help to be a bit
  //          generous with the elongation time.
  //   5. Repeat 2-4 39 times.
  //   6. 68°C for 20 mins
  //   7. 4°C forever
  //
  next_step("PCR conditions");
  pcr_init_denat(rxn_tube, 95, time(15, MINS));
  thermocycler(rxn_tube,
               39,
               94,
               time(30, SECS),
               56,
               time(30, SECS),
               68,
               time(X, SECS),
               NORMAL);
  comment(
    "Elongation time : 1 min per kb of expected product. I typically round up "
    "for this step. i.e. For a 3.6kb construct, I used a 4 min elongation "
    "time. It seems to help to be a bit generous with the elongation time.");
  pcr_final_ext(rxn_tube, 60, time(20, MINS), 4);

  // Run a gel to determine amplification product length.
  next_step();
  name_sample(rxn_tube, "PCR products");
  electrophoresis(rxn_tube);

  end_protocol();
}