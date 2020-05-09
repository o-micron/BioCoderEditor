#include "biocoder.h"

int
main()
{
  start_protocol("Chromosomal DNA isolation from E.coli");

  Fluid culture = new_fluid("culture grown in your favorite medium");
  Fluid killing_buffer = new_fluid("Killing Buffer", ICE_COLD);
  Fluid te = new_fluid("TE");
  Fluid sds = new_fluid("10% SDS");
  Fluid edta = new_fluid("0.5M EDTA");
  Fluid isopropanol = new_fluid("isopropanol");
  Fluid rnase = new_fluid("RNase A", "25mg/ml");
  Fluid proteinasek = new_fluid("proteinase K", "25mg/ml");
  Fluid ethanol = new_fluid("ethanol");
  Fluid naac = new_fluid("3M Na-Acetate");
  Fluid water = new_fluid("distilled water");

  Container eppendorf1 = new_container(EPPENDORF, culture);

  //	    *  grow culture in your favorite medium
  //    * Mix samples directly with ice cold Killing Buffer in ratio 1:1 and put
  //    on ice (samples should be processed as fast as possible)
  first_step();
  measure_prop(eppendorf1, killing_buffer, 1);
  vortex(eppendorf1);
  store(eppendorf1, ON_ICE);
  comment("Samples should be processed as fast as possible.");

  //    * Spin down cells 3 min max. speed at 4°C and discard supernatant
  next_step();
  centrifuge_pellet(eppendorf1, speed(SPEED_MAX, RPM), 4, time(3, MINS));

  //    * resuspend in 300μL TE and add 40μL 10%SDS and 3μL 0.5M EDTA
  next_step();
  measure_fluid(te, vol(300, UL), eppendorf1);
  measure_fluid(sds, vol(40, UL), eppendorf1);
  measure_fluid(edta, vol(3, UL), eppendorf1);
  resuspend(eppendorf1);

  //    * incubate 5 min at 65°C
  next_step();
  incubate(eppendorf1, 65, time(5, MINS));

  //    * add 750μL isopropanole and mix
  next_step();
  measure_fluid(isopropanol, vol(750, UL), eppendorf1);
  vortex(eppendorf1);

  //    * spin at max. speed for 5 min
  next_step();
  centrifuge_pellet(eppendorf1, speed(SPEED_MAX, RPM), RT, time(5, MINS));

  //    * resuspend pellet in 500μL TE and add 2μL RNase A (25mg/ml)
  next_step();
  measure_fluid(te, vol(500, UL), eppendorf1);
  measure_fluid(rnase, vol(2, UL), eppendorf1);
  resuspend(eppendorf1);

  //    * incubate for 30 min at 65°C
  next_step();
  incubate(eppendorf1, 65, time(30, MINS));

  //    * add 2μL proteinase K (25mg/ml) and incubate at 37°C for 15 min
  next_step();
  measure_fluid(proteinasek, vol(2, UL), eppendorf1);
  incubate(eppendorf1, 37, time(15, MINS));

  //    * phenol extract (2x phenol & 2x chlorophorm)
  next_step();
  to_do("phenol extract (2x phenol & 2x chlorophorm).");

  //    * precipitate over night with 1ml ethanol and 40μL 3M Na-Acetate
  next_step();
  measure_fluid(ethanol, vol(1, ML), eppendorf1);
  measure_fluid(naac, vol(40, UL), eppendorf1);
  store_for(eppendorf1, RT, time(12, HRS));

  //    * spin down DNA at 4°C for 15 min, wash in 70% ethanol and resuspend
  //    pellet in 50μL dH2O
  next_step();
  centrifuge_pellet(eppendorf1, speed(SPEED_MAX, RPM), 4, time(15, MINS));
  measure_fluid(water, vol(50, UL), eppendorf1);
  resuspend(eppendorf1);

  end_protocol();
}