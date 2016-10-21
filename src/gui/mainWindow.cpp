#include "mainWindow.hpp"

MainWindow::MainWindow()
{
  set_title( "Analyse d'image" ) ;
  set_border_width( 5 ) ;
  set_size_request( 0.9*gdk_screen_width(), 0.9*gdk_screen_height() ) ;
  int w, h ;
  get_size_request(w, h) ;


  
  /*******************
    MENU
  *******************/
  static Gtk::Toolbar toolbar ;
  toolbar.set_toolbar_style( Gtk::TOOLBAR_ICONS ) ;
  open.set_icon_name( "document-open" ) ;
  open.set_tooltip_text( "open new input" ) ;
  save.set_icon_name( "document-save" ) ;
  save.set_tooltip_text( "save output" ) ;  
  refresh.set_icon_name( "view-refresh" ) ;
  refresh.set_tooltip_text( "refresh" ) ;
  undo.set_icon_name( "edit-undo" ) ;
  undo.set_tooltip_text( "undo" ) ;
  redo.set_icon_name( "edit-redo" ) ;
  redo.set_tooltip_text( "redo" ) ;
  reset.set_stock_id( Gtk::Stock::CANCEL ) ;
  reset.set_tooltip_text( "reset" ) ;
  newBase.set_stock_id( Gtk::Stock::APPLY ) ;
  newBase.set_tooltip_text( "apply" ) ;
  zoomIn.set_icon_name( "zoom-in" ) ;
  zoomIn.set_tooltip_text( "zoom +" ) ;
  zoomOut.set_icon_name( "zoom-out" ) ;
  zoomOut.set_tooltip_text( "zoom -" ) ;
  zoomFit.set_icon_name( "zoom-fit" ) ;
  zoomFit.set_tooltip_text( "zoom fit" ) ;

  static Gtk::ToolButton sp1, sp2, sp7 ;
  sp1.set_label( "|" ) ;
  sp1.set_sensitive( false) ;
  sp2.set_label( "|" ) ;
  sp2.set_sensitive( false) ;
  sp7.set_label( "|" ) ;
  sp7.set_sensitive( false) ;

  toolbar.add( open ) ;
  toolbar.add( save ) ;
  toolbar.add( sp1 ) ;
  toolbar.add( refresh ) ;
  toolbar.add( undo ) ;
  toolbar.add( redo ) ;
  toolbar.add( reset ) ;
  toolbar.add( newBase ) ;
  toolbar.add( sp2 ) ;
  toolbar.add( zoomOut ) ;
  toolbar.add( zoomIn ) ;
  toolbar.add( zoomFit ) ;
  toolbar.add( sp7 ) ;
  
  zoom_actu = 512 ;




  /*******************
    BUTTON BOX
  *******************/
  /*******************
    UTILS
  *******************/

  static Gtk::HButtonBox pan_desaturate ;
  btn_desaturate.set_label( "Désaturer" ) ;
  
  m_refTreeModel = Gtk::ListStore::create(m_Columns);
  combo_desaturate.set_model(m_refTreeModel);
  //Fill the ComboBox's Tree Model:
  Gtk::TreeModel::Row row = *(m_refTreeModel->append());
  row[m_Columns.m_col_id] = 1;
  row[m_Columns.m_col_name] = "Clarté";
  combo_desaturate.set_active(row);
  row = *(m_refTreeModel->append());
  row[m_Columns.m_col_id] = 2;
  row[m_Columns.m_col_name] = "Luminosité";
  row = *(m_refTreeModel->append());
  row[m_Columns.m_col_id] = 3;
  row[m_Columns.m_col_name] = "Moyenne";
  combo_desaturate.pack_start(m_Columns.m_col_name);
  pan_desaturate.add( combo_desaturate ) ;
  pan_desaturate.add( btn_desaturate ) ;

  btn_inverse.set_label( "Inverser" ) ;

  static Gtk::HButtonBox pan_rotate ;
  btn_rotate90.set_label( "rotate90" ) ;
  pan_rotate.add( btn_rotate90 ) ;
  btn_rotate180.set_label( "rotate180" ) ;
  pan_rotate.add( btn_rotate180 ) ;



  /*******************
    LISSAGE
  *******************/

  btn_lissage.set_label( "Lissage" ) ;


  static Gtk::HButtonBox lisT ;
  static Gtk::Label labLissageT ;
  labLissageT.set_markup("<b>Type de filtre</b>");
  lisT.add( labLissageT ) ;
  m_refTreeModel4 = Gtk::ListStore::create(m_Columns4);
  lissage_type.set_model(m_refTreeModel4);
  Gtk::TreeModel::Row row4 = *(m_refTreeModel4->append());
  row4[m_Columns4.m_col_id] = MOYENNE;
  row4[m_Columns4.m_col_name] = "Moyenne";
  row4 = *(m_refTreeModel4->append());
  row4[m_Columns4.m_col_id] = MEDIANE;
  row4[m_Columns4.m_col_name] = "Mediane";
  row4 = *(m_refTreeModel4->append());
  row4[m_Columns4.m_col_id] = GAUSSIEN;
  row4[m_Columns4.m_col_name] = "Gaussien";
  lissage_type.set_active(row4);
  lissage_type.pack_start(m_Columns4.m_col_name);
  lisT.add( lissage_type ) ;
  tabLissage.add( lisT ) ;

  static Gtk::HButtonBox lis ;
  static Gtk::Label labLissage ;
  labLissage.set_markup("<b>Taille du filtre</b>");
  lis.add( labLissage ) ;
  lissage_size.set_digits(0);
  lissage_size.set_range(3, 99);
  lissage_size.set_increments(2, 2);   
  lissage_size.set_value( option.lissage_size ) ;
  lis.add( lissage_size ) ;
  tabLissage.add( lis ) ;


  static Gtk::HButtonBox lis2 ;
  static Gtk::Label labLissage2 ;
  labLissage2.set_markup("<b>Sigma (gaussien)</b>");
  lis2.add( labLissage2 ) ;
  lissage_sigma.set_digits(3);
  lissage_sigma.set_range(0, 99);
  lissage_sigma.set_increments(0.1, 0.1);  
  lissage_sigma.set_value( option.lissage_sigma ) ;
  lis2.add( lissage_sigma ) ;
  tabLissage.add( lis2 ) ;


  /*******************
    FILTRAGE
  *******************/

  btn_filtre.set_label( "Filtre Différentiel" ) ;

  static Gtk::Label labF ;
  labF.set_markup("<b>Filtre</b>");


  static Gtk::VButtonBox h5 ;
  static Gtk::HButtonBox def ;
  h5.add(labF);
  h5.add(def);
  filtreP.set_label("Prewitt");
  filtreS.set_label("Sobel");
  filtreK.set_label("Kirsch");
  filtre5.set_label("filtre5");
  static Gtk::VButtonBox filtreDefauts ;
  Gtk::RadioButton::Group group = filtreP.get_group();
  filtreS.set_group(group);
  filtreK.set_group(group);
  filtre5.set_group(group);

  filtreDefauts.add(filtreP);
  filtreDefauts.add(filtreS);
  filtreDefauts.add(filtreK);
  filtreDefauts.add(filtre5);
  on_filtre_prewitt() ;
  def.add( showFiltre ) ;
  def.add( filtreDefauts ) ;
  tabFiltre.add( h5 ) ;


  m_refTreeModel2 = Gtk::ListStore::create(m_Columns2);
  filtre_direction.set_model(m_refTreeModel2);
  Gtk::TreeModel::Row row2 = *(m_refTreeModel2->append());
  row2[m_Columns2.m_col_id] = HORIZONTAL;
  row2[m_Columns2.m_col_name] = "Horizontal";
  row2 = *(m_refTreeModel2->append());
  row2[m_Columns2.m_col_id] = VERTICAL;
  row2[m_Columns2.m_col_name] = "Vertical";
  row2 = *(m_refTreeModel2->append());
  row2[m_Columns2.m_col_id] = BI_DIRECTIONNEL;
  row2[m_Columns2.m_col_name] = "Bi-Directionnel";
  row2 = *(m_refTreeModel2->append());
  row2[m_Columns2.m_col_id] = MULTI_DIRECTIONNEL;
  row2[m_Columns2.m_col_name] = "Multi-Directionnel";
  filtre_direction.set_active(row2);
  filtre_direction.pack_start(m_Columns2.m_col_name);
  tabFiltre.add( filtre_direction ) ;


  /*******************
    SEUILLAGE
  *******************/

  btn_seuillage.set_label( "Seuillage" ) ;

  m_refTreeModel3 = Gtk::ListStore::create(m_Columns3);
  seuil_type.set_model(m_refTreeModel3);
  Gtk::TreeModel::Row row3 = *(m_refTreeModel3->append());
  row3[m_Columns3.m_col_id] = UNIQUE;
  row3[m_Columns3.m_col_name] = "Seuillage Unique";
  row3 = *(m_refTreeModel3->append());
  row3[m_Columns3.m_col_id] = GLOBAL;
  row3[m_Columns3.m_col_name] = "Seuillage Global";
  row3 = *(m_refTreeModel3->append());
  row3[m_Columns3.m_col_id] = LOCAL;
  row3[m_Columns3.m_col_name] = "Seuillage Local";
  row3 = *(m_refTreeModel3->append());
  row3[m_Columns3.m_col_id] = HYSTERESIS_AUTO;
  row3[m_Columns3.m_col_name] = "Seuillage Hysteresis Auto";
  row3 = *(m_refTreeModel3->append());
  row3[m_Columns3.m_col_id] = HYSTERESIS;
  row3[m_Columns3.m_col_name] = "Seuillage Hysteresis";
  seuil_type.set_active(row3);
  seuil_type.pack_start(m_Columns3.m_col_name);
  tabSeuillage.add( seuil_type ) ;

  m_refTreeModel33 = Gtk::ListStore::create(m_Columns33);
  seuil_type_calcul.set_model(m_refTreeModel33);
  Gtk::TreeModel::Row row33 = *(m_refTreeModel33->append());
  row33[m_Columns33.m_col_id] = MOYENNE;
  row33[m_Columns33.m_col_name] = "Moyenne";
  seuil_type_calcul.set_active(row33);
  row33 = *(m_refTreeModel33->append());
  row33[m_Columns33.m_col_id] = MEDIANE;
  row33[m_Columns33.m_col_name] = "Mediane";
  row33 = *(m_refTreeModel33->append());
  row33[m_Columns33.m_col_id] = ECART_TYPE;
  row33[m_Columns33.m_col_name] = "Ecart-type";
  seuil_type_calcul.pack_start(m_Columns33.m_col_name);
  tabSeuillage.add( seuil_type_calcul ) ;

  static Gtk::HButtonBox h36 ;
  static Gtk::Label labSeuil_fenetre ;
  labSeuil_fenetre.set_markup("<b>fenêtre</b>"); 
  h36.add( labSeuil_fenetre ) ;
  seuil_fenetre.set_digits(0);
  seuil_fenetre.set_range(3, 99);
  seuil_fenetre.set_increments(2, 2); 
  seuil_fenetre.set_value( option.seuil_fenetre );  
  h36.add( seuil_fenetre ) ;
  tabSeuillage.add( h36 ) ;

  static Gtk::HButtonBox h361 ;
  static Gtk::Label labSeuil_val ;
  labSeuil_val.set_markup("<b>seuil fixe</b>");
  h361.add( labSeuil_val ) ;
  seuil_val.set_digits(0);
  seuil_val.set_range(0, 255);
  seuil_val.set_increments(1, 1); 
  seuil_val.set_value( option.seuil_val );   
  h361.add( seuil_val ) ;
  tabSeuillage.add( h361 ) ;

  static Gtk::HButtonBox h362 ;
  static Gtk::Label labSeuil_bas ;
  labSeuil_bas.set_markup("<b>seuil bas</b>");
  h362.add( labSeuil_bas ) ;
  seuil_bas.set_digits(0);
  seuil_bas.set_range(0, 255);
  seuil_bas.set_increments(1, 1); 
  seuil_bas.set_value( option.seuil_bas );   
  h362.add( seuil_bas ) ;
  tabSeuillage.add( h362 ) ;

  static Gtk::HButtonBox h363 ;
  static Gtk::Label labSeuil_haut ;
  labSeuil_haut.set_markup("<b>seuil haut</b>");
  h363.add( labSeuil_haut ) ;
  seuil_haut.set_digits(0);
  seuil_haut.set_range(0, 255);
  seuil_haut.set_increments(1, 1); 
  seuil_haut.set_value( option.seuil_haut );   
  h363.add( seuil_haut ) ;
  tabSeuillage.add( h363 ) ;


  /*******************
    AFFINAGE
  *******************/

  btn_affinage.set_label( "Affinage" ) ;
  

  /*******************
    FERMETURE
  *******************/

  fermeture.set_label( "Fermeture" ) ;


  static Gtk::HButtonBox hFerm ;
  static Gtk::Label labFermture_size ;
  labFermture_size.set_markup("<b>distance de recherche</b>");
  hFerm.add( labFermture_size ) ;
  fermeture_size.set_digits(0);
  fermeture_size.set_range(0, 255);
  fermeture_size.set_increments(1, 1); 
  fermeture_size.set_value( option.fermeture_size );   
  hFerm.add( fermeture_size ) ;
  tabFermeture.add( hFerm ) ;

  static Gtk::HButtonBox hFerm2 ;
  static Gtk::Label labFermture_seuil ;
  labFermture_seuil.set_markup("<b>seuil de fermeture</b>");
  hFerm2.add( labFermture_seuil ) ;
  fermeture_seuil.set_digits(0);
  fermeture_seuil.set_range(0, 255);
  fermeture_seuil.set_increments(1, 1); 
  fermeture_seuil.set_value( option.fermeture_seuil );   
  hFerm2.add( fermeture_seuil ) ;
  tabFermeture.add( hFerm2 ) ;
  

  /*******************
    COLOR
  *******************/

  color.set_label( "Colorise" ) ;  
  
  /*******************
    DETECTION_CONTOUR
  *******************/

  detection.set_label( "Détection de contours" ) ;

  static Gtk::Label labColor ;
  labColor.set_markup("<b>Couleur en fonction des directions</b>");
  static Gtk::Label labNorme ;
  labNorme.set_markup("<b>Nuancer les couleurs en fonction des normes</b>");

  static Gtk::HButtonBox pan_show_color ;
  pan_show_color.add( show_color ) ;
  pan_show_color.add( labColor ) ;
  show_color.set_active( true ) ;

  static Gtk::HButtonBox pan_keep_norme ;
  pan_keep_norme.add( keep_norme ) ;
  pan_keep_norme.add( labNorme ) ;
  keep_norme.set_active( true ) ;


  
  /*******************
    VIEW
  *******************/
  static Gtk::ScrolledWindow scrollImage ;
  scrollImage.set_size_request((w/2)+100, h-80) ;
  static Gtk::HBox panelImage ;
  outputGtk.set_margin_left(10) ;

  panelImage.add( inputGtk ) ;
  panelImage.add( outputGtk ) ;
  scrollImage.add( panelImage ) ;



  /*******************
    DEBUG
  *******************/
  static Gtk::ScrolledWindow scrollConsole ;
  scrollConsole.set_size_request((w/4)-120, h-80) ;
  consoleBox.set_margin_left(35) ;
  consoleBox.override_background_color ( Gdk::RGBA( "black" )    ) ;
  consoleBox.override_color ( Gdk::RGBA( "white" )    ) ;
  consoleBox.set_editable(false) ;
  
  bufferConsole = Gtk::TextBuffer::create();
  consoleBox.set_buffer(bufferConsole);
  scrollConsole.add( consoleBox ) ;

  info_file.set_alignment( 0 ) ;


  /***********************
    ORGANISATION
  ***********************/

  static Gtk::ScrolledWindow scrollButtonBox ;
  scrollButtonBox.set_size_request((w/4)-100, h-80) ;
  static Gtk::VButtonBox buttonBox ;
  buttonBox.set_valign(Gtk::ALIGN_START);
  scrollButtonBox.add( buttonBox ) ; 

  tabOption.set_size_request( (w/4)-100, -1 ) ;
  tabOption.append_page(tabLissage, "Lissage");
  tabOption.append_page(tabFiltre, "Filtre");
  tabOption.append_page(tabSeuillage, "Seuillage");
  tabOption.append_page(tabAffinage, "Affinage");
  tabOption.append_page(tabFermeture, "Fermeture");
  tabOption.append_page(tabDetection, "Détection");

  static Gtk::HSeparator sep1 ; buttonBox.add( sep1 ) ;
  buttonBox.add( pan_desaturate ) ;
  buttonBox.add( btn_inverse ) ;
  buttonBox.add( pan_rotate ) ;
  static Gtk::HSeparator sep2 ; buttonBox.add( sep2 ) ;
  buttonBox.add( btn_lissage ) ;
  buttonBox.add( btn_filtre ) ;
  buttonBox.add( btn_seuillage ) ;
  buttonBox.add( btn_affinage ) ;
  buttonBox.add( fermeture ) ;
  buttonBox.add( color ) ;
  static Gtk::HSeparator sep3 ; buttonBox.add( sep3 ) ;
  buttonBox.add( detection ) ;

  static Gtk::HSeparator sep4 ; buttonBox.add( sep4 ) ;
  buttonBox.add( pan_show_color ) ;
  buttonBox.add( pan_keep_norme ) ;
  static Gtk::HSeparator sep5 ; buttonBox.add( sep5 ) ;
  buttonBox.add( tabOption );


  static Gtk::HBox panelView ;
  panelView.pack_start( scrollButtonBox, TRUE, TRUE, 5 ) ;
  panelView.pack_start( scrollImage, TRUE, TRUE, 5 ) ;
  panelView.pack_start( scrollConsole, TRUE, TRUE, 5 ) ;

  
  static Gtk::VBox mainPanel ;
  mainPanel.pack_start( toolbar, FALSE, FALSE, 5 ) ;
  mainPanel.pack_start( panelView, FALSE, FALSE, 5 ) ;
  mainPanel.pack_start( info_file, FALSE, FALSE, 5 ) ;

  add( mainPanel ) ;
  
  initSignals() ;
  maximize() ;
  show_all();
  on_open() ;
}

