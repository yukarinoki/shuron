#include <iostream>
#include <ctime>
#include <cmath>
#include <vector>
#include <fstream>
#include <utility>
#include <string>
#define ll long long


struct AdjListNode {
    unsigned int dest;
    ll weight;
    struct AdjListNode* next;
};

struct AdjList {
    struct AdjListNode *head;
};

struct Graph {
    unsigned int v;
    int opt; 
    struct AdjList* array;
};

struct AdjListNode* newAdjListNode(unsigned int dest, ll weight) {
    struct AdjListNode* newNode = (struct AdjListNode*) malloc(sizeof(struct AdjListNode));
    newNode -> dest = dest;
    newNode -> weight = weight;
    newNode -> next = NULL;
    return newNode;
}

//Make graph with number of vertices is "v" and no edge
struct Graph* createGraph(unsigned int v) {
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph -> v = v;
    graph -> array = (struct AdjList*) malloc(v * sizeof(struct AdjList));
    unsigned int i;
    for (i = 0; i < v; ++i)
        graph -> array[i].head = NULL;

    return graph;
}

//Add edge ("src", "dest") which weight is "weight" to "graph"
void addEdge(struct Graph* graph, unsigned int src, unsigned int dest, ll weight) {
    struct AdjListNode* newNode = newAdjListNode(dest, weight);

    newNode -> next = graph -> array[src].head;
    graph -> array[src].head = newNode;

    newNode = newAdjListNode(src, weight);
    newNode -> next = graph -> array[dest].head;
    graph->array[dest].head = newNode;
}

struct Graph* inputGraph(char* filename) {
    FILE *fp;
    unsigned int n,m;
    ll weight;
    struct Graph* graph;

    fp = fopen(filename,"r");

    if (fp == NULL){
        printf("file can't open.\n");
    }

    fscanf(fp, "%u %u", &n, &m);
    graph = createGraph(n);
    while (fscanf(fp, "%u %u %lld",&n, &m, &weight) != EOF)
        addEdge(graph, n, m, weight);
    
    return graph;
}

struct Bucket {
    //std::vector<unsigned int>* array0;
    //std::vector<unsigned int>* array1;
    ll* gain;
    //ll central;
    //ll max_gain0;
    //ll max_gain1;
};

ll calculate_cut(Graph* graph,std::vector<bool> cut){
    ll obj = 0;

    for (unsigned int i = 0; i < graph->v; i++) {
        struct AdjListNode* objNode = graph->array[i].head;
        while (objNode) {
            if (cut[i] != cut[objNode->dest]) obj += objNode->weight;
            objNode = objNode->next;
        }
    }

    return obj / 2;
}

ll calculate_gain(Graph* graph, std::vector<bool> cut, unsigned int n) {
    ll obj = 0;
    struct AdjListNode* objNode = graph->array[n].head;

    while (objNode) {
        if (cut[n] != cut[objNode->dest]) obj -= objNode->weight;
        else obj += objNode->weight;

        objNode = objNode->next;
    }

    return obj;
}

struct Bucket* initialize_bucket (Graph* graph, std::vector<bool> cut) {
    struct Bucket* bucket = (struct Bucket*) malloc(sizeof(struct Bucket));
    unsigned int v;
    ll w;
    ll obj = 0;
    ll max_gain = 0;

    /*
    for (unsigned int i = 0; i < graph->v; i++) {
        obj = 0;
        struct AdjListNode* objNode = graph->array[i].head;
        
        while (objNode) {
            w = objNode->weight;
            if (w > 0) obj += w;
            else obj -= w;

            objNode = objNode->next;
        }

        if (max_gain < obj) max_gain = obj;
    }
    */

    //bucket->array0 = (std::vector<unsigned int>*) malloc((max_gain * 2 + 1) * sizeof(std::vector<unsigned int>));
    //bucket->array1 = (std::vector<unsigned int>*) malloc((max_gain * 2 + 1) * sizeof(std::vector<unsigned int>));
    bucket->gain = (ll*) malloc(graph->v * sizeof(ll));
    //bucket->central = max_gain;

    /*
    for (unsigned int i = 0; i < max_gain * 2 + 1; i++) {
        bucket->array0[i].clear();
        bucket->array1[i].clear();
    }
    */

    for (unsigned int i = 0; i < graph->v; i++) {
        bucket->gain[i] = calculate_gain(graph, cut, i);

        //if (cut[i]) bucket->array1[bucket->gain[i] + bucket->central].push_back(i);
        //else bucket->array0[bucket->gain[i] + bucket->central].push_back(i);
    }

