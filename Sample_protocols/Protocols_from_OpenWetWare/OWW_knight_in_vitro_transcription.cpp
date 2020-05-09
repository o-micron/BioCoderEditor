#include "biocoder.h"

int
main()
{
  start_protocol("Knight- Invitro Transcription");

  Fluid repressor = new_fluid("repressor");
  Fluid pcr_template = new_fluid("PCR template");
  Fluid trans_buffer =
    new_fluid("5X E. coli RNA polymerase transcription buffer");
  Fluid tcep = new_fluid("TCEP");
  Fluid ntp = new_fluid("2.5 mM each NTP");
  Fluid water = new_fluid("RNase-free water");
  Fluid holoenzyme = new_fluid("E. coli RNA polymerase holoenzyme");
  Fluid dnase_buffer = new_fluid("DNaseI buffer");
  Fluid dnase = new_fluid("DNase");

  Container rxn_tube1 = new_container(RXN_TUBE);
  Container rxn_tube2 = new_container(RXN_TUBE);
  // Prepare template DNA
  //
  //   1. Generate linearized template via PCR. Do a 100 μL reaction using VF2
  //   and VR.
  //          * Can be done once, frozen and reused.
  first_step("Prepare template DNA");
  first_sub_step();
  to_do("Generate linearized template via PCR. Do a 100 μL reaction using VF2 "
        "and VR.");
  comment("Can be done once, frozen and reused.");

  // Option 1: Preincubate repressor and DNA
  //
  //   1. Mix
  //          * 20 μL repressor
  //          * 2 μL of PCR template
  //                o Do the same for relevant controls.
  //   2. Incubate 2 hours on benchtop.
  //   3. Make up 50 μL reaction
  //          * 22 μL repressor-DNA mixture
  //          * 10 μL 5X E. coli RNA polymerase transcription buffer
  //          * 0.5 μL of 500 mM TCEP since DTT chelates zinc
  //          * 10 μL of 2.5 mM each NTP
  //          * 5 μL RNase free H2O
  //          * 2.5 μL E. coli RNA polymerase holoenzyme
  //   4. Incubate at 37°C for 1 hr.
  next_step();
  first_option("Preincubate repressor and DNA");

  first_sub_step();
  measure_fluid(repressor, vol(20, UL), rxn_tube1);
  measure_fluid(pcr_template, vol(2, UL), rxn_tube1);
  comment("Do the same for relevant controls.");

  next_sub_step();
  incubate(rxn_tube1, RT, time(2, HRS));

  next_sub_step();
  name_sample(rxn_tube1, "repressor-DNA mixture");
  {
    Fluid fluid_array[6] = { rxn_tube1.contents, trans_buffer, tcep, ntp,
                             holoenzyme,         water };
    char* tube[1] = { "Reaction" };
    Volume* volumes[6] = { vol(22, UL), vol(10, UL),  vol(0.5, UL),
                           vol(10, UL), vol(2.5, UL), vol(5, UL) };
    mixing_table(2, 7, fluid_array, tube, volumes, vol(50, UL), rxn_tube2);
  }

  next_sub_step();
  incubate(rxn_tube2, 37, time(1, HRS));

  // Option 2: Set up transcription reaction
  //
  //   1. Make up 50 μL reaction
  //          * 25 μL RNase free H2O
  //          * 10 μL 5X E. coli RNA polymerase transcription buffer
  //          * 0.5 μL of 500 mM TCEP (since DTT chelates zinc)
  //          * 10 μL of 2.5 mM each NTP
  //          * 2 μL of PCR template <-perhaps cut this down? DNA is a pretty
  //          bright band?
  //          * 2.5 μL E. coli RNA polymerase holoenzyme
  //   2. Incubate at 37°C for 1 hr.
  next_option("Set up transcription reaction");

  first_sub_step();
  {
    Fluid fluid_array[6] = { water, trans_buffer, tcep,
                             ntp,   pcr_template, holoenzyme };
    char* tubes[1] = { "Reaction" };
    Volume* volumes[6] = { vol(25, UL), vol(10, UL), vol(0.5, UL),
                           vol(10, UL), vol(2, UL),  vol(2.5, UL) };
    mixing_table(2, 7, fluid_array, tubes, volumes, vol(50, UL), rxn_tube2);
  }

  next_sub_step();
  incubate(rxn_tube2, 37, time(1, HRS));

  end_option();
  // DNase treatment (optional)
  //
  // This step hasn't been tried.
  //
  // An optional step is to treat the reaction with RNase free DNaseI to remove
  // the template DNA.
  //
  //   1. Add 6 μL DNaseI buffer
  //   2. Add 3 μL H2O
  //   3. Add 1μL DNaseI
  //   4. Incubate 1 hr at 37°C
  //   5. Heat inactivate for 10 mins at 75°C
  optional_step("DNase treatment");
  comment("This step hasn't been tried.");
  comment("An optional step is to treat the reaction with RNase free DNaseI to "
          "remove the template DNA.");
  measure_fluid(dnase_buffer, vol(6, UL), rxn_tube2);
  measure_fluid(water, vol(3, UL), rxn_tube2);
  measure_fluid(dnase, vol(1, UL), rxn_tube2);
  incubate(rxn_tube2, 37, time(1, HRS));
  store_for(rxn_tube2, 75, time(10, MINS));
  comment("This is to heat-inactivate the enzyme.");

  end_protocol();
}