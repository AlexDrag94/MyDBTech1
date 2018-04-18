//
// Created by Nikolay Yakovets on 2018-02-02.
//

#ifndef QS_SIMPLEEVALUATOR_H
#define QS_SIMPLEEVALUATOR_H


#include <memory>
#include <cmath>
#include "SimpleGraph.h"
#include "RPQTree.h"
#include "Evaluator.h"
#include "Graph.h"

class SimpleEvaluator : public Evaluator {

    std::shared_ptr<SimpleGraph> graph;
    std::shared_ptr<SimpleEstimator> est;

public:

    explicit SimpleEvaluator(std::shared_ptr<SimpleGraph> &g);
    ~SimpleEvaluator() = default;

    void prepare() override ;
    cardStat evaluate(RPQTree *query) override ;

    void attachEstimator(std::shared_ptr<SimpleEstimator> &e);

    std::vector<std::pair> evaluate_aux(RPQTree *q);
    static std::vector<std::pair> project(uint32_t label, bool inverse, std::shared_ptr<SimpleGraph> &g);
    static std::vector<std::pair> join(std::vector<std::pair> &left, std::vector<std::pair> &right);

    std::vector<RPQTree*> getLeaves(RPQTree *query);
    RPQTree* optimizeQuery(RPQTree *query);


    static cardStat computeStats(std::vector<std::pair> &g);

};


#endif //QS_SIMPLEEVALUATOR_H
