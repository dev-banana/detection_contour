#ifndef OPTION_HPP
#define OPTION_HPP

#include <cstdlib>
#include <iostream>
#include <cstring>
#include <opencv2/opencv.hpp>

#include "filtre.hpp"


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
        MEDIANE,
        ECART_TYPE,
        GAUSSIEN
} ;

inline const std::string ToStringTypeCalcul(e_type_calcul v)
{
    switch (v)
    {
        case MOYENNE:   return "MOYENNE" ;
        case MEDIANE:   return "MEDIANE" ;
        case ECART_TYPE: return "ECART_TYPE" ;
        case GAUSSIEN: return "GAUSSIEN" ;
        default:      return "[Unknown SEUIL CALCUL]";
    }
}


enum e_type_norme
{
        EUCLIDIENNE=0,
        ABSOLUE,
        MAX
} ;

inline const std::string ToStringTypeNorme(e_type_norme v)
{
    switch (v)
    {
        case EUCLIDIENNE:   return "EUCLIDIENNE" ;
        case ABSOLUE:   return "ABSOLUE" ;
        case MAX: return "MAX" ;
        default:      return "[Unknown TYPE NORME]";
    }
}

enum e_hough_type
{
        BOTH=0,
        DROITE,
        CERCLE
} ;

inline const std::string ToStringHoughType(e_hough_type v)
{
    switch (v)
    {
        case BOTH:   return "BOTH" ;
        case DROITE:   return "DROITE" ;
        case CERCLE: return "CERCLE" ;
        default:      return "[Unknown HOUGH TYPE]";
    }
}

/**
* Classe permettant de passer facilement les options choisies par l'utilisateurs
* au diverses fonctions de traitement.
*/

class Option
{
        private :    

        public :
            
            Option() ;
            ~Option(){}


                e_type_calcul lissage_type ;
                unsigned int lissage_size ;
                float lissage_sigma ;

                Filtre filtre ;
                e_direction direction ;
                e_type_norme type_norme ;

                e_seuil seuil ;
                e_type_calcul seuil_calcul ;
                unsigned int seuil_val ;
                unsigned int seuil_fenetre ;
                unsigned int seuil_bas ;
                unsigned int seuil_haut ;

                unsigned int fermeture_size ;
                unsigned int fermeture_seuil ;
                
                bool show_color ;
                bool keep_norme ;

                unsigned int hough_seuil_lines ;
                unsigned int hough_seuil_circles ;
                unsigned int hough_rayon_min;
                unsigned int hough_rayon_max ;
                unsigned int hough_distance_min ;
                bool hough_calcul_edge ;
                bool hough_on_origin ;
                bool hough_precis ;
                bool hough_affiche_acc ;
                e_hough_type hough_type ;

                
            void set_direction( unsigned int ) ;
            void set_type_norme( unsigned int ) ;
            void set_seuil( unsigned int ) ;
            void set_seuil_calcul( unsigned int ) ;
            void set_lissage_type( unsigned int ) ;
            void set_hough_type( unsigned int ) ;

            friend std::ostream& operator<<(std::ostream& out, const Option& r){
              return out << "{ "<<
                "lissage_type : "<<ToStringTypeCalcul(r.lissage_type)<<"; "<<
                "lissage_size : "<<std::to_string(r.lissage_size)<<"; "<<
                "lissage_sigma : "<<std::to_string(r.lissage_sigma)<<"; "<<
                "filtre : "<<(r.filtre.name)<<"; "<<
                "direction : "<<ToStringDir(r.direction)<<"; "<<
                "type_norme : "<<ToStringTypeNorme(r.type_norme)<<"; "<<
                "seuil : "<<ToStringSeuil(r.seuil)<<"; "<<
                "seuil_calcul : "<<ToStringTypeCalcul(r.seuil_calcul)<<"; "<<
                "seuil_val : "<<std::to_string(r.seuil_val)<<"; "<<
                "seuil_fenetre : "<<std::to_string(r.seuil_fenetre)<<"; "<<
                "seuil_bas : "<<std::to_string(r.seuil_bas)<<"; "<<
                "seuil_haut : "<<std::to_string(r.seuil_haut)<<"; "<<
                "fermeture_size : "<<std::to_string(r.fermeture_size)<<"; "<<
                "fermeture_seuil : "<<std::to_string(r.fermeture_seuil)<<"; "<<
                "hough_seuil_lines : "<<std::to_string(r.hough_seuil_lines)<<"; "<<
                "hough_seuil_circles : "<<std::to_string(r.hough_seuil_circles)<<"; "<<
                "hough_rayon_min : "<<std::to_string(r.hough_rayon_min)<<"; "<<
                "hough_rayon_max : "<<std::to_string(r.hough_rayon_max)<<"; "<<
                "hough_distance_min : "<<std::to_string(r.hough_distance_min)<<"; "<<
                ((r.hough_calcul_edge)?"hough_calcul_edge : oui":"hough_calcul_edge : non")<<"; "<<
                ((r.hough_on_origin)?"hough_on_origin : oui":"hough_on_origin : non")<<"; "<<
                ((r.hough_precis)?"hough_precis : oui":"hough_precis : non")<<"; "<<
                "hough_type : "<<ToStringHoughType(r.hough_type)<<"; "<<
                ((r.show_color)?"couleur : oui":"couleur : non")<<"; "<<
                ((r.keep_norme)?"color_norme : oui":"color_norme : non")<<
              " }";
            }
} ;

inline const std::string ToStringOption(Option v)
{
    std::stringstream buffer;
    buffer << v ;
    return buffer.str() ;
}

#endif // OPTION_HPP