#include "graph.h"


int make_power_of_2(int a)
{
    int b=1;
    for (int i=0; i<a; i++)
    {
        b = 2 * b;
    }
    return b;
}

/***************************************************
                    VERTEX
****************************************************/

/// Le constructeur met en place les éléments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de réglage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0 , 100.0); // Valeurs arbitraires, à adapter...
    m_slider_value.set_dim(20,80);
    m_slider_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);

    // Label de visualisation de valeur
    m_top_box.add_child( m_label_value );
    m_label_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down);

    // Une illustration...
    if (pic_name!="")
    {
        m_top_box.add_child( m_img );
        m_img.set_pic_name(pic_name);
        m_img.set_pic_idx(pic_idx);
        m_img.set_gravity_x(grman::GravityX::Right);
    }

    // Label de visualisation d'index du sommet dans une boite
    m_top_box.add_child( m_box_label_idx );
    m_box_label_idx.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Down);
    m_box_label_idx.set_dim(20,12);
    m_box_label_idx.set_bg_color(BLANC);

    m_box_label_idx.add_child( m_label_idx );
    m_label_idx.set_message( std::to_string(idx) );
}


/// Gestion du Vertex avant l'appel à l'interface
void Vertex::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_value vers le slider associé
    m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donnée m_value vers le label sous le slider
    m_interface->m_label_value.set_message( std::to_string( (int)m_value) );
}


/// Gestion du Vertex après l'appel à l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_value locale
    m_value = m_interface->m_slider_value.get_value();
}



/***************************************************
                    EDGE
****************************************************/

/// Le constructeur met en place les éléments de l'interface
EdgeInterface::EdgeInterface(Vertex& from, Vertex& to)
{
    // Le WidgetEdge de l'interface de l'arc
    if ( !(from.m_interface && to.m_interface) )
    {
        std::cerr << "Error creating EdgeInterface between vertices having no interface" << std::endl;
        throw "Bad EdgeInterface instanciation";
    }
    m_top_edge.attach_from(from.m_interface->m_top_box);
    m_top_edge.attach_to(to.m_interface->m_top_box);
    m_top_edge.reset_arrow_with_bullet();

    // Une boite pour englober les widgets de réglage associés
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(24,60);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de réglage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0 , 100.0); // Valeurs arbitraires, à adapter...
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);

}


/// Gestion du Edge avant l'appel à l'interface
void Edge::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_weight vers le slider associé
    m_interface->m_slider_weight.set_value(m_weight);

    /// Copier la valeur locale de la donnée m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( std::to_string( (int)m_weight ) );
}

/// Gestion du Edge après l'appel à l'interface
void Edge::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_weight locale
    m_weight = m_interface->m_slider_weight.get_value();
}



/***************************************************
                    GRAPH
****************************************************/

/// Ici le constructeur se contente de préparer un cadre d'accueil des
/// éléments qui seront ensuite ajoutés lors de la mise ne place du Graphe
GraphInterface::GraphInterface(int x, int y, int w, int h)
{
    m_top_box.set_dim(1000,740);
    m_top_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);

    m_top_box.add_child(m_tool_box);
    m_tool_box.set_dim(80,720);
    m_tool_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_tool_box.set_bg_color(BLANCBLEU);

    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(908,720);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_main_box.set_bg_color(BLANCJAUNE);
}


