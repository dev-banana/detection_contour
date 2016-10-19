#include "mainWindow.hpp"

MainWindow::MainWindow()
{
  set_title( "Analyse d'image" ) ;
  set_border_width( 5 ) ;
  set_size_request(0.9*gdk_screen_width(), 0.9*gdk_screen_height()) ;
  int w, h ;
  get_size_request(w, h) ;

  static Gtk::VBox mainPanel ;
  static Gtk::Toolbar toolbar ;
  static Gtk::HBox panelView ;

  static Gtk::ScrolledWindow scrollButtonBox ;
  scrollButtonBox.set_size_request((w/4)-100, h-80) ;
  static Gtk::VButtonBox buttonBox ;
  buttonBox.set_valign(Gtk::ALIGN_START);
  scrollButtonBox.add( buttonBox ) ;

  static Gtk::ScrolledWindow scrollConsole ;
  scrollConsole.set_size_request((w/4)-120, h-80) ;
  consoleBox.set_margin_left(35) ;
  consoleBox.override_background_color ( Gdk::RGBA( "black" )    ) ;
  consoleBox.override_color ( Gdk::RGBA( "white" )    ) ;
  consoleBox.set_editable(false) ;

  //TOOLBAR
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



  //LABELS
  static Gtk::Label labLissage ;
  labLissage.set_markup("<b>Taille du filtre</b>");
  static Gtk::Label labLissageT ;
  labLissageT.set_markup("<b>Type de filtre</b>");

  static Gtk::Label labSize ;
  labSize.set_markup("<b>Taille du filtre</b>");
  static Gtk::Label labI ;
  labI.set_markup("<b>i</b>");
  static Gtk::Label labJ ;
  labJ.set_markup("<b>j</b>");
  static Gtk::Label labV ;
  labV.set_markup("<b>Valeur [i][j]</b>");
  static Gtk::Label labF ;
  labF.set_markup("<b>Filtre</b>");
  static Gtk::Label labColor ;
  labColor.set_markup("<b>Montrer les couleurs</b>");
  static Gtk::Label labNorme ;
  labNorme.set_markup("<b>Couleur relative à la norme du gradient</b>");

  static Gtk::Label labSizeFenetre ;
  labSizeFenetre.set_markup("<b>fenêtre</b>");
  static Gtk::Label labSeuil_val ;
  labSeuil_val.set_markup("<b>seuil fixe</b>");
  static Gtk::Label labSeuil_bas ;
  labSeuil_bas.set_markup("<b>seuil bas</b>");
  static Gtk::Label labSeuil_haut ;
  labSeuil_haut.set_markup("<b>seuil haut</b>");



  //TOOLS

  static Gtk::HButtonBox h0 ;
  desaturate.set_label( "Désaturer" ) ;
  
  m_refTreeModel = Gtk::ListStore::create(m_Columns);
  desaturateType.set_model(m_refTreeModel);
  //Fill the ComboBox's Tree Model:
  Gtk::TreeModel::Row row = *(m_refTreeModel->append());
  row[m_Columns.m_col_id] = 1;
  row[m_Columns.m_col_name] = "Clarté";
  desaturateType.set_active(row);
  row = *(m_refTreeModel->append());
  row[m_Columns.m_col_id] = 2;
  row[m_Columns.m_col_name] = "Luminosité";
  row = *(m_refTreeModel->append());
  row[m_Columns.m_col_id] = 3;
  row[m_Columns.m_col_name] = "Moyenne";
  desaturateType.pack_start(m_Columns.m_col_name);
  h0.add( desaturateType ) ;
  h0.add( desaturate ) ;
  buttonBox.add( h0 ) ;

  inverse.set_label( "Inverser" ) ;
  buttonBox.add( inverse ) ;

  static Gtk::HButtonBox hr ;
  rotate90.set_label( "rotate90" ) ;
  hr.add( rotate90 ) ;
  rotate180.set_label( "rotate180" ) ;
  hr.add( rotate180 ) ;
  buttonBox.add( hr ) ;


  static Gtk::HSeparator sp4 ;
  buttonBox.add( sp4 ) ;

  // FONCTIONS
  filtreLissage.set_label( "Filtre de Lissage" ) ;
  buttonBox.add( filtreLissage ) ;
  filtreDifferentiel.set_label( "Filtre Différentiel" ) ;
  buttonBox.add( filtreDifferentiel ) ;
  seuillage.set_label( "Seuillage" ) ;
  buttonBox.add( seuillage ) ;
  affinage.set_label( "Affinage" ) ;
  buttonBox.add( affinage ) ;
  fermeture.set_label( "Fermeture" ) ;
  fermeture.set_sensitive(false) ;
  buttonBox.add( fermeture ) ;
  detection.set_label( "Détection de contours" ) ;
  buttonBox.add( detection ) ;


  static Gtk::HSeparator sp3 ;
  buttonBox.add( sp3 ) ;

  static Gtk::HButtonBox h25 ;
  h25.add( show_color ) ;
  h25.add( labColor ) ;
  show_color.set_active( true ) ;
  buttonBox.add( h25 ) ;

  static Gtk::HButtonBox h26 ;
  h26.add( keep_norme ) ;
  h26.add( labNorme ) ;
  keep_norme.set_active( true ) ;
  buttonBox.add( h26 ) ;


  static Gtk::HSeparator sp667 ;
  buttonBox.add( sp667 ) ;

  // FILTRE

  static Gtk::HButtonBox lisT ;
  lisT.add( labLissageT ) ;
  m_refTreeModel4 = Gtk::ListStore::create(m_Columns4);
  lissage_type.set_model(m_refTreeModel4);
  Gtk::TreeModel::Row row4 = *(m_refTreeModel4->append());
  row4[m_Columns4.m_col_id] = MOYENNE;
  row4[m_Columns4.m_col_name] = "Moyenne";
  lissage_type.set_active(row4);
  row4 = *(m_refTreeModel4->append());
  row4[m_Columns4.m_col_id] = PASSE_BAS_3;
  row4[m_Columns4.m_col_name] = "Passe bas 3";
  row4 = *(m_refTreeModel4->append());
  row4[m_Columns4.m_col_id] = PASSE_BAS_5;
  row4[m_Columns4.m_col_name] = "Passe bas 5";
  lissage_type.pack_start(m_Columns4.m_col_name);
  lisT.add( lissage_type ) ;
  panelLissage.add( lisT ) ;

  static Gtk::HSeparator sp555 ;
  panelLissage.add( sp555 ) ;


  static Gtk::HButtonBox lis ;
  lis.add( labLissage ) ;
  size_lissage.set_digits(0);
  size_lissage.set_range(3, 99);
  size_lissage.set_increments(2, 2);   
  lis.add( size_lissage ) ;
  panelLissage.add( lis ) ;


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
  panelFiltre.add( h5 ) ;

  static Gtk::HSeparator sp42 ;
  panelFiltre.add( sp42 ) ;

  static Gtk::HButtonBox h1 ;
  size.set_digits(0);
  size.set_range(3, 99);
  size.set_increments(2, 2);   
  h1.add( labSize ) ;
  h1.add( size ) ;
  panelFiltre.add( h1 ) ;

  static Gtk::HButtonBox h2 ;
  filtreI.set_digits(0);
  filtreI.set_range(0, option.filtre_size-1);
  filtreI.set_increments(1, 1); 
  h2.add(labI);
  h2.add( filtreI ) ;
  panelFiltre.add( h2 ) ;

  static Gtk::HButtonBox h3 ;
  filtreJ.set_digits(0);
  filtreJ.set_range(0, option.filtre_size-1);
  filtreJ.set_increments(1, 1); 
  h3.add(labJ);
  h3.add( filtreJ ) ;
  panelFiltre.add( h3 ) ;

  static Gtk::HButtonBox h4 ;
  filtreV.set_digits(0);
  filtreV.set_range(-99, 99);
  filtreV.set_increments(1, 1); 
  h4.add(labV);
  h4.add( filtreV ) ;
  panelFiltre.add( h4 ) ;

  applyFiltre.set_label( "MAJ Valeur" ) ;
  panelFiltre.add( applyFiltre ) ;


  static Gtk::HSeparator sp5 ;
  panelFiltre.add( sp5 ) ;

  m_refTreeModel2 = Gtk::ListStore::create(m_Columns2);
  direction.set_model(m_refTreeModel2);
  Gtk::TreeModel::Row row2 = *(m_refTreeModel2->append());
  row2[m_Columns2.m_col_id] = HORIZONTAL;
  row2[m_Columns2.m_col_name] = "Horizontal";
  row2 = *(m_refTreeModel2->append());
  row2[m_Columns2.m_col_id] = VERTICAL;
  row2[m_Columns2.m_col_name] = "Vertical";
  row2 = *(m_refTreeModel2->append());
  row2[m_Columns2.m_col_id] = BI_DIRECTIONNEL;
  row2[m_Columns2.m_col_name] = "Bi-Directionnel";
  direction.set_active(row2);
  row2 = *(m_refTreeModel2->append());
  row2[m_Columns2.m_col_id] = MULTI_DIRECTIONNEL;
  row2[m_Columns2.m_col_name] = "Multi-Directionnel";
  direction.pack_start(m_Columns2.m_col_name);
  panelFiltre.add( direction ) ;

  static Gtk::HSeparator sp55 ;
  panelFiltre.add( sp55 ) ;

  
  m_refTreeModel3 = Gtk::ListStore::create(m_Columns3);
  seuil.set_model(m_refTreeModel3);
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
  row3[m_Columns3.m_col_id] = ECART_TYPE;
  row3[m_Columns3.m_col_name] = "Seuillage Ecart-type";
  row3 = *(m_refTreeModel3->append());
  row3[m_Columns3.m_col_id] = HYSTERESIS_AUTO;
  row3[m_Columns3.m_col_name] = "Seuillage Hysteresis Auto";
  row3 = *(m_refTreeModel3->append());
  row3[m_Columns3.m_col_id] = HYSTERESIS;
  row3[m_Columns3.m_col_name] = "Seuillage Hysteresis";
  seuil.set_active(row3);
  seuil.pack_start(m_Columns3.m_col_name);
  panelSeuillage.add( seuil ) ;

  static Gtk::HButtonBox h36 ;
  sizeFenetre.set_digits(0);
  sizeFenetre.set_range(3, 99);
  sizeFenetre.set_increments(2, 2); 
  sizeFenetre.set_value(15);   
  h36.add( labSizeFenetre ) ;
  h36.add( sizeFenetre ) ;
  panelSeuillage.add( h36 ) ;

  static Gtk::HButtonBox h361 ;
  seuil_val.set_digits(0);
  seuil_val.set_range(0, 255);
  seuil_val.set_increments(1, 1); 
  seuil_val.set_value(50);   
  h361.add( labSeuil_val ) ;
  h361.add( seuil_val ) ;
  panelSeuillage.add( h361 ) ;

  static Gtk::HButtonBox h362 ;
  seuil_bas.set_digits(0);
  seuil_bas.set_range(0, 255);
  seuil_bas.set_increments(1, 1); 
  seuil_bas.set_value(44);   
  h362.add( labSeuil_bas ) ;
  h362.add( seuil_bas ) ;
  panelSeuillage.add( h362 ) ;

  static Gtk::HButtonBox h363 ;
  seuil_haut.set_digits(0);
  seuil_haut.set_range(0, 255);
  seuil_haut.set_increments(1, 1); 
  seuil_haut.set_value(60);   
  h363.add( labSeuil_haut ) ;
  h363.add( seuil_haut ) ;
  panelSeuillage.add( h363 ) ;

  static Gtk::HSeparator sp66 ;
  panelSeuillage.add( sp66 ) ;



  tabOption.set_size_request( (w/4)-100, -1 ) ;
  tabOption.append_page(panelLissage, "Lissage");
  tabOption.append_page(panelFiltre, "Filtre");
  tabOption.append_page(panelSeuillage, "Seuillage");
  tabOption.append_page(panelAffinage, "Affinage");
  tabOption.append_page(panelFermeture, "Fermeture");
  buttonBox.add(tabOption);


  //IMAGES
  static Gtk::ScrolledWindow scrollImage ;
  scrollImage.set_size_request((w/2)+100, h-80) ;
  static Gtk::HBox panelImage ;
  outputGtk.set_margin_left(10) ;

  panelImage.add( inputGtk ) ;
  panelImage.add( outputGtk ) ;
  scrollImage.add( panelImage ) ;

  
  //DEBUG
  debug.set_alignment( 0 ) ;
  bufferConsole = Gtk::TextBuffer::create();
  consoleBox.set_buffer(bufferConsole);
  scrollConsole.add( consoleBox ) ;




  //PACK
  panelView.pack_start( scrollButtonBox, TRUE, TRUE, 5 ) ;
  panelView.pack_start( scrollImage, TRUE, TRUE, 5 ) ;
  panelView.pack_start( scrollConsole, TRUE, TRUE, 5 ) ;

  mainPanel.pack_start( toolbar, FALSE, FALSE, 5 ) ;
  mainPanel.pack_start( panelView, FALSE, FALSE, 5 ) ;
  mainPanel.pack_start( debug, FALSE, FALSE, 5 ) ;

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
  size.signal_value_changed().connect( sigc::mem_fun(*this, &MainWindow::on_size) ) ;
  size_lissage.signal_value_changed().connect( sigc::mem_fun(*this, &MainWindow::on_size_lissage) ) ;
  sizeFenetre.signal_value_changed().connect( sigc::mem_fun(*this, &MainWindow::on_sizeFenetre) ) ;
  seuil_val.signal_value_changed().connect( sigc::mem_fun(*this, &MainWindow::on_seuilVal) ) ;
  seuil_bas.signal_value_changed().connect( sigc::mem_fun(*this, &MainWindow::on_seuilBas) ) ;
  seuil_haut.signal_value_changed().connect( sigc::mem_fun(*this, &MainWindow::on_seuilHaut) ) ;
  applyFiltre.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_filtre) ) ;
  desaturate.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_desaturate) ) ;
  inverse.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_inverse) ) ;
  rotate90.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_rotate90) ) ;
  rotate180.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_rotate180) ) ;
  filtreLissage.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_filtreLissage) ) ;
  filtreDifferentiel.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_filtreDifferentiel) ) ;
  seuillage.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_filtreSeuil) ) ;
  affinage.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_filtreAffinage) ) ;
  fermeture.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_filtreFermeture) ) ;
  detection.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_filtreDetection) ) ;
  
  filtreP.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_filtre_prewitt) ) ;
  filtreS.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_filtre_sobel) ) ;
  filtreK.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_filtre_kirsch) ) ;
  filtre5.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_filtre_5) ) ;
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
      debug.set_text( "   >:  " + path + " - " + std::to_string(input.rows) +" * "+ std::to_string(input.cols) ) ;
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

