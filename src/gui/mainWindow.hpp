#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "option.hpp"
#include "image.hpp"

#include <string> 
#include <gtkmm.h>
#include <chrono>
#include <cmath>

class MainWindow : public Gtk::Window
{

private:
public:

  MainWindow() ;
  virtual ~MainWindow() ;

protected:
	
	/* MEMBERS */
	Gtk::Image inputGtk, outputGtk ;
	Image input ;
	std::list<Image> output ;
	std::list<Image>::iterator actu ;
	Option option ;
	std::list<std::string> output_option ;
	std::list<std::string>::iterator actu_option ;

	
	std::string getFilePath() ;
	void initSignals() ;

	/***************************
		Models pour Combo Box -> id/name
	***************************/
	class ModelColumns : public Gtk::TreeModel::ColumnRecord
	{
	public:

	  ModelColumns()
	  { add(m_col_id); add(m_col_name);}

	  Gtk::TreeModelColumn<int> m_col_id;
	  Gtk::TreeModelColumn<Glib::ustring> m_col_name;
	};
	
	Gtk::Notebook tabOption ;


	/*******************
		MENU
	*******************/
	unsigned int zoom_actu ;
	Gtk::ToolButton open, save, undo, redo, reset, refresh, newBase, zoomIn, zoomOut, zoomFit ;
	/////////////////
	void on_open() ;
	void on_save() ;
	void on_refresh() ;
	void on_undo() ;
	void on_redo() ;
	void on_reset() ;
	void on_zoomIn() ;
	void on_zoomOut() ;
	void on_zoomFit() ;
	void on_newBase() ;


	/*******************
		DEBUG
	*******************/

	Gtk::Label info_file ;
	Gtk::TextView consoleBox ;
	Glib::RefPtr< Gtk::TextBuffer > bufferConsole ;
	/////////////////
	void log(std::string) ;


	/*******************
		UTILS
	*******************/

	Gtk::Button btn_desaturate, btn_inverse, btn_rotate90, btn_rotate180 ;

	ModelColumns m_Columns;
	Gtk::CellRendererText m_cell;
	Glib::RefPtr<Gtk::ListStore> m_refTreeModel;
	Gtk::ComboBox combo_desaturate ;
	/////////////////
	void on_inverse() ;
	void on_desaturate() ;
	void on_rotate90() ;
	void on_rotate180() ;


	/*******************
		LISSAGE
	*******************/

	Gtk::Button btn_lissage ;
	Gtk::VButtonBox tabLissage ;
	Gtk::ComboBox lissage_type ;
	Gtk::SpinButton lissage_size ;
	Gtk::SpinButton lissage_sigma ;

	ModelColumns m_Columns4;
	Gtk::CellRendererText m_cell4;
	Glib::RefPtr<Gtk::ListStore> m_refTreeModel4;
	/////////////////	
	void on_lissage() ;
	void on_lissage_type() ;
	void on_lissage_size() ;
	void on_lissage_sigma() ;


	/*******************
		FILTRAGE
	*******************/

	Gtk::Button btn_filtre ;
	Gtk::VButtonBox tabFiltre ;
	Gtk::RadioButton filtreP, filtreS, filtreK, filtre5;
	Gtk::Label showFiltre ;
	Gtk::ComboBox filtre_direction ;

	//text input { ; ; ; ... } a parser
	// Gtk::Button applyFiltre ;

	ModelColumns m_Columns2;
	Gtk::CellRendererText m_cell2;
	Glib::RefPtr<Gtk::ListStore> m_refTreeModel2;
	/////////////////	
	void on_filtre() ;
	void on_filtre_prewitt() ;
	void on_filtre_sobel() ;
	void on_filtre_kirsch() ;
	void on_filtre_5() ;
	void update_show_filtre() ;
	void on_filtre_direction() ;


	/*******************
		SEUILLAGE
	*******************/

	Gtk::Button btn_seuillage ;
	Gtk::VButtonBox tabSeuillage ;
	Gtk::ComboBox seuil_type ;
	Gtk::ComboBox seuil_type_calcul ;
	Gtk::SpinButton seuil_fenetre, seuil_val, seuil_bas, seuil_haut ;

	ModelColumns m_Columns3, m_Columns33;
	Gtk::CellRendererText m_cell3, m_cell33;
	Glib::RefPtr<Gtk::ListStore> m_refTreeModel3, m_refTreeModel33;
	/////////////////	
	void on_seuil() ;
	void on_seuil_type() ;
	void on_seuil_type_calcul() ;
	void on_seuil_fenetre() ;
	void on_seuil_val() ;
	void on_seuil_bas() ;
	void on_seuil_haut() ;


	/*******************
		AFFINAGE
	*******************/

	Gtk::Button btn_affinage ;
	Gtk::VButtonBox tabAffinage ;
	/////////////////	
	void on_affinage() ;


	/*******************
		FERMETURE
	*******************/

	Gtk::Button fermeture ;
	Gtk::VButtonBox tabFermeture ;
	Gtk::SpinButton fermeture_size, fermeture_seuil ;
	/////////////////	
	void on_fermeture() ;
	void on_fermeture_size() ;
	void on_fermeture_seuil() ;


	/*******************
		COLOR
	*******************/

	Gtk::Button color ;
	/////////////////	
	void on_color() ;


	/*******************
	  DETECTION_CONTOUR
	*******************/

	Gtk::Button detection ;
	Gtk::VButtonBox tabDetection ;
	/////////////////	 
	void on_detection() ;


	/*******************
	  GENERAL
	*******************/

	Gtk::CheckButton show_color, keep_norme, affine ;
	/////////////////	 
	void on_show_color() ;
	void on_keep_norme() ;
	void on_affine() ;

};

#endif // MAINWINDOW_H