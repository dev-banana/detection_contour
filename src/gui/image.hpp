#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "option.hpp"

#include <cstdlib>
#include <cstring>
#include <opencv2/opencv.hpp>
#include <omp.h>
#include <gtkmm/image.h>
#include <gtk/gtk.h>

#define ICI fprintf(stderr,"%s:%d\n",__FILE__,__LINE__)

class Image : public cv::Mat
{
        private :    

                float * dirs ;
                float * normes ;
                float * gradients ;

                Glib::RefPtr<Gdk::Pixbuf> imgGtk ;
                cv::Mat rotate45( const cv::Mat& ) ;
                void seuil_unique( const Option ) ;
                void seuil_global( const Option ) ;
                void seuil_local( const Option ) ;
                void seuil_ecart_type( const Option ) ;
                void seuil_hysteresis( const Option ) ;

                void color( const Option ) ;

        public :

                Image() ;
                Image( const cv::Mat ) ;
                Image( const Image& ) ;
                ~Image() ;
                Image copy() ;

                bool isRGB() ;
                Image toGray( unsigned int ) ;
                Image inverse() ;
                Image normalize() ;
                Image rotate90() ;
                Image rotate180() ;

                Glib::RefPtr<Gdk::Pixbuf> toGtk( int ) ;

                // FILTRES
                Image filtre_lissage( const Option ) ;
                Image filtre_differentiel( const Option ) ;
                Image filtre_seuillage( const Option ) ;
                Image filtre_affinage( const Option ) ;
                Image fermeture( const Option ) ;

                void Vfiltre_lissage( const Option ) ;
                void Vfiltre_differentiel( const Option ) ;
                void Vfiltre_seuillage( const Option ) ;
                void Vfiltre_affinage( const Option ) ;
                void Vfermeture( const Option ) ;

                Image detection_contour( const Option ) ;
} ;

#endif // IMAGE_HPP