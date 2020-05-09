#include "biocoder.h"

int
main()
{
  start_protocol("Size specific DNA precipitation");

  Fluid dna =
    new_fluid("DNA sample", "DNA to be separated (e.g. PCR reaction mixture)");
  Fluid peg = new_fluid(
    "PEG/MgCl2",
    "30% (w/v) PEG 8000/30 mM MgCl2 (concentration of PEG 8000 can be varied "
    "to shift the size of the percipitated DNA. The concentration used here "
    "will remove DNA fragments with less than 300bp)");
  Fluid te = new_fluid("TE buffer", "10 mM TRIS-HCl, 1 mM EDTA, pH 8.0");
  Fluid buffer = new_fluid("buffer of choice");

  Container eppendorf = new_container(EPPENDORF);

  //* Mix 50 μL of sample with 150 µL of TE
  first_step();
  measure_fluid(dna, vol(50, UL), eppendorf);
  measure_fluid(te, vol(150, UL), eppendorf);
  tap(eppendorf);

  //* Add 100 µL of PEG/MgCl2
  next_step();
  measure_fluid(peg, vol(10, UL), eppendorf);

  //* Vortex
  next_step();
  vortex(eppendorf);

  //* Centrifuge 15 min at 10.000 rcf at roomtemperature
  next_step();
  centrifuge_pellet(eppendorf, speed(10000, G), RT, time(15, MINS));

  //* Carefully remove supernatant not to disturb the pellet, which will be
  //invisible
  next_step();
  comment("Carefully remove supernatant not to disturb the pellet, which will "
          "be invisible.");

  //* Dissolve the pellet in a appropriate amount of buffer of choice
  next_step();
  measure_fluid(buffer, eppendorf);
  comment("Add appropriate volume of buffer.");
  dissolve(eppendorf);

  end_protocol();
}