MainWindow::~MainWindow()
{
}

void MainWindow::initSignals()
{
  open.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_open) ) ;
  save.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_save) ) ;
  refresh.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_refresh) ) ;
  undo.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_undo) ) ;
  redo.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_redo) ) ;
  reset.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_reset) ) ;
  zoomIn.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_zoomIn) ) ;
  zoomOut.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_zoomOut) ) ;
  zoomFit.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_zoomFit) ) ;
  newBase.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_newBase) ) ;
  
  btn_desaturate.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_desaturate) ) ;
  btn_inverse.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_inverse) ) ;
  btn_rotate90.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_rotate90) ) ;
  btn_rotate180.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_rotate180) ) ;
  
  btn_lissage.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_lissage) ) ;
  lissage_type.signal_changed().connect( sigc::mem_fun(*this, &MainWindow::on_lissage_type) ) ;
  lissage_size.signal_value_changed().connect( sigc::mem_fun(*this, &MainWindow::on_lissage_size) ) ;
  lissage_sigma.signal_value_changed().connect( sigc::mem_fun(*this, &MainWindow::on_lissage_sigma) ) ;
  
  btn_filtre.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_filtre) ) ;
  filtreP.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_filtre_prewitt) ) ;
  filtreS.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_filtre_sobel) ) ;
  filtreK.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_filtre_kirsch) ) ;
  filtre5.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_filtre_5) ) ;
  filtre_direction.signal_changed().connect( sigc::mem_fun(*this, &MainWindow::on_filtre_direction) ) ;

  btn_seuillage.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_seuil) ) ;
  seuil_type.signal_changed().connect( sigc::mem_fun(*this, &MainWindow::on_seuil_type) ) ;
  seuil_type_calcul.signal_changed().connect( sigc::mem_fun(*this, &MainWindow::on_seuil_type_calcul) ) ;
  seuil_fenetre.signal_value_changed().connect( sigc::mem_fun(*this, &MainWindow::on_seuil_fenetre) ) ;
  seuil_val.signal_value_changed().connect( sigc::mem_fun(*this, &MainWindow::on_seuil_val) ) ;
  seuil_bas.signal_value_changed().connect( sigc::mem_fun(*this, &MainWindow::on_seuil_bas) ) ;
  seuil_haut.signal_value_changed().connect( sigc::mem_fun(*this, &MainWindow::on_seuil_haut) ) ;
  
  btn_affinage.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_affinage) ) ;
  
  fermeture.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_fermeture) ) ;
  fermeture_size.signal_value_changed().connect( sigc::mem_fun(*this, &MainWindow::on_fermeture_size) ) ;
  fermeture_seuil.signal_value_changed().connect( sigc::mem_fun(*this, &MainWindow::on_fermeture_seuil) ) ;
  
  color.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_color) ) ;
  
  detection.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_detection) ) ;
  
  show_color.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_show_color) ) ;
  keep_norme.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_keep_norme) ) ;
}


