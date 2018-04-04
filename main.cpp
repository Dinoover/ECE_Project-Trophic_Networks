#include "grman/grman.h"
#include <iostream>

#include "graph.h"



int main()
{
    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du répertoire où se trouvent les images à charger
    grman::set_pictures_path("pics");

    Graph clown, donjon, classique, g;

    std::string a;

    clown.load(1);
    donjon.load(2);
    classique.load(3);

    std::cout<< std::endl <<"attention, si vous revenez sur un graphe, vous reviendrez sur les poids au lancement du programme"<<std::endl;
    std::cout<<"choisissez un graphe ou appuyez sur 0 pour arreter le processus"<<std::endl;
    std::cin >> a;

    while(a!="0")
    {
        if(a.compare("classique")==0&&g.m_name!="classique")
        {
            g=classique;
        }else if(a.compare("clown")==0&&g.m_name!="clown")
        {
            g=clown;
        }else if(a.compare("donjon")==0&&g.m_name!="donjon")
        {
            g=donjon;
        }else if(a.compare("new")==0)
        {
           g.new_sommet();
        }else if(a.compare("delete")==0)
        {
            g.delete_sommet();
        }else if(a.compare("kconexe")==0)
        {
            g.kconexe(g);
        }


        /// Vous gardez la main sur la "boucle de jeu"
        /// ( contrairement à des frameworks plus avancés )
        while (!key[KEY_ESC])
        {
            /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
            g.update();

            /// Mise à jour générale (clavier/souris/buffer etc...)
            grman::mettre_a_jour();
        }
        g.save();
        std::cout<<"choisissez un graphe ou appuyez sur 0 pour arreter le processus"<<std::endl;
        std::cin>>a;
    }


    grman::fermer_allegro();

    return 0;
}
END_OF_MAIN();



