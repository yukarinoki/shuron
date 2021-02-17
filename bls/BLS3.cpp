#include <iostream>
#include <ctime>
#include <random>
#include <vector>
#include <fstream>
#include <utility>
#include <string>
#include <vector>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#define ll long long

std::mt19937 mt{ std::random_device{}() };

typedef
    boost::property<boost::edge_weight_t,int> EdgeWeightProperty;

typedef
    boost::adjacency_list <
        boost::listS,
        boost::vecS,
        boost::undirectedS,
        boost::no_property,
        EdgeWeightProperty> Graph;

typedef std::pair<unsigned int, unsigned int> Edge;

struct Bucket {
    std::vector<unsigned int>* array0;
    std::vector<unsigned int>* array1;
    ll* gain;
    ll central;
    ll max_gain0;
    ll max_gain1;
};

ll calculate_cut(Graph graph,std::vector<bool> cut){
    ll obj = 0;
    Edge e;
    BGL_FORALL_EDGES(e,graph,Graph) {
        if (cut[target(e, graph)] != cut[source(e, graph)])
            obj += boost::get(boost::edge_weight,graph,e);
    }

    return obj;
}

ll calculate_gain(Graph graph, std::vector<bool> cut, unsigned int n) {
    ll obj = 0;
    Edge e;

    BGL_FORALL_OUTEDGES_T(n, e, graph, Graph) {
        if (cut[target(e, graph)] ^ cut[source(e, graph)]) obj -= boost::get(boost::edge_weight,graph,e);
        else obj += boost::get(boost::edge_weight,graph,e);
    }

    return obj;
}

struct Bucket* initialize_bucket (Graph graph, std::vector<bool> cut) {
    struct Bucket* bucket = (struct Bucket*) malloc(sizeof(struct Bucket));
    Edge e;
    unsigned int v;
    int w;
    ll obj = 0;
    ll max_gain = 0;
    BGL_FORALL_VERTICES_T (v, graph, Graph) {
        obj = 0;
        BGL_FORALL_OUTEDGES_T(v,e, graph, Graph) {
            w = boost::get(boost::edge_weight,graph,e);
            if (w > 0) obj += w;
            else obj -= w;
        }
        if (max_gain < obj) max_gain = obj;
    }

    bucket->array0 = (std::vector<unsigned int>*) malloc((max_gain * 2 + 1) * sizeof(std::vector<unsigned int>));
    bucket->array1 = (std::vector<unsigned int>*) malloc((max_gain * 2 + 1) * sizeof(std::vector<unsigned int>));
    bucket->gain = (ll*) malloc(num_vertices(graph) * sizeof(ll));
    bucket->central = max_gain;

    for (unsigned int i = 0; i < max_gain * 2 + 1; i++) {
        bucket->array0[i].clear();
        bucket->array1[i].clear();
    }

    BGL_FORALL_VERTICES_T (v, graph, Graph) {
        bucket->gain[v] = calculate_gain(graph, cut, v);

        if (cut[v]) bucket->array1[bucket->gain[v] + bucket->central].push_back(v);
        else bucket->array0[bucket->gain[v] + bucket->central].push_back(v);
    }

    unsigned int i = bucket->central * 2 - 1;
    while (bucket->array1[i].empty() && i > 0) i--;
    bucket->max_gain1 = i - bucket->central;

    i = bucket->central * 2 - 1;
    while (bucket->array0[i].empty() && i > 0) i--;
    bucket->max_gain0 = i - bucket->central;

    return bucket;
};

Graph input_Graph (char* input_file) {
    unsigned int n,e,a,b;
    int w;
    std::ifstream fin;

    fin.open(input_file,std::ios::in);

    fin>>n>>e;

    Graph g(n);

    while (fin>>a>>b>>w) add_edge(a, b, EdgeWeightProperty(w), g);

    return g;
}

void erase(struct Bucket* bucket, std::vector<bool> cut, unsigned int v) {
    ll w = bucket->gain[v]  + bucket->central;

    if (cut[v]) {
        auto itr = bucket->array1[w].begin();
        while (itr != bucket->array1[w].end()){
            if ((*itr) == v) {
                //std::cout << v << " erased" << std::endl;
                itr = bucket->array1[w].erase(itr);
                break;
            }
            else itr++;
        }
    }

    else {
        auto itr = bucket->array0[w].begin();
        while (itr != bucket->array0[w].end()){
            if ((*itr) == v) {
                //std::cout << v << " erased" << std::endl;
                itr = bucket->array0[w].erase(itr);
                break;
            }
            else itr++;
        }
    }
}

