#include "biocoder.h"

int
main()
{
  start_protocol("DNA Extraction from Tissue");
  Fluid extraction_buffer = new_fluid(
    "DNA extraction buffer", "98µl ReagentB + 2µl ProteinaseK, mix fresh");
  Fluid chloro_iaa = new_fluid("24:1 Chloroform:Isoamyl alcohol");
  Fluid pci_mix =
    new_fluid("PCI mix",
              "One part tris-saturated phenol to one part 24:1 "
              "Chloroform:Isoamyl alcohol, shake thoroughly to make emulsion");
  Fluid etoh100 = new_fluid("100% EtOH");
  Fluid naac = new_fluid("3M sodium acetate pH 5.0");
  Fluid etoh70 = new_fluid("70% EtOH");
  Fluid water = new_fluid("MilliQ water");

  Solid tissue = new_solid("small piece of tissue or embryo");

  Container microfuge_tube1 = new_container(STERILE_MICROFUGE_TUBE);
  Container microfuge_tube2 = new_container(STERILE_MICROFUGE_TUBE);
  Container microfuge_tube3 = new_container(STERILE_MICROFUGE_TUBE);

  // Proteinase K digestion
  first_step("Proteinase K digestion");
  //   1. Mix DNA extraction buffer
  //          * 98 μl ReagentB
  //          * 2 μl ProteinaseK
  //          * Mix fresh. 100 μl is enough for a small pea size chunk of tissue
  //          or one embryo
  //   2. Place small piece of tissue or embryo into a microfuge tube containing
  //   100 μl of extraction buffer
  first_sub_step();
  measure_fluid(extraction_buffer, vol(100, UL), microfuge_tube1);
  comment(
    "100 µl is enough for a small pea size chunk of tissue or one embryo.");

  next_sub_step();
  measure_solid(tissue, microfuge_tube1);

  //   3. Incubate at 50°C overnight
  next_sub_step();
  incubate(microfuge_tube1, 50, time(12, HRS));

  // Phenol/chloroform/isoamyl (PCI)
  next_step("Phenol/chloroform/isoamyl (PCI)");
  //   1. Prepare PCI mix
  //         1. One part tris-saturated phenol to one part 24:1
  //         Chloroform:Isoamyl alcohol
  //         2. Shake thoroughly to make emulsion
  //   2. Add one volume of PCI to extracted sample
  first_sub_step();
  measure_prop(microfuge_tube1, pci_mix, 1);

  //   3. Shake tubes for 10 seconds
  next_sub_step();
  vortex(microfuge_tube1, time(10, SECS));

  //   4. Centrifuge at max speed for 5 minutes
  //   5. Remove aqueous phase to a new tube
  //   6. Repeat as needed
  next_sub_step();
  centrifuge_phases_top(
    microfuge_tube1, speed(SPEED_MAX, RPM), RT, time(5, MINS), microfuge_tube2);
  to_do("Repeat this step as needed.");

  //   7. Add one volume 24:1 chloroform:isoamyl alcohol
  next_sub_step();
  measure_prop(microfuge_tube2, chloro_iaa, 1);

  //   8. Shake tubes for 10 seconds
  next_sub_step();
  vortex(microfuge_tube2, time(10, SECS));

  //   9. Centrifuge at max speed for 5 minutes
  //  10. Remove aqueous phase to a new tube
  //  11. Continue to precipitation
  next_sub_step();
  centrifuge_phases_top(
    microfuge_tube2, speed(SPEED_MAX, RPM), RT, time(5, MINS), microfuge_tube3);

  // Ethanol precipitation
  next_step("Ethanol precipitation");
  //   1. Add 2 volumes 100% EtOH
  first_sub_step();
  measure_prop(microfuge_tube3, etoh100, 2);

  //   2. Add 1/10 volume 3M Sodium Acetate pH 5.0
  next_sub_step();
  measure_prop(microfuge_tube3, naac, 0.1);

  //   3. Centrifuge at max speed for 10 minutes
  //   4. Decant ethanol
  next_sub_step();
  centrifuge_pellet(microfuge_tube3, speed(SPEED_MAX, RPM), RT, time(10, MINS));

  //   5. Add 150 μl 70% EtOH
  next_sub_step();
  measure_fluid(etoh70, vol(150, UL), microfuge_tube3);

  //   6. Centrifuge at max speed for 2 minutes
  //   7. Pipette out ethanol
  next_sub_step();
  centrifuge_pellet(microfuge_tube3, speed(SPEED_MAX, RPM), RT, time(2, MINS));

  //   8. Airdry pellet
  next_sub_step();
  dry_pellet(microfuge_tube3, IN_AIR);

  //   9. Resuspend pellet in MilliQ water
  next_sub_step();
  measure_fluid(water, microfuge_tube3);
  resuspend(microfuge_tube3);

  end_protocol();
}