//
// Created by Nikolay Yakovets on 2018-02-02.
//

#include "SimpleEstimator.h"
#include "SimpleEvaluator.h"

SimpleEvaluator::SimpleEvaluator(std::shared_ptr<SimpleGraph> &g) {

    // works only with SimpleGraph
    graph = g;
    est = nullptr; // estimator not attached by default
}

void SimpleEvaluator::attachEstimator(std::shared_ptr<SimpleEstimator> &e) {
    est = e;
}

void SimpleEvaluator::prepare() {

    // if attached, prepare the estimator
    if(est != nullptr) est->prepare();

    // prepare other things here.., if necessary

}

cardStat SimpleEvaluator::computeStats(std::shared_ptr<std::vector<std::pair<uint32_t,uint32_t>>> &g) {

    cardStat stats {};

    uint32_t last = 0;
    std::sort(g->begin(), g->end(), SimpleGraph::sortPairsFirst);
    for(uint32_t i = 0; i < g->size(); i ++) {
        if(i == 0 || g->at(i).first != last) {
            stats.noOut++;
        }

        last = g->at(i).first;
    }

    std::sort(g->begin(), g->end(), SimpleGraph::sortPairsSecond);
    for(uint32_t i = 0; i < g->size(); i ++) {
        if(i == 0 || g->at(i).second != last) {
            stats.noIn++;
        }

        last = g->at(i).second;
    }

    std::sort(g->begin(), g->end(), SimpleGraph::sortPairsFirst);
    uint32_t prevFrom = 0;
    uint32_t prevTo = 0;
    bool first = true;

    for(uint32_t i = 0; i < g->size(); i ++) {
        if (first || !(prevFrom == g->at(i).first && prevTo == g->at(i).second)) {
            first = false;
            stats.noPaths ++;
            prevFrom = g->at(i).first;
            prevTo = g->at(i).second;
        }
    }


    return stats;
}

std::shared_ptr<std::vector<std::pair<uint32_t,uint32_t>>> SimpleEvaluator::project(uint32_t projectLabel, bool inverse, std::shared_ptr<SimpleGraph> &in) {

    auto out = std::make_shared<std::vector<std::pair<uint32_t,uint32_t>>>();

    for (const auto &edge : in->adj[projectLabel]) {
        if(!inverse)
            out->emplace_back(edge.first, edge.second);
        else
            out->emplace_back(edge.second, edge.first);
    }

    return out;
}

std::shared_ptr<std::vector<std::pair<uint32_t,uint32_t>>> SimpleEvaluator::join(std::shared_ptr<std::vector<std::pair<uint32_t,uint32_t>>> &left, std::shared_ptr<std::vector<std::pair<uint32_t,uint32_t>>> &right) {

    auto out = std::make_shared<std::vector<std::pair<uint32_t,uint32_t>>>();

    std::sort(left->begin(), left->end(), SimpleGraph::sortPairsSecond);
    std::sort(right->begin(), right->end(), SimpleGraph::sortPairsFirst);

    uint32_t start = 0;
    for(uint32_t i = 0; i < left->size(); i ++) {
        for(uint32_t j = start; j < right->size(); j ++) {

            if(left->at(i).second > right->at(j).first) {
                start = j + 1;
            }
            else if(left->at(i).second == right->at(j).first)
                out->emplace_back(left->at(i).first, right->at(j).second);
            else break;
        }
    }

    return out;
}

std::shared_ptr<std::vector<std::pair<uint32_t,uint32_t>>> SimpleEvaluator::evaluate_aux(RPQTree *q) {

    // evaluate according to the AST bottom-up

    if(q->isLeaf()) {
        // project out the label in the AST
        std::regex directLabel (R"((\d+)\+)");
        std::regex inverseLabel (R"((\d+)\-)");

        std::smatch matches;

        uint32_t label;
        bool inverse;

        if(std::regex_search(q->data, matches, directLabel)) {
            label = (uint32_t) std::stoul(matches[1]);
            inverse = false;
        } else if(std::regex_search(q->data, matches, inverseLabel)) {
            label = (uint32_t) std::stoul(matches[1]);
            inverse = true;
        } else {
            std::cerr << "Label parsing failed!" << std::endl;
            return nullptr;
        }

        return SimpleEvaluator::project(label, inverse, graph);
    }

    if(q->isConcat()) {

        // evaluate the children
        auto leftGraph = SimpleEvaluator::evaluate_aux(q->left);
        auto rightGraph = SimpleEvaluator::evaluate_aux(q->right);

        // join left with right
        return SimpleEvaluator::join(leftGraph, rightGraph);

    }

    return nullptr;
}


std::vector<RPQTree*> SimpleEvaluator::getLeaves(RPQTree *query) {
    if (query->isLeaf()) {
        return {query};
    }

    std::vector<RPQTree*> result;
    if (query->left) {
        auto rec = getLeaves(query->left);
        result.insert(result.end(), rec.begin(), rec.end());
    }
    if (query->right) {
        auto rec = getLeaves(query->right);
        result.insert(result.end(), rec.begin(), rec.end());
    }

    return result;
}

RPQTree* SimpleEvaluator::optimizeQuery(RPQTree *query) {
    std::vector<RPQTree*> leaves = getLeaves(query);

    while (leaves.size() > 1) {
        uint32_t bestScore = 0;
        RPQTree *bestTree = nullptr;
        int index = -1;

        for (int i = 0; i < leaves.size()-1; ++i) {
            std::string data("/");
            auto *currentTree = new RPQTree(data, leaves[i], leaves[i+1]);
            uint32_t currentScore = est->estimate(currentTree).noPaths;

            if (bestScore == 0 || bestScore > currentScore) {
                bestScore = currentScore;
                bestTree = currentTree;
                index = i;
            }
        }

        leaves.erase(leaves.begin() + index + 1);
        leaves[index] = bestTree;
    }

    return leaves[0];
}

cardStat SimpleEvaluator::evaluate(RPQTree *query) {
    RPQTree* optimized = optimizeQuery(query);
    optimized->print();

    auto res = evaluate_aux(optimized);
    return SimpleEvaluator::computeStats(res);
}