void move(Graph graph, struct Bucket* bucket, std::vector<bool>& cut, unsigned int m, ll *f_c) {
    Edge e;
    ll w;
    unsigned int v;

    //std::cout << "Move is " << m << std::endl;
    /*
    for (unsigned int i = 0; i < bucket->central * 2; i++) {
        auto itr = bucket->array0[i].begin();
        while (itr != bucket->array0[i].end()){
            if ((*itr) > num_vertices(graph)) std::cout << "array 0 " << "gain " << i << " num " << (*itr) << std::endl;
            itr++;
        }
        itr = bucket->array1[i].begin();
        while (itr != bucket->array1[i].end()){
            if ((*itr) > num_vertices(graph)) std::cout << "array 1 " << "gain " << i << " num " << (*itr) << std::endl;
            itr++;
        }
    }
    */

    BGL_FORALL_OUTEDGES_T(m, e, graph, Graph) {
        v = target(e,graph);

        erase(bucket,cut,v);

        if (cut[v] ^ cut[m]) bucket->gain[v] += 2 * boost::get(boost::edge_weight,graph,e);
        else bucket->gain[v] -= 2 * boost::get(boost::edge_weight,graph,e);

        if (cut[v]) bucket->array1[bucket->gain[v] + bucket->central].push_back(v);
        else bucket->array0[bucket->gain[v] + bucket->central].push_back(v);
    }

    erase(bucket, cut, m);

    cut[m] = !cut[m];
    (*f_c) += bucket->gain[m];
    bucket->gain[m] *= -1;

    if (cut[m]) bucket->array1[bucket->gain[m] + bucket->central].push_back(m);
    else bucket->array0[bucket->gain[m] + bucket->central].push_back(m);

    unsigned int i = bucket->central * 2 - 1;
    while (bucket->array1[i].empty() && i > 0) i--;
    /*
    while (i > 0) {
        if (bucket->array1[i].empty()) i--;
        else {
            if (bucket->array1[i][0] > num_vertices(graph)) {
                std::cout << "check" << std::endl;
                bucket->array1[i].clear();
                i--;
            }
            else break;
        }
    }
    */
    bucket->max_gain1 = i - bucket->central;

    i = bucket->central * 2 - 1;
    while (bucket->array0[i].empty() && i > 0) i--;
    /*
    while (i > 0) {
        if (bucket->array0[i].empty()) i--;
        else {
            if (bucket->array0[i][0] > num_vertices(graph)) {
                std::cout << "check" << std::endl;
                bucket->array0[i].clear();
                i--;
            }
            else break;
        }
    }
    */

    bucket->max_gain0 = i - bucket->central;

    //std::cout << "max gain is " << bucket->max_gain0 << " " << bucket->max_gain1 << std::endl;
    //std::cout << "max gain num is " << bucket->array0[bucket->max_gain0 + bucket->central][0] << " " << bucket->array1[bucket->max_gain1 + bucket->central][0] << std::endl;
}

void perturb (Graph graph, struct Bucket* bucket, std::vector<bool>& cut, unsigned int l, int pattern, std::vector<ll>& tabu, ll *iter, ll f_best, ll *f_c) {
    std::uniform_int_distribution<ll> tabu_tenure(3, num_vertices(graph) / 10);
    if (pattern == 1) {
        //std::cout << "Pattern 1" << std::endl;
        ll g;
        unsigned int m;
        for (unsigned int i = 0; i < l; i++) {
            g = bucket->central * 2 - 1;
            int sw = 0;
            while (sw == 0) {
                if (!bucket->array0[g].empty()) {
                    for (auto it = bucket->array0[g].begin(); it != bucket->array0[g].end(); ++it) {
                        if (tabu[(*it)] < (*iter) || g > (f_best - (*f_c) + bucket->central)) {
                            sw = 1;
                            m = (*it);
                            break;
                        }
                    }
                }

                else if (!bucket->array1[g].empty()) {
                    for (auto it = bucket->array1[g].begin(); it != bucket->array1[g].end(); ++it) {
                        if (tabu[(*it)] < (*iter) || g > (f_best - (*f_c) + bucket->central)) {
                            sw = 1;
                            m = (*it);
                            break;
                        }
                    }
                }

                g--;
            }
           
            move(graph, bucket, cut, m, f_c);
            tabu[m] = (*iter) + tabu_tenure(mt);
            (*iter) += 1;
        }
    }

    else if (pattern == 2) {
        //std::cout << "Pattern 2" << std::endl;
        ll g1, g0;
        unsigned int m1, m0;
        for (unsigned int i = 0; i < l; i++) {
            g1 = bucket->central * 2 - 1;
            g0 = bucket->central * 2 - 1;
            int sw1 = 0;
            int sw0 = 0;
            while (bucket->array0[g0].empty() && g0 > 0) g0--;
            while (bucket->array1[g1].empty() && g1 > 0) g1--;
            if ((g0 + g1) > (f_best - (*f_c) + bucket->central * 2)) {
                m0 = bucket->array0[g0][0];
                m1 = bucket->array1[g1][0];
            }

            else {
                while (sw0 == 0) {
                    if (!bucket->array0[g0].empty()) {
                        for (auto it = bucket->array0[g0].begin(); it != bucket->array0[g0].end(); ++it) {
                            if (tabu[(*it)] < (*iter)) {
                                sw0 = 1;
                                m0 = (*it);
                                break;
                            }
                        }
                    }
                    g0--;
                }

                while (sw1 == 0){
                    if (!bucket->array1[g1].empty()) {
                        for (auto it = bucket->array1[g1].begin(); it != bucket->array1[g1].end(); ++it) {
                            if (tabu[(*it)] < (*iter)) {
                                sw1 = 1;
                                m1 = (*it);
                                break;
                            }
                        }

                    }
                    g1--;
                }
            }

            move(graph, bucket, cut, m0, f_c);
            move(graph, bucket, cut, m1, f_c);
            tabu[m0] = (*iter) + tabu_tenure(mt);
            tabu[m1] = (*iter) + tabu_tenure(mt);
            (*iter) += 1;
        }
    }

    else {
        //std::cout << "Pattern 3" << std::endl;
        unsigned int m;
        std::uniform_int_distribution<ll> random_move(0, num_vertices(graph) - 1);
        for (unsigned int i = 0; i < l; i++) {
            m = random_move(mt);
            move(graph, bucket, cut, m, f_c);
            tabu[m] = (*iter) + tabu_tenure(mt);
            (*iter) += 1;
        }
    }
}