void MainWindow::on_size()
{
  if( (int)size.get_value() % 2 == 0)
  {
    size.set_value( size.get_value() - 1 ) ;
    return ;
  }
  option.filtre_size = size.get_value() ;

  filtreI.set_range(0, option.filtre_size-1);
  filtreJ.set_range(0, option.filtre_size-1);

  on_filtre_reset(option.filtre_size) ;

  updateFiltre() ;
}

void MainWindow::on_size_lissage()
{
  if( (int)size_lissage.get_value() % 2 == 0)
  {
    size_lissage.set_value( size_lissage.get_value() - 1 ) ;
    return ;
  }
  option.lissage_size = size_lissage.get_value() ;
}

void MainWindow::on_sizeFenetre()
{
  if( (int)sizeFenetre.get_value() % 2 == 0)
  {
    sizeFenetre.set_value( sizeFenetre.get_value() - 1 ) ;
    return ;
  }
  option.seuil_fenetre = sizeFenetre.get_value() ;
}

void MainWindow::on_seuilVal()
{
  option.seuil_val = seuil_val.get_value() ;
}

void MainWindow::on_seuilBas()
{
  option.seuil_bas = seuil_bas.get_value() ;
}

void MainWindow::on_seuilHaut()
{
  option.seuil_haut = seuil_haut.get_value() ;
}