    /*
    unsigned int i = bucket->central * 2 - 1;
    while (bucket->array1[i].empty() && i > 0) i--;
    bucket->max_gain1 = i - bucket->central;

    i = bucket->central * 2 - 1;
    while (bucket->array0[i].empty() && i > 0) i--;
    bucket->max_gain0 = i - bucket->central;
    */

    return bucket;
};

/*
void erase(struct Bucket* bucket, std::vector<bool> cut, unsigned int v) {
    ll w = bucket->gain[v]  + bucket->central;

    if (cut[v]) {
        std::vector<unsigned int>::iterator itr = bucket->array1[w].begin();
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
        std::vector<unsigned int>::iterator itr = bucket->array0[w].begin();
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
*/

void move(Graph* graph, struct Bucket* bucket, std::vector<bool>& cut, unsigned int m, ll *f_c) {
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

    struct AdjListNode* objNode = graph->array[m].head;
    while (objNode) {
        v = objNode->dest;
        //erase(bucket,cut,v);

        if (cut[v] != cut[m]) bucket->gain[v] += 2 * objNode->weight;
        else bucket->gain[v] -= 2 * objNode->weight;

        //if (cut[v]) bucket->array1[bucket->gain[v] + bucket->central].push_back(v);
        //else bucket->array0[bucket->gain[v] + bucket->central].push_back(v);

        objNode = objNode->next;
    }

    //erase(bucket, cut, m);

    cut[m] = !cut[m];
    (*f_c) += bucket->gain[m];
    bucket->gain[m] *= -1;

    //if (cut[m]) bucket->array1[bucket->gain[m] + bucket->central].push_back(m);
    //else bucket->array0[bucket->gain[m] + bucket->central].push_back(m);

    //unsigned int i = bucket->central * 2 - 1;
    //while (bucket->array1[i].empty() && i > 0) i--;
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
    //bucket->max_gain1 = i - bucket->central;

    //i = bucket->central * 2 - 1;
    //while (bucket->array0[i].empty() && i > 0) i--;
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

    //bucket->max_gain0 = i - bucket->central;

    //std::cout << "max gain is " << bucket->max_gain0 << " " << bucket->max_gain1 << std::endl;
    //std::cout << "max gain num is " << bucket->array0[bucket->max_gain0 + bucket->central][0] << " " << bucket->array1[bucket->max_gain1 + bucket->central][0] << std::endl;
}
/*
void perturb (Graph* graph, struct Bucket* bucket, std::vector<bool>& cut, unsigned int l, int pattern, std::vector<ll>& tabu, ll *iter, ll f_best, ll *f_c) {
    if (pattern == 1) {
        //std::cout << "Pattern 1" << std::endl;
        ll g;
        unsigned int m;
        for (unsigned int i = 0; i < l; i++) {
            g = bucket->central * 2 - 1;
            int sw = 0;
            while (sw == 0) {
                if (!bucket->array0[g].empty()) {
                    for (std::vector<unsigned int>::iterator it = bucket->array0[g].begin(); it != bucket->array0[g].end(); ++it) {
                        if (tabu[(*it)] < (*iter) || g > (f_best - (*f_c) + bucket->central)) {
                            sw = 1;
                            m = (*it);
                            break;
                        }
                    }
                }

                else if (!bucket->array1[g].empty()) {
                    for (std::vector<unsigned int>::iterator it = bucket->array1[g].begin(); it != bucket->array1[g].end(); ++it) {
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
            tabu[m] = (*iter) + rand() % (std::max((unsigned int)1,(graph->v/10 - 3))) + 3;
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
                        for (std::vector<unsigned int>::iterator it = bucket->array0[g0].begin(); it != bucket->array0[g0].end(); ++it) {
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
                        for (std::vector<unsigned int>::iterator it = bucket->array1[g1].begin(); it != bucket->array1[g1].end(); ++it) {
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
            tabu[m0] = (*iter) + rand() % (std::max((unsigned int)1,(graph->v/10 - 3))) + 3;
            tabu[m1] = (*iter) + rand() % (std::max((unsigned int)1,(graph->v/10 - 3))) + 3;
            (*iter) += 1;
        }
    }

    else {
        //std::cout << "Pattern 3" << std::endl;
        unsigned int m;
        for (unsigned int i = 0; i < l; i++) {
            m = rand() % graph->v;
            move(graph, bucket, cut, m, f_c);
            tabu[m] = (*iter) + rand() % (std::max((unsigned int)1,(graph->v/10 - 3))) + 3;
            (*iter) += 1;
        }
    }
}
*/

