#include "biocoder.h"

int
main()
{
  start_protocol(
    "Engineering BioBrick vectors from BioBrick parts - Restriction digest");

  /*    * 1X NEB2 buffer
* 100 μg/mL BSA
* 1 μL BioBrick enzyme 1
* 1 μL BioBrick enzyme 2

deionized, sterile H2O to 50 μ*/

  Fluid neb_buffer = new_fluid("10X NEB2 buffer");
  Fluid bsa = new_fluid("Bovine Serum Albumin", "1 mg/ml");
  Fluid re1 = new_fluid("BioBrick enzyme 1");
  Fluid re2 = new_fluid("BioBrick enzyme 2");
  Fluid dna = new_fluid("DNA", "10 µg/ml");
  Fluid water = new_fluid("de-ionized water");

  Container tube1 = new_container(STERILE_PCR_TUBE);

  first_step();
  Fluid fluid_array[6] = { neb_buffer, bsa, re1, re2, dna, water };
  Volume* volume[6] = { vol(5, UL), vol(5, UL), vol(1, UL),
                        vol(1, UL), vol(5, UL), vol(XVAL, UL) };
  char* tubes[1] = { "Restriction digest" };
  mixing_table(2, 7, fluid_array, tubes, volume, vol(50, UL), tube1);

  next_step();
  thermocycler(tube1, 37, time(2, HRS));

  next_step();
  thermocycler(tube1, 80, time(2, MINS));
  comment("This is done to heat inactivate the enzyme. This step is sufficient "
          "to inactivate even Pst I.");

  next_step();
  thermocycler(tube1, 4, time(FOREVER, MINS));

  end_protocol();
}