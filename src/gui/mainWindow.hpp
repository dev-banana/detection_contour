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
	void log(std::string) ;
public:
  MainWindow() ;
  virtual ~MainWindow() ;

protected:

	void initSignals() ;
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
	void on_size() ;
	void on_size_lissage() ;
	void on_sizeFenetre() ;
	void on_seuilVal() ;
	void on_seuilBas() ;
	void on_seuilHaut() ;

	void on_filtre_reset(int) ;
	void on_filtre_prewitt() ;
	void on_filtre_sobel() ;
	void on_filtre_kirsch() ;
	void on_filtre_5() ;
	void on_filtre() ;
	void on_show_color() ;
	void on_keep_norme() ;
	void on_affine() ;
	void updateFiltre() ;

	void on_inverse() ;
	void on_desaturate() ;
	void on_rotate90() ;
	void on_rotate180() ;
	void on_filtreLissage() ;
	void on_filtreDifferentiel() ;
	void on_filtreSeuil() ;
	void on_filtreAffinage() ;
	void on_filtreFermeture() ;
	void on_filtreDetection() ;

	std::string getFilePath() ;


	//Member widgets:
	Gtk::ToolButton open, save, undo, redo, reset, refresh, newBase, zoomIn, zoomOut, zoomFit ;
	unsigned int zoom_actu ;
	Gtk::Label showFiltre ;
	Gtk::VButtonBox panelLissage, panelFiltre, panelSeuillage, panelAffinage, panelFermeture ;
	Gtk::RadioButton filtreP, filtreS, filtreK, filtre5;
	Gtk::SpinButton size, size_lissage, filtreI, filtreJ, filtreV, sizeFenetre, seuil_val, seuil_bas, seuil_haut ;
	Gtk::Button applyFiltre ;
	Gtk::CheckButton show_color, keep_norme, affine ;
	Gtk::Notebook tabOption ;

	Gtk::Button desaturate, inverse, rotate90, rotate180 ;
	
	//Tree model columns GRAY LEVEL:
	class ModelColumns : public Gtk::TreeModel::ColumnRecord
	{
	public:

	  ModelColumns()
	  { add(m_col_id); add(m_col_name);}

	  Gtk::TreeModelColumn<int> m_col_id;
	  Gtk::TreeModelColumn<Glib::ustring> m_col_name;
	};
	ModelColumns m_Columns;
	Gtk::CellRendererText m_cell;
	Glib::RefPtr<Gtk::ListStore> m_refTreeModel;
	Gtk::ComboBox desaturateType ;


	ModelColumns m_Columns2;
	Gtk::CellRendererText m_cell2;
	Glib::RefPtr<Gtk::ListStore> m_refTreeModel2;
	Gtk::ComboBox direction ;

	ModelColumns m_Columns3;
	Gtk::CellRendererText m_cell3;
	Glib::RefPtr<Gtk::ListStore> m_refTreeModel3;
	Gtk::ComboBox seuil ;

	ModelColumns m_Columns4;
	Gtk::CellRendererText m_cell4;
	Glib::RefPtr<Gtk::ListStore> m_refTreeModel4;
	Gtk::ComboBox lissage_type ;


	Gtk::Button filtreLissage, filtreDifferentiel, seuillage, affinage, fermeture, detection ;

	Gtk::Image inputGtk, outputGtk ;

	Gtk::Label debug ;
	Gtk::TextView consoleBox ;
	Glib::RefPtr< Gtk::TextBuffer > bufferConsole ;

	//Member
	Image input ;
	std::list<Image> output ;
	std::list<Image>::iterator actu ;
	std::list<std::string> output_option ;
	std::list<std::string>::iterator actu_option ;
	
	//Option du filtre
	Option option ;
};

#endif // MAINWINDOW_H