void perturb (Graph* graph, struct Bucket* bucket, std::vector<bool>& cut, unsigned int l, int pattern, std::vector<ll>& tabu, ll *iter, ll f_best, ll *f_c) {
    if (pattern == 1) {
        //std::cout << "Pattern 1" << std::endl;
        unsigned int m;
        for (unsigned int i = 0; i < l; i++) {
            m = 0;
            for (unsigned int j = 1; j < graph->v; j++) {
                if (bucket->gain[j] > bucket->gain[m]) {
                    if (bucket->gain[j] > f_best - (*f_c)) m = j;
                    else if (tabu[j] < (*iter)) m = j;
                }

                else if (bucket->gain[j] == bucket->gain[m] && tabu[j] < tabu[m]) m = j;
            }
           
            move(graph, bucket, cut, m, f_c);
            tabu[m] = (*iter) + rand() % (std::max((unsigned int)1,(graph->v/10 - 3))) + 3;
            (*iter) += 1;
        }
    }

    else if (pattern == 2) {
        //std::cout << "Pattern 2" << std::endl;
        ll g1, g0;
        unsigned int m1, m0;
        for (unsigned int i = 0; i < l; i++) {
            unsigned int max_gain_m0, max_gain_m1;
            unsigned int tabu_m0, tabu_m1;

            for (unsigned int j = 0; j < graph->v; j++) {
                if (cut[j]) {
                    max_gain_m1 = j;
                    tabu_m1 = j;
                    break;
                }
            }

            for (unsigned int j = 0; j < graph->v; j++) {
                if (!cut[j]) {
                    max_gain_m0 = j;
                    tabu_m0 = j;
                    break;
                }
            }

            for (unsigned int j = 0; j < graph->v; j++) {
                if (cut[j]) {
                    if (bucket->gain[j] > bucket->gain[max_gain_m1]) max_gain_m1 = j;
                    if (tabu[j] < (*iter)) {
                        if (bucket->gain[j] > bucket->gain[tabu_m1]) tabu_m1 = j;
                        else if (bucket->gain[j] == bucket->gain[tabu_m1] && tabu[j] > tabu[tabu_m1]) tabu_m1 = j;
                    }
                }

                else {
                    if (bucket->gain[j] > bucket->gain[max_gain_m0]) max_gain_m0 = j;
                    if (tabu[j] < (*iter)) {
                        if (bucket->gain[j] > bucket->gain[tabu_m0]) tabu_m0 = j;
                        else if (bucket->gain[j] == bucket->gain[tabu_m0] && tabu[j] > tabu[tabu_m0]) tabu_m0 = j;
                    }
                }
            }

            if (bucket->gain[max_gain_m0] + bucket->gain[max_gain_m1] > f_best - (*f_c)) {
                m0 = max_gain_m0;
                m1 = max_gain_m1;
            }

            else {
                m0 = tabu_m0;
                m1 = tabu_m1;
            }

            move(graph, bucket, cut, m0, f_c);
            move(graph, bucket, cut, m1, f_c);
            tabu[m0] = (*iter) + rand() % (std::max((unsigned int)1,(graph->v/10 - 3))) + 3;
            tabu[m1] = (*iter) + rand() % (std::max((unsigned int)1,(graph->v/10 - 3))) + 3;
            (*iter) += 1;
        }
    }

    else {
        //std::cout << "Pattern 3" << std::endl;
        unsigned int m;
        for (unsigned int i = 0; i < l; i++) {
            m = rand() % graph->v;
            move(graph, bucket, cut, m, f_c);
            tabu[m] = (*iter) + rand() % (std::max((unsigned int)1,(graph->v/10 - 3))) + 3;
            (*iter) += 1;
        }
    }
}

void perturbation (Graph* graph, struct Bucket* bucket, std::vector<bool>& cut, unsigned int l, std::vector<ll>& tabu, ll *iter, int omega, ll f_best, ll *f_c) {
    if (omega == 0) perturb(graph, bucket, cut, l, 3, tabu, iter, f_best, f_c);
    else {
        double prob = ((double)rand())/((double)RAND_MAX+1.0);
        double P = std::max(std::exp(- (double)omega / (double) 1000), 0.8);

        if (prob < P * 0.5) perturb(graph, bucket, cut, l, 1, tabu, iter, f_best, f_c);
        else if (prob < P) perturb(graph, bucket, cut, l, 2, tabu, iter, f_best, f_c);
        else perturb(graph, bucket, cut, l, 3, tabu, iter, f_best, f_c);
    }

}

