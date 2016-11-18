#ifndef HOUGH_HPP
#define HOUGH_HPP


#include <opencv2/opencv.hpp>
#include <chrono>

#include <omp.h>

#include "image.hpp"


void hough_lines( Image, Image, unsigned int, bool ) ;
void hough_circles( Image, Image, unsigned int, unsigned int, unsigned int, unsigned int, bool ) ;


// std::vector< std::pair< cv::Point, cv::Point > > hough_get_lines( unsigned int ) ;
// void hough_draw_lines( const Option, Image ) ;


// //circles
// void hough_draw_circles( const Option, Image ) ;

#endif // HOUGH_HPP