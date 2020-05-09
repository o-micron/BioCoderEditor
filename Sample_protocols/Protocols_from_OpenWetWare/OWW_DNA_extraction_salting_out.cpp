#include "biocoder.h"

int
main()
{
  start_protocol("DNA Extraction - salting out");

  Fluid dig_buffer =
    new_fluid("Digestion Buffer",
              "10mM NaCl, 10mM TRIS (pH 8.0), 10mM EDTA (pH 8.0), 0.5% SDS");
  Fluid proteinasek = new_fluid("Proteinase K", "20mg/ml");
  Fluid naac = new_fluid("Sodium Acetate pH 5.2", "3M");
  Fluid eth98 = new_fluid("98% ethanol", ICE_COLD);
  Fluid eth70 = new_fluid("70% ethanol", ICE_COLD);
  Fluid te = new_fluid("1X TE");
  Fluid water = new_fluid("water");

  Tissue req_tissue = new_solid("tissue");

  Container sterile_microfuge_tube1 =
    new_container(STERILE_MICROFUGE_TUBE, req_tissue);
  Container sterile_microfuge_tube2 = new_container(STERILE_MICROFUGE_TUBE);
  Container sterile_microfuge_tube3 = new_container(STERILE_MICROFUGE_TUBE);
  Container sterile_microfuge_tube4 = new_container(STERILE_MICROFUGE_TUBE);

  // * Tissue Digestion
  first_step("Tissue Digestion");
  first_sub_step();
  // 1. Add 5μL Proteinase K to each mL of Digestion Buffer (final 0.5mg/mL)
  measure_fluid(dig_buffer, sterile_microfuge_tube2);
  measure_prop(sterile_microfuge_tube2, proteinasek, 0.005);
  comment("That is, for each ml of Digestion Buffer, add 5 µl of ProteinaseK.");
  name_sample(sterile_microfuge_tube2, "solution");
  // 2. Homogenise (or simply place) tissue in solution
  next_sub_step();
  homogenize_tissue(sterile_microfuge_tube1, sterile_microfuge_tube2.contents);
  // 3. Incubate at 55°C for 1 hour to overnight
  next_sub_step();
  incubate(sterile_microfuge_tube2, 55, time_range(1, 12, HRS));
  // 4. Mix by vortexing then centrifuge at maximum speed in a benchtop
  // centrifuge for 2 minutes
  next_sub_step();
  vortex(sterile_microfuge_tube2);
  centrifuge_phases_top(sterile_microfuge_tube2,
                        speed(SPEED_MAX, RPM),
                        4,
                        time(2, MINS),
                        sterile_microfuge_tube3);
  // 5. Transfer supernatant into a new tube

  // * Precipitation of Protein and Cell Debris
  next_step("Precipitation of Protein and Cell Debris");
  // 1. Add 1/10 volume of Sodium Acetate 3M pH 5.2 (final 0.3M)
  first_sub_step();
  measure_prop(sterile_microfuge_tube3, naac, 0.1);
  // 2. Invert to mix and incubate at -20°C for ~15 minutes
  next_sub_step();
  invert(sterile_microfuge_tube3);
  incubate(sterile_microfuge_tube3, -20, time(15, MINS));
  // 3. Centrifuge (preferably at 4°C) at maximum speed in a benchtop centrifuge
  // for 20 minutes
  centrifuge_phases_top(sterile_microfuge_tube3,
                        speed(SPEED_MAX, RPM),
                        4,
                        time(20, MINS),
                        sterile_microfuge_tube4);
  comment("Be careful not to transfer any of the white solid (cell debris and "
          "SDS) into the fresh tube.");
  // 4. Transfer supernatant to a new tube

  // * Precipitation of Nucleic Acids
  next_step("Precipitation of Nucleic Acids");
  // 1. Add ~2 volumes of 98% ethanol (final 60-80%)
  first_sub_step();
  measure_prop(sterile_microfuge_tube4, eth98, 2);
  // 2. Invert to mix and incubate at -20°C for ~15 minutes
  next_sub_step();
  invert(sterile_microfuge_tube4);
  incubate(sterile_microfuge_tube4, -20, time(15, MINS));
  // 3. Centrifuge (preferably at 4°C) at maximum speed in a benchtop centrifuge
  // for 20 minutes
  next_sub_step();
  centrifuge_pellet(
    sterile_microfuge_tube4, speed(SPEED_MAX, RPM), 4, time(20, MINS));
  // 4. Wash pellet with 98% ethanol, and once or twice with 70%. Allow to air
  // dry then resuspend in water or 1xTE
  next_sub_step();
  measure_fluid(eth98, vol(1, ML), sterile_microfuge_tube4);
  vortex(sterile_microfuge_tube4);
  centrifuge_pellet(
    sterile_microfuge_tube4, speed(SPEED_MAX, RPM), 4, time(5, MINS));
  measure_fluid(eth70, vol(1, ML), sterile_microfuge_tube4);
  vortex(sterile_microfuge_tube4);
  centrifuge_pellet(
    sterile_microfuge_tube4, speed(SPEED_MAX, RPM), 4, time(5, MINS));
  optional_step();
  measure_fluid(eth70, vol(1, ML), sterile_microfuge_tube4);
  vortex(sterile_microfuge_tube4);
  centrifuge_pellet(
    sterile_microfuge_tube4, speed(SPEED_MAX, RPM), 4, time(5, MINS));

  next_step();
  dry_pellet(sterile_microfuge_tube4, IN_AIR);
  first_option();
  measure_fluid(te, vol(10, UL), sterile_microfuge_tube4);
  next_option();
  measure_fluid(water, vol(10, UL), sterile_microfuge_tube4);
  end_option();
  resuspend(sterile_microfuge_tube4);
  comment("Ensure to dry the pelletted DNA completely before attempting to "
          "resuspend.");

  end_protocol();
}