void perturbation (Graph graph, struct Bucket* bucket, std::vector<bool>& cut, unsigned int l, std::vector<ll>& tabu, ll *iter, int omega, ll f_best, ll *f_c) {
    std::uniform_real_distribution<double> dist(0,1);
    if (omega == 0) perturb(graph, bucket, cut, l, 3, tabu, iter, f_best, f_c);
    else {
        double prob = dist(mt);
        double P = std::max(std::exp(- (double)omega / (double) 1000), 0.8);

        if (prob < P * 0.5) perturb(graph, bucket, cut, l, 1, tabu, iter, f_best, f_c);
        else if (prob < P) perturb(graph, bucket, cut, l, 2, tabu, iter, f_best, f_c);
        else perturb(graph, bucket, cut, l, 3, tabu, iter, f_best, f_c);
    }

}

int BreakoutLocalSearch (Graph graph, ll max_step) {

    std::vector<bool> cut(num_vertices(graph), false);
    std::vector<bool> cut_p(cut.size());
    std::vector<bool> cut_best(cut.size());
    BGL_FORALL_VERTICES_T (i,graph,Graph) cut[i] = rand() % 2;
    std::copy(cut.begin(), cut.end(), cut_p.begin());
    std::copy(cut.begin(), cut.end(), cut_best.begin());

    ll f_c = calculate_cut(graph, cut);
    ll f_best = f_c;

    int omega = 0;
    ll iter = 0;
    unsigned int l_0 = num_vertices(graph) / 100;
    unsigned int l = l_0;
    unsigned int i;
    int j;

    std::vector<ll> tabu(num_vertices(graph), 0);
    std::uniform_int_distribution<ll> tabu_tenure(3, num_vertices(graph) / 10);

    std::cout << "Initialize bucket... ";
    struct Bucket* bucket = initialize_bucket(graph, cut);
    std::cout << "finished" << std::endl;

    while (iter < max_step) {
        while (bucket->max_gain0 > 0 || bucket->max_gain1 > 0) {
            if (bucket->max_gain0 > bucket->max_gain1) i = (*bucket->array0[bucket->max_gain0 + bucket->central].begin());
            else i = (*bucket->array1[bucket->max_gain1 + bucket->central].begin());
            j = cut[i];
            move(graph, bucket, cut, i, &f_c);
            tabu[i] = iter + tabu_tenure(mt);

            iter++;
        }

        if (f_c > f_best) {
            f_best = f_c;
            std::cout << "----" << std::endl;
            std::cout << "steps : " << iter << ", best cut size : " << f_best << std::endl;
            std::cout << "----" << std::endl;
            std::copy(cut.begin(), cut.end(), cut_best.begin());
            omega = 0;
        }

        else omega++;

        if (omega > 1000) omega = 0;

        if (cut == cut_p) l++;
        else l = l_0;

        perturbation(graph, bucket, cut, l, tabu, &iter, omega, f_best, &f_c);

    }

    std::cout << "BLS finished" << std::endl;

    return 0;
}

int main(int argc, char* argv[]) {
    srand(time(0));
    std::cout << "Reading graph... ";
    Graph graph = input_Graph(argv[1]);
    std::cout << "finished" << std::endl;

    BreakoutLocalSearch(graph,1000);

    return 0;
}