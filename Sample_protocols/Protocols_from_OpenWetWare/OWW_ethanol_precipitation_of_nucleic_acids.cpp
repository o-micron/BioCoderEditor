#include "biocoder.h"

int
main()
{
  start_protocol("Ethanol Precipitation of Nucleic Acids");

  Fluid nucleic_acid = new_fluid("sample", vol(20, UL));
  Fluid ethanol = new_fluid("100% ethanol");
  Fluid naac = new_fluid("3M sodium acetate, pH 5.2");
  Fluid water = new_fluid("water");

  Container tube1 = new_container(STERILE_MICROFUGE_TUBE, nucleic_acid);

  // 1. Add the following to your sample:
  //       * 2-3 volumes of 100% Ethanol
  //       * 1/10 volume of 3M sodium acetate, pH 5.2
  first_step();
  first_option();
  measure_prop(tube1, ethanol, 2);
  next_option();
  measure_prop(tube1, ethanol, 3);
  end_option();
  measure_prop(tube1, naac, 0.1);

  // 2. Mix and freeze overnight in -20. This step some say is unnecessary but
  // others swear by it. If you are in a rush you can also put it in the -80 for
  // ten minutes to a few hours. Dry ice for 10-15 minutes also works.--Heather
  //       * In general, the time you need to incubate in the freezer depends on
  //       how much nucleic acid you have, how big it is and the volume it is
  //       in. My general protocol is to freeze for 20 min to 1 hr at -80 ˚C.
  //       This seems to work well for most things, but you may want to freeze
  //       longer if you have only a small concentration of nucleic acid or if
  //       it is small in size(<15 nucleotides).--Kathleen
  //       * If you are in a hurry, you can also dip you epi shortly into liquid
  //       nitrogen. If you added enough ethanol, the mix won't freeze. Careful
  //       with isopropanol - it freezes more quickly. This works well for me
  //       and saves me a lengthy incubation in the fridge. --Jasu
  next_step();
  pipet(tube1);
  store_for(tube1, -20, time(12, HRS));
  comment("--This step some say is unnecessary but others swear by it. If you "
          "are in a rush you can also put it in the -80 for ten minutes to a "
          "few hours. Dry ice for 10-15 minutes also works.");
  comment("--In general, the time you need to incubate in the freezer depends "
          "on how much nucleic acid you have, how big it is and the volume it "
          "is in. The general protocol is to freeze for 20 min to 1 hr at -80 "
          "°C. This seems to work well for most things, but you may want to "
          "freeze longer if you have only a small concentration of nucleic "
          "acid or if it is small in size(<15 nucleotides).");
  comment("--If you are in a hurry, you can also dip you epi shortly into "
          "liquid nitrogen. If you added enough ethanol, the mix won't freeze. "
          "Careful with isopropanol - it freezes more quickly. This works well "
          "and saves a lengthy incubation in the fridge.");

  // 3. Spin at full speed in a standard microcentrifuge at 4 degrees for 30
  // minutes. Make sure to mark the outermost edge of the tube so you can find
  // the pellet easily (or just put the hinge portion of the tube to the
  // outside). It is clear and usually looks like a little smudge on the tube.
  // 4. Decant (or carefully pipet off) the supernatant.
  next_step();
  centrifuge_pellet(tube1, speed(SPEED_MAX, RPM), 4, time(30, MINS));
  comment(
    "--Make sure to mark the outermost edge of the tube so you can find the "
    "pellet easily (or just put the hinge portion of the tube to the outside). "
    "It is clear and usually looks like a little smudge on the tube.");

  // 5. Dry the pellet. For this you can air dry (tubes open, ~15 min) or dry in
  // a speedvac. DNA and RNA (if you don't have RNases in your sample) are
  // typically hearty enough for you to air dry at 37 ˚C, if desired.
  //       * Overdrying can make DNA hard to re-dissolve. Especially for longer
  //       DNA, I avoid vacuum drying and airdry only briefly before
  //       re-dissolving. --Jasu
  next_step();
  first_option();
  dry_pellet(tube1, IN_AIR, approx_time(15, MINS));
  next_option();
  dry_pellet(tube1, IN_VACUUM, approx_time(15, MINS));
  end_option();
  comment("--DNA and RNA (if you don't have RNases in your sample) are "
          "typically hearty enough for you to air dry at 37 ˚C, if desired.");
  comment("--Overdrying can make DNA hard to re-dissolve.");

  // 6. Add your desired quantity of water. Vortex and spin down to resuspend.
  //       * Beware of using water unless you are sure of what you are getting
  //       in to. The "pH" of water can vary widely (I've seen from pH 5 to
  //       pH 8.5), and depurination of DNA at low pH or degradation of RNA at
  //       high pH are possibilities. Water also typically contains trace
  //       metals, which can accelerate these reactions. I typically recommend
  //       resuspension in TE (10 mM Tris-HCl, pH 7.5, 1 mM EDTA). This makes
  //       sure your nucleic acid is at a neutral pH and the EDTA will chelate
  //       any trace metals. Since they are in such small amounts, neither the
  //       buffer nor the EDTA will affect most downstream reactions.--Kathleen
  next_step();
  measure_fluid(water, tube1);
  resuspend(tube1);
  comment("--Beware of using water unless you are sure of what you are getting "
          "in to. The pH of water can vary widely (I've seen from pH 5 to pH "
          "8.5), and depurination of DNA at low pH or degradation of RNA at "
          "high pH are possibilities. Water also typically contains trace "
          "metals, which can accelerate these reactions. I typically recommend "
          "resuspension in TE (10 mM Tris-HCl, pH 7.5, 1 mM EDTA). This makes "
          "sure your nucleic acid is at a neutral pH and the EDTA will chelate "
          "any trace metals. Since they are in such small amounts, neither the "
          "buffer nor the EDTA will affect most downstream reactions.");

  end_protocol();
}