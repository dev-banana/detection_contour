
#include "hough.hpp"

void hough_lines( Image input, Image output, unsigned int seuil, bool precision )
{ 
    //FEED ACCUMULATEUR

    unsigned int * accumulator ;
    std::vector<cv::Point> * accumulatorPoints ;

    double hough_h = (double)sqrt(pow((double)input.cols,2) + pow((double)input.rows,2)) ;
    double accu_w = hough_h + 1 ;
    double accu_h = 360 ;
    int sizeAccu = (int)(accu_h *  accu_w) + 1 ;

    accumulator  = ( unsigned int * )calloc( sizeAccu, sizeof(unsigned int) ) ;  
    accumulatorPoints = ( std::vector<cv::Point> * )calloc( sizeAccu, sizeof(std::vector<cv::Point>) ) ;  

    double center_x = input.cols/2;  
    double center_y = input.rows/2;  

    for(int i=0;i<input.rows;i++)
    {  
        for(int j=0;j<input.cols;j++)  
        {  
            if( input.is_contour(i, j) && i != 0 && j != 0 && i != input.rows-1 && j != input.cols-1 )
            {
                for(int t=0;t<180;t++)  
                {  
                     double r = ( ((double)j - center_x) * cos((double)t * DEG2RAD)) + (((double)i - center_y) * sin((double)t * DEG2RAD)) ;
                     
                     //tableau 2D ->  1D
                     int indice = (int)((round(r + hough_h) * 180.0)) + t ;
                     accumulator[indice]++ ;
                     accumulatorPoints[indice].push_back(cv::Point(j,i)) ;
                } 
            }  
        }  
    }


    //GET LINES

    std::vector< std::pair< cv::Point, cv::Point > > lines;  

    for(int t=0;t<accu_w;t++)  
    {  
        for(int r=0;r<accu_h;r++)  
        {  
            unsigned int index = (r*accu_w) + t ;
             if( accumulator[ index ] >= seuil && seuil > 0)  
             {
                //check maximas locaux
                unsigned int max = accumulator[ index ];  
                for(int lx=-4;lx<=4;lx++)  
                  {  
                        for(int ly=-4;ly<=4;ly++)  
                        {  
                            if( (ly+r>=0 && ly+r<accu_h) && (lx+t>=0 && lx+t<accu_w) )  
                            {  
                                 if( accumulator[ (int)( ((r+ly)*accu_w) + (t+lx) ) ] > max )  
                                 {  
                                      max = accumulator[ (int)( ((r+ly)*accu_w) + (t+lx) ) ];  
                                      ly = lx = 5;  
                                 }  
                            }  
                       }  
                  }  
                  if( max > accumulator[ index ])  
                       continue;  

                   std::vector<cv::Point> pts = accumulatorPoints[ index ] ;

                   int x1, y1, x2, y2 ;

                    x1 = pts[0].x;
                    y1 = pts[0].y;
                    x2 = pts[0].x;
                    y2 = pts[0].y;

                    for( unsigned int i = 0 ; i < pts.size() ; i++ )
                    {
                        if( pts[i].x < x1 )
                        {
                            x1 = pts[i].x ;
                            y1 = pts[i].y ;
                        }
                        else if( pts[i].x == x1 && pts[i].y < y1 )
                        {
                            x1 = pts[i].x ;
                            y1 = pts[i].y ;
                        }
                        
                        if( pts[i].x > x2 )
                        {
                            x2 = pts[i].x ;
                            y2 = pts[i].y ;
                        }
                        else if( pts[i].x == x2 && pts[i].y > y2 )
                        {
                            x2 = pts[i].x ;
                            y2 = pts[i].y ;
                        }
                    }

                  lines.push_back(std::pair< cv::Point, cv::Point >(std::pair<cv::Point, cv::Point>( cv::Point(x1,y1), cv::Point(x2,y2) ) ));  

             }  
        }  
   }  


   // DRAW LINES

    std::vector< std::pair< cv::Point, cv::Point > >::iterator it;  

    for(it=lines.begin();it!=lines.end();it++)  
    {  

        if( precision ) //points par points
        {
            cv::LineIterator it2(output, it->first, it->second, 8) ;
            for(int i = 0; i < it2.count; i++, ++it2)
            {
                if( input.is_contour(it2.pos().y,it2.pos().x) )
                {
                    output(it2.pos().y, it2.pos().x)[0] = 0 ;
                    output(it2.pos().y, it2.pos().x)[1] = 0 ;
                    output(it2.pos().y, it2.pos().x)[2] = 255 ;
                }
            }
        }
        else
        {
            int xmin, xmax, ymin, ymax ;
            xmin = xmax = ymin = ymax = -1 ;

            cv::LineIterator it2(output, it->first, it->second, 8) ;
            for(int i = 0; i < it2.count; i++, ++it2)
            {
                if( input.is_contour(it2.pos().y,it2.pos().x) )
                {
                    if( xmin == -1 )
                    {
                        xmin = it2.pos().x ;
                        ymin = it2.pos().y ;

                        xmax = it2.pos().x ;
                        ymax = it2.pos().y ;
                    }
                    else if( it2.pos().x > xmax || (it2.pos().x == xmax && it2.pos().y > ymax) )
                    {
                        xmax = it2.pos().x ;
                        ymax = it2.pos().y ;
                    }
                }
            }

            if( xmin == -1 )
            {
                xmin = it->first.x ; 
                ymin = it->first.y ; 
            }
            if( xmax == -1 )
            {
                xmax = it->second.x ; 
                ymax = it->second.y ; 
            }

            cv::line(
                output,
                cv::Point(xmin, ymin),
                cv::Point(xmax, ymax),
                cv::Scalar( 0, 0, 255),
                1, 8
            );            
        }
    } 

}