void MainWindow::log(std::string text)
{
  bufferConsole->insert(bufferConsole->end(), " > "+text+"\n ");
  Gtk::TextBuffer::iterator buffer_it_ = bufferConsole->end();
  consoleBox.scroll_to(buffer_it_);
}

std::string MainWindow::getFilePath()
{
  Gtk::FileChooserDialog dialog("Please choose a file",
            Gtk::FILE_CHOOSER_ACTION_OPEN);
  dialog.set_transient_for(*this);

  //Add response buttons the the dialog:
  dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
  dialog.add_button("_Open", Gtk::RESPONSE_OK);

  //Add filters, so that only certain file types can be selected:
  auto filter_image = Gtk::FileFilter::create();
  filter_image->set_name("Image files");
  filter_image->add_mime_type("image/jpeg");
  filter_image->add_mime_type("image/png");
  dialog.add_filter(filter_image);

  int result = dialog.run();
  switch(result)
  {
    case(Gtk::RESPONSE_OK):
    {
      return dialog.get_filename();
      break;
    }
    case(Gtk::RESPONSE_CANCEL):
    {
      break;
    }
    default:
    {
      break;
    }
  }

  return "" ;
}

void MainWindow::on_open()
{
  Gtk::MessageDialog dialog("confirm");
  dialog.set_title("Attention !") ;
  dialog.set_message("Tous les changements seront perdus") ;
  dialog.set_transient_for(*this);
  dialog.add_button("Annuler", Gtk::RESPONSE_CANCEL);

  int result = Gtk::RESPONSE_OK ;
  if( !output.empty() )
  {
    result = dialog.run();
  }
  if( result == Gtk::RESPONSE_OK )
  {
    std::string path = getFilePath() ;

    if(path != "")
    {
      input = Image( cv::imread( path ) ) ;

      log( "Open : "+path) ;
      info_file.set_text( "   >:  " + path + " - " + std::to_string(input.rows) +" * "+ std::to_string(input.cols) ) ;
      on_reset() ;
    }
  }
}

