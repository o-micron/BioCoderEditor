#include "biocoder.h"

int
main()
{
  start_protocol("Qiagen QIAprep Spin Miniprep Kit Using a Microcentrifuge");

  Fluid bacteria = new_fluid("bacterial culture");
  Fluid p1 = new_fluid("Buffer P1",
                       "50 mM Tris-HCl pH 8.0, 10 mM EDTA, 100 µg/ml RNaseA",
                       ICE_COLD);
  Fluid p2 = new_fluid("Buffer P2", "200 mM NaOH, 1% SDS");
  Fluid n3 =
    new_fluid("Buffer N3", "4.2 M Gu-HCl, 0.9 M potassium acetate, pH 4.8");
  Fluid pb = new_fluid("Buffer PB", "5 M Gu-HCl, 30% ethanol");
  Fluid pe = new_fluid("Buffer PE", "10 mM Tris-HCl pH 7.5, 80% ethanol");
  Fluid eb = new_fluid("Buffer EB", "10 mM Tris·Cl, pH 8.5");

  Container flask = new_container(FLASK, bacteria);
  Container microcentrifuge_tube1 = new_container(STERILE_MICROFUGE_TUBE);
  Container microcentrifuge_tube2 = new_container(STERILE_MICROFUGE_TUBE);

  Column qiaprep = new_column("QIAprep spin column");

  // 1. Resuspend pelleted bacterial cells in 250 µl Buffer P1 (kept at 4 °C)
  // and transfer to a microcentrifuge tube.
  first_step();
  measure_fluid(flask, vol(1.5, ML), microcentrifuge_tube1);
  centrifuge_pellet(
    microcentrifuge_tube1, speed(SPEED_MAX, RPM), RT, time(1, MINS));
  measure_fluid(p1, vol(250, UL), microcentrifuge_tube1);
  resuspend(microcentrifuge_tube1);
  comment("Ensure that RNase A has been added to Buffer P1. No cell clumps "
          "should be visible after resuspension of the pellet.");

  // 2. Add 250 μl Buffer P2 and gently invert the tube 4–6 times to mix.
  next_step();
  measure_fluid(p2, vol(250, UL), microcentrifuge_tube1);
  invert(microcentrifuge_tube1, 4, 6);
  comment("Mix gently by inverting the tube. Do not vortex, as this will "
          "result in shearing of genomic DNA. If necessary, continue inverting "
          "the tube until the solution becomes viscous and slightly clear.");
  time_constraint(microcentrifuge_tube1, time(5, MINS), NEXTSTEP);

  // 3. Add 350 μl Buffer N3 and invert the tube immediately but gently 4–6
  // times.
  next_step();
  measure_fluid(n3, vol(250, UL), microcentrifuge_tube1);
  time_constraint(microcentrifuge_tube1, time(0, SECS), NEXTSTEP);

  next_step();
  invert(microcentrifuge_tube1, 4, 6);
  comment("To avoid localized precipitation, mix the solution gently but "
          "thoroughly, immediately after addition of Buffer N3. The solution "
          "should become cloudy.");

  // 4. Centrifuge for 10 min at 13,000 rpm (~17,900 x g) in a table-top
  // microcentrifuge.
  // 5. Apply the supernatants from step 4 to the QIAprep spin column by
  // decanting or pipetting.
  next_step();
  centrifuge_phases_top(
    microcentrifuge_tube1, speed(13000, RPM), RT, time(10, MINS), qiaprep);
  comment("A compact white pellet will form the bottom layer.");

  // 6. Centrifuge for 30–60 s. Discard the flow-through.
  centrifuge_column(qiaprep, RT, time_range(30, 60, SECS));
  comment("Centrifuging for 60 seconds produces good results.");

  // 7. (Optional): Wash the QIAprep spin column by adding 0.5 ml Buffer PB and
  // centrifuging for 30–60 s. Discard the flow-through.
  optional_step();
  add_to_column(qiaprep, pb, vol(0.5, ML));
  centrifuge_column(qiaprep, RT, time_range(30, 60, SECS));
  comment(
    "This step is necessary to remove trace nuclease activity when using endA+ "
    "strains such as the JM series, HB101 and its derivatives, or any "
    "wild-type strain, which have high levels of nuclease activity or high "
    "carbohydrate content. Host strains such as XL-1 Blue and DH5α™ do not "
    "require this additional wash step. Although they call this step optional, "
    "it does not really hurt your yield and you may think you are working with "
    "an endA- strain when in reality you are not. Again for this step, "
    "spinning for 60 seconds produces good results.");

  // 8. Wash QIAprep spin column by adding 0.75 ml Buffer PE and centrifuging
  // for 30–60 s.
  next_step();
  add_to_column(qiaprep, pe, vol(0.75, ML));
  centrifuge_column(qiaprep, RT, time_range(30, 60, SECS));
  comment("Centrifuging for 60 seconds produces good results.");

  // 9. Discard the flow-through, and centrifuge for an additional 1 min to
  // remove residual wash buffer.
  next_step();
  centrifuge_column(qiaprep, RT, time(1, MINS));
  comment("This is to remove the residual wash bufer.");
  comment("IMPORTANT: Residual wash buffer will not be completely removed "
          "unless the flow-through is discarded before this additional "
          "centrifugation. Residual ethanol from Buffer PE may inhibit "
          "subsequent enzymatic reactions. They are right about this.");

  // 10. Place the QIAprep column in a clean 1.5 ml microcentrifuge tube. To
  // elute DNA, add 50 μl Buffer EB (10 mM Tris·Cl, pH 8.5) or water to the
  // center of each QIAprep spin column, let stand for 1 min, and centrifuge for
  // 1 min.
  next_step();
  transfer_column(qiaprep, microcentrifuge_tube2);
  add_to_column(qiaprep, eb, vol(50, UL));
  store_for(qiaprep, RT, time(1, MINS));
  centrifuge_flow_through(qiaprep, RT, time(1, MINS), microcentrifuge_tube2);
  comment("The flow-through contains the DNA.");
  comment(
    "If you are concerned about the concentration of the DNA, you can "
    "alternatively add 30 µl water to the center of the column, incubate at "
    "room temperature on the bench for 5 mins and then centrifuge for 1 min. "
    "This will increase the concentration of DNA in your final sample which "
    "can be useful in some cases. See notes below for why you should elute in "
    "water rather than the Buffer EB they recommend if you plan to sequence "
    "your sample. Even if you are not sequencing, it may be beneficial to "
    "elute in water. For instance, if you elute in buffer EB and you are using "
    "this DNA in a restriction digest, then the additional salts in your "
    "sample can affect the salt content of your digest. This may matter with "
    "some finicky enzymes.");

  end_protocol();
}