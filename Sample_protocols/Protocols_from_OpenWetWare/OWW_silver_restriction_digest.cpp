#include "biocoder.h"

int
main()
{
  start_protocol("Silver- Restriction Digest");

  Fluid enzyme1 = new_fluid("restriction enzyme1", "20 units/�l");
  Fluid enzyme2 = new_fluid("restriction enzyme2", "20 units/�l");
  Fluid buffer = new_fluid("10X restriction buffer");
  Fluid bsa = new_fluid("10X BSA");
  Fluid dna = new_fluid("PCR product DNA");
  Fluid water = new_fluid("distilled H2O");
  Fluid vector = new_fluid("BioBrick vector", "BBa_V0002 or BBa_V0100");
  Fluid cip = new_fluid("CIP", "10 units/�l");

  Container rxn_tube1 = new_container(RXN_TUBE);
  Container rxn_tube2 = new_container(RXN_TUBE);

  Symbol v = new_symbol("V", "Volume of PCR product DNA obtained (�l).");
  Symbol c = new_symbol("C", "Concentration of BioBrick vector (�g/�l).");

  //#  Mix:
  //
  //    * All of PCR product DNA (~28 �L if PCR purification was eluted in 30
  //    �L)
  //    * 3.5 �L 10x BSA
  //    * 3.5 �L 10x buffer (see NEB website for optimal double digest buffer
  //    choices)
  //    * 0.2 �L enzyme 1 (20 units/�L)
  //    * 0.2 �L enzyme 2 (20 units/�L)
  //    * distilled water to 35 �L total volume
  //    * Note: keep the glycerol concentration below 5%, the volume of both
  //    restriction enzymes added should not exceed 5% of the total reaction
  //    volume.

  first_step("Digestion of PCR product");
  {
    Fluid fluid_array[6] = { dna, bsa, buffer, enzyme1, enzyme2, water };
    char* tubes[1] = { "Restriction Digest - PCR product" };
    Volume* volume[6] = { s_vol(v),     vol(3.5, UL), vol(3.5, UL),
                          vol(0.2, UL), vol(0.2, UL), vol(XVAL, UL) };
    mixing_table(2, 7, fluid_array, tubes, volume, vol(35, UL), rxn_tube1);
  }
  comment("Note: keep the glycerol concentration below 5%, the volume of both "
          "restriction enzymes added should not exceed 5% of the total "
          "reaction volume.");

  //# Incubate at least an hour (better overnight) at 37 �C.
  next_step();
  incubate(rxn_tube1, 37, min_time(1, HRS));
  comment("Incubation overnight is better.");

  //# Purify digested insert using PCR product purification kit
  next_step();
  comment("Purify digested insert using PCR product purification kit.");

  next_step("Digestion of BioBrick vector: Recommended procedure");
  /* 1.  Mix:
   * 700 ng BioBrick vector (BBa_V0002 or BBa_V0100)
   * 1 �L 10 x BSA
   * 1 �L 10x buffer (see NEB website for optimal double digest buffer choices)
   * 0.2 �L enzyme 1 (20 units/�L)
   * 0.2 �L enzyme 2 (20 units/�L)
   * distilled water to 10 �L total volume
   * Note: To keep the glycerol concentration below 5%, the volume of both
   * restriction enzymes added should not exceed 10% of the total reaction
   * volume. */
  {
    Fluid fluid_array[6] = { vector, bsa, buffer, enzyme1, enzyme2, water };
    char* tubes[1] = { "Restriction Digest - vector" };
    Volume* volume[6] = { s_vol(divide(vol(7, UL), s_vol(c))),
                          vol(1, UL),
                          vol(1, UL),
                          vol(0.2, UL),
                          vol(0.2, UL),
                          vol(XVAL, UL) };
    mixing_table(2, 7, fluid_array, tubes, volume, vol(10, UL), rxn_tube2);
  }
  comment("Note: keep the glycerol concentration below 5%, the volume of both "
          "restriction enzymes added should not exceed 5% of the total "
          "reaction volume.");

  // 2. Incubate overnight at 37 �C.
  next_step();
  incubate(rxn_tube1, 37, time(12, HRS));

  // 3. The next morning, add 0.1 �L CIP (10 units/�L) and incubate for 1 hr. at
  // 37 �C.
  next_step();
  measure_fluid(cip, vol(0.1, UL), rxn_tube2);
  incubate(rxn_tube2, 37, time(1, HRS));

  end_protocol();
}