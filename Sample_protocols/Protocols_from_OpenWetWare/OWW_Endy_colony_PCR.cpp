#include "biocoder.h"
#define X 60

int
main()
{
  start_protocol("Endy - Colony PCR");

  Fluid buffer = new_fluid("Thermo polymerase buffer");
  Fluid dntps = new_fluid("dNTPS");
  Fluid f_primer = new_fluid("Forward primer");
  Fluid r_primer = new_fluid("Reverse primer");
  Fluid taq = new_fluid("Taq or Vent Polymerase");
  Fluid temp_sus = new_fluid("Template suspension");
  Fluid h2o = new_fluid("H<sub>2</sub>O");

  Container rxn_tube = new_container(RXN_TUBE);

  first_step();
  comment("Use a sterile toothpick or pipet tip to resuspend a plated colony "
          "in 50 μl sterile water.");

  next_step();
  comment("Store the colony resuspension at 4°C so you can start cultures if "
          "necessary (should be OK for a couple days, if you need it to last "
          "longer you should use an Index plate.");
  // Reaction Mix
  //
  // Use the following reaction mix for each PCR:
  //
  //    * 1 μl 10x Thermo polymerase buffer
  //    * 1 μl 10x dNTPs (10x = 2.5 mM each dNTP)
  //    * 0.15 μl 40 μM FWD primer
  //    * 0.15 μl 40 μM REV primer
  //    * 0.1 μl Polymerase (taq or vent)
  //    * 6.6 μl H2O
  //    * 1.0 μl template suspension
  next_step("Reaction mix");
  {
    Fluid fluid_array[7] = { buffer, dntps,    f_primer, r_primer,
                             taq,    temp_sus, h2o };
    char*(
      initial_conc)[7] = { "10X", "10X", "40 µM", "40 µM", "--", "--", "--" };
    char*(final_conc)[7] = { "1X", "1X", "0.6 µM", "0.6 µM", "--", "--", "--" };
    Volume* volume[7] = { vol(1, UL),    vol(1, UL),   vol(0.15, UL),
                          vol(0.15, UL), vol(0.1, UL), vol(6.6, UL),
                          vol(1, UL) };
    mixing_table_pcr(
      8, vol(10, UL), fluid_array, initial_conc, final_conc, volume, rxn_tube);
  }

  //
  // PCR protocol
  //
  //    * 95 C for 6 minutes (disrupt cells, separate DNA)
  //    * Cycle 35 times:
  //          o 95 C for 30 s (melting)
  //          o 53 C (or whatever temperature is appropriate) for 30 s
  //          (annealing) o 72 C for X s (elongation)
  //    * 72 C for 10 minutes (final elongation)
  //    * 4 C forever
  //    * For long amplicons, X = 1 minute + 2.5 s per 100bp
  //    * For shorter amplicons, under ~1kb, this can be shortened judiciously.
  next_step("PCR protocol");
  pcr_init_denat(rxn_tube, 95, time(6, MINS));
  thermocycler(rxn_tube,
               35,
               95,
               time(30, SECS),
               53,
               time(30, SECS),
               72,
               time(X, SECS),
               NORMAL);
  pcr_final_ext(rxn_tube, 72, time(10, MINS), 4);
  comment("For long amplicons, elongation time = 1 minute + 2.5 s per 100bp.");
  comment(
    "For shorter amplicons, under ~1kb, this can be shortened judiciously.");
  end_protocol();
}