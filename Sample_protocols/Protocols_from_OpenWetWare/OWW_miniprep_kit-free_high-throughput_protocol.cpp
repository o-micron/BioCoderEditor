#include "biocoder.h"

int
main()
{

  start_protocol("Miniprep - Kit-free high-throughput protocol");

  Fluid culture = new_fluid("overnight culture containing your plasmid");
  Fluid stet = new_fluid(
    "STET buffer",
    "8% sucrose, 50 mM Tris-HCl (pH 8), 0.5% Triton X-100, 50 mM EDTA");
  Fluid lysozyme = new_fluid("lysozyme (10mg/ml)");
  Fluid isoprop = new_fluid("isopropanol", ICE_COLD);
  Fluid eth80 = new_fluid("80% ethanol", ICE_COLD);
  Fluid te = new_fluid("TE buffer", "10 mM Tris-HCl (pH 8), 1 mM EDTA ");

  Container flask = new_container(FLASK, culture);
  Container eppendorf1 = new_container(EPPENDORF);

  // 1. Transfer 1.5 mL of an overnight culture containing your plasmid to an
  // eppendorf tube and spin at 5000 rpm for 5 min in a tabletop centrifuge to
  // pellet the cells.
  // 2. Remove and discard the supernatent.
  first_step();
  measure_fluid(flask, vol(1.5, ML), eppendorf1);
  centrifuge_pellet(eppendorf1, speed(5000, RPM), RT, time(5, MINS));

  // 3. Add 300 μL STET buffer, and resuspend cells by vortexing.
  next_step();
  measure_fluid(stet, vol(300, UL), eppendorf1);
  resuspend(eppendorf1);

  // 4. Add 10 μL lysozyme (10 mg/mL), vortex, and submerse in boiling water for
  // 40 sec.
  next_step();
  measure_fluid(lysozyme, vol(10, UL), eppendorf1);
  vortex(eppendorf1);
  store_for(eppendorf1, 100, time(40, SECS));

  // 5. Spin for 30 min in a tabletop centrifuge at maximum speed at 4 ˚C.
  next_step();
  centrifuge_pellet(eppendorf1, speed(SPEED_MAX, RPM), 4, time(30, MINS));

  // 6. Remove pellet from each tube with a toothpick. The cellular debris
  // should stick well to the toothpick. Try to insert and remove the toothpick
  // from the center of the tube so you don't get any cellular debris on the
  // sides of the tube.
  next_step();
  comment("Remove pellet from each tube with a toothpick. The cellular debris "
          "should stick well to the toothpick. Try to insert and remove the "
          "toothpick from the center of the tube so you don't get any cellular "
          "debris on the sides of the tube.");

  // 7. Add 300 μL ice cold isopropanol to precipitate the DNA (or 300μL of 2:1
  // isopropanol:ammonium acetate, mixed just before you use it. See this
  // discussion of precipitating nucleic acids.)
  next_step();
  measure_fluid(isoprop, vol(300, UL), eppendorf1);
  comment("This is done to precipitate the DNA.");

  // 8. Spin for 10 min in a tabletop centrifuge at maximum speed at 4 ˚C.
  // 9. Remove supernatent.
  next_step();
  centrifuge_pellet(eppendorf1, speed(SPEED_MAX, RPM), 4, time(10, MINS));

  // 10. Add 200 μL ice cold 80% ethanol to wash pellet and spin for 5 min in a
  // tabletop centrifuge at maximum speed at 4 ˚C.
  // 11. Remove supernatent.
  next_step();
  measure_fluid(eth80, vol(200, UL), eppendorf1);
  comment("This is to wash the pellet.");
  centrifuge_pellet(eppendorf1, speed(SPEED_MAX, RPM), 4, time(5, MINS));

  // 12. Dry pellet (air dry at room temperature or 37 ˚C or dry in a speedvac).
  next_step();
  first_option();
  dry_pellet(eppendorf1, IN_AIR);
  next_option();
  dry_pellet(eppendorf1, IN_VACUUM);
  end_option();

  // 13. Rehydrate in 50 μL TE.
  next_step();
  measure_fluid(te, vol(50, UL), eppendorf1);
  resuspend(eppendorf1);

  end_protocol();
}