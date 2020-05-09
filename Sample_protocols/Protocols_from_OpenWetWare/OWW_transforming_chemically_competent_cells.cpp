#include "biocoder.h"

int
main()
{
  start_protocol("Transforming chemically competent cells");

  Fluid tss = new_fluid("TSS cells");
  Fluid dna = new_fluid("DNA");
  Fluid soc = new_fluid("SOC", RT);
  Plate plate = new_plate("plate made with appropriate antibiotic");

  Container tube1 = new_container(STERILE_MICROFUGE_TUBE, tss);

  // 1. Thaw TSS cells on ice.
  first_step();
  store_until(tube1, ON_ICE, THAW_ICE);

  // 2. Add DNA, pipette gently to mix (1μl of prepped plasmid is more than
  // enough).
  //        * Note: If you are adding small volumes (~1μl), be careful to mix
  //        the culture well. Diluting the plasmid back into a larger volume can
  //        also help.
  next_step();
  measure_fluid(dna, tube1);
  comment("(1µl of prepped plasmid is more than enough)");
  pipet(tube1);
  comment("Note: If you are adding small volumes (~1µl), be careful to mix the "
          "culture well. Diluting the plasmid back into a larger volume can "
          "also help.");

  // 3. Let sit for 30 minutes on ice.
  //        * Note: If you are in a rush, you can shorten this incubation time
  //        to 5-10 min.
  next_step();
  store_for(tube1, ON_ICE, time(30, MINS));
  comment("Note: If you are in a rush, you can shorten this incubation time to "
          "5-10 min.");

  // 4. Incubate cells for 30 seconds at 42oC.
  //        * Note: According to the original TSS paper and qualitative
  //        experience (JM), this step is completely optional and may actually
  //        reduce transformation efficiency.
  next_step();
  incubate(tube1, 42, time(30, SECS));
  comment("Note: According to the original TSS paper and qualitative "
          "experience (JM), this step is completely optional and may actually "
          "reduce transformation efficiency.");

  // 5. Incubate cells on ice for 2 min.
  next_step();
  incubate(tube1, ON_ICE, time(2, MINS));

  // 6. Add 1 mL SOC (2XYT and LB are also suitable, original paper suggests LB
  // + 20mM glucose) at room temp.
  next_step();
  measure_fluid(soc, vol(1, ML), tube1);
  comment("(2XYT and LB are also suitable, original paper suggests LB + 20mM "
          "glucose)");

  // 7. Incubate for 1 hour at 37oC on shaker.
  //        * Note: Can also save some time here by reducing incubation to ~45
  //        min.
  next_step();
  incubate(tube1, 37, time(1, HRS), 200);
  comment(
    "Note: Can also save some time here by reducing incubation to ~45 min.");

  // 8. Spread 100-300 μl onto a plate made with appropriate antibiotic.
  next_step();
  plate_out(plate, tube1, vol_range(100, 300, UL));

  // 9. Grow overnight at 37 °C.
  next_step();
  incubate_plate(plate, 37, time(12, HRS));

  // 10. Save the rest of the transformants in liquid culture at 4 °C. If
  // nothing appears on your plate, you can spin this down, resuspend in enough
  // medium to spread on one plate and plate it all. This way you will find even
  // small numbers of transformants.
  next_step();
  comment("Save the rest of the transformants in liquid culture at 4°C. If "
          "nothing appears on your plate, you can spin this down, resuspend in "
          "enough medium to spread on one plate and plate it all. This way you "
          "will find even small numbers of transformants.");

  end_protocol();
}