void MainWindow::on_save()
{
  std::string path = getFilePath() ;

  if(path != "")
  {
    log( "Save file : " + path ) ;
    cv::imwrite( path, (*actu) ) ;
  }
}

void MainWindow::on_refresh()
{
  outputGtk.set((*actu).toGtk(zoom_actu));
}

void MainWindow::on_undo()
{

  if(actu != output.begin()){
    log( "Undo : "+(*actu_option) ) ;
    --actu;
    --actu_option;
    outputGtk.set((*actu).toGtk(zoom_actu));
  }
}

void MainWindow::on_redo()
{
  ++actu ;
  ++actu_option ;
  
  if(actu != output.end()){
    log( "Redo : "+(*actu_option) ) ;
    outputGtk.set((*actu).toGtk(zoom_actu));
  }else{
    --actu ;
    --actu_option ;
  }
}

void MainWindow::on_reset()
{

  bufferConsole->set_text("") ;
  Gtk::TextBuffer::iterator buffer_it_ = bufferConsole->end();
  consoleBox.scroll_to(buffer_it_);

  log( "Reset" ) ;
  output.clear() ;
  output_option.clear() ;

  output.push_back( Image(input) ) ;
  output_option.push_back( "base" ) ;
  actu = output.begin();
  actu_option = output_option.begin();
  inputGtk.set(input.toGtk(zoom_actu));
  outputGtk.set((*actu).toGtk(zoom_actu));
}

