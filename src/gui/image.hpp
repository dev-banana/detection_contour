#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "bases.hpp"
#include "filtre.hpp"
#include "option.hpp"

#include <cstdlib>
#include <cstring>
#include <opencv2/opencv.hpp>
#include <omp.h>
#include <gtkmm/image.h>
#include <gtk/gtk.h>

class Image : public cv::Mat
{
        private :    
                               
                Glib::RefPtr<Gdk::Pixbuf> imgGtk ;

                std::vector<std::vector<float>> dirs ;
                std::vector<std::vector<float>> normes ;
                std::vector<std::vector<float>> gradients ;

                void seuil_unique( const Option ) ;
                void seuil_global( const Option ) ;
                void seuil_local( const Option ) ;
                void seuil_ecart_type( const Option ) ;
                void seuil_hysteresis( const Option ) ;

                Image simple_convolution( const Filtre ) ;
                Image mediane( int ) ;

                float normes_mediane( int, int, int, int ) ;
                void normes_stats( float &, float &, int, int, int, int ) ;
                
                
        public :
                Image() ;
                Image( const cv::Mat ) ;
                Image( const Image& ) ;
                ~Image() ;
                Image clone() ;

                cv::Vec3b &operator()( unsigned short int, unsigned short int ) ;
                cv::Vec3b operator()( unsigned short int, unsigned short int ) const ;
                
                float getGradient(unsigned int, unsigned int) ;
                float getNorme(unsigned int, unsigned int) ;
                float getDir(unsigned int, unsigned int) ;

                Glib::RefPtr<Gdk::Pixbuf> toGtk( int ) ;

                bool is_contour( const unsigned int, const unsigned int ) ;
                bool isRGB() ;
                Image toGray( unsigned int ) ;
                Image inverse() ;
                Image rotate90() ;
                Image rotate180() ;


                // FILTRES
                Image lissage( const Option ) ;
                Image filtre_differentiel( const Option ) ;
                Image seuillage( const Option ) ;
                Image affinage( const Option ) ;
                Image fermeture( const Option ) ;

                Image detection_contour( const Option ) ;

                Image color_direction( const Option ) ;


                //HOUGH
                Image hough_transform( const Option, Image& ) ;
} ;

#endif // IMAGE_HPP