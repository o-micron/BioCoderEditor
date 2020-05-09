#include "biocoder.h"

int
main()
{
  start_protocol("Qiagen Plasmid Plus Midi high-yield protocol");

  // reagents used in the protocols
  Fluid bacterial_culture = new_fluid("Bacterial culture");
  Fluid p1 = new_fluid("Qiagen buffer P1");
  Fluid p2 = new_fluid("Qiagen buffer P2");
  Fluid s1 = new_fluid("Qiagen buffer S3");
  Fluid bb = new_fluid("Qiagen buffer BB");
  Fluid etr = new_fluid("Qiagen buffer ETR");
  Fluid pe = new_fluid("Qiagen buffer PE");
  Fluid eb = new_fluid("Qiagen buffer EB");

  // column used
  Column spin_col = new_column("QIAGEN Plasmid Plus Spin Columns");

  // containers required
  Container sterile_microfuge_tube = new_container(STERILE_MICROFUGE_TUBE);
  Container sterile_microfuge_tube1 = new_container(STERILE_MICROFUGE_TUBE);
  Container qia_cartridge = new_container(QIA_CARTRIDGE);
  Container new_tube = new_container(FRESH_COLL_TUBE);
  Container coll_tube = new_container(FRESH_COLL_TUBE);
  // harvesting the cells
  first_step();
  measure_fluid(bacterial_culture, vol(2, ML), sterile_microfuge_tube);
  centrifuge_pellet(
    sterile_microfuge_tube, speed(SPEED_MAX, RPM), 4, time(30, SECS));

  // 1. Resuspend pelleted bacteria in  2 ML or 4 ML Buffer P1.
  next_step();
  measure_fluid(p1, vol(2, ML), sterile_microfuge_tube);
  resuspend(sterile_microfuge_tube);

  // 2. Add 􀁓 2 ML or 􀁺 4 ML Buffer P2, gently mix by inverting, and incubate at
  // room temperature for 3 min.
  next_step();
  measure_fluid(p2, vol(2, ML), sterile_microfuge_tube);
  invert(sterile_microfuge_tube);
  incubate(sterile_microfuge_tube, RT, time(3, MINS));
  time_constraint(sterile_microfuge_tube, time(5, MINS), NEXTSTEP);

  // 3. Add 􀁓 2 ML or 􀁺 4 ML Buffer S3 to the lysate and mix immediately by
  // inverting 4–6 times. Proceed directly to step 4. Do not incubate the lysate
  // on ice.
  next_step();
  measure_fluid(s1, vol(2, ML), sterile_microfuge_tube);
  invert(sterile_microfuge_tube, 4, 6);
  time_constraint(sterile_microfuge_tube, time(0, SECS), NEXTSTEP);

  // 4. Transfer the lysate to the QIAfilter cartridge and incubate at room
  // temperature for 10 min.
  next_step();
  name_sample(sterile_microfuge_tube, "lysate");
  transfer(sterile_microfuge_tube, qia_cartridge);
  incubate(qia_cartridge, RT, time(10, MINS));

  // 5. During incubation, prepare the vacuum manifold and the QIAGEN Plasmid
  // Plus Midi spin columns.
  next_step();
  comment("During incubation, prepare the vacuum manifold and the QIAGEN "
          "Plasmid Plus Midi spin columns.");

  // 6. Gently insert the plunger into the QIAfilter cartridge and filter the
  // cell lysate into a new tube, allowing space for the addition of Buffer BB.
  next_step();
  comment("Gently insert the plunger into the QIAfilter cartridge and filter "
          "the cell lysate into a new tube, allowing space for the addition of "
          "Buffer BB.");
  name_sample(qia_cartridge, "cleared lysate");
  set_container(qia_cartridge.contents, new_tube);

  // 7. Add 2 ML Buffer BB to the cleared lysate and mix by inverting 4–6 times.
  next_step();
  measure_fluid(bb, vol(2, ML), new_tube);
  invert(new_tube, 4, 6);

  // 8. Transfer lysate to a QIAGEN Plasmid Plus Midi spin column with a tube
  // extender attached on the QIAvac 24 Plus. printf("Transfer lysate to a
  // QIAGEN Plasmid Plus Midi spin column with a tube extender attached on the
  // QIAvac 24 Plus\n\n");
  next_step();
  name_sample(new_tube, "lysate");
  add_to_column(spin_col, new_tube);

  // 9. Switch on the vacuum source, apply approx. –300 mbar, and draw the
  // solution through the QIAGEN Plasmid Plus Midi spin column.After the liquid
  // has been drawn through all columns, switch off the vacuum source.
  next_step();
  comment("Switch on the vacuum source and draw the solution through the "
          "QIAGEN Plasmid Plus Midi spin column.");
  comment("After the liquid has been drawn through all columns, switch off the "
          "vacuum source.");
  name_sample(new_tube, "impure DNA on column");

  // 10. To wash the DNA, add 0.7 ML Buffer ETR and proceed with one of the
  // following two steps:Washing using a vacuum/Washing using a microcentrifuge
  next_step();
  add_to_column(spin_col, etr, vol(0.7, ML));
  first_option();
  comment("Switch on the vacuum source. After the liquid has been drawn "
          "through all columns, switch off the vacuum source.");
  next_option();
  comment("Discard the tube extenders.");
  transfer_column(spin_col, coll_tube);
  // fprintf(fp,"Place the QIAGEN Plasmid Plus Midi spin column into the 2 ML
  // collection tube provided.<br>");
  centrifuge_column(spin_col, speed(10000, G), RT, time(1, MINS));
  end_option();

  // 11.To further wash the DNA, add 0.7 ML Buffer PE and proceed with one of
  // the following two steps:Washing using a vacuum/Washing using a
  // microcentrifuge
  next_step();
  add_to_column(spin_col, pe, vol(0.7, ML));

  next_step();
  first_option();
  comment("Switch on the vacuum source. After the liquid has been drawn "
          "through all columns, switch off the vacuum source.");
  next_option();
  transfer_column(spin_col, coll_tube);
  centrifuge_column(spin_col, speed(10000, G), RT, time(1, MINS));

  // 12.To completely remove the residual wash buffer, centrifuge the column for
  // 1 min at 10,000 x g in a microcentrifuge.
  next_step();
  centrifuge_column(spin_col, speed(10000, G), RT, time(1, MINS));

  // 13.Place the QIAGEN Plasmid Plus Midi spin column into a clean 1.5 ML
  // microcentrifuge tube. To elute the DNA, add 200 μl of Buffer EB (10 mM
  // Tris·Cl, pH 8.5) or water to the center of the QIAGEN Plasmid Plus Midi
  // spin column, let it stand for at least 1 min, and centrifuge for 1 min.
  next_step();
  transfer_column(spin_col, sterile_microfuge_tube1);
  add_to_column(spin_col, eb, vol(200, UL));
  incubate(coll_tube, RT, time(1, MINS));

  next_step();
  centrifuge_flow_through(
    spin_col, speed(10000, G), RT, time(1, MINS), sterile_microfuge_tube1);
  name_sample(sterile_microfuge_tube1, "DNA");
  name_sample(sterile_microfuge_tube1, "eluted DNA");
  ;
  comment("The flow-through contains DNA.");
  store(sterile_microfuge_tube1, -20);

  end_protocol();
}