void MainWindow::on_zoomIn()
{
  zoom_actu += 50 ;

  inputGtk.set(input.toGtk(zoom_actu));
  outputGtk.set((*actu).toGtk(zoom_actu));
}
 void MainWindow::on_zoomOut()
{
  zoom_actu -= 50 ;
  if(zoom_actu < 512)
      zoom_actu = 512 ;

  inputGtk.set(input.toGtk(zoom_actu));
  outputGtk.set((*actu).toGtk(zoom_actu));
}
void MainWindow::on_zoomFit()
{
  zoom_actu = 512 ;

  inputGtk.set(input.toGtk(zoom_actu));
  outputGtk.set((*actu).toGtk(zoom_actu));
}

void MainWindow::on_newBase()
{
  log( "Swap Output -> Input" ) ;
  input = (*actu).clone() ;
  on_reset() ;
}



/*
UTILS
*/

void MainWindow::on_desaturate()
{
  Gtk::TreeModel::iterator iter = combo_desaturate.get_active();
  int id = (*iter)[m_Columns.m_col_id];
  Glib::ustring name = (*iter)[m_Columns.m_col_name];
  log( "Desaturate : " + name ) ;
  Image temp = *actu ;
  output.push_back( temp.toGray( id ) ) ;
  actu = --(output.end());

  output_option.push_back("Desaturate : "+ name+" "+ToStringOption(option) ) ;
  actu_option = --(output_option.end());

  outputGtk.set((*actu).toGtk(zoom_actu));
}