/// Méthode spéciale qui construit un graphe arbitraire (démo)
/// Cette méthode est à enlever et remplacer par un système
/// de chargement de fichiers par exemple.
/// Bien sûr on ne veut pas que vos graphes soient construits
/// "à la main" dans le code comme ça.
void Graph::load(int a)
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 1000, 1000);
    // La ligne précédente est en gros équivalente à :
    // m_interface = new GraphInterface(50, 0, 750, 600);
    int idx, x, y;
    double p;
    std::string name;
    std::string test;
    switch(a)
    {
    case 1:
        {
            m_locator_S = "clown/sommet.txt";
            m_locator_A = "clown/arete.txt";
            m_name= "clown";
            std::ifstream sommet_clown(m_locator_S, std::ios::in);
            if (sommet_clown)
            {
                while(getline(sommet_clown,test))
                {
                    sommet_clown >> idx >> p >> x >> y >> name;
                    add_interfaced_vertex(idx, p, x, y, name);
                }
                sommet_clown.close();
            }else
            {
                std::cout<< "error";
            }

            std::ifstream arete_clown( m_locator_A, std::ios::in);
            if (arete_clown)
            {
                while(getline(arete_clown,test))
                {
                    arete_clown >> idx >> x >> y >> p;
                    add_interfaced_edge(idx, x, y, p);
                }
                arete_clown.close();
            }else
            {
                std::cout<< "error";
            }
            break;
        }

    case 2:
        {
            m_locator_S = "donjon/sommet.txt";
            m_locator_A = "donjon/arete.txt";
            m_name= "donjon";
            std::ifstream sommet_donjon(m_locator_S, std::ios::in);
            if (sommet_donjon)
            {
                while(getline(sommet_donjon,test))
                {
                    sommet_donjon >> idx >> p >> x >> y >> name;
                    add_interfaced_vertex(idx, p, x, y, name);
                }
                sommet_donjon.close();
            }else
            {
                std::cout<< "error";
            }

            std::ifstream arete_donjon( m_locator_A, std::ios::in);
            if (arete_donjon)
            {
                while(getline(arete_donjon,test))
                {
                    arete_donjon >> idx >> x >> y >> p;
                    add_interfaced_edge(idx, x, y, p);
                }
                arete_donjon.close();
            }else
            {
                std::cout<< "error";
            }
            break;
        }
    case 3:
        {
            {
            m_locator_S = "classique/sommet.txt";
            m_locator_A = "classique/arete.txt";
            m_name= "classique";
            std::ifstream sommet_classique(m_locator_S, std::ios::in);
            if (sommet_classique)
            {
                while(getline(sommet_classique,test))
                {
                    sommet_classique >> idx >> p >> x >> y >> name;
                    add_interfaced_vertex(idx, p, x, y, name);
                }
                sommet_classique.close();
            }else
            {
                std::cout<< "error";
            }

            std::ifstream arete_classique( m_locator_A, std::ios::in);
            if (arete_classique)
            {
                while(getline(arete_classique,test))
                {
                    arete_classique >> idx >> x >> y >> p;
                    add_interfaced_edge(idx, x, y, p);
                }
                arete_classique.close();
            }else
            {
                std::cout<< "error";
            }
            break;
        }
        }
    default:
        {
            std::cout << "ce graphe n'existe pas";
        }

    }
}

void Graph::save()
{
    std::ofstream sommet(m_locator_S, std::ios::out);
    if (sommet)
    {
        for(auto& x: m_vertices)
        {
            sommet<< std::endl<< x.first << " "<< x.second.m_value << " "<< x.second.m_interface->m_top_box.get_posx() << " " << x.second.m_interface->m_top_box.get_posy() << " " << x.second.m_interface->m_img.get_name();
        }
    sommet.close();
    }else
    {
        std::cout<< "error";
    }

    std::ofstream arete(m_locator_A, std::ios::out);
    if (arete)
    {
        for(auto& x: m_edges)
        {
            arete<< std::endl<< x.first << " "<<x.second.m_from << " "<< x.second.m_to << " " << x.second.m_weight;
        }
    arete.close();
    }else
    {
        std::cout<< "error";
    }
}

void Graph::new_sommet()
{
    std::string name;
    int a;
    int idx;

    std::cout<< "indiquez le nouveau sommet sous la forme : nom_image"<<std::endl;
    std::cin>> name;
    idx=free_vertex_idx();
    add_interfaced_vertex(idx, 0, 100, 100, name);

    do
    {
        std::cout<< "mange/subit indice_sommet (ecrire -fin 0- pour terminer)"<<std::endl;
        std::cin>>name>> a;
        if(name.compare("subit")==0)
        {
            add_interfaced_edge(free_edge_idx(), idx, a, 0);
        }else if(name.compare("mange")==0)
        {
            add_interfaced_edge(free_edge_idx(), a, idx, 0);
        }
    }while(name.compare("fin")!=0);
}

void Graph::delete_sommet()
{
    int idx;

    std::cout<<"numero du sommet"<<std::endl;
    std::cin>>idx;

    m_vertices.erase(idx);

    for(auto& x: m_edges)
    {
        if(x.second.m_from==idx||x.second.m_to==idx)
        {
            m_edges.erase(x.first);
        }
    }
    std::cout<<"le sommet "<<idx<<" a ete efface, il ne peut plus etre modifie et au prochain lancement il n'apparaitra plus"<<std::endl;
}

void Graph::delete_sommet(int idx)
{
    m_vertices.erase(idx);

    for(auto& x: m_edges)
    {
        if(x.second.m_from==idx||x.second.m_to==idx)
        {
            m_edges.erase(x.first);
        }
    }
}

int Graph::free_edge_idx()
{
    int i=0;
    for(auto& x: m_edges)
    {
        if(i!=x.first)
        {
            return i;
        }
        i++;
    }
    return m_edges.size();
}

int Graph::free_vertex_idx()
{
    int i=0;
    for(auto& x: m_vertices)
    {
        if(i!=x.first)
        {
            return i;
        }
        i++;
    }
    return m_vertices.size();
}

void Graph::DFS(int idx)
{
    m_vertices[idx].m_marque=true;
    for(auto& x: m_edges)
    {
        if(x.second.m_from==idx&&m_vertices[x.second.m_to].m_marque!=true)
        {
            DFS(x.second.m_to);
        }
        if(x.second.m_to==idx&&m_vertices[x.second.m_from].m_marque!=true)
        {
            DFS(x.second.m_from);
        }
    }
}

