#include "biocoder.h"

int
main()
{
  start_protocol(
    "Engineering BioBrick vectors from BioBrick parts - Colony PCR");

  Fluid pcr_mix = new_fluid("PCR SuperMix High Fidelity");
  Fluid vf2 = new_fluid("VF2 primer", "5'-TGCCACCTGACGTCTAAGAA-3'");
  Fluid vr = new_fluid("VR primer", "5'-ATTACCGCCTTTGAGTGAGC-3'");
  Fluid colony =
    new_fluid("colony suspension", "1 colony diluted in 100 μl water");
  Fluid water = new_fluid("de-ionized water");

  Container tube1 = new_container(STERILE_PCR_TUBE);

  Symbol x = new_symbol("X", "concentration of primers (µmole/µl)");

  // PCR mix
  //
  //    * 9 μL PCR SuperMix High Fidelity
  //    * 6.25 pmoles VF2 primer
  //    * 6.25 pmoles VR primer
  //    * 1 μL colony suspension
  //          o dilute 1 colony in 100 μL water
  //
  first_step("PCR mix");
  Fluid fluid_array[5] = { pcr_mix, vf2, vr, colony, water };
  Symbol y = divide(vol(0.125, UL), s_vol(x));
  Volume* volume[5] = {
    vol(9, UL), s_vol(y), s_vol(y), vol(1, UL), vol(XVAL, UL)
  };
  char* tubes[1] = { "Colony PCR" };
  mixing_table(2, 6, fluid_array, tubes, volume, vol(20, UL), tube1);

  // PCR conditions
  //
  //   1. 95°C for 15 minutes
  //   2. 94°C for 30 seconds
  //   3. 62°C for 30 seconds
  //   4. 68°C for 3.5 minutes
  //   5. Repeat 2-4 39 times.
  //   6. 68°C for 20 mins
  //   7. 4°C forever
  next_step("PCR conditions");
  pcr_init_denat(tube1, 95, time(15, MINS));
  thermocycler(tube1,
               39,
               94,
               time(30, SECS),
               62,
               time(30, SECS),
               68,
               time(3.5, MINS),
               NORMAL);
  pcr_final_ext(tube1, 68, time(20, MINS), 4);

  end_protocol();
}