void MainWindow::on_show_color()
{
  option.show_color = !option.show_color ;
}

void MainWindow::on_keep_norme()
{
  option.keep_norme = !option.keep_norme ;
}

void MainWindow::on_filtre_reset(int sizeIn)
{
  size.set_value( sizeIn ) ;
  option.reset_filtre( sizeIn ) ;
}

void MainWindow::on_filtre_prewitt()
{
  on_filtre_reset(3) ;
  option.prewitt() ;
  updateFiltre() ;
}
void MainWindow::on_filtre_sobel()
{
  on_filtre_reset(3) ;
  option.sobel() ;
  updateFiltre() ;
}
void MainWindow::on_filtre_kirsch()
{
  on_filtre_reset(3) ;
  option.kirsch() ;
  updateFiltre() ;
}
void MainWindow::on_filtre_5()
{
  on_filtre_reset(5) ;
  option.filtre_5() ;
  updateFiltre() ;
}

void MainWindow::updateFiltre()
{
  std::stringstream buffer;
  buffer << option.filtre ;
  std::string mat = buffer.str() ;
  showFiltre.set_text( mat ) ;
}

void MainWindow::on_filtre()
{
  int i = filtreI.get_value() ;
  int j = filtreJ.get_value() ;
  int v = filtreV.get_value() ;

  if( i  < option.filtre_size && j < option.filtre_size)
  {
    option.filtre.at<float>(i,j) = v ;
  }

  updateFiltre() ;
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

void MainWindow::on_desaturate()
{
  Gtk::TreeModel::iterator iter = desaturateType.get_active();
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

void MainWindow::on_filtreLissage()
{
  Gtk::TreeModel::iterator iter = lissage_type.get_active();
  int id = (*iter)[m_Columns.m_col_id];
  option.set_lissage_type( id ) ;

  log( "-------------------------\nFiltre Moyen");
  log( "  * fenetre : "+std::to_string(option.lissage_size) ) ;
  log( "  * fenetre : "+ToStringTypeCalcul(option.lissage_type) ) ;
  log("...en cours") ;
  Image temp = *actu ;

  std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
  output.push_back( temp.filtre_lissage( option ) ) ;
  std::chrono::duration<double> sec = std::chrono::system_clock::now() - start;

  actu = --(output.end());

  output_option.push_back("Filtre Moyen : "+ToStringOption(option) ) ;
  actu_option = --(output_option.end());

  outputGtk.set((*actu).toGtk(zoom_actu));
  log( " -->  " + std::to_string(sec.count()) + " seconds\n-------------------------" ) ;
}

void MainWindow::on_filtreDifferentiel()
{
  Gtk::TreeModel::iterator iter = direction.get_active() ;
  int id = (*iter)[m_Columns.m_col_id];

  Gtk::TreeModel::iterator iter2 = seuil.get_active() ;
  int id2 = (*iter2)[m_Columns.m_col_id];

  option.set_direction( id ) ;
  option.set_seuil( id2 ) ;

  std::stringstream buffer;
  buffer << option.filtre ;
  std::string mat = buffer.str() ;

  log( "-------------------------\nDétection de Contours");
  log( "  * taille filtre : "+std::to_string(option.filtre_size)) ;
  log( "  * filtre : \n"+mat) ;
  log( "  * direction : "+ToStringDir(option.direction)) ;
  log( (option.show_color)?"  * couleur : oui":"  * couleur : non") ;
  log( (option.keep_norme)?"  * intensité en fonction de la norme : oui":"  * intensité en fonction de la norme : non") ;
  log("...en cours") ;
  
  Image temp = *actu ;
  std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
  
  output.push_back( temp.filtre_differentiel( option ) ) ;
  
  std::chrono::duration<double> sec = std::chrono::system_clock::now() - start;

  actu = --(output.end());

  output_option.push_back("Detection Contours : "+ToStringOption(option) ) ;
  actu_option = --(output_option.end());

  outputGtk.set((*actu).toGtk(zoom_actu));
  log( " -->  " + std::to_string(sec.count()) + " seconds\n-------------------------" ) ;
}

void MainWindow::on_filtreSeuil()
{
  Gtk::TreeModel::iterator iter2 = seuil.get_active() ;
  int id2 = (*iter2)[m_Columns.m_col_id];
  option.set_seuil( id2 ) ;

  log( "-------------------------\nSeuillage");
  log( "  * seuillage : "+ToStringSeuil(option.seuil)) ;
  log( "  * val seuillage : "+std::to_string(option.seuil_val)) ;
  log( "  * fenetre seuillage : "+std::to_string(option.seuil_fenetre)) ;
  log("...en cours") ;
  Image temp = *actu ;

  std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
  output.push_back( temp.filtre_seuillage( option ) ) ;
  std::chrono::duration<double> sec = std::chrono::system_clock::now() - start;
  
  actu = --(output.end());

  output_option.push_back("Seuillage : "+ToStringOption(option) ) ;
  actu_option = --(output_option.end());

  outputGtk.set((*actu).toGtk(zoom_actu));
  log( " -->  " + std::to_string(sec.count()) + " seconds\n-------------------------" ) ;
}

void MainWindow::on_filtreAffinage()
{
  log( "-------------------------\nFiltre Moyen");
  log("...en cours") ;
  Image temp = *actu ;

  std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
  output.push_back( temp.filtre_affinage( option ) ) ;
  std::chrono::duration<double> sec = std::chrono::system_clock::now() - start;
  
  actu = --(output.end());

  output_option.push_back("Affinage : "+ToStringOption(option) ) ;
  actu_option = --(output_option.end());

  outputGtk.set((*actu).toGtk(zoom_actu));
  log( " -->  " + std::to_string(sec.count()) + " seconds\n-------------------------" ) ;
}

void MainWindow::on_filtreFermeture()
{
  log( "-------------------------\nFermeture");
  log("...en cours") ;
  
  Image temp = *actu ;
  std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
  output.push_back( temp.fermeture( option ) ) ;
  std::chrono::duration<double> sec = std::chrono::system_clock::now() - start;

  actu = --(output.end());

  output_option.push_back("Fermeture : "+ToStringOption(option) ) ;
  actu_option = --(output_option.end());

  outputGtk.set((*actu).toGtk(zoom_actu));
  log( " -->  " + std::to_string(sec.count()) + " seconds\n-------------------------" ) ;
}

void MainWindow::on_filtreDetection()
{
  log( "-------------------------\nDétection de contour");
  log("...en cours") ;
  Image temp = *actu ;

  std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
  output.push_back( temp.detection_contour( option ) ) ;
  std::chrono::duration<double> sec = std::chrono::system_clock::now() - start;
  
  actu = --(output.end());

  output_option.push_back("Détection contour : "+ToStringOption(option) ) ;
  actu_option = --(output_option.end());

  outputGtk.set((*actu).toGtk(zoom_actu));
  log( " -->  " + std::to_string(sec.count()) + " seconds\n-------------------------" ) ;
}