int Graph::conexe()
{
    int i=0;
    for(auto& x: m_vertices)
    {
        if(x.second.m_marque!=true)
        {
            DFS(x.first);
            i++;
        }

    }
    for(auto& x: m_vertices)
    {
        x.second.m_marque=false;
    }
    return i;
}

int Graph::kconexe_find(int a)
{
    int i=0;
    int c=0;

    for(auto& x : m_vertices)
    {
        c = a&make_power_of_2(i);

        if(c!=0)
        {
            delete_sommet(x.first);
        }
        i++;
    }
    return conexe();
}

void Graph::afficher_ans(std::vector<int> ans)
{
    std::vector<std::vector<int>> walla;
    std::vector<int> S;
    int k_min = m_vertices.size();
    int k=0;
    int j=0;
    int c;

    for(int i = 0; i<ans.size(); i++)
    {
        k=0;
        j=0;

        for(auto& x: m_vertices)
        {
            c = ans[i]&make_power_of_2(j);

            if(c!=0)
            {
                k++;
                S.push_back(x.first);
            }
            j++;
        }

        if(k<k_min)
        {
            k_min=k;
        }
        if(k=k_min)
        {
            walla.push_back(S);
        }
        S.resize(0);
    }

    for(int i=0; i<walla.size();i++)
    {
        if(walla[i].size()!=k_min)
        {
            walla.erase(walla.begin()+i);
            i--;
        }
    }

    std::cout<<"le k_min est "<<k_min<<std::endl;
}


void Graph::kconexe(Graph g)
{
    int a=1;
    int b;
    int c;
    Graph tampon=g;
    std::vector<int> ans;

    b= make_power_of_2(m_vertices.size());
        while(a<b)
        {
            c=g.kconexe_find(a);

            if(c!=1&&c!=0)
            {
                ans.push_back(a);
            }
            g = tampon;
            a++;
        };

        afficher_ans(ans);
}

void Graph::sommet_update()
{
    std::vector<double> nourriture;
    std::vector<double> predation;
    double k_nourriture;
    double k_predation;
    double pop;
    double preda;
    double bouffe;

    for (auto& x: m_vertices)
    {
        pop = x.second.m_value;
        k_nourriture = 0;
        k_predation = 0;

        for(auto& y: m_edges)
        {
            if(y.second.m_from==x.first)
            {
                predation.push_back(y.second.m_weight * m_vertices[y.second.m_to].m_value);
            }
            if(y.second.m_to==x.first)
            {
                nourriture.push_back(y.second.m_weight * m_vertices[y.second.m_from].m_value);
            }
        }
        if (!nourriture.empty())
        {
            for(int i=0; i<nourriture.size(); i++)
            {
                k_nourriture = k_nourriture + nourriture[i];
            }
            for(int i=0; i<predation.size(); i++)
            {
                k_predation = k_predation + predation[i];
            }

            if (k_nourriture!=0)
            {
                bouffe = (pop/k_nourriture);
            }else bouffe = 10;

            x.second.m_value = pop + 0.005*( pop*(1 - bouffe) - pop*k_predation/1000);

            if(x.second.m_value<=0)
            {
               x.second.m_value=0;
            }
        }

        predation.resize(0);
        nourriture.resize(0);
    }
}

/// La méthode update à appeler dans la boucle de jeu pour les graphes avec interface
void Graph::update()
{
    if (!m_interface)
        return;

    for (auto &elt : m_vertices)
        elt.second.pre_update();

    for (auto &elt : m_edges)
        elt.second.pre_update();

    m_interface->m_top_box.update();

    for (auto &elt : m_vertices)
        elt.second.post_update();

    for (auto &elt : m_edges)
        elt.second.post_update();

}

/// Aide à l'ajout de sommets interfacés
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name, int pic_idx )
{
    if ( m_vertices.find(idx)!=m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    // Création d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, vi);
}

/// Aide à l'ajout d'arcs interfacés
void Graph::add_interfaced_edge(int idx, int id_vert1, int id_vert2, double weight)
{
    if ( m_edges.find(idx)!=m_edges.end() )
    {
        std::cerr << "Error adding edge at idx=" << idx << " already used..." << std::endl;
        throw "Error adding edge";
    }

    if ( m_vertices.find(id_vert1)==m_vertices.end() || m_vertices.find(id_vert2)==m_vertices.end() )
    {
        std::cerr << "Error adding edge idx=" << idx << " between vertices " << id_vert1 << " and " << id_vert2 << " not in m_vertices" << std::endl;
        throw "Error adding edge";
    }

    EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]);
    m_interface->m_main_box.add_child(ei->m_top_edge);
    m_edges[idx] = Edge(weight, ei);
    m_edges[idx].m_from = id_vert1;
    m_edges[idx].m_to = id_vert2;
}