void hough_circles( Image input, Image output, unsigned int seuil, unsigned int rayon_min_percent, unsigned int rayon_max_percent, unsigned int distance_min, bool affiche_acc )
{

    // FEED ACCUMULATORS

    unsigned int rayon_min = rayon_min_percent * input.cols / 100 ;
    unsigned int rayon_max = rayon_max_percent * input.cols / 100 ;

    unsigned int ** accumulatorCircles ;
    unsigned int rayonAmplitude = rayon_max - rayon_min + 1;
    accumulatorCircles = ( unsigned int ** )malloc( (rayonAmplitude)*sizeof(unsigned int *) ) ;
    for( unsigned int i = 0 ; i < (rayonAmplitude) ; i++ )
    {
        accumulatorCircles[i] = ( unsigned int * )calloc( (input.rows*input.cols)+1, sizeof(unsigned int) ) ;  
    }
    
    #pragma omp parallel for
    for(int i=0;i<input.rows;i++)
    {  
        #pragma omp parallel for
        for(int j=0;j<input.cols;j++)  
        {  
            if( input.is_contour(i, j) && i != 0 && j != 0 && i != input.rows-1 && j != input.cols-1 )
            {
                for (int k = i-(int)rayon_max; k <= i+(int)rayon_max && k < input.rows ; k++)
                {
                    for (int l = j-(int)rayon_max; l <= j+(int)rayon_max && l < input.cols ; l++)
                    {
                        unsigned int distance = sqrt( pow( (l-j), 2 ) + pow( (k-i), 2 ) );
                        
                        if( distance >= rayon_min && distance <= rayon_max )
                        {
                            int indice = k*input.cols + l ;
                            if( indice <= input.rows*input.cols && indice >= 0 )
                            {
                                accumulatorCircles[distance-rayon_min][ indice ]++ ;
                            }
                        }
                    }
                }
            }  
        }  
    }
    

    // DRAW CIRCLES

    if( affiche_acc )
    {
        int maxS = 0 ;
        #pragma omp parallel for
        for( int i = 0 ; i < input.rows ; i++ )
        {
            #pragma omp parallel for
            for( int j = 0 ; j < input.cols ; j++ )
            {
                int sum = 0 ;
                #pragma omp parallel for
                for( unsigned int r = 0; r < rayonAmplitude ; r++ )
                {
                    sum += accumulatorCircles[r][i*input.cols+j] ;
                }
                if( sum > maxS )
                    maxS = sum ;
            }
        }

        #pragma omp parallel for
        for( int i = 0 ; i < input.rows ; i++ )
        {
            #pragma omp parallel for
            for( int j = 0 ; j < input.cols ; j++ )
            {
                int sum = 0 ;
                #pragma omp parallel for
                for( unsigned int r = 0; r < rayonAmplitude ; r++ )
                {
                    sum += accumulatorCircles[r][i*input.cols+j] ;
                }
                int color = 255*sum/maxS ;
                output(i,j)[0] = color ;
                output(i,j)[1] = color ;
                output(i,j)[2] = color ;
            }
        }
    }
    else
    {
        float rf = (float)seuil/100.0*M_PI ;
        #pragma omp parallel for
        for( unsigned int r = 0; r < rayon_max-rayon_min ; r++ )
        {
            unsigned int R = r+rayon_min ;

            float seuil_r = rf*R ;

            #pragma omp parallel for
            for( int i = 0 ; i < input.rows ; i++ )
            {
                #pragma omp parallel for
                for( int j = 0 ; j < input.cols ; j++ )
                {
                    if( accumulatorCircles[r][i*input.cols+j] > seuil_r )
                    { 
                        unsigned int max = accumulatorCircles[r][i*input.cols+j];  

                        //maxima locaux (rayons et centre)
                        int distance_min_i = (int)distance_min ;
                        #pragma omp parallel for
                        for(int ly=-distance_min_i;ly<=distance_min_i;ly++)  
                        {
                            #pragma omp parallel for
                            for(int lx=-distance_min_i;lx<=distance_min_i;lx++)  
                            {
                                #pragma omp parallel for
                                for(int lr=-distance_min_i;lr<=distance_min_i;lr++)  
                                {
                                    if( (ly+j>=0 && ly+j<input.cols) && (lx+i>=0 && lx+i<input.rows) && (lr+r<rayonAmplitude) )  
                                    {
                                        if( accumulatorCircles[r+lr][ (int)( ((i+lx)*input.cols) + (j+ly) ) ] > max )  
                                        {
                                            max = accumulatorCircles[r+lr][ (int)( ((i+lx)*input.cols) + (j+ly) ) ] ;
                                            ly = lx = distance_min_i+1 ;
                                            lr = distance_min_i+1 ;
                                        }
                                    }
                                }
                            }
                        }

                        if( max <= accumulatorCircles[r][i*input.cols+j] )
                        {
                            int x = j ;
                            int y = i ;
                            cv::circle(
                                output,
                                cv::Point(x,y),
                                R,
                                cv::Scalar( 0, 255, 0)
                            ) ;
                        }
                    }
                }    
            }
        }
    }
}