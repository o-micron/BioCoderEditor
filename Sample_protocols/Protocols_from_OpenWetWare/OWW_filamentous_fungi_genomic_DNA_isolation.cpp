#include "biocoder.h"

int
main()
{

  start_protocol("Filamentous fungi genomic DNA isolation");

  Fluid lysis_buffer = new_fluid("lysis buffer",
                                 "50 mM Tris-HCL, 50 mM EDTA, 3% SDS, 1% "
                                 "2-mercaptoethanol (add just before use)");
  Fluid mercaptor = new_fluid("B-mercaptor");
  Fluid sevag = new_fluid("SEVAG", "chloroform:isoamyl alcohol, 24:1");
  Fluid naoc = new_fluid("3M NaOAC", "pH 8.0");
  Fluid isoprop = new_fluid("isopropanol");
  Fluid eb = new_fluid("EB");
  Fluid rnase = new_fluid("100 mg/ml RNAse");
  Fluid amm_acetate = new_fluid("7.5 M ammonium acetate");
  Fluid ethanol100 = new_fluid("100% EtOH");
  Fluid ethanol70 = new_fluid("70% EtOH");
  Fluid te = new_fluid("TE buffer", "pH 8.0");

  Solid fungi =
    new_solid("group lyophilized mycelium(wet)", "ground in liquid nitrogen");

  Container sterile_microfuge_tube1 = new_container(STERILE_MICROFUGE_TUBE);
  Container eppendorf1 = new_container(EPPENDORF);
  Container eppendorf2 = new_container(EPPENDORF);
  Container eppendorf3 = new_container(EPPENDORF);

  // 1.  Fill a 1.5 mL eppendorf microcentrifuge tube 2/3 to the joint with
  // group lyophilized mycelium (60-100 mg dry, or 0.5-1.0 g wet, ground in in
  // liquid nitrogen)
  //        * Alternatively you can grind 1 g of dried (vacuum filter mycelium
  //        first) in a mortar and pestle treating with liquid nitrogen 5-6
  //        times. our the frozen powder into the eppendorf tube.
  //        * Note: you only want to process about 1g in each eppendorf tube, if
  //        there is more than this, split to two separate tubes.
  first_step();
  measure_solid(fungi, 1, GR, sterile_microfuge_tube1);
  comment("If using dry fungi, use 60- 100 mg. Alternatively you can grind 1 g "
          "of dried (vacuum filter mycelium first) in a mortar and pestle "
          "treating with liquid nitrogen 5-6 times. Pour the frozen powder "
          "into the eppendorf tube.");
  comment("Note: you only want to process about 1g in each eppendorf tube, if "
          "there is more than this, split to two separate tubes.");

  // 2. Add 660-750μL + 10 μL Β-mercaptor.
  //        * Vortex to insure good mixing of solution, incubate in 65 °C water
  //        bath for 1hr.
  next_step();
  measure_fluid(lysis_buffer, vol_range(660, 750, UL), sterile_microfuge_tube1);
  measure_fluid(mercaptor, vol(10, UL), sterile_microfuge_tube1);
  vortex(sterile_microfuge_tube1);
  incubate(sterile_microfuge_tube1, 65, time(1, HRS));
  comment("Use a water bath for incubation.");

  // 3. Spin down to remove cell debris. 5min at 3400 RPM. Transfer aqueous
  // phase (top) to 1.mL eppendorf. Do not take any of the cellular debris from
  // the interface. Don't be greedy this prep extracts a lot of DNA.

  next_step();
  centrifuge_phases_top(
    sterile_microfuge_tube1, speed(3400, RPM), RT, time(5, MINS), eppendorf1);
  comment("Do not take any of the cellular debris from the interface. Don't be "
          "greedy this prep extracts a lot of DNA.");

  // 4. Add 700 μL of SEVAG; (adjust volume if needed to meet a 1:1 ratio of
  // SEVAGE and aqueous phase) and vortex. Microcentrofuge at 12,000 g for 10
  // min.
  //        * Note: some people top off with EB buffer so that total volume is
  //        so that volume is equal among the samples for spinning. Be careful
  //        as sometimes tops are loosened by chloroform.
  next_step();
  measure_fluid(sevag, vol(700, UL), eppendorf1);
  comment(
    "Adjust volume if needed to meet a 1:1 ratio of SEVAG and aqueous phase.");
  vortex(eppendorf1);
  comment("Note: some people top off with EB buffer so that total volume is so "
          "that volume is equal among the samples for spinning. Be careful as "
          "sometimes tops are loosened by chloroform.");
  centrifuge_phases_top(eppendorf1,
                        speed(12000, G),
                        RT,
                        time(10, MINS),
                        vol_range(550, 600, UL),
                        eppendorf2);

  // 5. Transfer aqueous phase (top) to new tube.
  //        * Note: these last two steps (SEVAG, spin, transfer) can be repeated
  //        to insure cleaner DNA prep depending on your needs and pipette
  //        techniques.
  next_step();
  comment(
    "Note: these last two steps (SEVAG, spin, transfer) can be repeated to "
    "insure cleaner DNA prep depending on your needs and pipette techniques.");

  // 6. Remove aqueous phase (top) to a new tube (approx. 550-600 μL). Add 20 μL
  // of 3 M NaOAc. Top off with isopropanol. Invert gently. You should see DNA
  // "ropes" precipitating.
  next_step();
  measure_fluid(naoc, vol(20, UL), eppendorf2);
  measure_fluid(isoprop, eppendorf2);
  comment("Top off with isopropanol.");
  invert(eppendorf2);
  comment("You should see DNA \"ropes\" precipitating.");

  // 7. Microcentrifuge for 2 min. Pour off supernatent (top layer). Invert tube
  // for 1 min to dry.
  next_step();
  centrifuge_pellet(eppendorf2, speed(SPEED_MAX, RPM), RT, time(2, MINS));
  invert_dry(eppendorf2, RT, time(1, MINS));

  // 8. Add 300 μL EB (or ddH20) and 1 μL of 100 mg/mL RNAse and place in 65°C
  // for 10-15 min. Finger vortex (you don't want to shear this nice long DNA
  // now do you?).
  next_step();
  measure_fluid(eb, vol(300, UL), eppendorf2);
  measure_fluid(rnase, vol(1, UL), eppendorf2);
  store_for(eppendorf2, 65, time_range(10, 15, MINS));
  vortex(eppendorf2);
  comment("Finger vortex the tube to prevent shearing of DNA.");

  // 9. (Optional) further cleanup with the PEG DNA protocol OR
  optional_step();
  to_do("Further cleanup with the PEG DNA protocol.");

  // 10. 250 μL 7.5M Ammonium acetate. Spin at max speed 5min to pellet protein
  // debris. Take supernatent out, add 750 isopropanol.
  next_step();
  measure_fluid(amm_acetate, vol(250, UL), eppendorf2);
  centrifuge_phases_top(
    eppendorf2, speed(SPEED_MAX, RPM), RT, time(5, MINS), eppendorf3);
  comment("This pellets the protein debris.");
  measure_fluid(isoprop, vol(750, UL), eppendorf3);

  // 11. Microcentrifuge 30s to 2 min to pellet the DNA. Pour off the
  // supernatent and rinse the pellet with 100% EtOH. Wash again with 70% EtOH.
  next_step();
  centrifuge_pellet(
    eppendorf3, speed(SPEED_MAX, RPM), RT, time_range(30, 120, SECS));
  measure_fluid(ethanol100, vol(1, ML), eppendorf3);
  vortex(eppendorf3);
  centrifuge_pellet(
    eppendorf3, speed(SPEED_MAX, RPM), RT, time_range(30, 120, SECS));
  measure_fluid(ethanol70, vol(1, ML), eppendorf3);
  vortex(eppendorf3);
  centrifuge_pellet(
    eppendorf3, speed(SPEED_MAX, RPM), RT, time_range(30, 120, SECS));

  // 12. Dry tubes in vacuum oven at 50°C for 15 min at most or dry in speed
  // vac.
  next_step();
  dry_pellet(eppendorf3, IN_VACUUM, max_time(15, MINS));
  end_option();

  // 13. Resuspend in pH 8 TE buffer.
  next_step();
  measure_fluid(te, eppendorf3);
  resuspend(eppendorf3);

  end_protocol();
}