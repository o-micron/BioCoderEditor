#include "biocoder.h"

int
main()
{
  start_protocol("Maxiprep of Plasmid DNA from E.coli");

  Fluid medium = new_fluid("LB broth + selective marker", vol(50, ML));
  Fluid glycerol = new_fluid("50% sterile glycerol");
  Fluid teg = new_fluid("TEG", "25mM Tris-Cl, 10mM EDTA, 50mM dextrose");
  Fluid lysozyme = new_fluid("20 mg/ml lysozyme");
  Fluid sds = new_fluid("10% SDS");
  Fluid naoh = new_fluid("4M NaOH");
  Fluid water = new_fluid("autoclaved water");
  Fluid sol3 = new_fluid(
    "Solution 3", "3M potassium-acetate, 2M acetic acid -- glacial is 17M");
  Fluid isoprop = new_fluid("isopropanol");
  Fluid eth70 = new_fluid("70% ethanol");
  Fluid te = new_fluid("TE buffer");
  Fluid licl = new_fluid("5M LiCl");
  Fluid rnase = new_fluid("1 mg/ml RNaseA");
  Fluid phe_chloro_iaa =
    new_fluid("phenol: chloroform: isoamyl alcohol", "25:24:1");
  Fluid chloro_iaa = new_fluid("chloroform: isoamyl alcohol", "24:1");
  Fluid eth100 = new_fluid("straight ethanol");
  Fluid naac = new_fluid("3M sodium acetate");

  Solid bac_col = new_solid("a single colony of E. coli");

  Container flask = new_container(FLASK, medium);
  Container eppendorf1 = new_container(EPPENDORF);
  Container eppendorf2 = new_container(EPPENDORF);
  Container eppendorf3 = new_container(EPPENDORF);
  Container eppendorf4 = new_container(EPPENDORF);
  Container eppendorf5 = new_container(EPPENDORF);
  Container eppendorf6 = new_container(EPPENDORF);
  Container oakridge = new_container(OAKRIDGE);
  Container oakridge1 = new_container(OAKRIDGE);

  //  1. Grow a single colony of E. coli overnight in 50mL LB broth + selective
  //  markers at 37°C.
  first_step();
  inoculation(flask, bac_col, 37, time(12, HRS), 1);

  // 2. The next morning, put 850μL of the culture in each of two Eppendorf
  // tubes, add 150μL sterile 50% glycerol, and store at -80°C. Pour as much
  // culture as will fit into an Oak Ridge tube and centrifuge at 5800g/6000rpm,
  // 4°C, for 10 minutes in a GSA rotor. Discard the supernatant, add the rest
  // of the culture, and repeat. Resuspend in 1mL TEG.
  next_step();
  measure_fluid(flask, vol(850, UL), eppendorf1);
  measure_fluid(flask, vol(850, UL), eppendorf2);
  measure_fluid(glycerol, vol(150, UL), eppendorf1);
  measure_fluid(glycerol, vol(150, UL), eppendorf2);
  store(eppendorf2, -80);
  store(eppendorf1, -80);

  next_step();
  name_sample(flask, "as much culture as will fit");
  measure_fluid(flask.contents, oakridge);
  centrifuge_pellet(oakridge, speed(5800, G), 4, time(10, MINS));
  name_sample(flask, "rest of the culture");
  measure_fluid(flask.contents, oakridge);
  centrifuge_pellet(oakridge, speed(5800, G), 4, time(10, MINS));
  measure_fluid(teg, vol(1, ML), oakridge);
  resuspend(oakridge);

  // 3. Add 111μL 20mg/mL lysozyme. Incubate on ice for 30 minutes. Meanwhile,
  // mix: 250μL 10% SDS, 125μL 4M NaOH, 2.125mL autoclaved water per culture.
  next_step();
  measure_fluid(lysozyme, vol(111, UL), oakridge);
  incubate(oakridge, ON_ICE, time(30, MINS));

  parallel_step();
  measure_fluid(sds, vol(250, UL), eppendorf3);
  measure_fluid(naoh, vol(125, UL), eppendorf3);
  measure_fluid(water, vol(2.125, ML), eppendorf3);
  vortex(eppendorf3);
  name_sample(eppendorf3, "SDS/NaOH mix");

  // 4. Add 2mL SDS/NaOH mix to each tube. Incubate on ice for 10 minutes.
  next_step();
  measure_fluid(eppendorf3, vol(2, ML), oakridge);
  incubate(oakridge, ON_ICE, time(10, MINS));

  // 5. Add 1.5mL Solution 3 (3M K+, 5M acetate). Incubate on ice for 10
  // minutes.
  next_step();
  measure_fluid(sol3, vol(1.5, ML), oakridge);
  incubate(oakridge, ON_ICE, time(10, MINS));

  // 6. Shake vigorously. Centrifuge in SS34 rotor at 17,200g/12,000rpm, 4°C,
  // for 15 minutes.
  next_step();
  vortex(oakridge);
  centrifuge_phases_top(
    oakridge, speed(17200, G), 4, time(15, MINS), oakridge1);

  // 7. Pour the supernatant into another Oak Ridge tube and discard the pellet.
  // Add 2.7mL isopropanol. Centrifuge at 17,200g/12,000rpm (room temperature)
  // for 10 minutes. Discard the supernatant.
  next_step();
  measure_fluid(isoprop, vol(2.7, ML), oakridge1);
  centrifuge_pellet(oakridge1, speed(17200, G), RT, time(10, MINS));

  // 8. Wash pellet with 1mL 70% ethanol. Air dry for 2-5 minutes on bench.
  // Resuspend in 500μL TE buffer. Add 500μL 5M LiCl. Incubate on ice for 5
  // minutes.
  next_step();
  measure_fluid(eth70, vol(1, ML), oakridge1);
  vortex(oakridge1);
  centrifuge_pellet(oakridge1, speed(17200, G), RT, time(10, MINS));
  dry_pellet(oakridge1, IN_AIR, time_range(2, 5, MINS));
  measure_fluid(te, vol(500, UL), oakridge1);
  resuspend(oakridge1);
  measure_fluid(licl, vol(500, UL), oakridge1);
  incubate(oakridge1, ON_ICE, time(5, MINS));

  // 9. Centrifuge at 17,200g/12,000rpm for 10 minutes.
  centrifuge_phases_top(
    oakridge1, speed(17200, G), RT, time(10, MINS), eppendorf4);

  // 10. Pour supernatant into an Eppendorf. Add 1mL isopropanol. Incubate on
  // the bench for 10 minutes.
  next_step();
  measure_fluid(isoprop, vol(1, ML), eppendorf4);
  incubate(eppendorf4, RT, time(10, MINS));

  // 11. Centrifuge at 17,200g/12,000rpm for 10 minutes.
  next_step();
  centrifuge_pellet(eppendorf4, speed(17200, G), RT, time(10, MINS));

  // 12. Discard the supernatant. Wash the pellets with 100μL 70% ethanol.
  // Resuspend in 375μL TE buffer. Add 7.5μL 1mg/mL RNaseA. Incubate at 37°C for
  // 30 minutes.
  next_step();
  measure_fluid(eth70, vol(100, UL), eppendorf4);
  vortex(eppendorf4);
  centrifuge_pellet(eppendorf4, speed(17200, G), RT, time(10, MINS));
  measure_fluid(te, vol(375, UL), eppendorf4);
  resuspend(eppendorf4);
  measure_fluid(rnase, vol(7.5, UL), eppendorf4);
  incubate(eppendorf4, 37, time(30, MINS));

  // 13. Add 700μL phenol:chloroform:isoamyl alcohol. Vortex until thoroughly
  // mixed. Centrifuge at top speed of microfuge for 2 minutes. Pipette aqueous
  // phase (the top one) into new Eppendorf. Repeat until the interface between
  // the phases is clear after centrifugation. Then repeat the procedure twice
  // with chloroform:isoamyl alcohol to remove any phenol.
  next_step();
  measure_fluid(phe_chloro_iaa, vol(700, UL), eppendorf4);
  vortex(eppendorf4);
  comment("The solution should be thoroughly mixed.");
  centrifuge_phases_top(
    eppendorf4, speed(SPEED_MAX, G), RT, time(2, MINS), eppendorf5);

  next_step();
  repeat(14);
  to_do("Repeat until the interface between the phases is clear after "
        "centrifugation");

  next_step();
  measure_fluid(chloro_iaa, vol(700, UL), eppendorf5);
  vortex(eppendorf5);
  comment("The solution should be thoroughly mixed.");
  centrifuge_phases_top(
    eppendorf5, speed(SPEED_MAX, G), RT, time(2, MINS), eppendorf6);

  next_step();
  repeat(16);
  comment("This removes phenol.");

  // 14. Add 750μL straight ethanol and 125μL 3M sodium acetate. Put at -80°C
  // for 30 minutes or -20°C overnight.
  next_step();
  measure_fluid(eth100, vol(750, UL), eppendorf6);
  measure_fluid(naac, vol(125, UL), eppendorf6);
  first_option();
  store_for(eppendorf6, -80, time(30, MINS));
  next_option();
  store_for(eppendorf6, -20, time(12, HRS));
  end_option();

  // 15. Centrifuge at 13,600g/12,000rpm, 4°C, for 15 minutes. Discard the
  // supernatant. Wash pellet with ~100μL 70% ethanol. Resuspend in 100-200μL TE
  // buffer.
  next_step();
  centrifuge_pellet(eppendorf6, speed(13600, G), 4, time(15, MINS));
  measure_fluid(eth70, approx_vol(100, UL), eppendorf6);
  vortex(eppendorf6);
  centrifuge_pellet(eppendorf6, speed(13600, G), 4, time(5, MINS));
  measure_fluid(te, vol_range(100, 200, UL), eppendorf6);
  resuspend(eppendorf6);

  end_protocol();
}