#include "biocoder.h"

int
main()
{
  start_protocol("High-efficiency Transformation of Yeast");

  Fluid ypd1 = new_fluid("liquid YPD", vol(5, ML));
  Fluid ypd2 = new_fluid("YPD medium", vol(50, ML));
  Fluid sc = new_fluid("SC", vol(10, ML));
  Solid colony = new_solid("a single yeast colony");
  Fluid water = new_fluid("sterile water");
  Fluid liac = new_fluid("100mM lithium acetate");
  Fluid carrier_dna = new_fluid("single stranded carrier DNA");
  Fluid peg = new_fluid("PEG (50% w/v)");
  Fluid plasmid = new_fluid("plasmid DNA in water", "1-10 µg");
  Plate selective_plate = new_plate("selective plate");

  Container flask1 = new_container(FLASK, ypd1);
  Container flask2 = new_container(FLASK, ypd2);
  Container cent_tube = new_container(CENTRI_TUBE_50ML);
  Container microfuge_tube1 = new_container(STERILE_MICROFUGE_TUBE);
  Container microfuge_tube2 = new_container(STERILE_MICROFUGE_TUBE);
  Container microfuge_tube3 = new_container(STERILE_MICROFUGE_TUBE);

  // 1. Inoculate 5 ml of liquid YPD or 10 ml of SC and incubate with shaking
  // overnight at 30C.
  first_step();
  first_option();
  inoculation(flask1, colony, 30, time(OVERNIGHT, HRS), 1);
  next_option();
  set_container(sc, flask1);
  inoculation(flask1, colony, 30, time(OVERNIGHT, HRS), 1);
  end_option();

  // 2. Count overnight culture and inoculate 50ml of YPD to a cell density of 5
  // X 106 cells/ml of culture.
  next_step();
  name_sample(flask1, "overnight culture");
  inoculation(flask2,
              flask1.contents,
              approx_vol(1, ML),
              30,
              time_range(3, 5, HRS),
              20,
              1);
  comment("Add overnight culture from first step to a cell density of "
          "5X10<sup>6</sup> cells/ml of culture.");

  // 3. Incubate the culture at 30C on a shaker at 200 rpm until it is at 2 x
  // 107 cell/ml. This typically takes 3-5 hours. This culture will give
  // sufficient cells for ten transformations. Notes: i. It is important to
  // allow the cells to complete at least two divisions. ii. Transformation
  // efficiency remains constant for three to four divisions.
  next_step();
  comment("This culture will give sufficient cells for ten transformations.");
  comment("Notes:");
  comment("i. It is important to allow the cells to complete at least two "
          "divisions.");
  comment("ii. Transformation efficiency remains constant for three to four "
          "divisions.");

  // 4. Harvest the culture in a sterile 50-ml centrifuge tube at 3000g (2500
  // rpm) for 5 minutes.
  next_step();
  transfer(flask2, cent_tube);
  centrifuge_pellet(cent_tube, speed(3000, G), RT, time(5, MINS));

  // 5. Pour off the medium, resuspend the cells in 25 ml of sterile H2O, and
  // centrifuge again.
  next_step();
  measure_fluid(water, vol(25, ML), cent_tube);
  resuspend(cent_tube);
  name_sample(cent_tube, "suspension");
  centrifuge_pellet(cent_tube, speed(3000, G), RT, time(5, MINS));

  // 6. Pour off the H2O, resuspend cells in 1.0 ml of 100mM lithium acetate
  // (LiAc), and transfer the suspension to a sterile 1.5-ml microfuge tube.
  next_step();
  measure_fluid(liac, vol(1, ML), cent_tube);
  resuspend(cent_tube);
  name_sample(cent_tube, "suspension");
  transfer(cent_tube, microfuge_tube1);

  // 7. Pellet the cells at top speed for 5 seconds and remove the LiAc with a
  // micropipette.
  next_step();
  centrifuge_pellet(microfuge_tube1, speed(SPEED_MAX, G), RT, time(5, SECS));

  // 8. Resuspend the cells to a final volume of 500 μl (2 x 109 cells/ml),
  // which is about 400 ml of 100 mM LiAc.
  next_step();
  measure_fluid(liac, vol(400, UL), microfuge_tube1);
  resuspend(microfuge_tube1);
  comment("The final volume is 500 µl (2 x 10<sup>9</sup> cells/ml).");

  // Note: If the cell titer of the culture is grater than 2 x 107 cell/ml, the
  // volume of the LiAc should be increased to maintain the titer of this
  // suspension at 2 x 109 cells/ml. If the titer of the culture is less than 2
  // x 107 cells/ml, decrease the amount of LiAc.
  comment("If the cell titer of the culture is greater than 2 x 10<sup>7</sup> "
          "cell/ml, the volume of the LiAc should be increased to maintain the "
          "titer of this suspension at 2 x 10<sup>9</sup> cells/ml. If the "
          "titer of the culture is less than 2 x 10<sup>7</sup> cells/ml, "
          "decrease the amount of LiAc.");

  // 9. Boil a 1.0-ml sample of single-stranded carrier DNA for 5 minutes and
  // quickly chill in ice water. Note: It is not necessary or desirable to boil
  // the carrier DNA every time. Keep a small aliquot in your freezer box and
  // boil after three or four freeze/thaws.
  next_step();
  measure_fluid(carrier_dna, vol(1, ML), microfuge_tube2);
  store_for(microfuge_tube2, 100, time(5, MINS));
  time_constraint(microfuge_tube2, time(0, SECS), NEXTSTEP);

  next_step();
  store(microfuge_tube2, ON_ICE);
  comment("Note: It is not necessary or desirable to boil the carrier DNA "
          "every time. Keep a small aliquot in your freezer box and boil after "
          "three or four freeze/thaws.");
  name_sample(microfuge_tube2,
              "boiled single-stranded carrier DNA (2.0 mg/ml)");

  // 10. Vortex the cell suspension and pipette 50-μl samples into labeled
  // microfuge tubes. Pellet the cells and remove the LiAc with a micropipette.
  next_step();
  name_sample(microfuge_tube1, "cell suspension");
  measure_fluid(microfuge_tube1, vol(50, UL), microfuge_tube3);
  centrifuge_pellet(microfuge_tube3, speed(3000, G), RT, time(5, MINS));

  // 11. The basic “transformation mix” consists of the following ingredients;
  // carefully add them in the order listed: 240 μl of PEG (50% w/v) 36 μl
  // of 1.0M LiAc 25 μl of single-stranded carrier DNA (2.0 mg/ml) 50 μL of H2O
  // and plasmid DNA (0/1-10μg) Note. The order is important here. The PEG,
  // which shields the cells from the detrimental effects of the high
  // concentration of LiAc, should go in first.
  next_step("Addition of Transformation Mix");
  measure_fluid(peg, vol(240, UL), microfuge_tube3);
  measure_fluid(liac, vol(36, UL), microfuge_tube3);
  measure_fluid(microfuge_tube2, vol(25, UL), microfuge_tube3);
  measure_fluid(plasmid, vol(50, UL), microfuge_tube3);
  comment("Note: The order is important here. The PEG, which shields the cells "
          "from the detrimental effects of the high concentration of LiAc, "
          "should go in first.");

  // 12. Vortex each tube vigorously until the cell pellet has been completely
  // mixed. This usually takes 1 minute.
  next_step();
  vortex(microfuge_tube3, time(1, MINS));
  comment("Make sure that the cell pellet has been completely mixed.");

  // 13. Incubate for 30 minutes at 30C.
  next_step();
  incubate(microfuge_tube3, 30, time(30, MINS));

  // 14. Heat shock for 20-25 minutes at 42C.
  // Note: The optimum time can vary for different yeast strains. Test this if
  // you need high efficiency from your transformations.
  next_step("Heat shock");
  store_for(microfuge_tube3, 42, time_range(20, 25, MINS));
  comment("Note: The optimum time can vary for different yeast strains. Test "
          "this if you need high efficiency from your transformations.");

  // 15. Microfuge at 6000-8000 rpm for 15 seconds and remove the transformation
  // mix with micropipette.
  next_step();
  centrifuge_pellet(
    microfuge_tube3, speed_range(6000, 8000, RPM), RT, time(15, SECS));
  pipet(microfuge_tube3);

  // 16. Pipette 0.2-1.0 ml of sterile H2O into each tube and resuspend the
  // pellet by pipetting it up and down gently. Note: Be as gentle as possible
  // if high efficiency is important.
  next_step();
  measure_fluid(water, vol_range(0.2, 1, ML), microfuge_tube3);
  comment("Note: Be as gentle as possible if high efficiency is important.");

  // 17. Plate from 200-μl aliquots of the transformation mix onto selective
  // plates.
  next_step();
  name_sample(microfuge_tube3, "transformation mix");
  plate_out(selective_plate, microfuge_tube3, vol(200, UL));

  end_protocol();
}
