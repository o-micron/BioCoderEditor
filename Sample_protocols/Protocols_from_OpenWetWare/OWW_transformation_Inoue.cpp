#include "biocoder.h"

int
main()
{
  start_protocol("Transformation of chemically competent cells - Inoue");

  Fluid cells = new_fluid("TB buffer cells");
  Fluid dna = new_fluid("DNA", vol(20, UL));
  Fluid soc = new_fluid("SOC", RT);
  Plate plate = new_plate("plate made with appropriate antibiotic");

  Container tube1 = new_container(STERILE_MICROFUGE_TUBE);

  // 1. Thaw 25 - 200 μl TB buffer cells on ice. Do not use glass tubes, which
  // adsorb DNA.
  first_step();
  measure_fluid(cells, vol_range(25, 200, UL), tube1);
  store_until(tube1, ON_ICE, THAW_ICE);
  comment("Do not use glass tubes which adsorb DNA.");

  // 2. Add DNA, pipette gently to mix (keep volume of DNA less than 5% of the
  // cell volume)
  next_step();
  measure_fluid(dna, tube1);
  pipet(tube1);
  comment("Keep volume of DNA less than 5% of the cell volume.");

  // 3. Incubate on ice for 30 minutes
  //       * Note: If you are in a rush, you can shorten this incubation time to
  //       5-10 min.
  next_step();
  incubate(tube1, ON_ICE, time(30, MINS));
  comment("Note: If you are in a rush, you can shorten this incubation time to "
          "5-10 min.");

  // 4. Incubate cells for 30 seconds at 42oC.
  next_step();
  incubate(tube1, 42, time(30, SECS));

  // 5. Incubate cells on ice for 2 min.
  next_step();
  incubate(tube1, ON_ICE, time(2, MINS));

  // 6. Add 4 volumes of room temperature SOC (not critical)
  next_step();
  measure_prop(tube1, soc, 4);
  comment("(not critical)");

  // 7. Incubate for 1 hour at 37oC on shaker.
  //        * Note: Can also save some time here by reducing incubation to ~45
  //        min.
  //        * Note: Step can be eliminated if plating on Amp plates, but not
  //        most other antibiotics
  next_step();
  incubate(tube1, 37, time(1, HRS), 200);
  comment(
    "Note: Can also save some time here by reducing incubation to ~45 min.");
  comment("Note: Step can be eliminated if plating on Amp plates, but not most "
          "other antibiotics.");

  // 8. Spread 100-300 μl onto a plate made with appropriate antibiotic.
  next_step();
  plate_out(plate, tube1, vol_range(100, 300, UL));

  // 9. Grow overnight at 37 °C.
  next_step();
  incubate_plate(plate, 37, time(12, HRS));

  end_protocol();
}