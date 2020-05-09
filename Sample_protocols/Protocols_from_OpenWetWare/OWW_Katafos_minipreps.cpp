#include "biocoder.h"

int
main()
{
  start_protocol("Katafos - Minipreps");

  Fluid lb = new_fluid("sterile LB medium (+ antibiotic)", vol(5, ML));
  Fluid p1 = new_fluid("P1 Buffer");
  Fluid p2 = new_fluid("P2 Buffer");
  Fluid p3 = new_fluid("P3 Buffer", ICE_COLD);
  Fluid etoh = new_fluid("absolute EtOH");
  Fluid etoh70 = new_fluid("70% ethanol");
  Fluid te = new_fluid("TE");
  Fluid water = new_fluid("ddH<sub>2</sub>O");
  Solid colony = new_solid("single bacterial colony");

  Container flask = new_container(FLASK, lb);
  Container eppendorf1 = new_container(EPPENDORF);
  Container eppendorf2 = new_container(EPPENDORF);

  // Day 1
  //
  //- inoculate 5 ml sterile LB medium (+ antibiotic ?) with single bacterial
  //colony
  //- grow ON @ 37˚C while shaking vigorously
  first_step("DAY 1");
  inoculation(flask, colony, 37, time(12, HRS), 1);

  //[edit] Day 2
  //
  //- spin 1.5 ml 2’ @ 8,000 rpm in table top centrifuge
  next_step("DAY 2");
  first_sub_step();
  measure_fluid(flask, vol(1.5, ML), eppendorf1);
  centrifuge_pellet(eppendorf1, speed(8000, RPM), RT, time(2, MINS));

  //- resuspend pellet in 100 μl P1 Buffer. Let sit 5’ @ RT
  next_sub_step();
  measure_fluid(p1, vol(100, UL), eppendorf1);
  resuspend(eppendorf1);
  store_for(eppendorf1, RT, time(5, MINS));

  //- add 200 μl P2 Buffer, mix and let sit no more than 5’ @ RT
  next_sub_step();
  measure_fluid(p2, vol(200, UL), eppendorf1);
  store_for(eppendorf1, RT, max_time(5, MINS));

  //- add 150 μl ice cold P3 Buffer, mix well and place 5 – 10’ on ice
  next_sub_step();
  measure_fluid(p3, vol(150, UL), eppendorf1);
  vortex(eppendorf1);
  store_for(eppendorf1, ON_ICE, time_range(5, 10, MINS));

  //- spin 10’ @ max rpm (4˚C if possible)
  next_sub_step();
  centrifuge_phases_top(
    eppendorf1, speed(SPEED_MAX, RPM), 4, time(10, MINS), eppendorf2);

  //- transfer supernatant to new tube and add 2 volumes of abs EtOH. Let sit
  //for 10 – 20’ on ice
  next_sub_step();
  measure_prop(eppendorf2, etoh, 2);
  store_for(eppendorf2, ON_ICE, time_range(10, 20, MINS));

  //- spin 10’ @ max. speed @ 4˚C
  next_sub_step();
  centrifuge_pellet(eppendorf2, speed(SPEED_MAX, RPM), 4, time(10, MINS));

  //- discard supernatant and wash pellet with 1ml of 70% EtOH
  next_sub_step();
  measure_fluid(etoh70, vol(1, ML), eppendorf2);
  pipet(eppendorf2);

  //- spin 5’ @ max speed
  next_sub_step();
  centrifuge_pellet(eppendorf2, speed(SPEED_MAX, RPM), RT, time(5, MINS));

  //- suck up or decant supernatant, speedvac or dry pellet @ 37˚C (do not
  //overdo it, though, or it will not properly dissolve again)
  next_step();
  first_option();
  dry_pellet(eppendorf2, IN_AIR);
  next_option();
  dry_pellet(eppendorf2, IN_VACUUM);
  end_option();
  comment(
    "Do not overdo drying, or the pellet will not properly dissolve again.");

  //- resuspend pellet in 30 μl TE Buffer or ddH20 and check on gel
  next_step();
  first_option();
  measure_fluid(te, vol(30, UL), eppendorf2);
  next_option();
  measure_fluid(water, vol(30, UL), eppendorf2);
  end_option();
  resuspend(eppendorf2);
  electrophoresis(eppendorf2);

  end_protocol();
}