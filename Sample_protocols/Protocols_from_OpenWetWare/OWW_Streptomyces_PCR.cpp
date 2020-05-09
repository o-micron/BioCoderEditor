#include "biocoder.h"

int
main()
{
  start_protocol("PCR");

  Fluid buffer = new_fluid("Buffer");
  Fluid dntp = new_fluid("dNTPs");
  Fluid mgcl2 = new_fluid("MgCl <sub>2</sub>");
  Fluid dmso = new_fluid("DMSO");
  Fluid f_primer = new_fluid("Upstream primer");
  Fluid r_primer = new_fluid("Downstream primer");
  Fluid dna = new_fluid("Template DNA");
  Fluid water = new_fluid("dH<sub>2</sub>O");
  Fluid dnapol = new_fluid("DNA Polymerase");

  Container rxn_tube = new_container(RXN_TUBE);

  first_step("For Promega GoTaq:");
  {
    Fluid fluid_array[9] = { buffer,   dntp, mgcl2, dmso,  f_primer,
                             r_primer, dna,  water, dnapol };
    char* initial_conc[9] = { "5X",     "1.25mM", "25mM", "50%",  "50pmol",
                              "50pmol", "??",     "N/A",  "5u/μL" };
    char* final_conc[9] = { "1X",    "0.25mM", "2.5mM", "5%",   "1pmol",
                            "1pmol", "~0.5µg", "N/A",   "1.25u" };
    Volume* volumes[9] = { vol(10, UL), vol(10, UL),    vol(5, UL),
                           vol(5, UL),  vol(1, UL),     vol(1, UL),
                           vol(1, UL),  vol(16.75, UL), vol(0.25, UL) };
    mixing_table_pcr(10,
                     vol(50, UL),
                     fluid_array,
                     initial_conc,
                     final_conc,
                     volumes,
                     rxn_tube);
  }

  next_step("For Roche High Fidelity:");
  {
    Fluid fluid_array[8] = { buffer,   dntp, dmso,  f_primer,
                             r_primer, dna,  water, dnapol };
    char* initial_conc[8] = { "10X",    "1.25mM", "50%", "50pmol",
                              "50pmol", "??",     "N/A", "5u/μL" };
    char* final_conc[8] = { "1X",    "0.25mM", "5%",  "1pmol",
                            "1pmol", "~0.5µg", "N/A", "2.5u" };
    Volume* volumes[8] = {
      vol(5, UL), vol(10, UL), vol(5, UL),     vol(1, UL),
      vol(1, UL), vol(1, UL),  vol(16.75, UL), vol(0.5, UL)
    };
    mixing_table_pcr(
      9, vol(50, UL), fluid_array, initial_conc, final_conc, volumes, rxn_tube);
  }

  next_step("An example program:");
  pcr_init_denat(rxn_tube, 96, time(5, MINS));
  comment("This denatures any double stranded DNA.");
  thermocycler(rxn_tube,
               25,
               96,
               time(1, MINS),
               55,
               time(30, SECS),
               72,
               time(1, MINS),
               NORMAL);
  pcr_final_ext(rxn_tube, 72, time(5, MINS), 4);
  comment("Final extension for incomplete strands and holding of sample at low "
          "temperature.");

  end_protocol();
}