void MainWindow::on_inverse()
{
  log( "Inverse Colors" ) ;
  Image temp = *actu ;
  output.push_back( temp.inverse() ) ;
  actu = --(output.end());

  output_option.push_back( "Inverse : "+ToStringOption(option) ) ;
  actu_option = --(output_option.end());

  outputGtk.set((*actu).toGtk(zoom_actu));
}

void MainWindow::on_rotate90()
{
  log( "Rotate 90° " ) ;
  Image temp = *actu ;

  if( temp.rows == temp.cols )
  {
    output.push_back( temp.rotate90() ) ;
    actu = --(output.end());

    output_option.push_back("Rotate90 : "+ToStringOption(option) ) ;
    actu_option = --(output_option.end());

    outputGtk.set((*actu).toGtk(zoom_actu));
  }
  else
  {
    log( " --> Impossible : Width != Height" ) ;
  }
}

void MainWindow::on_rotate180()
{
  log( "Rotate 180° " ) ;
  Image temp = *actu ;
  output.push_back( temp.rotate180() ) ;
  actu = --(output.end());

  output_option.push_back("Rotate180 : "+ToStringOption(option) ) ;
  actu_option = --(output_option.end());

  outputGtk.set((*actu).toGtk(zoom_actu));
}


/*
LISSAGE
*/
void MainWindow::on_lissage()
{
  log( "-------------------------\nLissage");
  log( "  * type : "+ToStringTypeCalcul(option.lissage_type) ) ;
  log( "  * size : "+std::to_string(option.lissage_size) ) ;
  log( "  * sigma : "+std::to_string(option.lissage_sigma) ) ;
  log("...en cours") ;

  Image temp = *actu ;

  std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
  output.push_back( temp.lissage( option ) ) ;
  std::chrono::duration<double> sec = std::chrono::system_clock::now() - start;

  actu = --(output.end());

  output_option.push_back("Lissage : "+ToStringOption(option) ) ;
  actu_option = --(output_option.end());

  outputGtk.set((*actu).toGtk(zoom_actu));
  log( " -->  " + std::to_string(sec.count()) + "  Seconds\n-------------------------" ) ;
}

