//
// Created by Nikolay Yakovets on 2018-02-01.
//

#include "SimpleGraph.h"
#include "SimpleEstimator.h"

uint32_t numLabels;
cardStat* labelData;
std::vector<std::pair<uint32_t, char>> queryVector;


SimpleEstimator::SimpleEstimator(std::shared_ptr<SimpleGraph> &g){

    // works only with SimpleGraph
    graph = g;


}

void SimpleEstimator::prepare() {
//    numLabels = graph.get()->getNoLabels();
//    labelData = new cardStat[numLabels];
//
//    uint32_t numIn;
//    uint32_t numOut;
//
//    for(auto i = 0; i < numLabels; i ++) {
//        if(graph.get()->adj[i].empty()) {
//            labelData[i] = {0, 0, 0};
//        }
//        else {
//
//            numIn = 0;
//            numOut = 0;
//
//            std::sort(graph.get()->adj[i].begin(), graph.get()->adj[i].end(), SimpleGraph::sortPairsFirst);
//            bool first = true;
//            for (auto j = 0; j < graph.get()->adj[i].size(); j++) {
//                if (first) {
//                    numOut++;
//                    first = false;
//                } else {
//                    if (graph.get()->adj[i][j].first != graph.get()->adj[i][j - 1].first)
//                        numOut++;
//                }
//            }
//
//            std::sort(graph.get()->adj[i].begin(), graph.get()->adj[i].end(), SimpleGraph::sortPairsSecond);
//            first = true;
//            for (auto j = 0; j < graph.get()->adj[i].size(); j++) {
//                if (first) {
//                    numIn++;
//                    first = false;
//                } else {
//                    if (graph.get()->adj[i][j].second != graph.get()->adj[i][j - 1].second)
//                        numIn++;
//                }
//            }
//
//            labelData[i] = {numOut, graph.get()->adj[i].size(), numIn};
//        }
//    }
}

//void treeToList(RPQTree* q) {
//    if(q->isConcat()) {
//        treeToList(q->left);
//        treeToList(q->right);
//    }
//    else if(q->isLeaf()) {
//        auto label = q->data.substr(0, q->data.size() - 1);
//        std::stringstream geek(label);
//        uint32_t labelInt;
//        geek >> labelInt;
//        queryVector.emplace_back(std::make_pair(labelInt, q->data.at(q->data.size() - 1)));
//    }
//}
//
//cardStat reverse(cardStat c) {
//    return {c.noIn, c.noPaths, c.noOut};
//}

cardStat SimpleEstimator::estimate(RPQTree *q) {
//       treeToList(q);
//
//    if(queryVector.empty())
//    {
//        queryVector.clear();
//        return cardStat{0,0,0};
//    }
//    else if(queryVector.size()==1)
//    {
//        cardStat onlyOne=labelData[queryVector[0].first];
//        queryVector.clear();
//        if(queryVector[0].second == '+')
//            return onlyOne;
//        else return reverse(onlyOne);
//    }
//    else
//    {
//        cardStat left;
//        if(queryVector[0].second == '+')
//            left = labelData[queryVector[0].first];
//        else left = reverse(labelData[queryVector[0].first]);
//
//        uint32_t total = (left.noIn + left.noOut) / 2;
//        uint32_t one = 1;
//        for(int i=1; i<queryVector.size();i++)
//        {
//            cardStat right;
//            if(queryVector[i].second == '+')
//                right = labelData[queryVector[i].first];
//            else right = reverse(labelData[queryVector[i].first]);
//
//            uint32_t in = (left.noIn)/ 4;
//            uint32_t out = (right.noOut) / 4;
//            total /= 4;
//            auto paths = std::min(2 * left.noPaths * right.noPaths / std::max((right.noIn + right.noOut + total), one),
//                                  2 * left.noPaths * right.noPaths / std::max((left.noOut + left.noIn + total), one));
//            cardStat processed = cardStat{std::min(out, paths), paths, std::min(in, paths)};
//            left = processed;
//        }
//        queryVector.clear();
//        return left;
//    }
    return {0,0,0};
}
