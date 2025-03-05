#include "CImg.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <bitset>
#include <vector>
#include <string>
#define cimg_use_jpeg

#include <algorithm>
#include <iterator>
using namespace cimg_library;
using namespace std;
const unsigned char black[] = {0, 0, 0};
const unsigned char white[] = {255, 255, 255};
string i,c;



int B2D(int n) //Fonction Binaire à Décimal
{
    int num = n;
    int dec_value = 0;

    // Initialisation de base à 1, soit 2^0
    int base = 1;

    int temp = num;
    while (temp)
    {
        int last_digit = temp % 10;
        temp = temp / 10;

        dec_value += last_digit * base;

        base = base * 2;
    }

    return dec_value;
}

CImg<unsigned char> median(CImg<unsigned char> T, int seuil, int ordre, ifstream& texte)
{
    CImg<unsigned char> imgfinal(T.width(),T.height(),1,3,0);
    vector<string> temoin((T.width())*(T.height()),"0"); //Initialisaition du tableau des résultats
    int valeur[T.width()][T.height()]= {0}; //Valeurs de l'image originale

    for(int x = 0; x < T.width(); ++x)
    {
        for(int y = 0; y < T.height(); ++y)
        {
            valeur[x][y]=round((T.atXY(x,y,0)+T.atXY(x,y,1)+T.atXY(x,y,2))/3);//RGB2Grayscale
        }
    }

    ostringstream ss;
    ss<<texte.rdbuf();
    const string& s = ss.str(); //string du texte
    vector<char> donqui(s.begin(),s.end());
    vector<char> bittexte;
    for(int i=0; i<donqui.size(); ++i)
    {
        bitset<8> char8=bitset<8>(donqui[i]); //conversion du char en 8-bits
        string charstr=char8.to_string(); //Convertion en string
        for(int j=0; j<8; j++)
        {
            bittexte.push_back(charstr[j]);
        }
    }

    for (int x=0; x<T.width()-2; x++)
        for(int y=0; y<T.height()-2; y++)
        {
            int liste[9]= {0};  //Initialisation de la liste des 9 valeurs de pixels
            for(int k=0; k<3; k++)
                for(int l=0; l<3; l++)
                    liste[k*3+l]=T(x+k,y+l,0); //recupération des 9 valeurs de pixels
            sort(liste,liste+9); //Tri dans l'orde croissant de la liste
            int med=liste[4]; //Récupération de la valeur médiane
            if(abs(T(x+1,y+1,0)-med)>seuil) //test T=|C-median(C)|
                temoin[(x+1)*T.height()+y+1]="1"; //Tableau indiquant les pixels altérables
        }

    int offset=0;
    int pixel_temoin=0;
    for(int x = 0; x < T.width()-2; ++x)
    {
        for(int y = 0; y < T.height()-2; ++y)
        {
            if(temoin[(x+1)*T.height()+y+1]=="1") // si le pixel est notée par la liste temoin et qu'il n'appartient pas au contour
            {
                bitset<8> i=bitset<8>(valeur[x+1][y+1]);
                string istr=i.to_string();
                istr.replace(istr.end()-ordre-1,istr.end()-1,bittexte.begin()+offset,bittexte.begin()+offset+ordre);
                istr.replace(7,1,"1"); //Remplacement du dernier bits par le temoin
                pixel_temoin++, //Compte du nombre de pixel comportant le texte
                             offset+=ordre;
                int image_dec=B2D(stoi(istr)); //Récupération du nouveaux bits en décimal
                valeur[x+1][y+1]=image_dec;
            }
            else
            {
                bitset<8> i=bitset<8>(valeur[x+1][y+1]);
                string istr=i.to_string();
                istr.replace(7,1,"0");
                int image_dec=B2D(stoi(istr));
//Récupération du nouveaux bits en décimal
                valeur[x+1][y+1]=image_dec;
            }

            const unsigned char col[] = {(unsigned char)valeur[x+1][y+1],(unsigned char)valeur[x+1][y+1],(unsigned char)valeur[x+1][y+1]};
            imgfinal.draw_point(x,y,col);



        }
    }
    cout<<pixel_temoin<<" pixels, soit "<<pixel_temoin*ordre/8<<"caracteres"<<endl;
    return imgfinal;
}
int main()
{
    int ordre=5;
    int seuil=5;
    ifstream texte("text.txt");
    CImg<unsigned char> image("Seatech.ppm"); //Ouverture de l'image
    //image.display();
    CImg<unsigned char> finale=median(image,seuil,ordre,texte);
    finale.display();

    ///Déchifremment
    vector<int> texteint;
    vector<char> textechar;
    string l="";
    int nbr_mots=0;
    for(int x = 0; x < finale.width()-2; ++x)
    {
        for(int y = 0; y < finale.height()-2; ++y)
        {
            bitset<8> i=bitset<8>(finale(x,y,0)); //On récupère chaque pixel sous forme d'un string de 8 char 0 ou 1
            string istr=i.to_string();
            if((int)istr.at(7)==49) //On regarde le 8ème bit pour savoir si c'est le témoin, (int)'1' vaut 49 pour un char
            {
                for (int i=8-ordre-1; i<7; i++) //On ne prend pas le dernier bit car témoin
                {
                    if (l.size()<8)
                    {
                        l.push_back(istr[i]);
                    }
                    else
                    {
                        texteint.push_back(stoi(l));
                        cout <<(char)stoi(l,nullptr,2);
                        nbr_mots++;
                        l=istr[i]; // On remplace par le ième bits qui devient donc le 1er du nouveau char
                    }
                }
            }
        }
    }
}
