#include "biocoder.h"

int
main()
{
  start_protocol("Ethanol Precipitation of small DNA fragments");

  Fluid ethanol = new_fluid("absolute ethanol", -20);
  Fluid dna = new_fluid("DNA sample", vol(20, UL));
  Fluid eth95 = new_fluid("95% ethanol", RT);
  Fluid water = new_fluid("water", vol(10, UL));

  Container eppendorf1 = new_container(EPPENDORF, dna);

  // 1. Add 2 volumes ice cold absolute ethanol to sample.
  first_step();
  measure_prop(eppendorf1, ethanol, 2);
  comment("Generally the sample is in a 1.5 mL eppendorf tube. I recommend "
          "storing the absolute ethanol at -20°C.");

  // 2. Incubate 1 hr at -80°C.
  next_step();
  incubate(eppendorf1, -80, time(1, HRS));
  comment("The long incubation time is critical for small fragments.");

  // 3. Centrifuge for 30 minutes at 0°C at maximum speed (generally >10000 g at
  // least).
  // 4. Remove supernatant.
  next_step();
  centrifuge_pellet(eppendorf1, min_speed(10000, G), 0, time(30, MINS));

  // 5. Wash with 750-1000 μL room-temperature 95% ethanol.
  next_step();
  measure_fluid(eth95, vol_range(750, 1000, UL), eppendorf1);
  comment(
    "Another critical step for small fragments under 200 base pairs. Generally "
    "washing involves adding the ethanol and inverting several times.");

  // 6. Centrifuge for 10 minutes at 4°C at maximum speed (generally >10000 g at
  // least).
  next_step();
  centrifuge_pellet(eppendorf1, min_speed(10000, G), 4, time(10, MINS));

  // 7. Let air dry on benchtop.
  next_step();
  dry_pellet(eppendorf1, IN_AIR);
  comment("I generally let the pellet air dry completely such that it becomes "
          "white so that all residual ethanol is eliminated.");

  // 8. Resuspend in an appropriate volume of H2O.
  next_step();
  measure_fluid(water, eppendorf1);
  comment("Use appropriate volume of water.");
  resuspend(eppendorf1);
  comment("Many protocols recommend resuspending in 10 mM Tris-HCl or TE. The "
          "advantage of TE is that EDTA chelates magnesium ions which makes it "
          "more difficult for residual DNases to degrade the DNA. I generally "
          "prefer H2O and don't seem to experience problems of this sort. If "
          "you plan to ultimately use electroporation to transform your DNA "
          "then resuspending in H2O has the advantage of keeping the salt "
          "content of your ligation reaction down.");

  end_protocol();
}