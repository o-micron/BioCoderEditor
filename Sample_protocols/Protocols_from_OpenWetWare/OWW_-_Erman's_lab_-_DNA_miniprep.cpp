#include "biocoder.h"

int
main()
{
  start_protocol("Erman's lab - DNA miniprep with alkaline lysis");

  Fluid lb = new_fluid("LB (+ antibiotics)", vol(2, ML));
  Fluid sln1 = new_fluid("Alkaline Lysis SLN1",
                         "20mM Tris(pH 8), 50mM Glucose, 10mM EDTA");
  Fluid al2 = new_fluid("freshly prepared AL2", "0.2N NaOH, 1% SDS");
  Fluid al3 = new_fluid("AL3", "3M KAc, glacial acetic acid");
  Fluid isoprop = new_fluid("isopropanol", RT);
  Fluid etoh70 = new_fluid("70% EtOH");
  Fluid te = new_fluid("TE");
  Fluid water = new_fluid("water");
  Solid colony = new_solid("single colony");

  Container flask = new_container(FLASK, lb);
  Container eppendorf1 = new_container(EPPENDORF);
  Container eppendorf2 = new_container(EPPENDORF);

  // 1. o/n grow single colony in 2ml of LB (+ antibiotics)
  first_step();
  inoculation(flask, colony, 37, time(12, HRS), 1);

  //	2. 1.5 ml into eppendorf.
  next_step();
  measure_fluid(flask, vol(2, ML), eppendorf1);

  //	3. Pellet cells at max speed in cold room for 1.5 min.
  next_step();
  centrifuge_pellet(eppendorf1, speed(SPEED_MAX, G), 4, time(1.5, MINS));

  //	4. Discard supernatant , leave pellet as dry as possible.
  next_step();
  comment("Leave pellet as dry as possible.");

  //	5. Resuspend pellet in 100 μL Alkaline Lysis SLN1,vortex.
  next_step();
  measure_fluid(sln1, vol(100, UL), eppendorf1);
  resuspend(eppendorf1);

  //	6. Add 200μL freshly prepared AL2, mix by inverting 5-6 times(DO NOT
  // vortex!)
  next_step();
  measure_fluid(al2, vol(200, UL), eppendorf1);
  invert(eppendorf1, 5, 6);
  comment("Do not vortex!");

  //	7. Put on ice.
  next_step();
  store(eppendorf1, ON_ICE);

  //	8. Add 300μL AL3,invert tubes to mix.
  next_step();
  measure_fluid(al3, vol(300, UL), eppendorf1);
  invert(eppendorf1);

  //	9. Incubate on ice for 5 min.
  next_step();
  incubate(eppendorf1, ON_ICE, time(5, MINS));

  //	10. Centrifuge at max speed for 5 min in cold room.
  //	11. Take the supernatant into a new tube.
  next_step();
  centrifuge_phases_top(
    eppendorf1, speed(SPEED_MAX, RPM), 4, time(5, MINS), eppendorf2);

  //	12. Add 900 isopropanol at RT. Vortex
  next_step();
  measure_fluid(isoprop, vol(900, UL), eppendorf2);
  vortex(eppendorf2);

  //	13. Centrifuge at max speed for 10 min at RT.
  next_step();
  centrifuge_pellet(eppendorf2, speed(SPEED_MAX, RPM), RT, time(10, MINS));

  //	14. Rinse the pellet with 1ml 70%EtOH.
  next_step();
  measure_fluid(etoh70, vol(1, ML), eppendorf2);
  vortex(eppendorf2);

  //	15. Centrifuge at max speed for 5 min at RT.
  next_step();
  centrifuge_pellet(eppendorf2, speed(SPEED_MAX, RPM), RT, time(5, MINS));

  //	16. Air dry the pellet.
  next_step();
  dry_pellet(eppendorf2, IN_AIR);

  //	17. Dissolve each in 50μL TE OR H2O.
  next_step();
  first_option();
  measure_fluid(te, vol(50, UL), eppendorf2);
  next_option();
  measure_fluid(water, vol(50, UL), eppendorf2);
  end_option();
  dissolve(eppendorf2);

  end_protocol();
}