int BreakoutLocalSearch (Graph* graph, ll max_step, char* filename) {
    FILE *fp;
    clock_t start,end;

    std::vector<bool> cut(graph->v, false);
    std::vector<bool> cut_p(cut.size());
    std::vector<bool> cut_best(cut.size());
    for (unsigned int i = 0; i < graph->v; i++) cut[i] = rand() % 2;
    std::copy(cut.begin(), cut.end(), cut_p.begin());
    std::copy(cut.begin(), cut.end(), cut_best.begin());

    ll f_c = calculate_cut(graph, cut);
    ll f_best = f_c;

    int omega = 0;
    ll iter = 0;
    unsigned int l_0 = graph->v / 100;
    unsigned int l = l_0;
    unsigned int i;
    int j;

    std::vector<ll> tabu(graph->v, 0);

    fp = fopen(filename,"w");

    if (fp == NULL){
        printf("file can't open.\n");
        return 1;
    }
    fprintf(fp, "#Iteration #Cut size #Time(s)\n");


    std::cout << "Initializing bucket... ";
    struct Bucket* bucket = initialize_bucket(graph, cut);
    std::cout << "finished" << std::endl;

    start = clock();

    while (iter < max_step) {
        /*
        while (bucket->max_gain0 > 0 || bucket->max_gain1 > 0) {
            if (bucket->max_gain0 > bucket->max_gain1) i = (*bucket->array0[bucket->max_gain0 + bucket->central].begin());
            else i = (*bucket->array1[bucket->max_gain1 + bucket->central].begin());
            j = cut[i];
            move(graph, bucket, cut, i, &f_c);
            tabu[i] = iter +  + rand() % (std::max((unsigned int)1,(graph->v/10 - 3))) + 3;

            iter++;
        }
        */

        while (1) {
            unsigned int m = 0;
            for (i = 0; i < graph->v; i++) {
                if (bucket->gain[i] > bucket->gain[m]) m = i;
            }

            if (bucket->gain[m] <= 0) break;

            move(graph, bucket, cut, m, &f_c);
            tabu[m] = iter + rand() % (std::max((unsigned int)1,(graph->v/10 - 3))) + 3;
            iter++;
        }


        if (f_c != calculate_cut(graph, cut)) std::cout << "Bug" << std::endl;

        if (f_c > f_best) {
            f_best = f_c;
            end = clock();
            std::cout << "------------ Best is updated ------------" << std::endl;
            std::cout << "steps : " << iter << ", best cut size : " << f_best << std::endl;
            std::cout << "time : " << (double)(end-start)/(double)CLOCKS_PER_SEC << "s" << std::endl;
            std::cout << "-----------------------------------------" << std::endl;
            std::cout << std::endl;
            fprintf(fp, "%lld %lld %f\n", iter, f_best, (double)(end-start)/(double)CLOCKS_PER_SEC);
            std::copy(cut.begin(), cut.end(), cut_best.begin());
            omega = 0;
        }

        else omega++;

        if (omega > 1000) omega = 0;

        if (cut == cut_p) l++;
        else l = l_0;

        perturbation(graph, bucket, cut, l, tabu, &iter, omega, f_best, &f_c);

    }

    end = clock();

    std::cout << "------------- BLS  finished -------------" << std::endl;
    std::cout << "steps : " << iter << ", best cut size : " << f_best << std::endl;
    std::cout << "time : " << (double)(end-start)/(double)CLOCKS_PER_SEC << "s" << std::endl;
    std::cout << "-----------------------------------------" << std::endl;
    std::cout << std::endl;
    fprintf(fp, "%lld %lld %f\n", iter, f_best, (double)(end-start)/(double)CLOCKS_PER_SEC);
    for (i = 0; i < graph->v; i++) fprintf(fp, "%d ", cut[i] ? 1 : 2);

    return 0;
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    std::cout << "Reading graph... ";
    Graph* graph = inputGraph(argv[1]);
    std::cout << "finished" << std::endl;

    BreakoutLocalSearch(graph, 10000000, argv[2]);

    return 0;
}