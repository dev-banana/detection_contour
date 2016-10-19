#include <cstdlib>
#include <cstdio>

#include "option.hpp"
#include "image.hpp"

using namespace std ;


bool quit = false ;
std::string window_title = "Analyse d'image : output" ;

std::vector<std::pair<std::string, Image>> images ;

Image actu ;


int lireInt()
{
	int val ;
	cin >> val ;
	while( cin.fail() )
	{
		cin.clear();
		cin.ignore(256,'\n');
		cin >> val ;
	}
	return val ;
}

std::string lireString()
{
	std::string val ;
	cin >> val ;

	while( cin.fail() )
	{
		cin.clear();
		cin.ignore(256,'\n');
		cin >> val ;
	}
	return val ;
}

int menu()
{
	std::vector<string> choix = {
		"open",
		"save",
		"afficher",
		"filtre moyen",
		"quit"
	};

	cout << endl << "MENU :" << endl ;
	
	for( unsigned int i = 0 ; i < choix.size() ; i++ )
	{
		cout << "   " << i+1 << ". " << choix[i] << endl ;
	}


	unsigned int val = 0 ;

	while( val <= 0 || val > choix.size() )
	{
		cout << endl << "votre choix ? " ;
		val = lireInt() ;
	}
 
	return val ;
}

int files()
{
	
	cout << endl << "Liste des images :" << endl ;
	
	for( unsigned int i = 0 ; i < images.size() ; i++ )
	{
		cout << "   " << i+1 << ". " << images[i].first << endl ;
	}


	unsigned int val = 0 ;

	while( val <= 0 || val > images.size() )
	{
		cout << endl << "fichier ? " ;
		val = lireInt() ;
	}
 
	return val-1 ;
}

std::pair<std::string, Image> open()
{
	std::pair<std::string, Image> pair ;
	Image im ;
	std::string path = "" ;
	do
	{
		cout << endl << "chemin du fichier ? " << endl ;
		path = lireString() ;

		im = Image( cv::imread( path ) ) ;

	} while( im.empty() ) ;
	
	pair.first = path ;
	pair.second = im ;
	
	return pair ;
}

int main (int argc, char *argv[])
{
	while( !quit )
	{
		int choix = menu() ;

		switch( choix )
		{
			case 1 : //open
			{
				images.push_back( open() ) ;
			} break ;
			case 2 : //save
			{

			} break ;
			case 3 : //afficher
			{
				int chooseFile = files() ;

				cv::namedWindow(window_title, CV_WINDOW_AUTOSIZE) ;
				cv::startWindowThread() ;

				cv::imshow( window_title, images[chooseFile].second ) ;
				cv::waitKey(0) ;

				cv::destroyAllWindows() ;

			} break ;
			case 4 : //filtre_moyen
			{
			} break ;
			case 5 : //quit
			{
				return 0 ;
			} break ;
			default : 
			{
			} break ;
		}
	}
	return 0 ;
}