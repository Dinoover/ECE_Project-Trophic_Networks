#include "grman/grman.h"
#include <iostream>

#include "graph.h"

int main()
{
    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du répertoire où se trouvent les images à charger
    grman::set_pictures_path("pics");

    Graph clown, donjon, g;
    int a;

    clown.load(1);
    donjon.load(2);

    std::cout<<"select a graph or say 0 to end process"<<std::endl;
    std::cin >> a;

    while(a!=0)
    {
        switch(a)
        {
            case 1: g = clown; break;
            case 2: g = donjon; break;
        };


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
        std::cout<<"select a graph or say 0 to end process"<<std::endl;
        std::cin>>a;
    }


    grman::fermer_allegro();

    return 0;
}
END_OF_MAIN();