void MainWindow::on_lissage_type()
{
  Gtk::TreeModel::iterator iter = lissage_type.get_active() ;
  int id = (*iter)[m_Columns.m_col_id];
  option.set_lissage_type( id ) ;
}

void MainWindow::on_lissage_size()
{
  if( (int)lissage_size.get_value() % 2 == 0)
  {
    lissage_size.set_value( lissage_size.get_value() - 1 ) ;
    return ;
  }
  option.lissage_size = lissage_size.get_value() ;
}
void MainWindow::on_lissage_sigma()
{
  option.lissage_sigma = lissage_sigma.get_value() ;
}


/*
FILTRE
*/

void MainWindow::on_filtre()
{
  std::stringstream buffer;
  buffer << option.filtre ;
  std::string mat = buffer.str() ;

  log( "-------------------------\nFiltre Différentiel");
  log( "  * taille filtre : "+std::to_string(option.filtre.rows)) ;
  log( "  * filtre : "+option.filtre.name) ;
  log( "  * filtre : \n"+mat) ;
  log( "  * direction : "+ToStringDir(option.direction)) ;
  log("...en cours") ;
  
  Image temp = *actu ;
  std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
  

  Image n = temp.filtre_differentiel( option ) ;
  output.push_back( n ) ;

  std::chrono::duration<double> sec = std::chrono::system_clock::now() - start;

  actu = --(output.end());

  output_option.push_back("Filtre Différentiel : "+ToStringOption(option) ) ;
  actu_option = --(output_option.end());

  outputGtk.set((*actu).toGtk(zoom_actu));
  log( " -->  " + std::to_string(sec.count()) + "  Seconds\n-------------------------" ) ;
}

void MainWindow::update_show_filtre()
{
  std::stringstream buffer;
  buffer << option.filtre ;
  std::string mat = buffer.str() ;
  showFiltre.set_text( mat ) ;
}

void MainWindow::on_filtre_prewitt()
{
  option.filtre.prewitt() ;
  update_show_filtre() ;
}
void MainWindow::on_filtre_sobel()
{
  option.filtre.sobel() ;
  update_show_filtre() ;
}
void MainWindow::on_filtre_kirsch()
{
  option.filtre.kirsch() ;
  update_show_filtre() ;
}
void MainWindow::on_filtre_5()
{
  option.filtre.lapalacien( 5 ) ;
  update_show_filtre() ;
}

void MainWindow::on_filtre_direction()
{
  Gtk::TreeModel::iterator iter = filtre_direction.get_active() ;
  int id = (*iter)[m_Columns.m_col_id];
  option.set_direction( id ) ;
}


/*
SEUILLAGE
*/

void MainWindow::on_seuil()
{
  log( "-------------------------\nSeuillage");
  log( "  * seuillage : "+ToStringSeuil(option.seuil)) ;
  log( "  * méthode : "+ToStringTypeCalcul(option.seuil_calcul)) ;
  log( "  * val unique : "+std::to_string(option.seuil_val)) ;
  log( "  * fenetre : "+std::to_string(option.seuil_fenetre)) ;
  log( "  * bas : "+std::to_string(option.seuil_bas)) ;
  log( "  * haut : "+std::to_string(option.seuil_haut)) ;
  log("...en cours") ;
  Image temp = *actu ;

  std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
  output.push_back( temp.seuillage( option ) ) ;
  std::chrono::duration<double> sec = std::chrono::system_clock::now() - start;
  
  actu = --(output.end());

  output_option.push_back("Seuillage : "+ToStringOption(option) ) ;
  actu_option = --(output_option.end());

  outputGtk.set((*actu).toGtk(zoom_actu));
  log( " -->  " + std::to_string(sec.count()) + "  Seconds\n-------------------------" ) ;
}

void MainWindow::on_seuil_type()
{
  Gtk::TreeModel::iterator iter2 = seuil_type.get_active() ;
  int id = (*iter2)[m_Columns.m_col_id];
  option.set_seuil( id ) ;
}

void MainWindow::on_seuil_type_calcul()
{
  Gtk::TreeModel::iterator iter2 = seuil_type_calcul.get_active() ;
  int id = (*iter2)[m_Columns.m_col_id];
  option.set_seuil_calcul( id ) ;
}

void MainWindow::on_seuil_fenetre()
{
  if( (int)seuil_fenetre.get_value() % 2 == 0)
  {
    seuil_fenetre.set_value( seuil_fenetre.get_value() - 1 ) ;
    return ;
  }
  option.seuil_fenetre = seuil_fenetre.get_value() ;
}

void MainWindow::on_seuil_val()
{
  option.seuil_val = seuil_val.get_value() ;
}

void MainWindow::on_seuil_bas()
{
  option.seuil_bas = seuil_bas.get_value() ;
}

