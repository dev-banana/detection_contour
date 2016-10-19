#ifndef OPTION_HPP
#define OPTION_HPP

#include <cstdlib>
#include <iostream>
#include <cstring>
#include <opencv2/opencv.hpp>
#include <omp.h>
#include <gtkmm/image.h>
#include <gtk/gtk.h>


enum e_direction
{
        HORIZONTAL=0,
        VERTICAL,
        BI_DIRECTIONNEL,
        MULTI_DIRECTIONNEL
} ;

inline const std::string ToStringDir(e_direction v)
{
    switch (v)
    {
        case HORIZONTAL:   return "HORIZONTAL" ;
        case VERTICAL:   return "VERTICAL" ;
        case BI_DIRECTIONNEL: return "BI_DIRECTIONNEL" ;
        case MULTI_DIRECTIONNEL: return "MULTI_DIRECTIONNEL" ;
        default:      return "[Unknown DIRECTION]";
    }
}

enum e_seuil
{
        UNIQUE=0,
        GLOBAL,
        LOCAL,
        HYSTERESIS_AUTO,
        HYSTERESIS
} ;

inline const std::string ToStringSeuil(e_seuil v)
{
    switch (v)
    {
        case UNIQUE:   return "UNIQUE" ;
        case GLOBAL:   return "GLOBAL" ;
        case LOCAL: return "LOCAL" ;
        case HYSTERESIS_AUTO: return "HYSTERESIS_AUTO" ;
        case HYSTERESIS: return "HYSTERESIS" ;
        default:      return "[Unknown SEUIL]";
    }
}

enum e_type_calcul
{
        MOYENNE=0,
        ECART_TYPE,
        MEDIANE,
        PASSE_BAS_3,
        PASSE_BAS_5
} ;

inline const std::string ToStringTypeCalcul(e_type_calcul v)
{
    switch (v)
    {
        case MOYENNE: return "MOYENNE" ;
        case ECART_TYPE: return "ECART_TYPE" ;
        case MEDIANE: return "MEDIANE" ;
        case PASSE_BAS_3: return "PASSE_BAS_3" ;
        case PASSE_BAS_5: return "PASSE_BAS_5" ;
        default:      return "[Unknown TYPE CALCUL]";
    }
}

enum e_moyenne
{
        EUCLIDIENNE=0,
        MANHATTAN,
        ABSOLU
} ;

inline const std::string ToStringMoyenne(e_moyenne v)
{
    switch (v)
    {
        case EUCLIDIENNE:   return "EUCLIDIENNE" ;
        case MANHATTAN:   return "MANHATTAN" ;
        case ABSOLU: return "ABSOLU" ;
        default:      return "[Unknown MOYENNE]";
    }
}

class Option
{
        private :    

        public :

                e_type_calcul lissage_type ;
                int lissage_size ;

                cv::Mat filtre ;
                int filtre_size ;
                e_moyenne type_moyenne ;
                e_direction direction ;

                e_seuil seuil ;
                e_type_calcul seuil_type ;
                unsigned int seuil_val ;
                unsigned int seuil_fenetre ;
                unsigned int seuil_bas ;
                unsigned int seuil_haut ;
                
                bool show_color ;
                bool keep_norme ;

                Option() ;
                ~Option(){}

                void set_direction( unsigned int ) ;
                void set_seuil( unsigned int ) ;
                void set_seuil_type( unsigned int ) ;
                void set_lissage_type( unsigned int ) ;
                void set_moyenne( unsigned int ) ;

                void reset_filtre( int ) ;
                void prewitt() ;
                void sobel() ;
                void kirsch() ;
                void filtre_5() ;

                friend std::ostream& operator<<(std::ostream& out, const Option& r){
                  return out << "{"<<
                    // "taille_filtre : "<<std::to_string(r.filtre_size)<<"; "<<
                    // "direction : "<<ToStringDir(r.direction)<<"; "<<
                    // "seuillage : "<<ToStringSeuil(r.seuil)<<"; "<<
                    // "val_seuillage : "<<std::to_string(r.seuil_val)<<"; "<<
                    // "fenetre_seuillage : "<<std::to_string(r.seuil_fenetre)<<"; "<<
                    // "seuil_bas : "<<std::to_string(r.seuil_bas)<<"; "<<
                    // "seuil_haut : "<<std::to_string(r.seuil_haut)<<"; "<<
                    // ((r.show_color)?"couleur : oui":"couleur : non")<<"; "<<
                    // ((r.keep_norme)?"color_norme : oui":"color_norme : non")<<"; "<<
                    // ((r.affine)?"affine : oui":"affine : non")<<
                  "}";
                }
} ;

inline const std::string ToStringOption(Option v)
{
    std::stringstream buffer;
    buffer << v ;
    return buffer.str() ;
}

#endif // OPTION_HPP