void MainWindow::on_seuil_haut()
{
  option.seuil_haut = seuil_haut.get_value() ;
}



/*
AFFINAGE
*/

void MainWindow::on_affinage()
{
  log( "-------------------------\nAffinage");
  log("...en cours") ;
  Image temp = *actu ;

  std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
  output.push_back( temp.affinage( option ) ) ;
  std::chrono::duration<double> sec = std::chrono::system_clock::now() - start;
  
  actu = --(output.end());

  output_option.push_back("Affinage : "+ToStringOption(option) ) ;
  actu_option = --(output_option.end());

  outputGtk.set((*actu).toGtk(zoom_actu));
  log( " -->  " + std::to_string(sec.count()) + "  Seconds\n-------------------------" ) ;
}

/*
FERMETURE
*/

void MainWindow::on_fermeture()
{
  log( "-------------------------\nFermeture");
  log( "  * size : "+std::to_string(option.fermeture_size)) ;
  log( "  * seuil : "+std::to_string(option.fermeture_seuil)) ;
  log("...en cours") ;
  
  Image temp = *actu ;
  std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
  output.push_back( temp.fermeture( option ) ) ;
  std::chrono::duration<double> sec = std::chrono::system_clock::now() - start;

  actu = --(output.end());

  output_option.push_back("Fermeture : "+ToStringOption(option) ) ;
  actu_option = --(output_option.end());

  outputGtk.set((*actu).toGtk(zoom_actu));
  log( " -->  " + std::to_string(sec.count()) + "  Seconds\n-------------------------" ) ;
}

void MainWindow::on_fermeture_size()
{
  option.fermeture_size = fermeture_size.get_value() ;
}

void MainWindow::on_fermeture_seuil()
{
  option.fermeture_seuil = fermeture_seuil.get_value() ;
}

/*
COLOR
*/

void MainWindow::on_color()
{
  log( "-------------------------\nColorise");
  log("...en cours") ;
  
  Image temp = *actu ;
  std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
  output.push_back( temp.color_direction( option ) ) ;
  std::chrono::duration<double> sec = std::chrono::system_clock::now() - start;

  actu = --(output.end());

  output_option.push_back("Colorise : "+ToStringOption(option) ) ;
  actu_option = --(output_option.end());

  outputGtk.set((*actu).toGtk(zoom_actu));
  log( " -->  " + std::to_string(sec.count()) + "  Seconds\n-------------------------" ) ;
}

/*
DETECTION
*/

void MainWindow::on_detection()
{
  log( "-------------------------\nDétection de contour");

  log( "--Lissage--");
  log( "  * type : "+ToStringTypeCalcul(option.lissage_type) ) ;
  log( "  * size : "+std::to_string(option.lissage_size) ) ;
  log( "  * sigma : "+std::to_string(option.lissage_sigma) ) ;
  log( "--Filtre Différentiel--");
  log( "  * taille filtre : "+std::to_string(option.filtre.rows)) ;
  log( "  * filtre : "+option.filtre.name) ;
  log( "  * direction : "+ToStringDir(option.direction)) ;
  log( "--Seuillage--");
  log( "  * seuillage : "+ToStringSeuil(option.seuil)) ;
  log( "  * méthode : "+ToStringTypeCalcul(option.seuil_calcul)) ;
  log( "  * val unique : "+std::to_string(option.seuil_val)) ;
  log( "  * fenetre : "+std::to_string(option.seuil_fenetre)) ;
  log( "  * bas : "+std::to_string(option.seuil_bas)) ;
  log( "  * haut : "+std::to_string(option.seuil_haut)) ;
  log( "--Fermeture--");
  log( "  * size : "+std::to_string(option.fermeture_size)) ;
  log( "  * seuil : "+std::to_string(option.fermeture_seuil)) ;
  log( "--Options--");
  log( (option.show_color)?"  * couleur : oui":"  * couleur : non") ;
  log( (option.keep_norme)?"  * intensité en fonction de la norme : oui":"  * intensité en fonction de la norme : non") ;
  log("...en cours") ;

  Image temp = *actu ;
  std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
  output.push_back( temp.detection_contour( option ) ) ;
  std::chrono::duration<double> sec = std::chrono::system_clock::now() - start;
  
  actu = --(output.end());

  output_option.push_back("Détection contour : "+ToStringOption(option) ) ;
  actu_option = --(output_option.end());

  outputGtk.set((*actu).toGtk(zoom_actu));
  log( " -->  " + std::to_string(sec.count()) + "  Seconds\n-------------------------" ) ;
}


/*
GENERAL
*/
void MainWindow::on_show_color()
{
  option.show_color = !option.show_color ;
}

void MainWindow::on_keep_norme()
{
  option.keep_norme = !option.keep_norme ;
}
